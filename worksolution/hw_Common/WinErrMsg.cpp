//==============================================================================
//
// 项目名 ：SORBA
// 文件名 ：WinErrMsg.cpp
// 作  者 ：高锐强
// 用  途 ：
// 
//==============================================================================
// 版本记录 
//==============================================================================
//
// V0.1 - 高锐强 2005-7-12 11:19:40
// 
// 
//==============================================================================

//==============================================================================
#include "stdafx.h"
#include  <lmerr.h>
//=============================================================================
//==============================================================================

std::string& GetWinErrMsg(DWORD dwErrorCode,std::string& strMsg)
{
	HMODULE hModule = NULL; // default to system source
	DWORD dwBufferLength = 0;
	char* lpMsgBuf = NULL;

	DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_IGNORE_INSERTS |
		FORMAT_MESSAGE_FROM_SYSTEM ;

	//
	// If dwErrorCode is in the network range, 
	//  load the message source.
	//

	if(dwErrorCode >= NERR_BASE && dwErrorCode <= MAX_NERR) 
	{
		hModule = LoadLibraryEx(
			TEXT("netmsg.dll"),
			NULL,
			LOAD_LIBRARY_AS_DATAFILE
			);

		if(hModule != NULL)
			dwFormatFlags |= FORMAT_MESSAGE_FROM_HMODULE;
	}

	//
	// Call FormatMessage() to allow for message 
	//  text to be acquired from the system 
	//  or from the supplied module handle.
	//

	dwBufferLength = FormatMessageA(
		dwFormatFlags,
		hModule, // module to get message from (NULL == system)
		dwErrorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
		(char*)&lpMsgBuf,
		0,
		NULL
		);

	
	//
	// If we loaded a message source, unload it.
	//
	if(hModule != NULL)
		FreeLibrary(hModule);

	if(dwBufferLength > 0)
	{
		//delete "enter"
		size_t strLen = strlen(lpMsgBuf);
		if(lpMsgBuf[strLen-1] == WSTR('\n'))
		{
			lpMsgBuf[strLen-1] = WSTR('\0');
		}
		strMsg = lpMsgBuf;

		LocalFree(lpMsgBuf);
		return strMsg;
	}
	else
	{
		strMsg.clear();
		return strMsg;
	}
}

std::wstring& GetWinErrMsg(DWORD dwErrorCode,std::wstring& strMsg)
{
	HMODULE hModule = NULL; // default to system source
	DWORD dwBufferLength = 0;
	wchar_t* lpMsgBuf = NULL;

	DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_IGNORE_INSERTS |
		FORMAT_MESSAGE_FROM_SYSTEM ;

	//
	// If dwErrorCode is in the network range, 
	//  load the message source.
	//

	if(dwErrorCode >= NERR_BASE && dwErrorCode <= MAX_NERR) 
	{
		hModule = LoadLibraryEx(
			TEXT("netmsg.dll"),
			NULL,
			LOAD_LIBRARY_AS_DATAFILE
			);

		if(hModule != NULL)
			dwFormatFlags |= FORMAT_MESSAGE_FROM_HMODULE;
	}

	//
	// Call FormatMessage() to allow for message 
	//  text to be acquired from the system 
	//  or from the supplied module handle.
	//

	dwBufferLength = FormatMessageW(
		dwFormatFlags,
		hModule, // module to get message from (NULL == system)
		dwErrorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
		(wchar_t*)&lpMsgBuf,
		0,
		NULL
		);

	
	//
	// If we loaded a message source, unload it.
	//
	if(hModule != NULL)
		FreeLibrary(hModule);

	if(dwBufferLength > 0)
	{
		//delete "enter"
		size_t strLen = wcslen(lpMsgBuf);
		if(lpMsgBuf[strLen-1] == WSTR('\n'))
		{
			lpMsgBuf[strLen-1] = WSTR('\0');
		}
		strMsg = lpMsgBuf;

		LocalFree(lpMsgBuf);
		return strMsg;
	}
	else
	{
		strMsg.clear();
		return strMsg;
	}
}

//==============================================================================
//==============================================================================
