//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	104Trans
///		�ļ���: HW104Subscription.cpp
///		��  ��: ����
///	</file>
///	<versions>
///		<version>
///			V0.10 : ���� [11/11/2009] �����ļ�
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

CHW104Subscription::CHW104Subscription(void):m_bThreadControl( false ),  //�߳̿���
											 m_hCallBackThread( NULL ),	 //�߳̾��
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
/// 	�����仯����
/// </summary>
bool CHW104Subscription::Start()
{
	if ( NULL == m_hCallBackThread )
	{
		//�߳�������־λ
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
/// 	ֹͣ�仯����
/// </summary>
void CHW104Subscription::End()
{
	//1. ֹͣ�߳�
	if ( true == m_bThreadControl )
	{
		m_bThreadControl = false;

		DWORD dwRet = WaitForSingleObject( m_hCallBackThread, 3000 );

		if ( WAIT_TIMEOUT == dwRet )
		{
			TerminateThread( m_hCallBackThread, 0 );
		}
	}

	//2. �ͷ��ڴ�
	for ( std::list< std::vector< KTNHW104_InfoUnit >* >::iterator it = m_ChangeList.begin();
		  it != m_ChangeList.end();
		  ++it  )
	{
		delete *it;
	}
}

/// <summary>
/// 	���ӱ仯�ӿ�
/// </summary>
/// <remarks>
///     ��ӳɹ�����true
///     ���ʧ�ܷ���false,ʧ������Ϊ�����Ѿ������������
/// </remarks>
bool CHW104Subscription::AddCallBackInterface( IHW104CollectorCallBack* pHW104CallBack )
{
	AutoLock auLock( &m_CallBackListLock );

	if ( m_CallBackList.size() == MAX_CALLBACK_NUM )
	{//�������ص�����
		return false;
	}
	else
	{
		m_CallBackList.push_back( pHW104CallBack );
	}
	
	return true;
}

/// <summary>
/// 	���ӱ仯����
/// </summary>
void CHW104Subscription::OnChange( 
			  const std::vector< KTNHW104_InfoUnit >& changeUnitArray )
{
	AutoLock auLock( &m_ChangeListLock );

	std::vector< KTNHW104_InfoUnit >* changeUnitArrayTemp = new  std::vector< KTNHW104_InfoUnit >;
	changeUnitArrayTemp->reserve( changeUnitArray.size() );
	changeUnitArrayTemp->assign( changeUnitArray.begin(), changeUnitArray.end() );

	//�������仯����
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo( GetCurrentProcess(), &pmc, sizeof(pmc));

	if ( pmc.WorkingSetSize >= m_nMaxMemory*1024*1024 )
	{//�ڴ泬���������,�����������
		if( !m_ChangeList.empty() ){
			std::vector< KTNHW104_InfoUnit >* front = m_ChangeList.front();
			delete front;
			m_ChangeList.pop_front();	
		}
	}
	
	m_ChangeList.push_back( changeUnitArrayTemp );
}

/// <summary>
/// 	�仯�ص������߳�
/// </summary>	
void CHW104Subscription::ThreadMoniterChange( )
{
	TRACE( "ThreadMoniterChange  Start: ThreadID %d\n" , m_unThreadID );

	std::list< std::vector< KTNHW104_InfoUnit >* > changeListTemp;

	while ( m_bThreadControl )
	{
		{//1. �������еı仯����
			AutoLock auLock( &m_ChangeListLock );
		
			changeListTemp.assign( m_ChangeList.begin(), m_ChangeList.end() );

			m_ChangeList.clear();			
		}
		
		if( changeListTemp.empty() == true )
		{
			Sleep(5);
			continue;
		}

		//2.  ���α仯�������еı仯����
		for ( std::list< std::vector< KTNHW104_InfoUnit >* >::iterator it = changeListTemp.begin();
			  it != changeListTemp.end();
			  ++it  )
		{
			AutoLock auLock2( &m_CallBackListLock );
	
			for ( std::list< IHW104CollectorCallBack* >::iterator it2 = m_CallBackList.begin();
				  it2 != m_CallBackList.end();
				  ++it2  )
			{//��ѯ�仯��������
				(*it2)->OnInfoUnitChange( *(*it) );
			}

			//�仯���ͽ�����,��ɾ��vector
			delete (*it);
		}
	}

	TRACE( "ThreadMoniterChange  Stop: ThreadID %d" , m_unThreadID );
}