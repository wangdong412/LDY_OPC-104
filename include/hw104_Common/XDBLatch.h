//==============================================================================
//
// ��Ŀ�� ����ҵʵʱ���ݿ�
// �ļ��� ��XDBLatch.h
// ��  �� ��������( Jack )
// ��  ; ��������ʵ�֡�
// 
//==============================================================================
// �汾��¼	
//==============================================================================
//
// V0.1	- ������ 2005-4-25 10:37:58
//	
// 
//==============================================================================
#ifndef __XDBLATCH__H__INCLUDED__
#define __XDBLATCH__H__INCLUDED__
//==============================================================================

//==============================================================================
#pragma pack( push )
#pragma pack( 1 )
#include "hw104_Common/XDBLinkList.h"
#include "hw104_Common/XDBSpinLock.h"

namespace NkxLatchLock 
{

#define 	WAIT_IMMEDIATE	( DWORD )0			// ���ȴ�
#define 	WAIT_FOREVER	( DWORD )INFINITE 	// ���޵ȴ�

// ���ٹ��������
//#define TRACE_LATCH_OWNER
#define TRACE_LATCH_OWNER_MAX		8
#define LATCHBLOCK_FREE_SIZE       256     // ��������д�С

/// <summary> 
/// ����ģʽ��
/// </summary>
enum LATCH_MODE
{
	LATCH_NL = 0,		// ������
	LATCH_SH = 1,		// ����
	LATCH_EX = 2,		// ��ռ
};

/// <summary> 
/// ����ģʽ��
/// </summary>
class XDBLatchBlock
{
public:
	/// <summary> 
	///		�ȴ������
	/// </summary>
	HRESULT wait( DWORD TimeOut = WAIT_FOREVER )
	{
		DWORD rc = ::WaitForSingleObject( m_hEvent,TimeOut );
		if( rc == WAIT_OBJECT_0 )
			return KTN_OK;

		return WAIT_TIMEOUT == rc ? KTN_OK : KTN_FAILED;
	}
	/// <summary> 
	///		���ѵȴ�
	/// </summary>
	BOOL wakeup( )
	{
		return ::SetEvent( m_hEvent );
	}

	/// <summary> 
	///		���ѵȴ�
	/// </summary>
	BOOL sleep( )
	{
		return ::ResetEvent( m_hEvent );
	}

	/// <summary> 
	///		���캯��
	/// </summary>
	XDBLatchBlock( )
	{
		m_hEvent = ::CreateEvent( NULL,FALSE,FALSE,NULL );
	}
	/// <summary> 
	///		��������
	/// </summary>
	~XDBLatchBlock( )
	{
		CloseHandle( m_hEvent );
	}
public:
	LATCH_MODE WaitLatchMode;  // �ȴ�����ģʽ
	XDBLink	   WaitLink;
	DWORD      ThreadId;
private:
	HANDLE  m_hEvent;
	
};

/// <summary> 
/// �������б�
/// </summary>
class XDBLatchBlockList
{
public:
	/// <summary> 
	/// ���һ��������
	/// </summary>
	XDBLatchBlock* pop( )
	{
		XDBAutoSpinLock AutoSpinLock( m_spinLock );
		return m_freeList.pop( );
	}
	/// <summary> 
	/// ѹ��һ��������
	/// </summary>
	void push( XDBLatchBlock* LatchBlock )
	{
		XDBAutoSpinLock AutoSpinLock( m_spinLock );
		LatchBlock->sleep( );
		m_freeList.append( LatchBlock );
	}

	/// <summary> 
	/// ���캯��
	/// </summary>
	XDBLatchBlockList( int size )
		: m_destory( false )
		, m_freeList( LINK_LIST_ARG( XDBLatchBlock , WaitLink ) ) 
	{
		for( int i=0;i<size;++i )
		{
			XDBLatchBlock* LatchBlock = new XDBLatchBlock;
			m_freeList.append( LatchBlock );
		}
	}
	/// <summary> 
	/// ��������
	/// </summary>
	~XDBLatchBlockList( )
	{
		XDBAutoSpinLock AutoSpinLock( m_spinLock );
		XDBLatchBlock* LatchBlock = NULL;
		while( ( LatchBlock = m_freeList.pop( ) )!= NULL )  delete LatchBlock;
		m_destory = true;
	}
private:
	XDBSpinLock					m_spinLock;			// SpinLock of latch self
	XDBLinkList<XDBLatchBlock>  m_freeList;         // free list
	bool                        m_destory;
};


/// <summary> 
/// 
///     �����ֲ�ͬ�ķ�ʽ��ʵ��������һ���Ǹ���ÿ���������ߵ��������
///	��һ��ֻ���ٶ�ռ�����ߵ������ǰ�ߵ��ŵ����ܹ���һ�����ĳ���������
/// ����ģʽ�����㷨�����ӣ�ռ���ڴ���ࣻ���ߵ��ŵ����㷨���򵥣�����
///	�������ĳ������Ը��ߵ�ģʽ�����������������򽫵�������������Ϊ�ڶ�
///	��ʵ���㷨��
/// 
/// </summary> 

/// <summary> 
/// ������
/// </summary> 
class KTN_COMMON_CLASSEXT XDBLatch
{
public:

	/// <summary> 
	/// ���캯����
	/// </summary>
	XDBLatch( );

	/// <summary> 
	/// ����������
	/// </summary> 
	~XDBLatch( );

	/// <summary> 
	///		������������
	/// </summary> 
	/// <param name="Mode">
	///		������������ģʽ��
	/// </param>
	/// <param name="TimeOut">
	///		��ʱ���ã��Ϸ���ֵΪWAIT_FOREVER��WAIT_IMMEDIATE��
	/// </param>
	/// <returns>
	///		�ɹ�ʱ����scSuccess��ʧ��ʱ������Ӧ�Ĵ����롣
	/// </returns>
	HRESULT acquire( 
		LATCH_MODE		Mode, 
		DWORD			TimeOut = WAIT_FOREVER );

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
	HRESULT upgrade( 
		DWORD		TimeOut = WAIT_FOREVER );


	/// <summary> 
	/// �ͷ�������
	/// </summary> 
	void release( );

	/// <summary> 
	/// ����ģʽ��
	/// </summary> 
	LATCH_MODE mode( );

	/// <summary> 
	/// �ǿ���״̬��
	/// </summary> 
	bool isFree( );

	/// <summary> 
	/// ����Ƿ����˳���������
	/// </summary> 
	bool isLatched( );

	/// <summary> 
	///		���ص�ǰ��˨������������
	/// </summary> 
	/// <remarks>
	///     ���ڵ��������
	/// </remarks>
	inline LONG ownerCount( )
	{
		return m_counter;
	}

	/// <summary> 
	///		���ص�ǰ�ȴ��ߵ���Ŀ��
	/// </summary> 
	///     ���ڵ��������
	/// </remarks>
	inline LONG waiterCount( )
	{
		return ( LONG )m_waiterList.size( );
	}

//========================================================= 
// ���ٹ�������������
//========================================================= 
#ifdef TRACE_LATCH_OWNER
//========================================================= 
	inline void setLineNo( LONG LineNo )
	{
		for( int _index = 0; _index < TRACE_LATCH_OWNER_MAX; _index++ )
		{
			if( m_holderOwner[ _index ] == ::GetCurrentThreadId( ) )
			{
				m_holderLine[ _index ] = LineNo;
				break;
			}
		}
	}
//========================================================= 
#endif // TRACE_LATCH_OWNER
//========================================================= 

protected:
	DWORD							m_exclusiveHolder;	// Exclusive Holder
	XDBSpinLock						m_spinLock;			// SpinLock of latch self
	LONG							m_mode		: 4;	// Current XDBLatch Mode
	LONG							m_destroyed	: 4;	// XDBLatch is destroyed?
	LONG							m_counter	: 24;	// XDBLatch Counter
	XDBLinkList<XDBLatchBlock>		m_waiterList;		// Waiter List

	static XDBLatchBlockList		m_freeList;         // free list

//========================================================= 
// ���ٹ�������������
//========================================================= 
#ifdef TRACE_LATCH_OWNER
	DWORD							m_holderOwner[ TRACE_LATCH_OWNER_MAX ];
	int								m_holderCount[ TRACE_LATCH_OWNER_MAX ];
	DWORD							m_holderLine[ TRACE_LATCH_OWNER_MAX ];
#endif 

private:
	// disable copy constructor
	XDBLatch( const XDBLatch& );
	XDBLatch& operator=( const XDBLatch& );
};



/// <summary> 
/// �Զ����������
/// </summary> 
class  KTN_COMMON_CLASSEXT XDBAutoLatch
{
public:
	/// <summary> 
	/// ���캯�����Զ����������
	/// </summary> 
	XDBAutoLatch( XDBLatch& LatchObj,LATCH_MODE Mode = LATCH_EX )
		:m_latch( LatchObj )
	{
		HRESULT rc = m_latch.acquire( Mode, WAIT_FOREVER );
		ASSERT( rc == KTN_OK );
	}

	/// <summary> 
	/// �����������Զ��ͷ�������
	/// </summary> 
	~XDBAutoLatch( )
	{
		m_latch.release( );
	}

private:
	XDBLatch&	m_latch;   // ��������

	// disable copy constructor
	XDBAutoLatch( const XDBAutoLatch& );
	XDBAutoLatch& operator=( const XDBAutoLatch& );

};

//==============================================================================
// 
//	������
// 
//==============================================================================
#ifdef  TRACE_LATCH_OWNER
#define TRACE_LATCH_LINE( x )		( x ).setLineNo( __LINE__ )
#else
#define TRACE_LATCH_LINE( x )	
#endif 

#define ACQUIRE_S_LATCH_NOWAIT( x )		( ( x ).acquire( LATCH_SH,WAIT_IMMEDIATE ) )	
#define ACQUIRE_X_LATCH_NOWAIT( x )		( ( x ).acquire( LATCH_EX,WAIT_IMMEDIATE ) )
#define ACQUIRE_S_LATCH( x )			( ( x ).acquire( LATCH_SH,WAIT_FOREVER ) )
#define ACQUIRE_X_LATCH( x )			( ( x ).acquire( LATCH_EX,WAIT_FOREVER ) )
#define RELEASE_S_LATCH( x )			( ( x ).release( ) )
#define RELEASE_X_LATCH( x )			( ( x ).release( ) )
#define RELEASE_LATCH( x )				( ( x ).release( ) )
#define ACQUIRE_LATCH( x,m )			( ( x ).acquire( ( m ),WAIT_FOREVER ) )
#define ACQUIRE_LATCH_NOWAIT( x,m )		( ( x ).acquire( ( m ),WAIT_IMMEDIATE ) )	
#define ACQUIRE_LATCH_TIMEOUT( x,m,t )	( ( x ).acquire( ( m ),( t ) ) )	
#define REFIX_LATCH( x,m )				( ( x ).refix( m ) )


}

//==============================================================================
#pragma pack( pop )
//==============================================================================
#endif // __XDBLATCH__H__INCLUDED__
//==============================================================================
