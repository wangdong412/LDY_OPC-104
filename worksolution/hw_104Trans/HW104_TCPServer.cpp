 //========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	104Trans
///		�ļ���: HW104_TCPServer.cpp
///		��  ��: shuqiang.luan
///	</file>
//========================================================================
#include "stdafx.h"
#include "HW104_TCPServer.h"
#include "HW104TransHelp.h"

//������־���
#define TCPSERVERNAME   L"CHW104_TCPServer"

//ȫ����־����
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

//�����߳�
unsigned int __stdcall ListenThreadSocket(void *lpParameter)
{
	CHW104_TCPServer *pHW104_TCPServer = (CHW104_TCPServer*)lpParameter;
	pHW104_TCPServer->ListenSocket();
	return 0;
}

//������ɶ˿ڵ��߳�
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
	//�ͷ�map��ָ��ָ����ڴ�
}

//socket��ʼ������
BOOL SocketInit(WSADATA *lpwsaData)
{
	WSADATA wsaData;
	if (lpwsaData == NULL)
		lpwsaData = &wsaData;    
	WORD wVersionRequested = MAKEWORD(2, 2);
	int nResult = WSAStartup(wVersionRequested, lpwsaData);
	if (nResult != 0)
	{
		//δ�ҵ����õ�WinSock DLL
		return FALSE;
	}
	//ȷ���ҵ���WinSock DLL֧�ְ汾2.2
	if ((LOBYTE(lpwsaData->wVersion) != 2)||(HIBYTE(lpwsaData->wVersion) != 2))
	{
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}

BOOL CHW104_TCPServer::Initial()
{
	//�����ļ�·��
	std::string strIniPath;
	CHW104TransHelp::GetIniPath( strIniPath );
	strIniPath += CONFIGFILENAME;
	
	//��ÿͻ��˸���
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

	//���MaxFrameLen
	BYTE maxFrameLen =  GetPrivateProfileIntA( 
						HW104SECTION,
						"MaxFrameLen",
						255,
						strIniPath.c_str()
						);
	if (maxFrameLen < 27)
	{
		HW104TransLog(L"CHW104_TCPServer", L"\r\n\tInitial(), maxFrameLen<27 is wrong!");
		// ���ȳ��������Ĭ�ϳ��� [12/4/2009 pks]
		maxFrameLen = 255;
	}	

	//��ñ���IP
	char szLocalIP[50] = {0};
	GetPrivateProfileStringA( 
		HW104SECTION,
		"localIP",
		"127.0.0.1",
		szLocalIP,
		50,
		strIniPath.c_str()
		);

	// ����
	for ( int i = 1; i<=m_IPNum; ++i )
	{
		char szSection[ 10 ] = {0};
		// �޸Ļ�����̫С��bug [12/4/2009 pks]
		sprintf_s( szSection , ARRAYLEN(szSection) , "IP%d", i  );

		//��ȡIP
		char szIP[50] = {0};
		GetPrivateProfileStringA( 
			szSection,
			IPADDRESSKEY,
			"127.0.0.1",
			szIP,
			50,
			strIniPath.c_str()
			);

		//��ȡslave����
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

		//�Ȱ����е���ĸ��ת����Ϊ��д��ĸ
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
			collectorType = KTN_HoneyWell;//add by ����ǿ��2010-3-11��Ϊhoneywell��Master
		}
		else
		{
			continue;
		}
		
		//��ȡCA
		int nCA = GetPrivateProfileIntA( 
						szSection,
						"CA",
						0,
						strIniPath.c_str()
						);
		if ( nCA <= 0 || nCA > 65535 )// ����CA���� [12/4/2009 pks]
		{
			HW104TransLog(L"CHW104_TCPServer", L"\r\n\tInitial(), CA<=0 is wrong!");		
			return FALSE;
		}

		CHW104_Slave *pHW104_Slave_temp = new CHW104_Slave;		
		pHW104_Slave_temp->m_clientIp.assign( szIP );
		pHW104_Slave_temp->m_localIP.assign( szLocalIP );
		//��IPתΪ���ַ���������־���
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

	//����socket��
	if (!SocketInit(NULL))
	{		
		return FALSE;		
	}	

	if (!Initial())
	{		
		return FALSE;		
	}	
	
	//������ɶ˿�---------------------------------------------------------------------
	//������ɶ˿�
	// Get the number of processors on the system.
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	// Create the I/O completion port. 
	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,
		                                       NULL,
											   NULL,
											   6);//�߳���
	if (m_hCompletionPort == NULL)
	{
		return FALSE;
	}
	//���������̣߳���ɶ˿ڵ��̳߳�
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
	
	//�����̼߳���2404�˿�--------------------------------------------------------------------
	//��������socket
	m_listenSocketData.m_hListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
	if(m_listenSocketData.m_hListenSocket == INVALID_SOCKET)
	{	
		return FALSE;	
	}	
	//���������¼�
	m_listenSocketData.m_wsaEvent = WSACreateEvent();	
	if(m_listenSocketData.m_wsaEvent == WSA_INVALID_EVENT)	
	{
		return FALSE;
	}	
	//��������socket�ͼ����¼�	
	WSAEventSelect(m_listenSocketData.m_hListenSocket,
		           m_listenSocketData.m_wsaEvent,
				   FD_ACCEPT);
	//���������̣߳����ڼ����ͻ�����������
	m_listenSocketData.m_bQuitListenThread = FALSE;
	unsigned int dwListenThreadID;
	m_listenSocketData.m_hListenThread = (HANDLE)_beginthreadex(NULL,
		                                                        0,
																ListenThreadSocket,
																this,
																CREATE_SUSPENDED,
																&dwListenThreadID);
	ResumeThread(m_listenSocketData.m_hListenThread);

	//������socket�ͼ����˿ڰ�
	SOCKADDR_IN  SockAddr_Server;   //��������socket��ַ      
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
	//��ʼ�����˿�                                
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
		//�߳��˳�
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
	//����Client����������
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
	//���IP��ַ�Ƿ������IP�б���
	std::string strIP_temp;  
	strIP_temp.assign( inet_ntoa(SocketAddr_Client.sin_addr) );
	//������־���
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
		//��IP�б���δ�ҵ���IP���ߵ�����
		CString strLog;
		strLog.Format( L"\r\n\tIP:%s connection is refused!", pWcharIP );
		HW104TransLog( TCPSERVERNAME, (LPCWSTR)strLog );
        closesocket(socket_temp);
		return 0L;
	}	
	//�жϸ�IP�Ƿ�����������������ת������У���ܾ��µ�����
	if ( iterator_temp->second->isInitial() )
	{
		//��IP�б���δ�ҵ���IP���ߵ�����
		CString strLog;
		strLog.Format( L"\r\n\tIP:%s has one normal connection, the new connection is refused!", pWcharIP );
		HW104TransLog( TCPSERVERNAME, (LPCWSTR)strLog );
		closesocket(socket_temp);
		return 0L;
	}	

	//IP�Ϸ������½��ܵ�����socket��������ɶ˿�
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

	//����ͻ���socket����
	//��ʼ��slave���APCI, socket, callback, Collector�ȶ����Ա, ͬʱ������ʱ����߳�, ����־�ļ�
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
	//ע��m_overlapped���³�ʼ��
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
	//������־���
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
			//����ʧ��
			continue;
		}

		if(dwNumBytes > 0)
		{
			//��������
			//����֡�������󳤶�Ϊ255����������������Ч֡��ֱ�Ӷ���,ͬʱ�ص�socket
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
     			//���ؼ��
			}				
		}
		else //�ر�����
		{
			//�ر�socket
			OnClientClose(strIP_temp);
		}
	}
}

BOOL CHW104_TCPServer::ServerEnd()
{	
	//1���ر���ɶ˿�
	//Ӧ���ȹ���ɶ˿ڣ���������shutdown������Socketʱ����ɶ˿ڻ���Ӧ��ִ��OnClientClose
	//����������closesocket
	m_bQuitThreadMonitorIoCompletionPort = TRUE;

	//������ɶ˿ڵĹ����߳�û�б��������m_hThreadMonitorIoCompletionPort��Ч
	if(m_hThreadMonitorIoCompletionPort != NULL)
	{		
		//���Ƚ����߳�
		m_bQuitThreadMonitorIoCompletionPort = TRUE;		
		WaitForSingleObject(m_hThreadMonitorIoCompletionPort, INFINITE);
		CloseHandle(m_hThreadMonitorIoCompletionPort);
		m_hThreadMonitorIoCompletionPort = NULL;

		CloseHandle(m_hCompletionPort);
		m_hCompletionPort = NULL;
	}
	HW104TransLog( L"Quit", L"IOCompletePort quit!" );
	//2���رշ������socket
	if(m_listenSocketData.m_hListenThread != NULL)
	{		
		//�˳������߳�
		m_listenSocketData.m_bQuitListenThread = TRUE;
		WSASetEvent(m_listenSocketData.m_wsaEvent);
		WaitForSingleObject(m_listenSocketData.m_hListenThread, INFINITE);
		WSACloseEvent(m_listenSocketData.m_wsaEvent);
		m_listenSocketData.m_wsaEvent = NULL;
		CloseHandle(m_listenSocketData.m_hListenThread);
		m_listenSocketData.m_hListenThread = NULL;
		//�ر�socket
		if (m_listenSocketData.m_hListenSocket != INVALID_SOCKET)
		{			
			shutdown(m_listenSocketData.m_hListenSocket, SD_BOTH);
			closesocket(m_listenSocketData.m_hListenSocket);
			m_listenSocketData.m_hListenSocket = INVALID_SOCKET;
		}
	}
	HW104TransLog( L"Quit", L"ListenSocket quit!" );
	//3���ر����ӵĸ��ͻ�socket---------------------------------------
	std::map<std::string, CHW104_Slave*>::iterator iterator_temp;
	for(iterator_temp = m_mapHW104_Slave.begin(); iterator_temp != m_mapHW104_Slave.end(); iterator_temp++)
	{		
		iterator_temp->second->End();
	}
	HW104TransLog( L"Quit", L"All client quit!" );
	
	return TRUE;
}