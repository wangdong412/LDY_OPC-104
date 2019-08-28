//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	104Trans
///		文件名: HW104Subscription.cpp
///		作  者: 王勃
///	</file>
///	<versions>
///		<version>
///			V0.10 : 王勃 [11/11/2009] 创建文件
///		</version>
///	</versions>
//========================================================================
#include "StdAfx.h"
#include "HW104Subscription.h"
#include "psapi.h"

unsigned int _stdcall ThreadCallBack( void* p )
{

	CHW104Subscription* pSubscription = ( CHW104Subscription* )p;

	pSubscription->ThreadMoniterChange();

	return 1;
}

unsigned int CHW104Subscription::m_nMaxMemory = DEFAULT_MEMORYBUFFER;

CHW104Subscription::CHW104Subscription(void):m_bThreadControl( false ),  //线程控制
											 m_hCallBackThread( NULL ),	 //线程句柄
											 m_unThreadID(0)
{
	std::wstring wstrIniPath;
	CHW104Help::GetConfigIniPath( wstrIniPath );

	m_nMaxMemory =GetPrivateProfileInt( SECTIONNAME,					//Section name
		MEMORYBUFFER,				//key name
		DEFAULT_MEMORYBUFFER,		//memory
		wstrIniPath.c_str() );	

	if ( m_nMaxMemory<50 || m_nMaxMemory > 300 )
	{
		m_nMaxMemory = DEFAULT_MEMORYBUFFER;
	}
}

CHW104Subscription::~CHW104Subscription(void)
{
	
}

/// <summary>
/// 	启动变化上送
/// </summary>
bool CHW104Subscription::Start()
{
	if ( NULL == m_hCallBackThread )
	{
		//线程启动标志位
		m_bThreadControl = true;

		m_hCallBackThread = (HANDLE)_beginthreadex( NULL,				//security description
													0,					//default stack size
													ThreadCallBack,
													this,
													0,					// create running
													&m_unThreadID
													);

		if ( NULL == m_hCallBackThread )
		{
			m_bThreadControl = false;
			return false;
		}
	}

	return true;
}

/// <summary>
/// 	停止变化上送
/// </summary>
void CHW104Subscription::End()
{
	//1. 停止线程
	if ( true == m_bThreadControl )
	{
		m_bThreadControl = false;

		DWORD dwRet = WaitForSingleObject( m_hCallBackThread, 3000 );

		if ( WAIT_TIMEOUT == dwRet )
		{
			TerminateThread( m_hCallBackThread, 0 );
		}
	}

	//2. 释放内存
	for ( std::list< std::vector< KTNHW104_InfoUnit >* >::iterator it = m_ChangeList.begin();
		  it != m_ChangeList.end();
		  ++it  )
	{
		delete *it;
	}
}

/// <summary>
/// 	增加变化接口
/// </summary>
/// <remarks>
///     添加成功返回true
///     添加失败返回false,失败是因为链表已经到达最大负载了
/// </remarks>
bool CHW104Subscription::AddCallBackInterface( IHW104CollectorCallBack* pHW104CallBack )
{
	AutoLock auLock( &m_CallBackListLock );

	if ( m_CallBackList.size() == MAX_CALLBACK_NUM )
	{//超过最大回调数量
		return false;
	}
	else
	{
		m_CallBackList.push_back( pHW104CallBack );
	}
	
	return true;
}

/// <summary>
/// 	增加变化队列
/// </summary>
void CHW104Subscription::OnChange( 
			  const std::vector< KTNHW104_InfoUnit >& changeUnitArray )
{
	AutoLock auLock( &m_ChangeListLock );

	std::vector< KTNHW104_InfoUnit >* changeUnitArrayTemp = new  std::vector< KTNHW104_InfoUnit >;
	changeUnitArrayTemp->reserve( changeUnitArray.size() );
	changeUnitArrayTemp->assign( changeUnitArray.begin(), changeUnitArray.end() );

	//拷贝到变化链表
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo( GetCurrentProcess(), &pmc, sizeof(pmc));

	if ( pmc.WorkingSetSize >= m_nMaxMemory*1024*1024 )
	{//内存超过这个上限,丢弃这个数据
		if( !m_ChangeList.empty() ){
			std::vector< KTNHW104_InfoUnit >* front = m_ChangeList.front();
			delete front;
			m_ChangeList.pop_front();	
		}
	}
	
	m_ChangeList.push_back( changeUnitArrayTemp );
}

/// <summary>
/// 	变化回调处理线程
/// </summary>	
void CHW104Subscription::ThreadMoniterChange( )
{
	TRACE( "ThreadMoniterChange  Start: ThreadID %d\n" , m_unThreadID );

	std::list< std::vector< KTNHW104_InfoUnit >* > changeListTemp;

	while ( m_bThreadControl )
	{
		{//1. 拷贝所有的变化数据
			AutoLock auLock( &m_ChangeListLock );
		
			changeListTemp.assign( m_ChangeList.begin(), m_ChangeList.end() );

			m_ChangeList.clear();			
		}
		
		if( changeListTemp.empty() == true )
		{
			Sleep(5);
			continue;
		}

		//2.  依次变化上送所有的变化数据
		for ( std::list< std::vector< KTNHW104_InfoUnit >* >::iterator it = changeListTemp.begin();
			  it != changeListTemp.end();
			  ++it  )
		{
			AutoLock auLock2( &m_CallBackListLock );
	
			for ( std::list< IHW104CollectorCallBack* >::iterator it2 = m_CallBackList.begin();
				  it2 != m_CallBackList.end();
				  ++it2  )
			{//轮询变化上送链表
				(*it2)->OnInfoUnitChange( *(*it) );
			}

			//变化上送结束后,就删除vector
			delete (*it);
		}
	}

	TRACE( "ThreadMoniterChange  Stop: ThreadID %d" , m_unThreadID );
}