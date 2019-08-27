// hw_104Trans.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "hw104_Common/hw104Common_Ext_inc.h"
#include "hw_104Trans.h"
#include "HW104TransServices.h"
#include "DataType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//用于日志输出
#define LMAINFUNC L"MainFunc"

// The one and only application object
CWinApp theApp;
using namespace std;
//HW104Trans.log
CHW104_Log global_HW104Log;
//全局服务句柄
extern CHW104TransServices g_HW104Services;


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		global_HW104Log.Open(TransLogName);
		HW104TransLog( LMAINFUNC, L"\r\n\t**********104Slave Start**********" );
				
		g_HW104Services.RegisterService( argc, argv );		
	//	g_HW104Services.Run( argc, argv );		
	}

	return nRetCode;
}