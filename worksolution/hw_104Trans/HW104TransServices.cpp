//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	104Trans
///		�ļ���: HW104TransServices.cpp
///	</file>
///	<versions>
///		<version>
///			V0.10 :  [11/27/2009] �����ļ�
///		</version>
///	</versions>
//========================================================================
#include "StdAfx.h"
#include "HW104TransServices.h"
#include "hw104_PKSCollector/HW104Collector_ExtInc.h"
#include "HW104_TCPServer.h"
#include "MyMD5.h"
#include "HW104TransHelp.h"
#include "DataType.h"

#define MAX_ERROR_LEN 256
//�������к�У��
#define KEYNAME L"SOFTWARE\\Honeywell\\KVDate\\XYZ" 
#define KVALUENAME L"KValue"
#define VVALUENAME L"VValue"
#define LICENSEFILENAME "license.dat"
#define LLICENSEFILENAME L"license.dat"

#define SECTNAME L"LICENSE"
#define KEYISREG L"HasRegister"
#define KEYREGCODE L"RegisterCode"
#define KEYLICENSE L"LicenseCode"

//ȫ�ַ�����
CHW104TransServices g_HW104Services;

/// <summary>
/// 	�ɼ�������
/// </summary>
IHW104CollectorControl* g_phwCollectorControl = NULL;

/// <summary>
/// 	�ɼ����೧
/// </summary>
IHW104CollectorFactory* g_phw104Factory = NULL;

/// <summary>
///   tcpServer��
/// </summary>
CHW104_TCPServer g_HW104TCPServer;

extern CHW104_Log global_HW104Log;

//��ȡLicense��Ϣ
static bool GetLicense(CString& strRegCode, CString& strLicense);
//У��License
static bool IsLicense(const CString& strRegCode, const CString& strLicense);
//����Ƿ�����Ȩ
static bool PassAuthor();


/// <summary>
///   �����߳�
/// </summary>
unsigned int _stdcall WorkThread( void* p )
{
	// TODO: code your application's behavior here.

	CHW104TransServices* pHW104TransServices = ( CHW104TransServices* )p;

	g_phw104Factory = ktn_GetHW104CollectorFactory();

	g_phw104Factory->CreateIHW104CollectorControl( g_phwCollectorControl );

	//�����ɼ�
	KTN_RET ret = g_phwCollectorControl->StartCollect();

	if ( KTNERR( ret ) )
	{
		wchar_t wstrError[ MAX_ERROR_LEN ] = {0};
		swprintf_s( wstrError, MAX_ERROR_LEN, L"StartCollect Failed errorID:%d", ret );
		g_HW104Services.ReportEvent( wstrError );
		pHW104TransServices->Stop();
		return 0;
	}

	//����TCPServer
	BOOL bRet = g_HW104TCPServer.ServerStart();

	if ( FALSE == bRet )
	{
		g_phwCollectorControl->EndCollect();

		wchar_t wstrError[ MAX_ERROR_LEN ] = {0};
		swprintf_s( wstrError, MAX_ERROR_LEN, L"ServerStart Failed errorID:%d,winError:%d", ret, ::GetLastError() );
		g_HW104Services.ReportEvent( wstrError );
		pHW104TransServices->Stop();
		return 0;
	}

	return 1;
}

/// <summary>
///   ��������
/// </summary>
#define HW104TRANSERVICES L"Experion PKS 104SlaveComm"							//������ʾ����
#define HW104TRANSDES	  L"Experion PKS 60870-5-104 Slave Communication"		//��������

CHW104TransServices::CHW104TransServices(void):NTService( HW104TRANSERVICES ),		//���������
											   m_hStopEvent( NULL ),				//ֹͣ�¼�
											   m_hWorkThread( NULL ),				//�߳̾��
											   m_dwWorkThreadID( 0 )				//�߳�ID
{
}

CHW104TransServices::~CHW104TransServices(void)
{
}

/// <summary>
///   ��װ��������
///   ����������Ϣ
/// </summary>
/// <remarks>
///   ����޸�������Ϣ
///   �����������޸�
/// </remarks>
BOOL CHW104TransServices::InstallService()
{
	BOOL bRet = NTService::InstallService();

	if ( bRet )
	{	
		SC_HANDLE schManager =  OpenSCManager(			
			NULL,						// local computer
			NULL,						// ServicesActive database 
			SC_MANAGER_ALL_ACCESS	);  // full access rights 


		if ( NULL == schManager )
		{	
			wchar_t wstrError[ MAX_ERROR_LEN ] = {0};
			swprintf_s( wstrError, MAX_ERROR_LEN, L" %s ErrorID: %d ", L" OpenSCManager error when setup HW104SlaveComm ", ::GetLastError() );
			ReportEvent( wstrError );
			return FALSE;
		}

		SC_HANDLE schService = OpenService(
			schManager,						//SCM database
			m_lpServiceName,				//name of services
			SERVICE_CHANGE_CONFIG	);		//need change config access

		if ( NULL == schManager )
		{
			wchar_t wstrError[ MAX_ERROR_LEN ] = {0};
			swprintf_s( wstrError, MAX_ERROR_LEN, L" %s ErrorID: %d ", L" OpenService error when setup HW104SlaveComm ", ::GetLastError() );
			ReportEvent( wstrError );
			CloseHandle( schManager );
			return FALSE;
		}

		SERVICE_DESCRIPTION sd;
		sd.lpDescription = HW104TRANSDES;

		if ( !ChangeServiceConfig2(
			schService,						//handle to services
			SERVICE_CONFIG_DESCRIPTION,	
			&sd	) )
		{
			wchar_t wstrError[ MAX_ERROR_LEN ] = {0};
			swprintf_s( wstrError, MAX_ERROR_LEN, L" %s ErrorID: %d ", L" HW104SlaveComm::ChangeServiceConfig2 failed ", ::GetLastError() );
			ReportEvent( wstrError );
		}

		CloseServiceHandle( schService );
		CloseServiceHandle( schManager );

		return TRUE;
	}

	return FALSE;
}

/// <summary>
/// 	���صĻ����е�Run
/// </summary>
/// <param name="argc">
///		����������
/// </param>
/// <param name="argv">
///		������
/// </param>
/// <example>
/// 	<code>
///		void CMyService::Run(DWORD argc, LPTSTR *argv) 
//		{
//			ReportStatus(SERVICE_START_PENDING);
//			do some parameter processing ...
//			ReportStatus(SERVICE_START_PENDING);
//			do first part of initialisation ...
//			ReportStatus(SERVICE_START_PENDING);
//			do next part of initialisation
//			 // ...
//			 m_hStop = CreateEvent(0, TRUE, FALSE, 0);
//			ReportStatus(SERVICE_RUNNING);
//			while(WaitForSingleObject(m_hStop, 10) != WAIT_OBJECT_0) 
//			{
//				 // do something
//			}
//			if (m_hStop)
//				 CloseHandle(m_hStop);
//		}
/// 	</code>
/// </example>
void CHW104TransServices::Run(DWORD argc, LPTSTR *argv)
{	
	ReportStatus(SERVICE_START_PENDING);

	wchar_t wstrError[ MAX_ERROR_LEN ] = {0};
	/************************************************************************/
	/* work thread                                                                     
	/************************************************************************/
	m_hWorkThread = (HANDLE)_beginthreadex( NULL,	
		0,
		WorkThread,			// work thread
		this,				    // arg
		0,						// create running
		&m_dwWorkThreadID);		// thread id

	if ( NULL == m_hWorkThread )
	{
		ReportEvent( L"CHW104TransServices::_beginthreadex failed" );
		return;
	}

	/************************************************************************/
	/* stop event                                                                     
	/************************************************************************/
	m_hStopEvent = CreateEvent( NULL,		// default security attribute
								TRUE,       // manual-reset
								FALSE,		// the initial state  is nonsignaled
								NULL);

	if ( NULL == m_hStopEvent )
	{
		ReportEvent( L"Create StopEvent failed" );
		return;
	}

	ReportStatus( SERVICE_RUNNING );
                            
	/************************************************************************/
	/* 1.ֹͣTCPServer                                                                     
	/* 2.ֹͣ�ɼ�
	/************************************************************************/
	unsigned int nCount = 0;
	while( WAIT_OBJECT_0 != WaitForSingleObject( m_hStopEvent , 1000 ) )
	{
		//if ( (++nCount) == 2*60 )//2����
		//{
		//	////������Ȩ���
		//	//if ( !PassAuthor() )
		//	//{
		//	//	HW104TransLog( L"CHW104TransServices", L"\r\n\tNo valid License! Please contact with Honeywell to get License!" );
		//	//	HW104TransLog( L"CHW104TransServices", L"\r\n\t**********104Slave End**********" );	
		//	//	Stop();				
		//	//}
		//}		
	}
	//ֹͣTCPServer
	g_HW104TCPServer.ServerEnd();      
	HW104TransLog( L"Quit", L"EndServer" );
	//ֹͣ�ɼ�
	g_phwCollectorControl->EndCollect();
	HW104TransLog( L"Quit", L"EndCollect" );

	if ( m_hStopEvent )
	{//�ͷ��¼����
		CloseHandle( m_hStopEvent );
		m_hStopEvent = NULL;
	}

	if ( m_hWorkThread )
	{//�ͷ��߳̾��
		CloseHandle( m_hWorkThread );
		m_hWorkThread = NULL;
	}

	ReportStatus( SERVICE_STOP );
}

/// <summary>
/// 	���صĻ����е�Stop
/// </summary>
/// <example>
/// 	<code>
//		void CMyService::Stop() 
//		{
//			if(m_hStop)
//				SetEvent(m_hStop);
//			ReportStatus(SERVICE_STOP_PENDING);
//		}
/// 	</code>
/// </example>
void CHW104TransServices::Stop()
{
	if ( m_hStopEvent )
	{
		SetEvent( m_hStopEvent );
	}
	ReportStatus(SERVICE_STOP_PENDING);
}


//��ȡLicense��Ϣ
static bool GetLicense(CString& strRegCode, CString& strLicense)
{
	std::wstring wstrLicensePath;
	CHW104TransHelp::GetIniPath(wstrLicensePath);
	wstrLicensePath += LLICENSEFILENAME;
	//�ļ�������,����false
	if ( !CHW104TransHelp::IsFileExist(wstrLicensePath.c_str()) )
	{
		return false;
	}

	int nIsRegister = GetPrivateProfileIntW( SECTNAME,
		KEYISREG,
		0,
		wstrLicensePath.c_str() );
	if ( nIsRegister != 1 )
	{
		return false;
	}
	
	//ע����
	char chBuf[32] = {0};
	wchar_t wchBuf[32] = {0};
	HKEY hKeyTemp = NULL;
	BOOL bRet = FALSE;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, KEYNAME, 0, KEY_READ, &hKeyTemp) == ERROR_SUCCESS)
	{
		if ( hKeyTemp != NULL )
		{					
			DWORD dwSize = sizeof(DWORD);
			DWORD dwVal = 0;				
			if (ERROR_SUCCESS == RegQueryValueEx( hKeyTemp, KVALUENAME,
													NULL, NULL,
													LPBYTE(&dwVal),	&dwSize ))
			{
				if (dwVal == 1)
				{
					dwSize = 52;
					if (ERROR_SUCCESS == RegQueryValueEx( hKeyTemp, VVALUENAME,
															NULL, NULL,
															LPBYTE(wchBuf),	&dwSize ))
					{	
#ifdef _UNICODE
						strRegCode.Format( L"%s", wchBuf);
#else
						WideCharToMultiByte( CP_ACP,
							0,
							wchBuf,
							wcslen(wchBuf),
							chBuf,
							32,
							NULL,
							NULL );
						strRegCode.Format( "%s", chBuf );
#endif
						bRet = TRUE;
					}
				}
			}
		}
	}
	if ( !bRet )
	{
		return false;
	}
		
	//License
	GetPrivateProfileStringW( SECTNAME,
		KEYLICENSE,
		L"",
		wchBuf,
		32,
		wstrLicensePath.c_str() );
#ifdef _UNICODE
	strLicense.Format( L"%s", wchBuf );
#else
	WideCharToMultiByte( CP_ACP,
		0,
		wchBuf,
		wcslen(wchBuf),
		chBuf,
		32,
		NULL,
		NULL );
	strLicense.Format("%s", chBuf);
#endif

	return true;
}

//У��License
static bool IsLicense(const CString& strRegCode, const CString& strLicense)
{
	//�ȼ����License
	CString strSrc = strRegCode;
	strSrc += _T("LSQMESSAGE-DIGEST0105101");

	char srcBuf[48] = {0};
#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP,
		0,
		strSrc,
		strSrc.GetLength(),
		srcBuf,
		48,
		NULL,
		NULL);
#else
	memcpy(srcBuf, strSrc.GetBuffer(), strSrc.GetLength());
	strSrc.ReleaseBuffer();
#endif

	unsigned char digestBuf[16] = {0};
	CMyMD5::GetMD5Code(digestBuf, (unsigned char*)srcBuf, 48);

	CString strDigest = _T("");
	//36���Ʊ���
	DWORD dwVal;
	for(int i=0; i<4; i++)
	{
		dwVal = *( (DWORD*)(digestBuf + i*4) );
		strDigest += CHW104TransHelp::Code36_DWORD(dwVal);
	}
	//������License�ıȽ�
	if ( strLicense.Compare(strDigest) != 0 )
	{
		return false;
	}

	return true;
}

//����Ƿ�����Ȩ
static bool PassAuthor()
{
	CString strRegCode;
	CString strLicense;

	if ( !GetLicense(strRegCode, strLicense) )
	{
		return false;
	}

	if ( !IsLicense(strRegCode, strLicense) )
	{
		return false;
	}

	return true;
}