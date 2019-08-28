// pksTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "pksTest.h"
#include "stdio.h"
#include "psapi.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "HW104CallBack.h"
#include "LogTest.h"

IHW104CollectorControl* phwCollectorControl;
IHW104Collector*		phwCollector[ 10 ] = {0};
int bControl;

/// <summary>
///   把一个双字符转换成为单字节
/// </summary>
bool ConvertToWideBytes( char* pChar, std::wstring& pwstrOut)
{
	DWORD dwNum = MultiByteToWideChar( CP_ACP,
		0,
		pChar,
		strlen( pChar )+1,
		NULL,
		0 );

	wchar_t* pTemp = new wchar_t[ dwNum ];

	if ( NULL == pTemp )
	{
		return false;
	}
	ZeroMemory( pTemp, dwNum*2 );

	DWORD dwRet = MultiByteToWideChar( CP_ACP,
		0,
		pChar,
		strlen( pChar )+1,
		pTemp,
		dwNum );

	if ( dwRet == 0 )
	{
		delete pTemp;
		TRACE( ("ConvertToWideBytes failed  error: %d \n"), ::GetLastError() );
		return false;
	}

	pwstrOut.assign( pTemp );
	delete pTemp;
	return true;
}



// The one and only application object

CWinApp theApp;

using namespace std;

void PrintMemoryInfo(  )
{
	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;

	// Print the process identifier.

	printf( "\nProcess ID: %u\n", GetCurrentProcessId() );

	// Print information about the memory usage of the process.

	if ( GetProcessMemoryInfo( GetCurrentProcess(), &pmc, sizeof(pmc)) )
	{
		printf( "\tPageFaultCount: 0x%08d\n", pmc.PageFaultCount );
		printf( "\tPeakWorkingSetSize: 0x%08d\n", 
			pmc.PeakWorkingSetSize );
		printf( "\tWorkingSetSize: 0x%08d\n", pmc.WorkingSetSize );
		printf( "\tQuotaPeakPagedPoolUsage: 0x%08d\n", 
			pmc.QuotaPeakPagedPoolUsage );
		printf( "\tQuotaPagedPoolUsage: 0x%08d\n", 
			pmc.QuotaPagedPoolUsage );
		printf( "\tQuotaPeakNonPagedPoolUsage: 0x%08d\n", 
			pmc.QuotaPeakNonPagedPoolUsage );
		printf( "\tQuotaNonPagedPoolUsage: 0x%08d\n", 
			pmc.QuotaNonPagedPoolUsage );
		printf( "\tPagefileUsage: 0x%08d\n", pmc.PagefileUsage ); 
		printf( "\tPeakPagefileUsage: 0x%08d\n", 
			pmc.PeakPagefileUsage );
	}


}


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
		// TODO: code your application's behavior here.

		char ch[  ] = "abcdefa";

		wstring strC;

		ConvertToWideBytes( ch, strC );

		IHW104CollectorFactory* pFactory = ktn_GetHW104CollectorFactory();

		for ( int i = 0 ;i<10 ;++i )
		{
			pFactory->CreateIHW104Collector( phwCollector[i] );
		}

		pFactory->CreateIHW104CollectorControl( phwCollectorControl );

		phwCollectorControl->StartCollect();

		Sleep(1000);
	}

	return nRetCode;
}
