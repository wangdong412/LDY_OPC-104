//==============================================================================
//
// 项目名 ：工业实时数据库
// 文件名 ：XDBSpinLock.h
// 作  者 ：段利军( Jack )
// 用  途 ：自旋锁的实现。
// 
//==============================================================================
// 版本记录	
//==============================================================================
//
// V0.1	- 段利军 2005-3-4 21:10:56
//	
// 
//==============================================================================
#ifndef __XDBSPINLOCK__H__INCLUDED__
#define __XDBSPINLOCK__H__INCLUDED__
//==============================================================================

namespace NkxLatchLock {

/// <summary> 
/// 原子锁。
/// </summary> 
class XDBSpinLock
{
public:
	/// <summary> 
	/// 构造函数。
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
	/// 获得自旋锁。
	/// </summary> 
	bool lock( )
	{
		if( 	0 == InterlockedCompareExchange( &m_spin, 1, 0 ) )
		{
	#ifdef TRACE_LOCK_OWNER
			m_ownerThreadId = ( LONG )GetCurrentThreadId( ) ;
	#endif 
			return true; // 获得自旋锁
		}


		// 单CPU？
		if( m_numberOfCPU == 1 )
		{
			while( 0 != InterlockedCompareExchange( &m_spin, 1, 0 ) )
			{
				// 使用SwitchToThread代替Sleep( 0 )，使得低优先级
				// 的线程能够得到调用
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
		else // 多CPU,SPIN
		{
			LONG counter = 0;
			while( 0 != InterlockedCompareExchange( &m_spin, 1, 0 ) )
			{
				for( counter = 0; counter < DEFAULT_SPIN_COUNT; counter++ )
				{
					if( 	0 == InterlockedCompareExchange( &m_spin, 1, 0 ) )
						return true; // 获得自旋锁
				}

				// 使用SwitchToThread代替Sleep( 0 )，使得低优先级
				// 的线程能够得到调用
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
		} // 多CPU
	}

	/// <summary> 
	/// 尝试加锁。
	/// </summary>
	/// <remark>
	///     如果返回true，则必须调用unlock释放锁。
	/// </remark>
	bool tryLock( )
	{
		if( 	0 == InterlockedCompareExchange( &m_spin, 1, 0 ) )
		{
		#ifdef TRACE_LOCK_OWNER
			m_ownerThreadId = ( LONG )GetCurrentThreadId( ) ;
		#endif 
			return true; // 获得自旋锁
		}
		else
			return false;
	}

	/// <summary> 
	/// 释放自旋锁。
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
	/// 是否为单CPU计算机。
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
	/// 获得持有自旋锁的线程ID。
	/// </summary> 
	LONG ownerThreadId( ) const
	{
		return m_ownerThreadId;
	}

	/// <summary> 
	/// 设置调用者行号，用于跟踪死锁。
	/// </summary> 
	void setLineNo( LONG LineNo )
	{
		m_lineNo = LineNo;
	}

//==============================================================================
#endif 
//==============================================================================


private:
	volatile LONG m_spin;			// 自旋变量

	// 注：此值在XDBPublic.cpp中初始化
	static   LONG m_numberOfCPU;	// CPU个数

	// 用于调试跟踪信息
#ifdef TRACE_LOCK_OWNER 
	LONG	m_lineNo;
	LONG	m_ownerThreadId;
#endif 

	// disable copy constructor
	XDBSpinLock( const XDBSpinLock& );
	XDBSpinLock& operator=( const XDBSpinLock& );

};

/// <summary> 
/// 自动获得自旋锁。
/// </summary> 
class XDBAutoSpinLock
{
public:
	/// <summary> 
	/// 构造函数。
	/// </summary>
	/// <remark>
	///     自动获得自旋锁。
	/// </remark>
	XDBAutoSpinLock( XDBSpinLock& LockObj )
		:m_spinLock( LockObj  )
	{
		m_spinLock.lock( );
	}

	/// <summary> 
	/// 析构函数。
	/// </summary>
	/// <remark>
	///     自动释放自旋锁。
	/// </remark>
	~XDBAutoSpinLock( )
	{
		m_spinLock.unlock( );
	}

private:
	XDBSpinLock& m_spinLock;	// 自旋锁对象

	// disable copy constructor
	XDBAutoSpinLock( const XDBAutoSpinLock& );
	XDBAutoSpinLock& operator=( const XDBAutoSpinLock& );
};

//========================================================= 
// 辅助宏
//========================================================= 
#define ACQUIRE_SPINLOCK( x )	( ( x ).lock( ) )
#define RELEASE_SPINLOCK( x )	( ( x ).unlock( ) )
#define TRY_SPINLOCK( x )			( ( x ).tryLock( ) )



}

#endif // __XDBSPINLOCK__H__INCLUDED__
//==============================================================================
