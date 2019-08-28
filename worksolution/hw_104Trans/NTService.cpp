/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1997 by Joerg Koenig and the ADG mbH, Mannheim, Germany
// All rights reserved
//
// Distribute freely, except: don't remove my name from the source or
// documentation (don't take credit for my work), mark your changes (don't
// get me blamed for your possible bugs), don't alter or remove this
// notice.
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc., and
// I'll try to keep a version up to date.  I can be reached as follows:
//    J.Koenig@adg.de                 (company site)
//    Joerg.Koenig@rhein-neckar.de    (private site)
/////////////////////////////////////////////////////////////////////////////
//
// MODIFIED BY TODD C. WILSON FOR THE ROAD RUNNER NT LOGIN SERVICE.
// HOWEVER, THESE MODIFICATIONS ARE BROADER IN SCOPE AND USAGE AND CAN BE USED
// IN OTHER PROJECTS WITH NO CHANGES.
// MODIFIED LINES FLAGGED/BRACKETED BY "//!! TCW MOD"
//
/////////////////////////////////////////////////////////////////////////////


// last revised: $Date: 11.05.98 21:09 $, $Revision: 3 $


/////////////////////////////////////////////////////////////////////////////
// Acknoledgements:
//	o	Thanks to Victor Vogelpoel (VictorV@Telic.nl) for his bug-fixes
//		and enhancements.
//	o	Thanks to Todd C. Wilson (todd@mediatec.com) for the
//		"service" on Win95
//
// Changes:
//	01/21/99
//	o	Bug fixed in "DeregisterApplicationLog()"
//		thanks to Grahame Willis (grahamew@questsoftware.com.au):
//
//	04/30/98
//	o	Added two more switches to handle command line arguments:
//		-e will force a running service to stop (corresponding
//		method in this class: virtual BOOL EndService();) and
//		-s will force the service to start (method:
//		virtual BOOL StartupService())
//
//	02/05/98
//	o	Added the methods "RegisterApplicationLog()" and
//		"DeregisterApplicationLog()" (both virtual). The first one will be
//		called from "InstallService()" and creates some registry-entries
//		for a better event-log. The second one removes these entries when
//		the service will uninstall (see "RemoveService()")
//	o	The service now obtains the security identifier of the current user
//		and uses this SID for event-logging.
//	o	The memory allocated by "CommandLineToArgvW()" will now release
//		(UNICODE version only)
//	o	The service now uses a simple message catalogue for a nicer
//		event logging
#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <crtdbg.h>
#include <io.h>			//!! TCW MOD
#include <fcntl.h>		//!! TCW MOD
#include "NTService.h"

#pragma warning( disable : 4996 )


#ifndef RSP_SIMPLE_SERVICE
	#define RSP_SIMPLE_SERVICE 1
#endif
#ifndef RSP_UNREGISTER_SERVICE
	#define RSP_UNREGISTER_SERVICE 0
#endif

//==============================================================================
BOOL NTService::m_bInstance = FALSE;
static NTService *gpTheService = NULL; // the one and only instance
//==============================================================================
/// <summary> 
///     获得全局唯一的服务实例
/// </summary> 
NTService* AfxGetService() 
{ 
	return gpTheService; 
}
//==============================================================================
static LPCTSTR gszAppRegKey = TEXT("SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\");
static LPCTSTR gszWin95ServKey = TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\RunServices");	//!! TCW MOD



/////////////////////////////////////////////////////////////////////////////
// class NTService -- construction/destruction
/////////////////////////////////////////////////////////////////////////////

/// <summary> 
///     构造函数
/// </summary> 
NTService::NTService(LPCTSTR lpServiceName, LPCTSTR lpDisplayName)
	: m_lpServiceName(lpServiceName)
	, m_lpDisplayName(lpDisplayName ? lpDisplayName : lpServiceName)
	, m_dwCheckPoint(0)
	, m_dwErr(0)
	, m_bDebug(FALSE)
	, m_sshStatusHandle(0)
	, m_dwControlsAccepted(SERVICE_ACCEPT_STOP)
	, m_pUserSID(0)
	, m_fConsoleReady(FALSE)
	// parameters to the "CreateService()" function:
	, m_dwDesiredAccess(SERVICE_ALL_ACCESS)
	, m_dwServiceType(SERVICE_INTERACTIVE_PROCESS|SERVICE_WIN32_OWN_PROCESS)
	, m_dwStartType(SERVICE_AUTO_START)
	, m_dwErrorControl(SERVICE_ERROR_NORMAL)
	, m_pszLoadOrderGroup(0)
	, m_dwTagID(0)
	, m_pszDependencies(0)
	, m_pszStartName(0)
	, m_pszPassword(0)
	, m_pszConfigFile(0)
{
	_ASSERTE(!m_bInstance);

	// 获得操作系统信息
	OSVERSIONINFO vi;
	// init this.
	vi.dwOSVersionInfoSize=sizeof(vi);
	// lint !e534
	GetVersionEx(&vi);
	m_bWinNT = (vi.dwPlatformId == VER_PLATFORM_WIN32_NT);

	// 设置全局变量
	m_bInstance = TRUE;
	gpTheService = this;
	
	// SERVICE_STATUS members that rarely change
	m_ssStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	m_ssStatus.dwServiceSpecificExitCode = 0;
	if (m_bWinNT) 
	{
		/////////////////////////////////////////////////////////////////////////
		// Providing a SID (security identifier) was contributed by Victor
		// Vogelpoel (VictorV@Telic.nl).
		// The code from Victor was slightly modified.
		// Get security information of current user
		BYTE	security_identifier_buffer[4096] = {0};
		DWORD	dwSizeSecurityIdBuffer = sizeof(security_identifier_buffer);
		PSID	user_security_identifier = NULL;
		TCHAR	sUserName[256] = {0};
		DWORD	dwSizeUserName = 255;
		TCHAR	sDomainName[256] = {0};
		DWORD	dwSizeDomainName = 255;
		SID_NAME_USE sidTypeSecurityId;
		::ZeroMemory(sUserName, sizeof(sUserName));
		::ZeroMemory(sDomainName, sizeof(sDomainName));
		::ZeroMemory(security_identifier_buffer, dwSizeSecurityIdBuffer);

		// 获得用户名
		::GetUserName(sUserName, &dwSizeUserName);
		
		// 查找帐号
		if (::LookupAccountName(0,
								sUserName,
								&security_identifier_buffer,
								&dwSizeSecurityIdBuffer,
								sDomainName,
								&dwSizeDomainName,
								&sidTypeSecurityId)) 
		{
			if (::IsValidSid(PSID(security_identifier_buffer))) 
			{
				DWORD dwSidLen = ::GetLengthSid(PSID(security_identifier_buffer));
				m_pUserSID = PSID(new BYTE[dwSidLen]);
				::CopySid(dwSidLen, m_pUserSID, security_identifier_buffer);
				_ASSERTE(::EqualSid(m_pUserSID, security_identifier_buffer));
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////
}

/// <summary> 
///     析构函数
/// </summary> 
NTService::~NTService() 
{
	_ASSERTE(m_bInstance);
	if (m_pUserSID != NULL)
	{
		delete []LPBYTE(m_pUserSID);
		m_pUserSID = NULL;
	}
	m_bInstance = FALSE;
	gpTheService = 0;
}

/////////////////////////////////////////////////////////////////////////////
// class NTService -- overridables
#define NEXT_ARG ((((*Argv)[2])==TEXT('\0'))?(--Argc,*++Argv):(*Argv)+2)

/// <summary> 
///     注册服务
/// </summary> 
BOOL NTService::RegisterService(int argc, TCHAR **argv) 
{
	BOOL (NTService:: * fnc)() = &NTService::StartDispatcher;
	int		Argc = argc;
	LPTSTR	*Argv = argv;
	LPTSTR  Token = NULL;
	while (++Argv, --Argc) 
	{
		Token = Argv[0];
		if ((Token[0] == TEXT('-') || Token[0] == TEXT('/')) && Token[1] != NULL)
		{
			++Token;
			switch (Token[0]) 
			{
			case TEXT('i'):	// install the service
			case TEXT('I'):	// install the service
				fnc = &NTService::InstallService;
				break;
			case TEXT('l'):	// login-account (only useful with -i)
			case TEXT('L'):	// login-account (only useful with -i)
				m_pszStartName = NEXT_ARG;
				break;
			case TEXT('p'):	// password (only useful with -i)
			case TEXT('P'):	// password (only useful with -i)
				m_pszPassword = NEXT_ARG;
				break;
			case TEXT('f'): // configuration
			case TEXT('F'): // configuration
				m_pszConfigFile = NEXT_ARG;
				break;
			case TEXT('n'): // service name
			case TEXT('N'): // service name
				m_lpServiceName = NEXT_ARG;
				break;
			case TEXT('m'):
			case TEXT('M'):
				m_lpDisplayName = NEXT_ARG;
				break;
			case TEXT('u'):	// uninstall the service
			case TEXT('U'):	// uninstall the service	
				fnc = &NTService::RemoveService;
				break;
			case TEXT('s'):	// start the service
			case TEXT('S'):	// start the service
				fnc = &NTService::StartupService;
				break;
			case TEXT('e'):	// end the service
			case TEXT('E'):	// end the service	
				fnc = &NTService::EndService;
				break;
			case TEXT('c'): // config the service
			case TEXT('C'): // config the service
				fnc = &NTService::ConfigService;
				break;
			case TEXT('?'):  // help service
			case TEXT('h'):  // 
			case TEXT('H'):  // 
				fnc = &NTService::HelpService;
				break;
			case TEXT('d'):	// debug the service
			case TEXT('D'):	// debug the service
				m_bDebug = TRUE;
				// pass original parameters to DebugService()
				return DebugService(argc, argv, FALSE); //!! TCW MOD faceless non-service (Win95) mode
			} // switch
		} // if
	} //while

	//!! TCW MOD START - if Win95, run as faceless app.
	if (fnc == &NTService::StartDispatcher && OsIsWin95()) 
	{
		// act as if -f was passed anyways.
		m_bDebug = TRUE;
		return DebugService(argc, argv, TRUE);
	}
	//!! TCW MOD END - if Win95, run as faceless app.

	return (this->*fnc)();
}

/// <summary> 
///     服务入口
/// </summary> 
BOOL NTService::StartDispatcher()
{
    // Default implementation creates a single threaded service.
	// Override this method and provide more table entries for
	// a multithreaded service (one entry for each thread).
	SERVICE_TABLE_ENTRY dispatchTable[] =
    {
        {LPTSTR(m_lpServiceName), (LPSERVICE_MAIN_FUNCTION)ServiceMain},
        {0,	0}
    };
	BOOL bRet = StartServiceCtrlDispatcher(dispatchTable);
	if (!bRet) 
	{
		TCHAR szBuf[256] = {0};
        ReportEvent(GetLastErrorText(szBuf, 255));
	}
	return bRet;
}

// Setup service configurations
BOOL NTService::ConfigService()
{
	// 派生类实现
	return TRUE;
}

// Show help information
BOOL NTService::HelpService()
{
	TCHAR	szPath[1024] = {0};
	TCHAR	*szExe = NULL;
	if (GetModuleFileName(0, szPath, 1023) == 0)
	{
		return FALSE;
	}

	// 拆分可执行文件名
	szExe = _tcsrchr(szPath, TEXT('\\'));
	if (szExe != NULL)
	{
		szExe++;
	}
	else
	{
		szExe = szPath;
	}

	// 显示帮助信息
	_tprintf(TEXT("Usage:\n\t%s command [options]\n"), szExe);
	_tprintf(TEXT("command:\n"));
	_tprintf(TEXT("\t-i install service\n"));
	_tprintf(TEXT("\t-u uninstall service\n"));
	_tprintf(TEXT("\t-s start service\n"));
	_tprintf(TEXT("\t-e end(stop) service\n"));
	_tprintf(TEXT("\t-c config service\n"));
	_tprintf(TEXT("\t-d debug service\n"));
	_tprintf(TEXT("\t-? help information\n"));
	_tprintf(TEXT("\t-h help information\n"));
	_tprintf(TEXT("options:\n"));
	_tprintf(TEXT("\t-f config file name(pure file name,not full path)\n"));
	_tprintf(TEXT("\t-m service display name(only useful with -i)\n"));
	_tprintf(TEXT("\t-n service name(only useful with -i,-u,-s,-e)\n"));
	_tprintf(TEXT("\t-l login-account (only useful with -i)\n"));
	_tprintf(TEXT("\t-p login-password(only useful with -i)\n"));	
	return TRUE;
}

/// <summary> 
///     安装服务例程
/// </summary> 
BOOL NTService::InstallService() 
{
	TCHAR szPath[1024] = {0};
	TCHAR szErr[256] = {0};
	SetupConsole();	//!! TCW MOD - have to show the console here for the
					// diagnostic or error reason: orignal class assumed
					// that we were using _main for entry (a console app).
					// This particular usage is a Windows app (no console),
					// so we need to create it. Using SetupConsole with _main
					// is ok - does nothing, since you only get one console.

	if (!GetConfigInfo())
	{
		_tprintf(TEXT("Can not get config info.\n"));
		return FALSE;
	}

	if (GetModuleFileName(0, szPath, 1023 ) == 0) 
	{
		_tprintf( TEXT("Unable to install %s - %s\n"), m_lpDisplayName, GetLastErrorText(szErr, 256));
		return FALSE;
	}

	// 增加参数
	if (m_pszConfigFile != NULL)
	{
		// 配置文件名m_pszConfigFile
		_tcscat(szPath, TEXT(" -f "));
		_tcscat(szPath, m_pszConfigFile);
	}

	BOOL bRet = FALSE;
	if (OsIsWin95())
	{	//!! TCW MOD - code added to install as Win95 service
		// Create a key for that application and insert values for
		// "EventMessageFile" and "TypesSupported"
		HKEY hKey = 0;
		LONG lRet = ERROR_SUCCESS;
		if (::RegCreateKey(HKEY_LOCAL_MACHINE, gszWin95ServKey , &hKey) == ERROR_SUCCESS) 
		{
			lRet = ::RegSetValueEx(hKey,			// handle of key to set value for
								   m_lpServiceName,	// address of value to set (NAME OF SERVICE)
								   0,				// reserved
								   REG_EXPAND_SZ,	// flag for value type
								   (CONST BYTE*)szPath,	// address of value data
								   (DWORD)(_tcslen(szPath) + 1)*sizeof(TCHAR));	// size of value data
			::RegCloseKey(hKey);
			bRet = TRUE;
		}
	} 
	else 
	{
		// Real NT services go here.
		SC_HANDLE schSCManager = OpenSCManager(0,					// machine (NULL == local)
											   0,					// database (NULL == default)
											   SC_MANAGER_ALL_ACCESS);// access required
		if (schSCManager) 
		{
			SC_HANDLE schService = CreateService(
						schSCManager,
						m_lpServiceName,
						m_lpDisplayName,
						m_dwDesiredAccess,
						m_dwServiceType,
						m_dwStartType,
						m_dwErrorControl,
						szPath,
						m_pszLoadOrderGroup,
						((m_dwServiceType == SERVICE_KERNEL_DRIVER ||
						m_dwServiceType == SERVICE_FILE_SYSTEM_DRIVER) &&
						(m_dwStartType == SERVICE_BOOT_START ||
						m_dwStartType == SERVICE_SYSTEM_START)) ? &m_dwTagID : 0,
						m_pszDependencies,
						m_pszStartName,
						m_pszPassword);

			if (schService) 
			{
				_tprintf(TEXT("%s installed.\n"), m_lpDisplayName);
				bRet = TRUE;
			} 
			else 
			{
				_tprintf(TEXT("CreateService failed - %s\n"), GetLastErrorText(szErr, 256));
				bRet = FALSE;
			}
			CloseServiceHandle(schSCManager);
		} 
		else 
		{
			_tprintf(TEXT("OpenSCManager failed - %s\n"), GetLastErrorText(szErr, 256));
			bRet = FALSE;
		}

		// CreateService成功
		if (bRet) 
		{
			// installation succeeded. Now register the message file
			RegisterApplicationLog(
				szPath,	// the path to the application itself
				EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE); // supported types
			
			// 显示信息：服务被成功安装
			ReportEvent(TEXT("Service installed"), EVENTLOG_INFORMATION_TYPE, CAT_INFO);
		}
	}	//!! TCW MOD

	return bRet;
}

/// <summary> 
///     删除服务
/// </summary> 
BOOL NTService::RemoveService() 
{
	BOOL bRet = FALSE;
	TCHAR szErr[256] = {0};
	SetupConsole();	//!! TCW MOD - have to show the console here for the
					// diagnostic or error reason: orignal class assumed
					// that we were using _main for entry (a console app).
					// This particular usage is a Windows app (no console),
					// so we need to create it. Using SetupConsole with _main
					// is ok - does nothing, since you only get one console.

	if (!GetConfigInfo())
	{
		_tprintf(TEXT("Can not get config info. \n"));
		return FALSE;
	}

	if (OsIsWin95()) 
	{	//!! TCW MOD - code added to install as Win95 service
		HKEY hKey = 0;
		LONG lRet = ERROR_SUCCESS;
		if (::RegCreateKey(HKEY_LOCAL_MACHINE, gszWin95ServKey , &hKey) == ERROR_SUCCESS) 
		{
			lRet = ::RegDeleteValue(hKey, m_lpServiceName);
			::RegCloseKey(hKey);
			bRet = TRUE;
		}
	} 
	else 
	{
		// Real NT services go here.
		SC_HANDLE schSCManager = OpenSCManager(
									0,						// machine (NULL == local)
									0,						// database (NULL == default)
									SC_MANAGER_ALL_ACCESS);	// access required
		if (schSCManager) 
		{
			SC_HANDLE schService =	OpenService(
										schSCManager,
										m_lpServiceName,
										SERVICE_ALL_ACCESS);

			if (schService) 
			{
				// try to stop the service
				if (ControlService(schService, SERVICE_CONTROL_STOP, &m_ssStatus)) 
				{
					_tprintf(TEXT("Stopping %s."), m_lpDisplayName);
					Sleep(1000);

					while (QueryServiceStatus(schService, &m_ssStatus)) 
					{
						if (m_ssStatus.dwCurrentState == SERVICE_STOP_PENDING) 
						{
							_tprintf(TEXT("."));
							Sleep(1000);
						} 
						else
						{
							break;
						}
					} // while loop
					if (m_ssStatus.dwCurrentState == SERVICE_STOPPED)
					{	
						_tprintf(TEXT("\n%s stopped.\n"), m_lpDisplayName);
					}
					else
					{
						_tprintf(TEXT("\n%s failed to stop.\n"), m_lpDisplayName);
					}
				}

				// now remove the service
				if (DeleteService(schService)) 
				{
					_tprintf(TEXT("%s removed.\n"), m_lpDisplayName);
					bRet = TRUE;
				} 
				else 
				{
					_tprintf(TEXT("DeleteService failed - %s\n"), GetLastErrorText(szErr, 256));
				}

				CloseServiceHandle(schService);
			} 
			else 
			{
				_tprintf(TEXT("OpenService failed - %s\n"), GetLastErrorText(szErr, 256));
			}

			CloseServiceHandle(schSCManager);
		} 
		else 
		{
			_tprintf(TEXT("OpenSCManager failed - %s\n"), GetLastErrorText(szErr, 256));
		}

		if (bRet)
		{
			DeregisterApplicationLog();
		}
	}

	return bRet;
}

/// <summary> 
///     停止服务
/// </summary> 
BOOL NTService::EndService() 
{
	BOOL bRet = FALSE;
	TCHAR szErr[256] = {0};
	SC_HANDLE schSCManager = ::OpenSCManager(
								0,						// machine (NULL == local)
								0,						// database (NULL == default)
								SC_MANAGER_ALL_ACCESS);	// access required
	if (schSCManager) 
	{
		if (!GetConfigInfo())
		{
			_tprintf(TEXT("Can not get config info. \n"));
			return FALSE;
		}
		
		SC_HANDLE schService = ::OpenService(
									schSCManager,
									m_lpServiceName,
									SERVICE_ALL_ACCESS);
		if (schService) 
		{
			// try to stop the service
			if (::ControlService(schService, SERVICE_CONTROL_STOP, &m_ssStatus)) 
			{
				_tprintf(TEXT("Stopping %s."), m_lpDisplayName);
				::Sleep(1000);

				while (::QueryServiceStatus(schService, &m_ssStatus)) 
				{
					if (m_ssStatus.dwCurrentState == SERVICE_STOP_PENDING) 
					{
						_tprintf(TEXT("."));
						::Sleep(1000);
					} 
					else
					{
						break;
					}
				}
				if (m_ssStatus.dwCurrentState == SERVICE_STOPPED)
				{	
					_tprintf(TEXT("\n%s stopped.\n"), m_lpDisplayName);
					bRet = TRUE;
				}
				else
				{
                    _tprintf(TEXT("\n%s failed to stop.\n"), m_lpDisplayName);
				}
			}

			::CloseServiceHandle(schService);
		} 
		else 
		{
			_tprintf(TEXT("OpenService failed - %s\n"), GetLastErrorText(szErr,256));
		}

        ::CloseServiceHandle(schSCManager);
    } 
	else 
	{
		_tprintf(TEXT("OpenSCManager failed - %s\n"), GetLastErrorText(szErr, 256));
	}

	return bRet;
}

/// <summary> 
///		启动服务
/// </summary> 
BOOL NTService::StartupService()
{
	BOOL bRet = FALSE;
	TCHAR szErr[256] = {0};

	SC_HANDLE schSCManager = ::OpenSCManager(
								0,						// machine (NULL == local)
								0,						// database (NULL == default)
								SC_MANAGER_ALL_ACCESS);	// access required
	if (schSCManager) 
	{
		if (!GetConfigInfo())
		{
			_tprintf( TEXT("Can not get config info. \n") );
			return FALSE;
		}

		SC_HANDLE schService = ::OpenService(
									schSCManager,
									m_lpServiceName,
									SERVICE_ALL_ACCESS);
		if (schService) 
		{
			// try to start the service
			_tprintf(TEXT("Starting up %s."), m_lpDisplayName);
			if (::StartService(schService, 0, 0)) 
			{
				Sleep(1000);
				while (::QueryServiceStatus(schService, &m_ssStatus)) 
				{
					if (m_ssStatus.dwCurrentState == SERVICE_START_PENDING) 
					{
						_tprintf(TEXT("."));
						Sleep(1000);
					}
					else
					{
						break;
					}
				}

				if (m_ssStatus.dwCurrentState == SERVICE_RUNNING)
				{
					_tprintf(TEXT("\n%s started.\n"), m_lpDisplayName);
					bRet = TRUE;
				}
				else
				{
					_tprintf(TEXT("\n%s failed to start.\n"), m_lpDisplayName);
				}
			}
			else 
			{
				// StartService failed
				_tprintf(TEXT("\n%s failed to start: %s\n"), m_lpDisplayName, GetLastErrorText(szErr, 256));
			}
			::CloseServiceHandle(schService);
		} 
		else 
		{
			_tprintf(TEXT("OpenService failed - %s\n"), GetLastErrorText(szErr, 256));
		}
        ::CloseServiceHandle(schSCManager);
    } 
	else 
	{
		_tprintf(TEXT("OpenSCManager failed - %s\n"), GetLastErrorText(szErr, 256));
	}

	return bRet;
}


////////////////////////////////////////////////////////////////////////////
//!! TCW MOD - faceless window procedure for usage within Win95 (mostly),
// but can be invoked under NT by using -f
LRESULT CALLBACK _FacelessWndProc_(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	if (uMsg == WM_QUERYENDSESSION || uMsg == WM_ENDSESSION || uMsg == WM_QUIT) 
	{
		if (lParam==NULL || uMsg==WM_QUIT) 
		{
			DestroyWindow(hwnd); // kill me
			if (AfxGetService() != NULL)
			{
				AfxGetService()->Stop(); // stop me.
			}
			return TRUE;
		}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
////////////////////////////////////////////////////////////////////////////

/// <summary> 
///		调试服务
/// </summary> 
BOOL NTService::DebugService(int argc, TCHAR ** argv, BOOL faceless) 
{
    int dwArgc = argc;
    LPTSTR *lpszArgv = argv;

	if (!faceless) 
	{	//!! TCW MOD - no faceless, so give it a face.
		SetupConsole();	//!! TCW MOD - make the console for debugging
	   _tprintf(TEXT("Debugging %s.\n"), m_lpDisplayName);

		SetConsoleCtrlHandler(ControlHandler, TRUE);
	}

	//!! TCW MOD START - if Win95, register server
	typedef DWORD (WINAPI * fp_RegServProc)(DWORD dwProcessId, DWORD dwType);
	fp_RegServProc fncptr = NULL;

	if (faceless /*&& OsIsWin95()*/) 
	{
		WNDCLASS wndclass;
		memset(&wndclass, 0, sizeof(WNDCLASS));
		wndclass.lpfnWndProc = _FacelessWndProc_;
		wndclass.hInstance = HINSTANCE(::GetModuleHandle(0));
		wndclass.lpszClassName = TEXT("RRL__FacelessWndProc_");
		ATOM atom = ::RegisterClass(&wndclass);
		HWND hwnd = ::CreateWindow(wndclass.lpszClassName,
								   TEXT(""),
								   0,
								   0,
								   0,
								   0,
								   0,
								   0,
								   0,
								   wndclass.hInstance,
								   0);
		HMODULE hModule = ::GetModuleHandle(TEXT("kernel32.dll"));
		// punch F1 on "RegisterServiceProcess" for what it does and when to use it.
		fncptr = (fp_RegServProc)::GetProcAddress(hModule, "RegisterServiceProcess");
		if (fncptr != NULL)
		{
			(*fncptr)(0, RSP_SIMPLE_SERVICE);
		}
	}
	//!! TCW MOD END - if Win95, register server

    Run(dwArgc, lpszArgv);

	if (fncptr != NULL)		//!! TCW MOD - if it's there, remove it: our run is over
	{
		(*fncptr)(0, RSP_UNREGISTER_SERVICE);
	}

	return TRUE;
}

BOOL NTService::GetConfigInfo()
{
	// 派生类中实现
	return TRUE;
}

void NTService :: Pause() 
{

}

void NTService :: Continue() 
{

}

void NTService :: Shutdown() 
{

}

/////////////////////////////////////////////////////////////////////////////
// class NTService -- default handlers
void WINAPI NTService::ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv) 
{
	_ASSERTE(gpTheService != 0);

	// register our service control handler:
	gpTheService->m_sshStatusHandle = RegisterServiceCtrlHandler(
											gpTheService->m_lpServiceName,
											NTService::ServiceCtrl);

	if (gpTheService->m_sshStatusHandle)
	{
		// report the status to the service control manager.
		if (gpTheService->ReportStatus(SERVICE_START_PENDING))
		{
			gpTheService->Run(dwArgc, lpszArgv);
		}
	}

	// try to report the stopped status to the service control manager.
	if (gpTheService->m_sshStatusHandle)
	{
		gpTheService->ReportStatus(SERVICE_STOPPED);
	}
}

void WINAPI NTService::ServiceCtrl(DWORD dwCtrlCode) 
{
	_ASSERTE(gpTheService != 0);

	// Handle the requested control code.
	switch (dwCtrlCode) 
	{
	case SERVICE_CONTROL_STOP:
		// Stop the service.
		gpTheService->m_ssStatus.dwCurrentState = SERVICE_STOP_PENDING;
		gpTheService->Stop();
		break;

	case SERVICE_CONTROL_PAUSE:
		gpTheService->m_ssStatus.dwCurrentState = SERVICE_PAUSE_PENDING;
		gpTheService->Pause();
		break;

	case SERVICE_CONTROL_CONTINUE:
		gpTheService->m_ssStatus.dwCurrentState = SERVICE_CONTINUE_PENDING;
		gpTheService->Continue();
		break;

	case SERVICE_CONTROL_SHUTDOWN:
		gpTheService->Shutdown();
		break;

	case SERVICE_CONTROL_INTERROGATE:
		// Update the service status.
		gpTheService->ReportStatus(gpTheService->m_ssStatus.dwCurrentState);
		break;

	default:
		// invalid control code
		break;
	}
}

BOOL WINAPI NTService::ControlHandler(DWORD dwCtrlType) 
{
	_ASSERTE(gpTheService != 0);
	switch (dwCtrlType) 
	{
	case CTRL_BREAK_EVENT:  // use Ctrl+C or Ctrl+Break to simulate
	case CTRL_C_EVENT:      // SERVICE_CONTROL_STOP in debug mode
		_tprintf(TEXT("Stopping %s.\n"), gpTheService->m_lpDisplayName);
		gpTheService->Stop();
		return TRUE;
	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// class NTService -- helpers

//!! TCW MOD - added DWORD dwErrExit for error exit value. Defaults to zero
BOOL NTService::ReportStatus(DWORD dwCurrentState,
							 DWORD dwWaitHint,
							 DWORD dwErrExit) 
{
	BOOL fResult = TRUE;
	if (!m_bDebug)  // when debugging we don't report to the SCM
	{ 
        if( dwCurrentState == SERVICE_START_PENDING)
		{
            m_ssStatus.dwControlsAccepted = 0;
		}
        else
		{
            m_ssStatus.dwControlsAccepted = m_dwControlsAccepted;
		}
		m_ssStatus.dwCurrentState = dwCurrentState;
        m_ssStatus.dwWin32ExitCode = NO_ERROR;
        m_ssStatus.dwWaitHint = dwWaitHint;

		//!! TCW MOD START - added code to support error exiting
		m_ssStatus.dwServiceSpecificExitCode = dwErrExit;
		if (dwErrExit != 0)
		{	
			m_ssStatus.dwWin32ExitCode = ERROR_SERVICE_SPECIFIC_ERROR;
		}
		//!! TCW MOD END - added code to support error exiting

        if (dwCurrentState == SERVICE_RUNNING || dwCurrentState == SERVICE_STOPPED)
		{
            m_ssStatus.dwCheckPoint = 0;
		}
		else
		{
            m_ssStatus.dwCheckPoint = ++m_dwCheckPoint;
		}

        // Report the status of the service to the service control manager.
        if (!(fResult = SetServiceStatus(m_sshStatusHandle, &m_ssStatus))) 
		{
           // ReportEvent(TEXT("SetServiceStatus() failed"));
        }
    }

    return fResult;
}

/// <summary> 
///     生成系统日志消息
/// </summary> 
void NTService::ReportEvent(LPCTSTR lpszMsg, WORD wEventType, WORD wEventCatagory, DWORD dwEventID) 
{
	m_dwErr = GetLastError();
	// use default message-IDs
	if (dwEventID == DWORD(-1))
	{
		dwEventID = GENERAL_MESSAGE_ID;
	}

	// Use event logging to log the error.
	HANDLE hEventSource = RegisterEventSource(NULL, m_lpServiceName);
	if (hEventSource != NULL) 
	{
		LPCTSTR lpszMessage = lpszMsg;
		::ReportEvent(
			hEventSource,	// handle of event source
			wEventType,		// event type
			wEventCatagory,	// event category
			dwEventID,		// event ID
			m_pUserSID,		// current user's SID
			1,				// strings in lpszStrings
			0,				// no bytes of raw data
			&lpszMessage,	// array of error strings
			0);				// no raw data
		::DeregisterEventSource(hEventSource);
    }
}

/// <summary> 
///     获得最后一次的系统错误描述
/// </summary> 
LPTSTR NTService::GetLastErrorText(LPTSTR lpszBuf, DWORD dwSize) 
{
	_ASSERTE(lpszBuf != NULL);
	_ASSERTE(dwSize > 0);
    LPTSTR lpszTemp = NULL;

	DWORD dwRet1 = ::GetLastError();

    DWORD dwRet = ::FormatMessage(
						FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_ARGUMENT_ARRAY,
						0,
						dwRet1,
						LANG_NEUTRAL,
						(LPTSTR)&lpszTemp,
						0,
						0);
	// 失败或者缓冲区长度不够
    if ((dwRet == 0) || (dwSize < dwRet+1))
	{
		lpszBuf[0] = TEXT('\0');
	}
	else 
	{
		// remove cr/nl characters
		lpszTemp[_tcsclen(lpszTemp)-2] = TEXT('\0');
		_tcscpy(lpszBuf, lpszTemp);
    }
    if (lpszTemp)
	{
        LocalFree(HLOCAL(lpszTemp));
	}

	return lpszBuf;
}

/////////////////////////////////////////////////////////////////////////////
// class NTService -- implementation
void NTService::RegisterApplicationLog(LPCTSTR lpszFileName, DWORD dwTypes) 
{
	TCHAR szKey[256] = {0};
	_tcscpy(szKey, gszAppRegKey);
	_tcscat(szKey, m_lpServiceName);
	HKEY hKey = 0;
	LONG lRet = ERROR_SUCCESS;
	
	// Create a key for that application and insert values for
	// "EventMessageFile" and "TypesSupported"
	if (::RegCreateKey(HKEY_LOCAL_MACHINE, szKey, &hKey) == ERROR_SUCCESS) 
	{
		// 设置资源文件
		lRet = ::RegSetValueEx(
					hKey,						// handle of key to set value for
					TEXT("EventMessageFile"),	// address of value to set
					0,							// reserved
					REG_EXPAND_SZ,				// flag for value type
					(CONST BYTE*)lpszFileName,	// address of value data
					(DWORD)(_tcslen(lpszFileName) + 1)*sizeof(TCHAR));	// size of value data

		// 设置分类资源文件
		lRet = ::RegSetValueEx(
					hKey, 
					TEXT("CategoryMessageFile"), 
					NULL, 
					REG_EXPAND_SZ, 
					(LPBYTE)lpszFileName, 
					(DWORD)(_tcslen(lpszFileName) + 1)*sizeof(TCHAR));
		
		// 设置分类个数
		DWORD dwCategoryCount = 6;
		lRet = ::RegSetValueEx(
					hKey, 
					TEXT("CategoryCount"), 
					NULL, 
					REG_DWORD,
					(LPBYTE)&dwCategoryCount, 
					sizeof(dwCategoryCount));
        
		// 设置支持的消息类型掩码
		lRet = ::RegSetValueEx(
					hKey,					// handle of key to set value for
					TEXT("TypesSupported"),	// address of value to set
					0,						// reserved
					REG_DWORD,				// flag for value type
					(CONST BYTE*)&dwTypes,	// address of value data
					sizeof(DWORD)			// size of value data
				);
		::RegCloseKey(hKey);
	}

	// Add the service to the "Sources" value
	lRet = ::RegOpenKeyEx( 
				HKEY_LOCAL_MACHINE,	// handle of open key 
				gszAppRegKey,		// address of name of subkey to open 
				0,					// reserved 
				KEY_ALL_ACCESS,		// security access mask 
				&hKey);				// address of handle of open key 
	if (lRet == ERROR_SUCCESS) 
	{
		DWORD dwSize = 0;
		// retrieve the size of the needed value
		lRet = ::RegQueryValueEx(
					hKey,			// handle of key to query 
					TEXT("Sources"),// address of name of value to query 
					0,				// reserved 
					0,				// address of buffer for value type 
					0,				// address of data buffer 
					&dwSize);		// address of data buffer size 
 		if (lRet == ERROR_SUCCESS) 
		{
			DWORD dwType = 0;
			DWORD dwNewSize = dwSize + (_tcslen(m_lpServiceName) + 1) * sizeof(TCHAR);
			LPBYTE Buffer = LPBYTE(::GlobalAlloc(GPTR, dwNewSize));
			lRet = ::RegQueryValueEx(
						hKey,			// handle of key to query 
						TEXT("Sources"),// address of name of value to query 
						0,				// reserved 
						&dwType,		// address of buffer for value type 
						Buffer,			// address of data buffer 
						&dwSize);		// address of data buffer size
			if (lRet == ERROR_SUCCESS) 
			{
				_ASSERTE(dwType == REG_MULTI_SZ);

				// check whether this service is already a known source
				register LPTSTR p = LPTSTR(Buffer);
				for (; *p; p += _tcslen(p)+1) 
				{
					if (_tcscmp(p, m_lpServiceName) == 0)
					{
						break;
					}
				}
				if (*p == NULL) 
				{
					// We're standing at the end of the stringarray
					// and the service does still not exist in the "Sources".
					// Now insert it at this point.
					// Note that we have already enough memory allocated
					// (see GlobalAlloc() above). We also don't need to append
					// an additional '\0'. This is done in GlobalAlloc() above
					// too.
					_tcscpy(p, m_lpServiceName);

					// OK - now store the modified value back into the
					// registry.
					lRet = ::RegSetValueEx(
								hKey,			// handle of key to set value for
								TEXT("Sources"),// address of value to set
								0,				// reserved
								dwType,			// flag for value type
								Buffer,			// address of value data
								dwNewSize);		// size of value data
				}
			}

			::GlobalFree(HGLOBAL(Buffer));
		}

		::RegCloseKey(hKey);
	}
}

void NTService::DeregisterApplicationLog()
{
	TCHAR szKey[256] = {0};
	_tcscpy(szKey, gszAppRegKey);
	_tcscat(szKey, m_lpServiceName);
	HKEY hKey = 0;
	LONG lRet = ERROR_SUCCESS;

	// 删除服务注册项
	lRet = ::RegDeleteKey(HKEY_LOCAL_MACHINE, szKey);

	// now we have to delete the application from the "Sources" value too.
	lRet = ::RegOpenKeyEx( 
				HKEY_LOCAL_MACHINE,	// handle of open key 
				gszAppRegKey,		// address of name of subkey to open 
				0,					// reserved 
				KEY_ALL_ACCESS,		// security access mask 
				&hKey);				// address of handle of open key 
	if (lRet == ERROR_SUCCESS) 
	{
		DWORD dwSize = 0;

		// retrieve the size of the needed value
		lRet = ::RegQueryValueEx(
					hKey,			// handle of key to query 
					TEXT("Sources"),// address of name of value to query 
					0,				// reserved 
					0,				// address of buffer for value type 
					0,				// address of data buffer 
					&dwSize);		// address of data buffer size 
 		if (lRet == ERROR_SUCCESS) 
		{
			DWORD dwType = 0;
			LPBYTE Buffer = LPBYTE(::GlobalAlloc(GPTR, dwSize));
			LPBYTE NewBuffer = LPBYTE(::GlobalAlloc(GPTR, dwSize));
			lRet = ::RegQueryValueEx(
						hKey,			// handle of key to query 
						TEXT("Sources"),// address of name of value to query 
						0,				// reserved 
						&dwType,		// address of buffer for value type 
						Buffer,			// address of data buffer 
						&dwSize);		// address of data buffer size 
			if (lRet == ERROR_SUCCESS) 
			{
				_ASSERTE(dwType == REG_MULTI_SZ);

				// check whether this service is already a known source
				register LPTSTR p = LPTSTR(Buffer);
				register LPTSTR pNew = LPTSTR(NewBuffer);
				BOOL bNeedSave = FALSE;	// assume the value is already correct
				for (; *p; p += _tcslen(p)+1) 
				{
					// except ourself: copy the source string into the destination
					if ( _tcscmp(p, m_lpServiceName) != 0) 
					{
						_tcscpy(pNew, p);
						pNew += (DWORD)_tcslen(pNew)+1;
					}
					else 
					{
						bNeedSave = TRUE;		// *this* application found
						dwSize -= (DWORD)(_tcslen(p)+1)*sizeof(TCHAR);	// new size of value
					}
				}
				if (bNeedSave) 
				{
					// OK - now store the modified value back into the
					// registry.
					lRet = ::RegSetValueEx(
								hKey,			// handle of key to set value for
								TEXT("Sources"),// address of value to set
								0,				// reserved
								dwType,			// flag for value type
								NewBuffer,		// address of value data
								dwSize);		// size of value data
				}
			}
			::GlobalFree(HGLOBAL(Buffer));
			::GlobalFree(HGLOBAL(NewBuffer));
		}

		::RegCloseKey(hKey);
	}
}

////////////////////////////////////////////////////////

//!! TCW MOD - function to create console for faceless apps if not already there
void NTService::SetupConsole() 
{
	if (!m_fConsoleReady) 
	{
		AllocConsole();	// you only get 1 console.

		// lovely hack to get the standard io (printf, getc, etc) to the new console. Pretty much does what the
		// C lib does for us, but when we want it, and inside of a Window'd app.
		// The ugly look of this is due to the error checking (bad return values. Remove the if xxx checks if you like it that way.
		DWORD astds[3] = {STD_OUTPUT_HANDLE, STD_ERROR_HANDLE, STD_INPUT_HANDLE};
		FILE *atrgs[3] = {stdout, stderr, stdin};
		for (register int i = 0; i < 3; i++) 
		{
			HANDLE hand = GetStdHandle(astds[i]);
			if (hand != INVALID_HANDLE_VALUE) 
			{
				int osf = _open_osfhandle((intptr_t)hand, _O_TEXT);
				if (osf != -1) 
				{
					FILE *fp = _fdopen(osf, (astds[i] == STD_INPUT_HANDLE) ? "r" : "w");
					if (fp != NULL) 
					{
						*(atrgs[i]) = *fp;
						setvbuf(fp, NULL,_IONBF, 0);
					}
				}
			}
		}// FOR LOOP
		m_fConsoleReady=TRUE;
	}
}