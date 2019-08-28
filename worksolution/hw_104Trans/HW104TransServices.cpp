//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	104Trans
///		文件名: HW104TransServices.cpp
///	</file>
///	<versions>
///		<version>
///			V0.10 :  [11/27/2009] 创建文件
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
//用于序列号校验
#define KEYNAME L"SOFTWARE\\Honeywell\\KVDate\\XYZ" 
#define KVALUENAME L"KValue"
#define VVALUENAME L"VValue"
#define LICENSEFILENAME "license.dat"
#define LLICENSEFILENAME L"license.dat"

#define SECTNAME L"LICENSE"
#define KEYISREG L"HasRegister"
#define KEYREGCODE L"RegisterCode"
#define KEYLICENSE L"LicenseCode"

//全局服务句柄
CHW104TransServices g_HW104Services;

/// <summary>
/// 	采集控制类
/// </summary>
IHW104CollectorControl* g_phwCollectorControl = NULL;

/// <summary>
/// 	采集器类厂
/// </summary>
IHW104CollectorFactory* g_phw104Factory = NULL;

/// <summary>
///   tcpServer类
/// </summary>
CHW104_TCPServer g_HW104TCPServer;

extern CHW104_Log global_HW104Log;

//读取License信息
static bool GetLicense(CString& strRegCode, CString& strLicense);
//校验License
static bool IsLicense(const CString& strRegCode, const CString& strLicense);
//检查是否已授权
static bool PassAuthor();


/// <summary>
///   工作线程
/// </summary>
unsigned int _stdcall WorkThread( void* p )
{
	// TODO: code your application's behavior here.

	CHW104TransServices* pHW104TransServices = ( CHW104TransServices* )p;

	g_phw104Factory = ktn_GetHW104CollectorFactory();

	g_phw104Factory->CreateIHW104CollectorControl( g_phwCollectorControl );

	//启动采集
	KTN_RET ret = g_phwCollectorControl->StartCollect();

	if ( KTNERR( ret ) )
	{
		wchar_t wstrError[ MAX_ERROR_LEN ] = {0};
		swprintf_s( wstrError, MAX_ERROR_LEN, L"StartCollect Failed errorID:%d", ret );
		g_HW104Services.ReportEvent( wstrError );
		pHW104TransServices->Stop();
		return 0;
	}

	//启动TCPServer
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
///   服务名称
/// </summary>
#define HW104TRANSERVICES L"Experion PKS 104SlaveComm"							//服务显示名称
#define HW104TRANSDES	  L"Experion PKS 60870-5-104 Slave Communication"		//服务描述

CHW104TransServices::CHW104TransServices(void):NTService( HW104TRANSERVICES ),		//服务的名称
											   m_hStopEvent( NULL ),				//停止事件
											   m_hWorkThread( NULL ),				//线程句柄
											   m_dwWorkThreadID( 0 )				//线程ID
{
}

CHW104TransServices::~CHW104TransServices(void)
{
}

/// <summary>
///   安装服务例程
///   加入描述信息
/// </summary>
/// <remarks>
///   如果修改描述信息
///   可以在这里修改
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
/// 	重载的基类中的Run
/// </summary>
/// <param name="argc">
///		命令行条数
/// </param>
/// <param name="argv">
///		命令行
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
	/* 1.停止TCPServer                                                                     
	/* 2.停止采集
	/************************************************************************/
	unsigned int nCount = 0;
	while( WAIT_OBJECT_0 != WaitForSingleObject( m_hStopEvent , 1000 ) )
	{
		//if ( (++nCount) == 2*60 )//2分钟
		//{
		//	////增加授权检查
		//	//if ( !PassAuthor() )
		//	//{
		//	//	HW104TransLog( L"CHW104TransServices", L"\r\n\tNo valid License! Please contact with Honeywell to get License!" );
		//	//	HW104TransLog( L"CHW104TransServices", L"\r\n\t**********104Slave End**********" );	
		//	//	Stop();				
		//	//}
		//}		
	}
	//停止TCPServer
	g_HW104TCPServer.ServerEnd();      
	HW104TransLog( L"Quit", L"EndServer" );
	//停止采集
	g_phwCollectorControl->EndCollect();
	HW104TransLog( L"Quit", L"EndCollect" );

	if ( m_hStopEvent )
	{//释放事件句柄
		CloseHandle( m_hStopEvent );
		m_hStopEvent = NULL;
	}

	if ( m_hWorkThread )
	{//释放线程句柄
		CloseHandle( m_hWorkThread );
		m_hWorkThread = NULL;
	}

	ReportStatus( SERVICE_STOP );
}

/// <summary>
/// 	重载的基类中的Stop
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


//读取License信息
static bool GetLicense(CString& strRegCode, CString& strLicense)
{
	std::wstring wstrLicensePath;
	CHW104TransHelp::GetIniPath(wstrLicensePath);
	wstrLicensePath += LLICENSEFILENAME;
	//文件不存在,返回false
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
	
	//注册码
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

//校验License
static bool IsLicense(const CString& strRegCode, const CString& strLicense)
{
	//先计算出License
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
	//36进制编码
	DWORD dwVal;
	for(int i=0; i<4; i++)
	{
		dwVal = *( (DWORD*)(digestBuf + i*4) );
		strDigest += CHW104TransHelp::Code36_DWORD(dwVal);
	}
	//与输入License的比较
	if ( strLicense.Compare(strDigest) != 0 )
	{
		return false;
	}

	return true;
}

//检查是否已授权
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