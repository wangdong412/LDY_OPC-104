 //========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	104Trans
///		文件名: HW104_TCPServer.cpp
///		作  者: shuqiang.luan
///	</file>
//========================================================================
#include "stdafx.h"
#include "HW104_TCPServer.h"
#include "HW104TransHelp.h"

//用于日志输出
#define TCPSERVERNAME   L"CHW104_TCPServer"

//全局日志对象
extern CHW104_Log global_HW104Log;

CListenSocketData::CListenSocketData()
{	
	m_hListenSocket = INVALID_SOCKET;
	m_wsaEvent = NULL;
	m_hListenThread = NULL;
	m_bQuitListenThread = TRUE;	
}

CListenSocketData::~CListenSocketData()
{	
}

//监听线程
unsigned int __stdcall ListenThreadSocket(void *lpParameter)
{
	CHW104_TCPServer *pHW104_TCPServer = (CHW104_TCPServer*)lpParameter;
	pHW104_TCPServer->ListenSocket();
	return 0;
}

//管理完成端口的线程
unsigned int __stdcall ThreadIoCompletionPort(void *lpParameter)
{
	CHW104_TCPServer *pHW104_TCPServer = (CHW104_TCPServer*)lpParameter;
	pHW104_TCPServer->MonitorIoCompletionPort();
	return 0;
}

CHW104_TCPServer::CHW104_TCPServer()
{
	m_listenPort = 2404;
	m_dwTimeOutForListenSocket = 2000;

	m_hCompletionPort = NULL;
	m_hThreadMonitorIoCompletionPort = NULL;
	m_bQuitThreadMonitorIoCompletionPort = TRUE;
		
	m_IPNum = 6;

	m_strLog = _T("");
}

CHW104_TCPServer::~CHW104_TCPServer()
{
	ServerEnd();	
	//释放map中指针指向的内存
}

//socket初始化函数
BOOL SocketInit(WSADATA *lpwsaData)
{
	WSADATA wsaData;
	if (lpwsaData == NULL)
		lpwsaData = &wsaData;    
	WORD wVersionRequested = MAKEWORD(2, 2);
	int nResult = WSAStartup(wVersionRequested, lpwsaData);
	if (nResult != 0)
	{
		//未找到可用的WinSock DLL
		return FALSE;
	}
	//确认找到的WinSock DLL支持版本2.2
	if ((LOBYTE(lpwsaData->wVersion) != 2)||(HIBYTE(lpwsaData->wVersion) != 2))
	{
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}

BOOL CHW104_TCPServer::Initial()
{
	//配置文件路径
	std::string strIniPath;
	CHW104TransHelp::GetIniPath( strIniPath );
	strIniPath += CONFIGFILENAME;
	
	//获得客户端个数
	m_IPNum = GetPrivateProfileIntA( 
						IPLISTSECTION,
						IPNUMKEY,
						6,
						strIniPath.c_str()
						);
	if ( m_IPNum <= 0 )
	{
		HW104TransLog( TCPSERVERNAME, L"\r\n\tInitial(), IPNum<=0 is wrong!" );
		::MessageBox(NULL, _T("IPNum<=0 is wrong in the ini file, please correct it!"), _T("Attention"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}	

	//获得MaxFrameLen
	BYTE maxFrameLen =  GetPrivateProfileIntA( 
						HW104SECTION,
						"MaxFrameLen",
						255,
						strIniPath.c_str()
						);
	if (maxFrameLen < 27)
	{
		HW104TransLog(L"CHW104_TCPServer", L"\r\n\tInitial(), maxFrameLen<27 is wrong!");
		// 长度超限则采用默认长度 [12/4/2009 pks]
		maxFrameLen = 255;
	}	

	//获得本地IP
	char szLocalIP[50] = {0};
	GetPrivateProfileStringA( 
		HW104SECTION,
		"localIP",
		"127.0.0.1",
		szLocalIP,
		50,
		strIniPath.c_str()
		);

	// 配置
	for ( int i = 1; i<=m_IPNum; ++i )
	{
		char szSection[ 10 ] = {0};
		// 修改缓冲区太小的bug [12/4/2009 pks]
		sprintf_s( szSection , ARRAYLEN(szSection) , "IP%d", i  );

		//获取IP
		char szIP[50] = {0};
		GetPrivateProfileStringA( 
			szSection,
			IPADDRESSKEY,
			"127.0.0.1",
			szIP,
			50,
			strIniPath.c_str()
			);

		//获取slave类型
		char szType[50] = {0};
		GetPrivateProfileStringA( 
					szSection,
					CLIENTTYPEKEY,
					"KTN_UNKOWN",
					szType,
					50,
					strIniPath.c_str()
					);

		KTNCollectorTpye collectorType = KTNCollector_Unknown;

		//先把所有的字母都转换成为大写字母
		_strupr( szType );

		if ( strcmp( "KTN_TELVENT" , szType ) == 0 )
		{
			collectorType = KTN_Telvent;
		}
		else if ( strcmp( "KTN_CEGELEC", szType ) == 0 )
		{
			collectorType = KTN_Cegelec;
		}
		else if ( strcmp( "KTN_HONEYWELL", szType ) == 0 )
		{
			collectorType = KTN_HoneyWell;//add by 栾述强，2010-3-11，为honeywell的Master
		}
		else
		{
			continue;
		}
		
		//读取CA
		int nCA = GetPrivateProfileIntA( 
						szSection,
						"CA",
						0,
						strIniPath.c_str()
						);
		if ( nCA <= 0 || nCA > 65535 )// 增加CA上限 [12/4/2009 pks]
		{
			HW104TransLog(L"CHW104_TCPServer", L"\r\n\tInitial(), CA<=0 is wrong!");		
			return FALSE;
		}

		CHW104_Slave *pHW104_Slave_temp = new CHW104_Slave;		
		pHW104_Slave_temp->m_clientIp.assign( szIP );
		pHW104_Slave_temp->m_localIP.assign( szLocalIP );
		//将IP转为宽字符，用于日志输出
		MultiByteToWideChar(CP_ACP,
							0, 
							(LPCSTR)(pHW104_Slave_temp->m_clientIp.c_str()),
							pHW104_Slave_temp->m_clientIp.size(),
							(LPWSTR)(pHW104_Slave_temp->m_pWcharIP),
							LENGTH_IP);
		pHW104_Slave_temp->m_clientType = (KTNCollectorTpye)collectorType;			
		pHW104_Slave_temp->m_maxFrameLen = maxFrameLen;	
		pHW104_Slave_temp->m_CA = nCA;

		m_mapHW104_Slave.insert( std::make_pair( pHW104_Slave_temp->m_clientIp, pHW104_Slave_temp) );
	}

	return TRUE;
}

BOOL CHW104_TCPServer::ServerStart()
{
	HW104TransLog( TCPSERVERNAME, L"\r\n\tTCPServer Start!!!" );

	//加载socket库
	if (!SocketInit(NULL))
	{		
		return FALSE;		
	}	

	if (!Initial())
	{		
		return FALSE;		
	}	
	
	//启动完成端口---------------------------------------------------------------------
	//创建完成端口
	// Get the number of processors on the system.
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	// Create the I/O completion port. 
	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,
		                                       NULL,
											   NULL,
											   6);//线程数
	if (m_hCompletionPort == NULL)
	{
		return FALSE;
	}
	//创建工作线程，完成端口的线程池
	m_bQuitThreadMonitorIoCompletionPort = FALSE;
	for (int i=0; i<6; i++)
	{
		unsigned int dwThreadID_tem;
		HANDLE hThread;
		if(( hThread =  (HANDLE)_beginthreadex(NULL,
											  0, 
											  ThreadIoCompletionPort,
											  this,
											  CREATE_SUSPENDED,
											  &dwThreadID_tem) ) == NULL)
		{
			return FALSE;
		}
		ResumeThread(hThread);
		CloseHandle( hThread );
	}	
	//----------------------------------------------------------------------------------------
	
	//启动线程监听2404端口--------------------------------------------------------------------
	//创建监听socket
	m_listenSocketData.m_hListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
	if(m_listenSocketData.m_hListenSocket == INVALID_SOCKET)
	{	
		return FALSE;	
	}	
	//创建监听事件
	m_listenSocketData.m_wsaEvent = WSACreateEvent();	
	if(m_listenSocketData.m_wsaEvent == WSA_INVALID_EVENT)	
	{
		return FALSE;
	}	
	//关联监听socket和监听事件	
	WSAEventSelect(m_listenSocketData.m_hListenSocket,
		           m_listenSocketData.m_wsaEvent,
				   FD_ACCEPT);
	//创建监听线程，用于监听客户端连接请求
	m_listenSocketData.m_bQuitListenThread = FALSE;
	unsigned int dwListenThreadID;
	m_listenSocketData.m_hListenThread = (HANDLE)_beginthreadex(NULL,
		                                                        0,
																ListenThreadSocket,
																this,
																CREATE_SUSPENDED,
																&dwListenThreadID);
	ResumeThread(m_listenSocketData.m_hListenThread);

	//将监听socket和监听端口绑定
	SOCKADDR_IN  SockAddr_Server;   //服务器端socket地址      
	SockAddr_Server.sin_port = htons(m_listenPort);
	SockAddr_Server.sin_family = AF_INET;
	SockAddr_Server.sin_addr.s_addr = INADDR_ANY;
	if (bind(m_listenSocketData.m_hListenSocket,
		(LPSOCKADDR)&SockAddr_Server,
		sizeof(SockAddr_Server)) == SOCKET_ERROR)	
	{	
		int ret = GetLastError();
		HW104TransLog( TCPSERVERNAME, L"2404 port bind failed!" );
		return FALSE;	
	}
	//开始监听端口                                
	if(listen(m_listenSocketData.m_hListenSocket, 16) == SOCKET_ERROR)
	{
		HW104TransLog( TCPSERVERNAME, L"2404 port listen failed!" );
		return FALSE;
	}		
	
	return TRUE;
}

void CHW104_TCPServer::ListenSocket()
{
	SOCKET *    pListenSocket;
	WSAEVENT*	pwsaEvent;
	BOOL*   	pQuitThread;
	pListenSocket = &m_listenSocketData.m_hListenSocket;
	pwsaEvent    = &m_listenSocketData.m_wsaEvent;
	pQuitThread  = &m_listenSocketData.m_bQuitListenThread;

	DWORD	dwWaitCode;
	WSANETWORKEVENTS	wsaNetWorkEvents;

	while(TRUE)		
	{
		//线程退出
		if(*pQuitThread) 
		{			
			return;
		}

		dwWaitCode = WSAWaitForMultipleEvents(1,
			                                  pwsaEvent,
											  FALSE,
											  m_dwTimeOutForListenSocket,
											  FALSE);
		if(dwWaitCode == WSA_WAIT_TIMEOUT)
		{
			continue;
		}
		if(dwWaitCode != WAIT_OBJECT_0)
		{
			//
			continue;
		}
		if(dwWaitCode == WSA_WAIT_FAILED)
		{
			//
			continue;
		}		
		
		if(SOCKET_ERROR == WSAEnumNetworkEvents(*pListenSocket,
			                                    *pwsaEvent,
												&wsaNetWorkEvents))
		{			
			continue;  
		}
		else
		{
			if(wsaNetWorkEvents.lNetworkEvents & FD_ACCEPT)
			{
				OnClientAccept(NULL, NULL);
			}
		}
	}
}

extern IHW104CollectorControl* g_phwCollectorControl;

LRESULT CHW104_TCPServer::OnClientAccept(WPARAM wParam, LPARAM lParam)
{	
	//接受Client的连接请求
	int nLength = sizeof(SOCKADDR);
	SOCKADDR_IN  SocketAddr_Client;
	SOCKET socket_temp = INVALID_SOCKET;
	socket_temp = accept(m_listenSocketData.m_hListenSocket,
		                 (LPSOCKADDR)&SocketAddr_Client,
		                 (LPINT)&nLength);
	if (socket_temp == INVALID_SOCKET)
	{		
		return 0L;		
	}
	//检查IP地址是否存在于IP列表中
	std::string strIP_temp;  
	strIP_temp.assign( inet_ntoa(SocketAddr_Client.sin_addr) );
	//用于日志输出
	WCHAR pWcharIP[LENGTH_IP] = {0};
	MultiByteToWideChar( CP_ACP,
						 0,
						 (LPCSTR)(strIP_temp.c_str()),
						 strIP_temp.size(),
						 (LPWSTR)pWcharIP,
						 LENGTH_IP
						 );
	
	CString strLog;
	strLog.Format( L"\r\n\tAccept IP:%s connection is OK, Checking permission!", pWcharIP );
	HW104TransLog( TCPSERVERNAME, (LPCWSTR)strLog );

	std::map< std::string , CHW104_Slave*>::iterator iterator_temp;
	iterator_temp = m_mapHW104_Slave.find(strIP_temp);
	if (m_mapHW104_Slave.end() == iterator_temp)
	{
		//在IP列表中未找到该IP，踢掉连接
		CString strLog;
		strLog.Format( L"\r\n\tIP:%s connection is refused!", pWcharIP );
		HW104TransLog( TCPSERVERNAME, (LPCWSTR)strLog );
        closesocket(socket_temp);
		return 0L;
	}	
	//判断该IP是否已有连接在正常运转，如果有，则拒绝新的连接
	if ( iterator_temp->second->isInitial() )
	{
		//在IP列表中未找到该IP，踢掉连接
		CString strLog;
		strLog.Format( L"\r\n\tIP:%s has one normal connection, the new connection is refused!", pWcharIP );
		HW104TransLog( TCPSERVERNAME, (LPCWSTR)strLog );
		closesocket(socket_temp);
		return 0L;
	}	

	//IP合法，将新接受的连接socket关联到完成端口
	HANDLE handle_temp = CreateIoCompletionPort(
		(HANDLE)(socket_temp),
		m_hCompletionPort,
		(ULONG_PTR)(&(iterator_temp->second->m_clientIp)),
		0);	                                           
	if(m_hCompletionPort != handle_temp)
	{		
		closesocket(socket_temp);
		socket_temp = INVALID_SOCKET;
		return 0L;
	}

	//保存客户端socket数据
	//初始化slave类的APCI, socket, callback, Collector等对象成员, 同时开启超时检查线程, 打开日志文件
	BOOL bRet = iterator_temp->second->Initial( socket_temp );
	if ( FALSE == bRet )
	{
		iterator_temp->second->Clear();
		return FALSE;
	}

	{
		CString strLog;
		strLog.Format( L"\r\n\tIP:%s connect successfully! ==============", pWcharIP );
		HW104TransLog( TCPSERVERNAME, (LPCWSTR)strLog );
		TRACE( "IP:%s connect\n", strIP_temp.c_str() );
	}


	//...........................	
	//注意m_overlapped重新初始化
	memset(&(iterator_temp->second->m_overlapped), 0, sizeof(OVERLAPPED));
	ReadFile((HANDLE)(socket_temp),
		     iterator_temp->second->m_byRecvBuf,
			 BUF_MAX_SIZE,
			 NULL,
			 &(iterator_temp->second->m_overlapped));

	return 0L;
}

void CHW104_TCPServer::OnClientClose(std::string strIP)
{	
	TRACE( "IP:%s OnClientClose!\n", strIP.c_str() );

	std::map<std::string, CHW104_Slave*>::iterator iterator_temp;
	iterator_temp = m_mapHW104_Slave.find(strIP);
	if (m_mapHW104_Slave.end() == iterator_temp)
	{
		return;
	}

	iterator_temp->second->Clear();
	//用于日志输出
	WCHAR pWcharIP[LENGTH_IP] = {0};
	MultiByteToWideChar( CP_ACP,
						 0,
						 (LPCSTR)(strIP.c_str()),
						 strIP.size(),
						 (LPWSTR)pWcharIP,
						 LENGTH_IP
						 );
	CString strLog;
	strLog.Format( L"\r\n\tIP:%s OnClientClose! ==============", pWcharIP );
	HW104TransLog( TCPSERVERNAME, (LPCWSTR)strLog );
}

void CHW104_TCPServer::MonitorIoCompletionPort()
{
	BOOL   bSuccess;
	DWORD  dwNumBytes;
	ULONG_PTR dwKey;
	LPOVERLAPPED completedOverlapped;

	while(true)
	{
		if(m_bQuitThreadMonitorIoCompletionPort)
		{			
			return;
		}
		
		dwKey = 0;
		bSuccess = GetQueuedCompletionStatus(m_hCompletionPort,
			                                 &dwNumBytes,
											 &dwKey,
											 &completedOverlapped,
											 m_dwTimeOutForListenSocket);
		if (!bSuccess && (completedOverlapped == NULL))
		{
			continue;
		}

		if (dwKey == 0)
		{
			continue;
		}

		std::string strIP_temp = *((std::string*)dwKey);
		std::map<std::string, CHW104_Slave*>::iterator iterator_temp;
		iterator_temp = m_mapHW104_Slave.find(strIP_temp);
		if (m_mapHW104_Slave.end() == iterator_temp)
		{
			//查找失败
			continue;
		}

		if(dwNumBytes > 0)
		{
			//处理数据
			//整个帧允许的最大长度为255，若超出，当做无效帧，直接丢掉,同时关掉socket
			if (dwNumBytes > iterator_temp->second->m_maxFrameLen )
			{
				TRACE( "dwNumBytes>m_maxFrameLen!\n" );
				WCHAR pWcharIP[LENGTH_IP] = {0};
				MultiByteToWideChar( CP_ACP,
									 0,
									 (LPCSTR)(strIP_temp.c_str()),
									 strIP_temp.size(),
									 (LPWSTR)pWcharIP,
									 LENGTH_IP
									 );
				CString strLog;
				strLog.Format( L"\r\n\tClient(IP:%s) dwNumBytes>m_maxFrameLen in MonitorIoCompletionPort!", pWcharIP );
				HW104TransLog( TCPSERVERNAME, (LPCWSTR)strLog );
				OnClientClose(strIP_temp);								
				continue;
			}
			else
			{
				BYTE recvBuf[BUF_MAX_SIZE] = {0};				
				memcpy(recvBuf, iterator_temp->second->m_byRecvBuf, dwNumBytes);

				iterator_temp->second->OnRecv(recvBuf, (BYTE)dwNumBytes);
     			//返回检查
			}				
		}
		else //关闭请求
		{
			//关闭socket
			OnClientClose(strIP_temp);
		}
	}
}

BOOL CHW104_TCPServer::ServerEnd()
{	
	//1、关闭完成端口
	//应该先关完成端口，否则下面shutdown关联的Socket时，完成端口会响应并执行OnClientClose
	//这样会两次closesocket
	m_bQuitThreadMonitorIoCompletionPort = TRUE;

	//监听完成端口的工作线程没有保留句柄，m_hThreadMonitorIoCompletionPort无效
	if(m_hThreadMonitorIoCompletionPort != NULL)
	{		
		//首先结束线程
		m_bQuitThreadMonitorIoCompletionPort = TRUE;		
		WaitForSingleObject(m_hThreadMonitorIoCompletionPort, INFINITE);
		CloseHandle(m_hThreadMonitorIoCompletionPort);
		m_hThreadMonitorIoCompletionPort = NULL;

		CloseHandle(m_hCompletionPort);
		m_hCompletionPort = NULL;
	}
	HW104TransLog( L"Quit", L"IOCompletePort quit!" );
	//2、关闭服务监听socket
	if(m_listenSocketData.m_hListenThread != NULL)
	{		
		//退出监听线程
		m_listenSocketData.m_bQuitListenThread = TRUE;
		WSASetEvent(m_listenSocketData.m_wsaEvent);
		WaitForSingleObject(m_listenSocketData.m_hListenThread, INFINITE);
		WSACloseEvent(m_listenSocketData.m_wsaEvent);
		m_listenSocketData.m_wsaEvent = NULL;
		CloseHandle(m_listenSocketData.m_hListenThread);
		m_listenSocketData.m_hListenThread = NULL;
		//关闭socket
		if (m_listenSocketData.m_hListenSocket != INVALID_SOCKET)
		{			
			shutdown(m_listenSocketData.m_hListenSocket, SD_BOTH);
			closesocket(m_listenSocketData.m_hListenSocket);
			m_listenSocketData.m_hListenSocket = INVALID_SOCKET;
		}
	}
	HW104TransLog( L"Quit", L"ListenSocket quit!" );
	//3、关闭连接的各客户socket---------------------------------------
	std::map<std::string, CHW104_Slave*>::iterator iterator_temp;
	for(iterator_temp = m_mapHW104_Slave.begin(); iterator_temp != m_mapHW104_Slave.end(); iterator_temp++)
	{		
		iterator_temp->second->End();
	}
	HW104TransLog( L"Quit", L"All client quit!" );
	
	return TRUE;
}