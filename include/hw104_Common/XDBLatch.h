//==============================================================================
//
// 项目名 ：工业实时数据库
// 文件名 ：XDBLatch.h
// 作  者 ：段利军( Jack )
// 用  途 ：闩锁的实现。
// 
//==============================================================================
// 版本记录	
//==============================================================================
//
// V0.1	- 段利军 2005-4-25 10:37:58
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

#define 	WAIT_IMMEDIATE	( DWORD )0			// 不等待
#define 	WAIT_FOREVER	( DWORD )INFINITE 	// 无限等待

// 跟踪共享持有者
//#define TRACE_LATCH_OWNER
#define TRACE_LATCH_OWNER_MAX		8
#define LATCHBLOCK_FREE_SIZE       256     // 闩锁块空闲大小

/// <summary> 
/// 闩锁模式。
/// </summary>
enum LATCH_MODE
{
	LATCH_NL = 0,		// 不锁定
	LATCH_SH = 1,		// 共享
	LATCH_EX = 2,		// 独占
};

/// <summary> 
/// 闩锁模式。
/// </summary>
class XDBLatchBlock
{
public:
	/// <summary> 
	///		等待获得锁
	/// </summary>
	HRESULT wait( DWORD TimeOut = WAIT_FOREVER )
	{
		DWORD rc = ::WaitForSingleObject( m_hEvent,TimeOut );
		if( rc == WAIT_OBJECT_0 )
			return KTN_OK;

		return WAIT_TIMEOUT == rc ? KTN_OK : KTN_FAILED;
	}
	/// <summary> 
	///		唤醒等待
	/// </summary>
	BOOL wakeup( )
	{
		return ::SetEvent( m_hEvent );
	}

	/// <summary> 
	///		唤醒等待
	/// </summary>
	BOOL sleep( )
	{
		return ::ResetEvent( m_hEvent );
	}

	/// <summary> 
	///		构造函数
	/// </summary>
	XDBLatchBlock( )
	{
		m_hEvent = ::CreateEvent( NULL,FALSE,FALSE,NULL );
	}
	/// <summary> 
	///		析构函数
	/// </summary>
	~XDBLatchBlock( )
	{
		CloseHandle( m_hEvent );
	}
public:
	LATCH_MODE WaitLatchMode;  // 等待锁的模式
	XDBLink	   WaitLink;
	DWORD      ThreadId;
private:
	HANDLE  m_hEvent;
	
};

/// <summary> 
/// 闩锁块列表
/// </summary>
class XDBLatchBlockList
{
public:
	/// <summary> 
	/// 获得一个闩锁块
	/// </summary>
	XDBLatchBlock* pop( )
	{
		XDBAutoSpinLock AutoSpinLock( m_spinLock );
		return m_freeList.pop( );
	}
	/// <summary> 
	/// 压入一个闩锁块
	/// </summary>
	void push( XDBLatchBlock* LatchBlock )
	{
		XDBAutoSpinLock AutoSpinLock( m_spinLock );
		LatchBlock->sleep( );
		m_freeList.append( LatchBlock );
	}

	/// <summary> 
	/// 构造函数
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
	/// 析构函数
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
///     有两种不同的方式来实现闩锁，一种是跟踪每个锁持有者的情况，而
///	另一种只跟踪独占持有者的情况。前者的优点是能够让一个锁的持有者升级
/// 锁的模式，但算法更复杂，占用内存更多；后者的优点是算法更简单，但不
///	允许锁的持有者以更高的模式重新申请闩锁，否则将导致死锁；以下为第二
///	种实现算法。
/// 
/// </summary> 

/// <summary> 
/// 闩锁。
/// </summary> 
class KTN_COMMON_CLASSEXT XDBLatch
{
public:

	/// <summary> 
	/// 构造函数。
	/// </summary>
	XDBLatch( );

	/// <summary> 
	/// 析构函数。
	/// </summary> 
	~XDBLatch( );

	/// <summary> 
	///		请求获得闩锁。
	/// </summary> 
	/// <param name="Mode">
	///		请求获得闩锁的模式。
	/// </param>
	/// <param name="TimeOut">
	///		超时设置，合法的值为WAIT_FOREVER和WAIT_IMMEDIATE。
	/// </param>
	/// <returns>
	///		成功时返回scSuccess，失败时返回相应的错误码。
	/// </returns>
	HRESULT acquire( 
		LATCH_MODE		Mode, 
		DWORD			TimeOut = WAIT_FOREVER );

	/// <summary> 
	///		升级闩锁到独占模式。
	/// </summary> 
	/// <param name="TimeOut">
	///		超时设置。
	/// </param>
	/// <returns>
	///		成功时返回scSuccess，失败时返回相应的错误码。
	/// </returns>
	/// <remarks>
	///     必须由调用者保证自身已经持有共享锁( 且仅持有一次，否则将导致死锁 )。
	///     因为如果共享锁被持有多次（ m_counter > 1 ）,无法判定共享锁是否全部
	///     被一个线程持有
	/// </remarks>
	HRESULT upgrade( 
		DWORD		TimeOut = WAIT_FOREVER );


	/// <summary> 
	/// 释放闩锁。
	/// </summary> 
	void release( );

	/// <summary> 
	/// 闩锁模式。
	/// </summary> 
	LATCH_MODE mode( );

	/// <summary> 
	/// 是空闲状态。
	/// </summary> 
	bool isFree( );

	/// <summary> 
	/// 检测是否有人持有闩锁。
	/// </summary> 
	bool isLatched( );

	/// <summary> 
	///		返回当前的栓锁持有数量。
	/// </summary> 
	/// <remarks>
	///     用于调试输出。
	/// </remarks>
	inline LONG ownerCount( )
	{
		return m_counter;
	}

	/// <summary> 
	///		返回当前等待者的数目。
	/// </summary> 
	///     用于调试输出。
	/// </remarks>
	inline LONG waiterCount( )
	{
		return ( LONG )m_waiterList.size( );
	}

//========================================================= 
// 跟踪共享闩锁持有者
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
// 跟踪共享闩锁持有者
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
/// 自动获得闩锁。
/// </summary> 
class  KTN_COMMON_CLASSEXT XDBAutoLatch
{
public:
	/// <summary> 
	/// 构造函数，自动获得闩锁。
	/// </summary> 
	XDBAutoLatch( XDBLatch& LatchObj,LATCH_MODE Mode = LATCH_EX )
		:m_latch( LatchObj )
	{
		HRESULT rc = m_latch.acquire( Mode, WAIT_FOREVER );
		ASSERT( rc == KTN_OK );
	}

	/// <summary> 
	/// 析构函数，自动释放闩锁。
	/// </summary> 
	~XDBAutoLatch( )
	{
		m_latch.release( );
	}

private:
	XDBLatch&	m_latch;   // 闩锁对象

	// disable copy constructor
	XDBAutoLatch( const XDBAutoLatch& );
	XDBAutoLatch& operator=( const XDBAutoLatch& );

};

//==============================================================================
// 
//	辅助宏
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
