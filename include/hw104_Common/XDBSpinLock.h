//==============================================================================
//
// ��Ŀ�� ����ҵʵʱ���ݿ�
// �ļ��� ��XDBSpinLock.h
// ��  �� ��������( Jack )
// ��  ; ����������ʵ�֡�
// 
//==============================================================================
// �汾��¼	
//==============================================================================
//
// V0.1	- ������ 2005-3-4 21:10:56
//	
// 
//==============================================================================
#ifndef __XDBSPINLOCK__H__INCLUDED__
#define __XDBSPINLOCK__H__INCLUDED__
//==============================================================================

namespace NkxLatchLock {

/// <summary> 
/// ԭ������
/// </summary> 
class XDBSpinLock
{
public:
	/// <summary> 
	/// ���캯����
	/// </summary>
	XDBSpinLock( )
		: m_spin( 0 )
	{
		if( m_numberOfCPU == 0 )
			numberOfCPU( );
#ifdef TRACE_LOCK_OWNER
		m_ownerThreadId = 0;
		m_lineNo		= 0;
#endif 

	}

	/// <summary> 
	/// �����������
	/// </summary> 
	bool lock( )
	{
		if( 	0 == InterlockedCompareExchange( &m_spin, 1, 0 ) )
		{
	#ifdef TRACE_LOCK_OWNER
			m_ownerThreadId = ( LONG )GetCurrentThreadId( ) ;
	#endif 
			return true; // ���������
		}


		// ��CPU��
		if( m_numberOfCPU == 1 )
		{
			while( 0 != InterlockedCompareExchange( &m_spin, 1, 0 ) )
			{
				// ʹ��SwitchToThread����Sleep( 0 )��ʹ�õ����ȼ�
				// ���߳��ܹ��õ�����
				// Sleep( 0 );
			#if( _WIN32_WINNT >= 0x0400 )
				if( !SwitchToThread( ) )Sleep( 1 );	
			#else 
				Sleep( 0 );
			#endif // _WIN32_WINNT >= 0x0400
			}

	#ifdef TRACE_LOCK_OWNER
			m_ownerThreadId = ( LONG )GetCurrentThreadId( ) ;
	#endif 
			return true;
		}
		else // ��CPU,SPIN
		{
			LONG counter = 0;
			while( 0 != InterlockedCompareExchange( &m_spin, 1, 0 ) )
			{
				for( counter = 0; counter < DEFAULT_SPIN_COUNT; counter++ )
				{
					if( 	0 == InterlockedCompareExchange( &m_spin, 1, 0 ) )
						return true; // ���������
				}

				// ʹ��SwitchToThread����Sleep( 0 )��ʹ�õ����ȼ�
				// ���߳��ܹ��õ�����
				// Sleep( 0 );
			#if( _WIN32_WINNT >= 0x0400 )
				if( !SwitchToThread( ) ) Sleep( 1 );
			#else 
				Sleep( 0 );
			#endif // _WIN32_WINNT >= 0x0400
			} // while loop	

	#ifdef TRACE_LOCK_OWNER
			m_ownerThreadId = ( LONG )GetCurrentThreadId( ) ;
	#endif 
			return true;
		} // ��CPU
	}

	/// <summary> 
	/// ���Լ�����
	/// </summary>
	/// <remark>
	///     �������true����������unlock�ͷ�����
	/// </remark>
	bool tryLock( )
	{
		if( 	0 == InterlockedCompareExchange( &m_spin, 1, 0 ) )
		{
		#ifdef TRACE_LOCK_OWNER
			m_ownerThreadId = ( LONG )GetCurrentThreadId( ) ;
		#endif 
			return true; // ���������
		}
		else
			return false;
	}

	/// <summary> 
	/// �ͷ���������
	/// </summary> 
	bool unlock( )
	{
		LONG Value = InterlockedCompareExchange( &m_spin,0,1 );
	#ifdef TRACE_LOCK_OWNER
		InterlockedCompareExchange( &m_ownerThreadId,0,( LONG )GetCurrentThreadId( ) );
	#endif 
		ASSERT( Value == 1 );
		return true;
	}

	/// <summary> 
	/// �Ƿ�Ϊ��CPU�������
	/// </summary> 
	static LONG numberOfCPU( )
	{
		if( m_numberOfCPU == 0 )
		{
			SYSTEM_INFO info = {0};
			GetSystemInfo( &info );
			m_numberOfCPU = ( LONG ) info.dwNumberOfProcessors;
		}
		return m_numberOfCPU;
	}

//==============================================================================
#ifdef TRACE_LOCK_OWNER 
//==============================================================================

	/// <summary> 
	/// ��ó������������߳�ID��
	/// </summary> 
	LONG ownerThreadId( ) const
	{
		return m_ownerThreadId;
	}

	/// <summary> 
	/// ���õ������кţ����ڸ���������
	/// </summary> 
	void setLineNo( LONG LineNo )
	{
		m_lineNo = LineNo;
	}

//==============================================================================
#endif 
//==============================================================================


private:
	volatile LONG m_spin;			// ��������

	// ע����ֵ��XDBPublic.cpp�г�ʼ��
	static   LONG m_numberOfCPU;	// CPU����

	// ���ڵ��Ը�����Ϣ
#ifdef TRACE_LOCK_OWNER 
	LONG	m_lineNo;
	LONG	m_ownerThreadId;
#endif 

	// disable copy constructor
	XDBSpinLock( const XDBSpinLock& );
	XDBSpinLock& operator=( const XDBSpinLock& );

};

/// <summary> 
/// �Զ������������
/// </summary> 
class XDBAutoSpinLock
{
public:
	/// <summary> 
	/// ���캯����
	/// </summary>
	/// <remark>
	///     �Զ������������
	/// </remark>
	XDBAutoSpinLock( XDBSpinLock& LockObj )
		:m_spinLock( LockObj  )
	{
		m_spinLock.lock( );
	}

	/// <summary> 
	/// ����������
	/// </summary>
	/// <remark>
	///     �Զ��ͷ���������
	/// </remark>
	~XDBAutoSpinLock( )
	{
		m_spinLock.unlock( );
	}

private:
	XDBSpinLock& m_spinLock;	// ����������

	// disable copy constructor
	XDBAutoSpinLock( const XDBAutoSpinLock& );
	XDBAutoSpinLock& operator=( const XDBAutoSpinLock& );
};

//========================================================= 
// ������
//========================================================= 
#define ACQUIRE_SPINLOCK( x )	( ( x ).lock( ) )
#define RELEASE_SPINLOCK( x )	( ( x ).unlock( ) )
#define TRY_SPINLOCK( x )			( ( x ).tryLock( ) )



}

#endif // __XDBSPINLOCK__H__INCLUDED__
//==============================================================================
