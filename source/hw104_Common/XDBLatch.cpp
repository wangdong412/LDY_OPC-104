//==============================================================================
//
// ��Ŀ�� ����ҵʵʱ���ݿ�
// �ļ��� ��XDBLatch.cpp
// ��  �� ��������( Jack )
// ��  ; ��������ʵ�֡�
// 
//==============================================================================
// �汾��¼	
//==============================================================================
//
// V0.1	- ������ 2005-4-25 10:43:25
//	
// 
//==============================================================================
#include "stdafx.h"
#include "hw104_Common/hw104Common_Ext_inc.h"
//==============================================================================


//========================================================= 
//  ���Ժ�
//========================================================= 
#define __TRACE_LATCH__


#ifdef  __TRACE_LATCH__

// ����ģʽ���ַ�������
static const WCHAR* _xdbLatchModeDesc[  ] =
{
	WSTR( "NL" ),WSTR( "SH" ),WSTR( "EX" ),
};

// ������������
#define TRACE_LATCH_CALL( RequireMode ) \
	{									\
		WCHAR wszTempBuffer[ MAX_TRACE_BUFFER ] = {  0 }; \
		swprintf( wszTempBuffer,MAX_TRACE_BUFFER,						\
				  WSTR( "LatchObject = %08X  CurrentThreadId = %04X  CurrentMode = %s ExclusiveHolder = %04X OwnerCount	 = %02d WaiterCount    = %02d RequireMode = %s FunctionCall	 = %s" ),	\
				  this,GetCurrentThreadId( ), _xdbLatchModeDesc[ m_mode ],m_exclusiveHolder,\
				  m_counter,m_waiterList.size( ),_xdbLatchModeDesc[ RequireMode ],__WFUNCTION__ ); \
		DEBUG_TRACE_DEBUG( wszTempBuffer ); \
	}

#else
	#define TRACE_LATCH_CALL( RequireMode )	
#endif 


XDBLatchBlockList  XDBLatch::m_freeList( LATCHBLOCK_FREE_SIZE );

LONG XDBSpinLock::m_numberOfCPU = 0;

/// <summary> 
///		����������
/// </summary> 
/// <param name="Mode">
///		����������ģʽ��
/// </param>
/// <param name="TimeOut">
///		����ʱ���á�
/// </param>
/// <returns>
///		�ɹ�ʱ����scSuccess��ʧ��ʱ������Ӧ�Ĵ����롣
/// </returns>
HRESULT XDBLatch::acquire( 
	LATCH_MODE	Mode, 
	DWORD		TimeOut /* = WAIT_FOREVER  */ )
{
	////========================================================= 
	//// ����LATCH����
	//TRACE_LATCH_CALL( Mode );
	////========================================================= 

	// �õ���ǰ�߳�ID
	DWORD dwThreadId = ::GetCurrentThreadId( );

	KXASSERT( ( Mode == LATCH_SH ) || ( Mode == LATCH_EX ) );

	// ���������
	ACQUIRE_SPINLOCK( m_spinLock );
	if( m_destroyed )
	{
		// ��Ҫ�����ͷ�������
		RELEASE_SPINLOCK( m_spinLock );
		return KXSTATUS_FAILED;
	}

	// ���ȼ���Ƿ��˳�������
	HRESULT ret = KXSTATUS_SUCCESS;
	if( m_counter == 0 )
	{
		// ���û�г��и��ţ������̳ɹ��������
		m_exclusiveHolder = ( Mode == LATCH_EX )? dwThreadId : NULL;
		m_mode = Mode;
		m_counter++;
	}
	else if( ( m_mode == LATCH_EX ) && ( m_exclusiveHolder == dwThreadId ) )
	{
		// ����Լ��Ѿ����ж�ռ������������������Զ�ת��Ϊ��ռ��
		m_counter++;
	}
	else if( ( m_mode == LATCH_SH ) && ( Mode == LATCH_SH ) && m_waiterList.empty( ) )
	{
		// ���������ǰģʽΪ��������������ҲΪ�����������ҵȴ�����Ϊ�գ����������裻
		m_counter++;
	}
	else // �������������Ҫ�ȴ����򷵻�ʧ��
	{
		if( TimeOut == WAIT_IMMEDIATE ) // NO WAIT
		{
			ret = KXSTATUS_FAILED;
		}
		else // ���ȴ�
		{
			// ���һ���ȴ��ſ鲢���뵽�ȴ����У��������õȴ�ģʽ��
			XDBLatchBlock* LatchBlock = m_freeList.pop( );
			if( LatchBlock == NULL )
			{
				LatchBlock = new XDBLatchBlock;
			}
			LatchBlock->ThreadId = dwThreadId;
			LatchBlock->WaitLatchMode = Mode;

			m_waiterList.append( LatchBlock );
			

			// �ڽ���ȴ�״̬֮ǰ���ͷ���������
			RELEASE_SPINLOCK( m_spinLock );

			// ��ʱ�ȴ���
			ret = LatchBlock->wait( TimeOut );

			// ���»����
			ACQUIRE_SPINLOCK( m_spinLock );

			// ����Ѿ��������,�ȴ��ſ�Ӧ���Ѿ��ӵȴ����жϿ�
			LatchBlock->WaitLink.detach( );

			if( ret != KXSTATUS_SUCCESS )
			{
				// ���¼���Ƿ�������
				ret = LatchBlock->wait( WAIT_IMMEDIATE );
			}

			// �ͷŵȴ����ſ�
			m_freeList.push( LatchBlock );

			// ���ȴ����
            if( KXSTATUS_SUCCESS == ret ) // OK
			{
				// �ٴμ��latch��״̬
				if( m_destroyed )
				{
					RELEASE_SPINLOCK( m_spinLock );
					return KXSTATUS_FAILED;
				}

				// ����Ƿ������Ƿ���������
				if( (  m_mode != Mode ) || ( m_counter <= 0 ) ||
					( ( m_mode == LATCH_EX )  && ( m_exclusiveHolder != dwThreadId ) ) )
				{
					KXASSERT( FALSE );
					RELEASE_SPINLOCK( m_spinLock );
					return KXSTATUS_FAILED;
				}
			}// wait success

		} // NOT NO WAIT
	}// other case

	// У���˨���ɹ���m_counter�϶�����1
	KXASSERT( !( ( ret == KXSTATUS_SUCCESS ) && m_counter == 0 ) );


//========================================================= 
// ���ٹ�������������
//========================================================= 
#ifdef TRACE_LATCH_OWNER
//========================================================= 
	{
		int  _index = 0;
		bool _found = false;
		int  _emptyIndex = -1;
		for( _index = 0; _index < TRACE_LATCH_OWNER_MAX; _index++ )
		{
			if( m_holderOwner[ _index ] == dwThreadId )
			{
				m_holderCount[ _index ]++;
				_found = true;
				break;
			}
			else if( ( _emptyIndex < 0 ) && ( m_holderOwner[ _index ] == NULL ) )
			{
				_emptyIndex = _index;
			}
		}
		if( ( !_found ) && ( _emptyIndex >= 0 ) )
		{
			m_holderOwner[ _emptyIndex ]	= dwThreadId;
			m_holderCount[ _emptyIndex ]	= 1;
		}
	}
//========================================================= 
#endif 
//========================================================= 

	// �ͷ������� 
	RELEASE_SPINLOCK( m_spinLock );

	// ���ؽ��
	return ret;
}

/// <summary> 
///	�ͷ�������	
/// </summary> 
void XDBLatch::release( )
{
	////========================================================= 
	//// ����LATCH����
	//TRACE_LATCH_CALL( 0 );
	////========================================================= 

	// ���������
	ACQUIRE_SPINLOCK( m_spinLock );
	if( m_destroyed )
	{
		// ��Ҫ�����ͷ�������
		RELEASE_SPINLOCK( m_spinLock );
		return;
	}
	// ��С���ü���
	m_counter--;
	KXASSERT( m_counter >= 0 );
	if( 0 == m_counter ) // û������������
	{
		m_exclusiveHolder = NULL;
		m_mode			  = LATCH_NL;
	}

//========================================================= 
// ���ٹ�������������
//========================================================= 
#ifdef TRACE_LATCH_OWNER
//========================================================= 
	{
		DWORD dwThreadId = ::GetCurrentThreadId( );
		for( int _index = 0; _index < TRACE_LATCH_OWNER_MAX; _index++ )
		{
			if( 0 == m_counter )
			{
				m_holderCount[ _index ] = 0;
				m_holderOwner[ _index ] = 0;
				m_holderLine[ _index ]  = 0;
			}
			else if( m_holderOwner[ _index ] == dwThreadId )
			{
				m_holderCount[ _index ]--;
				if( m_holderCount[ _index ] <= 0 )
				{
					m_holderOwner[ _index ] = 0;
					m_holderLine[ _index ]  = 0;
					m_holderCount[ _index ] = 0;
				}
				break;
			}
		}
	}
//========================================================= 
#endif // TRACE_LATCH_OWNER
//========================================================= 



	// ���ȴ�����
	if( ( 0 == m_counter ) && !m_waiterList.empty( ) )
	{
		XDBLinkListIterator<XDBLatchBlock> Iterator( m_waiterList );
		XDBLatchBlock* waiter = Iterator.next( );	// ע�������������m_waiterList.pop( )
		if( waiter->WaitLatchMode == LATCH_EX )
		{
			// �ӵȴ��б��жϿ�
			waiter->WaitLink.detach( );
			// �������
			m_mode = LATCH_EX;
			m_counter++;
			m_exclusiveHolder = waiter->ThreadId;
			// ���ѵȴ��߳�
			waiter->wakeup( );
		} // LATCH_EX
		else
		{
			// �������������������߳�
			while( ( waiter != NULL ) && ( waiter->WaitLatchMode == LATCH_SH ) )
			{
				waiter->WaitLink.detach( );
				// �������
				m_mode = LATCH_SH;
				m_counter++;
				// ���ѵȴ��߳�
				waiter->wakeup( );
				waiter = Iterator.next( );
			}
		}// LATCH_SH
	} // m_counter == 0 

	// �ͷ�������
	RELEASE_SPINLOCK( m_spinLock );
}


/// <summary> 
///		������������ռģʽ��
/// </summary> 
/// <param name="TimeOut">
///		��ʱ���á�
/// </param>
/// <returns>
///		�ɹ�ʱ����scSuccess��ʧ��ʱ������Ӧ�Ĵ����롣
/// </returns>
/// <remarks>
///     �����ɵ����߱�֤�����Ѿ����й�����( �ҽ�����һ�Σ����򽫵������� )��
///     ��Ϊ��������������ж�Σ� m_counter > 1 ��,�޷��ж��������Ƿ�ȫ��
///     ��һ���̳߳���
/// </remarks>
HRESULT XDBLatch::upgrade( 
	DWORD		TimeOut /*= WAIT_FOREVER*/ )
{
	////========================================================= 
	//// ����LATCH����
	//TRACE_LATCH_CALL( LATCH_EX );
	////========================================================= 

	DWORD dwThreadId = ::GetCurrentThreadId( );

	//========================================================= 
	// �������״̬�����Ƿ��ܹ���������
	//========================================================= 
	{
		XDBAutoSpinLock AutoLock( m_spinLock );
		if( m_destroyed ) 
			return KXSTATUS_FAILED;
	
		// ��鵱ǰģʽ( �����Ѿ����й���ģʽ������ )
		if( ( m_counter <= 0 ) ||( m_mode != LATCH_SH ) )
			return KXSTATUS_FAILED;
		
		// ����ܷ����������ɹ�
		if( ( m_counter == 1 ) && ( m_waiterList.empty( ) ) )
		{
			//========================================================= 
			// 
			// ע�⣺����XDBLatch��ʵ�ֲ�û�и��������Ĺ�������ߣ����
			//		 ����������Լ���֤������й���ģʽ�����������򽫵�
			//		 ���޷�Ԥ�ϵĺ����
			//
			//========================================================= 

			// �������������������ü���
			m_mode				= LATCH_EX;
			m_exclusiveHolder	= dwThreadId;
			return KXSTATUS_SUCCESS;
		}

		// ��������������������鳬ʱ����
		if( TimeOut == WAIT_IMMEDIATE )
			return KXSTATUS_FAILED;

	} // �Զ��ͷ�������

	// ���������������������ͷ�������е�������Ȼ�����»����
	// �ͷ�����
	release( );
	
	// Ȼ����������������
	return acquire( LATCH_EX,TimeOut ) ;
}

/// <summary> 
///	����������	
/// </summary> 
XDBLatch::~XDBLatch( )
{
	// ���������
	ACQUIRE_SPINLOCK( m_spinLock );

	// ����ɾ��������־
	m_destroyed = true;

	// �������еĵȴ��̣߳��������Լ��Ӷ�����ɾ��
	XDBLinkListIterator<XDBLatchBlock> Iterator( m_waiterList );
	XDBLatchBlock* waiter = Iterator.next( );
	while( waiter != NULL )
	{
		waiter->wakeup( );
		waiter = Iterator.next( );
	}

	// �ȴ��̴߳ӵȴ��������Լ�ɾ��
	while( !m_waiterList.empty( ) )
	{
		// ��Ҫ���ͷ���
		RELEASE_SPINLOCK( m_spinLock );

		// �߳��л�
		if( !SwitchToThread( ) )
			Sleep( 1 );

		// �ٴλ����
		ACQUIRE_SPINLOCK( m_spinLock );
	}

	// ���ö�ռ��Ա
	m_exclusiveHolder = NULL;

	// �ͷ���
	RELEASE_SPINLOCK( m_spinLock );
}

/// <summary> 
/// ���캯����
/// </summary> 
XDBLatch::XDBLatch( ):
	m_destroyed( false ),
	m_exclusiveHolder( NULL ),
	m_counter( 0 ),
	m_mode( LATCH_NL ),
	m_waiterList( LINK_LIST_ARG( XDBLatchBlock , WaitLink ) ) 
{
//========================================================= 
// ���ٹ�������������
//========================================================= 
#ifdef TRACE_LATCH_OWNER
	for( int index = 0; index < TRACE_LATCH_OWNER_MAX; index++ )
	{
		m_holderOwner[ index ] = NULL;
		m_holderCount[ index ] = 0;
		m_holderLine[ index ]  = 0;
	}
#endif 
//========================================================= 

}

/// <summary> 
/// �ǿ���״̬��
/// </summary> 
bool XDBLatch::isFree( )
{
	XDBAutoSpinLock AutoLock( m_spinLock );
	return ( ( m_counter == 0  ) && ( m_waiterList.empty( ) ) );
}


/// <summary> 
/// ����Ƿ����˳���������
/// </summary> 
bool XDBLatch::isLatched( )
{	
	XDBAutoSpinLock AutoLock( m_spinLock );
	return ( m_counter > 0 );
}


/// <summary> 
/// ����ģʽ��
/// </summary> 
LATCH_MODE XDBLatch::mode( )
{
	XDBAutoSpinLock AutoLock( m_spinLock );
	return ( LATCH_MODE )m_mode;
}
