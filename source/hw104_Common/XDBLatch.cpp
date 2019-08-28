//==============================================================================
//
// 项目名 ：工业实时数据库
// 文件名 ：XDBLatch.cpp
// 作  者 ：段利军( Jack )
// 用  途 ：闩锁的实现。
// 
//==============================================================================
// 版本记录	
//==============================================================================
//
// V0.1	- 段利军 2005-4-25 10:43:25
//	
// 
//==============================================================================
#include "stdafx.h"
#include "hw104_Common/hw104Common_Ext_inc.h"
//==============================================================================


//========================================================= 
//  调试宏
//========================================================= 
#define __TRACE_LATCH__


#ifdef  __TRACE_LATCH__

// 闩锁模式的字符串描述
static const WCHAR* _xdbLatchModeDesc[  ] =
{
	WSTR( "NL" ),WSTR( "SH" ),WSTR( "EX" ),
};

// 跟踪闩锁调用
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
///		请求闩锁。
/// </summary> 
/// <param name="Mode">
///		请求闩锁的模式。
/// </param>
/// <param name="TimeOut">
///		请求超时设置。
/// </param>
/// <returns>
///		成功时返回scSuccess，失败时返回相应的错误码。
/// </returns>
HRESULT XDBLatch::acquire( 
	LATCH_MODE	Mode, 
	DWORD		TimeOut /* = WAIT_FOREVER  */ )
{
	////========================================================= 
	//// 跟踪LATCH调用
	//TRACE_LATCH_CALL( Mode );
	////========================================================= 

	// 得到当前线程ID
	DWORD dwThreadId = ::GetCurrentThreadId( );

	KXASSERT( ( Mode == LATCH_SH ) || ( Mode == LATCH_EX ) );

	// 获得自旋锁
	ACQUIRE_SPINLOCK( m_spinLock );
	if( m_destroyed )
	{
		// 不要忘记释放自旋锁
		RELEASE_SPINLOCK( m_spinLock );
		return KXSTATUS_FAILED;
	}

	// 首先检查是否被人持有闩锁
	HRESULT ret = KXSTATUS_SUCCESS;
	if( m_counter == 0 )
	{
		// 如果没有持有该闩，则立刻成功获得闩锁
		m_exclusiveHolder = ( Mode == LATCH_EX )? dwThreadId : NULL;
		m_mode = Mode;
		m_counter++;
	}
	else if( ( m_mode == LATCH_EX ) && ( m_exclusiveHolder == dwThreadId ) )
	{
		// 如果自己已经持有独占锁，则后续的锁请求都自动转换为独占锁
		m_counter++;
	}
	else if( ( m_mode == LATCH_SH ) && ( Mode == LATCH_SH ) && m_waiterList.empty( ) )
	{
		// 如果闩锁当前模式为共享锁，而请求也为共享锁，并且等待队列为空，则请求被授予；
		m_counter++;
	}
	else // 其他情况，或需要等待，或返回失败
	{
		if( TimeOut == WAIT_IMMEDIATE ) // NO WAIT
		{
			ret = KXSTATUS_FAILED;
		}
		else // 将等待
		{
			// 获得一个等待闩块并加入到等待队列，并且设置等待模式。
			XDBLatchBlock* LatchBlock = m_freeList.pop( );
			if( LatchBlock == NULL )
			{
				LatchBlock = new XDBLatchBlock;
			}
			LatchBlock->ThreadId = dwThreadId;
			LatchBlock->WaitLatchMode = Mode;

			m_waiterList.append( LatchBlock );
			

			// 在进入等待状态之前先释放自旋锁。
			RELEASE_SPINLOCK( m_spinLock );

			// 超时等待锁
			ret = LatchBlock->wait( TimeOut );

			// 重新获得锁
			ACQUIRE_SPINLOCK( m_spinLock );

			// 如果已经获得闩锁,等待闩块应该已经从等待队列断开
			LatchBlock->WaitLink.detach( );

			if( ret != KXSTATUS_SUCCESS )
			{
				// 重新检查是否获得闩锁
				ret = LatchBlock->wait( WAIT_IMMEDIATE );
			}

			// 释放等待的闩块
			m_freeList.push( LatchBlock );

			// 检查等待结果
            if( KXSTATUS_SUCCESS == ret ) // OK
			{
				// 再次检查latch的状态
				if( m_destroyed )
				{
					RELEASE_SPINLOCK( m_spinLock );
					return KXSTATUS_FAILED;
				}

				// 检查是否闩锁是否被真正授予
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

	// 校验加栓锁成功后，m_counter肯定大于1
	KXASSERT( !( ( ret == KXSTATUS_SUCCESS ) && m_counter == 0 ) );


//========================================================= 
// 跟踪共享闩锁持有者
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

	// 释放自旋锁 
	RELEASE_SPINLOCK( m_spinLock );

	// 返回结果
	return ret;
}

/// <summary> 
///	释放闩锁。	
/// </summary> 
void XDBLatch::release( )
{
	////========================================================= 
	//// 跟踪LATCH调用
	//TRACE_LATCH_CALL( 0 );
	////========================================================= 

	// 获得自旋锁
	ACQUIRE_SPINLOCK( m_spinLock );
	if( m_destroyed )
	{
		// 不要忘记释放自旋锁
		RELEASE_SPINLOCK( m_spinLock );
		return;
	}
	// 减小引用计数
	m_counter--;
	KXASSERT( m_counter >= 0 );
	if( 0 == m_counter ) // 没有闩锁持有者
	{
		m_exclusiveHolder = NULL;
		m_mode			  = LATCH_NL;
	}

//========================================================= 
// 跟踪共享闩锁持有者
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



	// 检查等待队列
	if( ( 0 == m_counter ) && !m_waiterList.empty( ) )
	{
		XDBLinkListIterator<XDBLatchBlock> Iterator( m_waiterList );
		XDBLatchBlock* waiter = Iterator.next( );	// 注意这儿不能误用m_waiterList.pop( )
		if( waiter->WaitLatchMode == LATCH_EX )
		{
			// 从等待列表中断开
			waiter->WaitLink.detach( );
			// 获得闩锁
			m_mode = LATCH_EX;
			m_counter++;
			m_exclusiveHolder = waiter->ThreadId;
			// 唤醒等待线程
			waiter->wakeup( );
		} // LATCH_EX
		else
		{
			// 激活请求共享锁的所有线程
			while( ( waiter != NULL ) && ( waiter->WaitLatchMode == LATCH_SH ) )
			{
				waiter->WaitLink.detach( );
				// 获得闩锁
				m_mode = LATCH_SH;
				m_counter++;
				// 唤醒等待线程
				waiter->wakeup( );
				waiter = Iterator.next( );
			}
		}// LATCH_SH
	} // m_counter == 0 

	// 释放自旋锁
	RELEASE_SPINLOCK( m_spinLock );
}


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
HRESULT XDBLatch::upgrade( 
	DWORD		TimeOut /*= WAIT_FOREVER*/ )
{
	////========================================================= 
	//// 跟踪LATCH调用
	//TRACE_LATCH_CALL( LATCH_EX );
	////========================================================= 

	DWORD dwThreadId = ::GetCurrentThreadId( );

	//========================================================= 
	// 检查闩锁状态，看是否能够立即升级
	//========================================================= 
	{
		XDBAutoSpinLock AutoLock( m_spinLock );
		if( m_destroyed ) 
			return KXSTATUS_FAILED;
	
		// 检查当前模式( 必须已经持有共享模式的闩锁 )
		if( ( m_counter <= 0 ) ||( m_mode != LATCH_SH ) )
			return KXSTATUS_FAILED;
		
		// 检查能否立即升级成功
		if( ( m_counter == 1 ) && ( m_waiterList.empty( ) ) )
		{
			//========================================================= 
			// 
			// 注意：由于XDBLatch的实现并没有跟踪闩锁的共享持有者，因此
			//		 必须调用者自己保证自身持有共享模式的闩锁，否则将导
			//		 致无法预料的后果。
			//
			//========================================================= 

			// 升级闩锁并不增加引用计数
			m_mode				= LATCH_EX;
			m_exclusiveHolder	= dwThreadId;
			return KXSTATUS_SUCCESS;
		}

		// 不能立即提升闩锁，检查超时设置
		if( TimeOut == WAIT_IMMEDIATE )
			return KXSTATUS_FAILED;

	} // 自动释放自旋锁

	// 不能立即提升闩锁，先释放自身持有的闩锁，然后重新获得锁
	// 释放闩锁
	release( );
	
	// 然后重新请求获得闩锁
	return acquire( LATCH_EX,TimeOut ) ;
}

/// <summary> 
///	析构函数。	
/// </summary> 
XDBLatch::~XDBLatch( )
{
	// 获得自旋锁
	ACQUIRE_SPINLOCK( m_spinLock );

	// 设置删除闩锁标志
	m_destroyed = true;

	// 唤醒所有的等待线程，让它们自己从队列中删除
	XDBLinkListIterator<XDBLatchBlock> Iterator( m_waiterList );
	XDBLatchBlock* waiter = Iterator.next( );
	while( waiter != NULL )
	{
		waiter->wakeup( );
		waiter = Iterator.next( );
	}

	// 等待线程从等待队列中自己删除
	while( !m_waiterList.empty( ) )
	{
		// 需要先释放锁
		RELEASE_SPINLOCK( m_spinLock );

		// 线程切换
		if( !SwitchToThread( ) )
			Sleep( 1 );

		// 再次获得锁
		ACQUIRE_SPINLOCK( m_spinLock );
	}

	// 设置独占成员
	m_exclusiveHolder = NULL;

	// 释放锁
	RELEASE_SPINLOCK( m_spinLock );
}

/// <summary> 
/// 构造函数。
/// </summary> 
XDBLatch::XDBLatch( ):
	m_destroyed( false ),
	m_exclusiveHolder( NULL ),
	m_counter( 0 ),
	m_mode( LATCH_NL ),
	m_waiterList( LINK_LIST_ARG( XDBLatchBlock , WaitLink ) ) 
{
//========================================================= 
// 跟踪共享闩锁持有者
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
/// 是空闲状态。
/// </summary> 
bool XDBLatch::isFree( )
{
	XDBAutoSpinLock AutoLock( m_spinLock );
	return ( ( m_counter == 0  ) && ( m_waiterList.empty( ) ) );
}


/// <summary> 
/// 检测是否有人持有闩锁。
/// </summary> 
bool XDBLatch::isLatched( )
{	
	XDBAutoSpinLock AutoLock( m_spinLock );
	return ( m_counter > 0 );
}


/// <summary> 
/// 闩锁模式。
/// </summary> 
LATCH_MODE XDBLatch::mode( )
{
	XDBAutoSpinLock AutoLock( m_spinLock );
	return ( LATCH_MODE )m_mode;
}
