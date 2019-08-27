//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	KingModel
///		文件名: HW_104Log.cpp
///		作  者: 王勃
///	</file>
///	<copyright>
/// 	本代码所有版权为所有，任何个人或者组织不得以任何方式复制、转载以挪作
/// 他用，否则将构成对的产权知识侵权。
///	
///		
///	</copyright>
///	<versions>
///		<version>
///			V0.10 : 王勃 2009-10-30 15:42 创建文件
///		</version>
///	</versions>
//========================================================================
#include "stdafx.h"
#include "hw104_Common/hw104Common_Ext_inc.h"

std::wstring& GetWinErrMsg(DWORD dwErrorCode,std::wstring& strMsg);

/************************************************************************/
/* 构造析构函数                                                                     
/************************************************************************/
CHW104_Log::CHW104_Log()
{
	GetModuleFileNameW(NULL,m_wszTraceFileDirectory,MAX_PATH_LEN );

	LPWSTR pwszEsc = wcsrchr( m_wszTraceFileDirectory, L'\\' );
	if( NULL != pwszEsc )
		ZeroMemory(pwszEsc+1,wcslen(pwszEsc)-1);
	wcscat_s( m_wszTraceFileDirectory, MAX_PATH_LEN, L"log\\" );	//日志输出到log文件夹下
	if ( wcslen(m_wszTraceFileDirectory ) <= 0)
	{
		wcscpy_s( m_wszTraceFileDirectory, MAX_PATH, DEFAULT_TRACE_DIRECTORY ); 
	}

	ZeroMemory( m_wszLogFileName, MAX_PATH_LEN * sizeof(WCHAR) );

	m_hLogFile = INVALID_HANDLE_VALUE;
}

CHW104_Log::~CHW104_Log()
{
	Close();
}

void CHW104_Log::Open( const WCHAR* wszLogFileName )
{
	NkxLatchLock::XDBAutoSpinLock _lock( m_TraceLock ); 

	if ( INVALID_HANDLE_VALUE == m_hLogFile)
	{
		BOOL directoryExist = CreateDirectoryW(m_wszTraceFileDirectory,NULL);

		if( ( false == directoryExist )&&( ERROR_ALREADY_EXISTS != ::GetLastError() ) )
		{
			return ;
		}

		wchar_t wszFileName[ MAX_PATH_LEN ] = {0};

		wcscpy_s( m_wszLogFileName, MAX_PATH_LEN, wszLogFileName );
		wcscat_s( wszFileName, MAX_PATH_LEN, m_wszTraceFileDirectory);
		wcscat_s( wszFileName, MAX_PATH_LEN, m_wszLogFileName );

		// 打开文件
		m_hLogFile = CreateFileW( 
			wszFileName ,  
			GENERIC_WRITE ,  
			FILE_SHARE_READ | FILE_SHARE_WRITE,  
			NULL ,  
			OPEN_ALWAYS,  
			FILE_ATTRIBUTE_NORMAL, 
			NULL ); 

		if ( INVALID_HANDLE_VALUE == m_hLogFile )
		{
			return;
		}
	}
}

void CHW104_Log::Close()
{
	if ( 	m_hLogFile != INVALID_HANDLE_VALUE )
	{
		CloseHandle( m_hLogFile );
		m_hLogFile = INVALID_HANDLE_VALUE;
	}
}

void CHW104_Log::WriteLog( const wchar_t* wszLog , int nLogWLen  )
{
	NkxLatchLock::XDBAutoSpinLock _lock( m_TraceLock ); 

	if ( INVALID_HANDLE_VALUE != m_hLogFile )
	{
		SetFilePointer( m_hLogFile,  0L , NULL, FILE_END);

		// 检查文件大小
		if( GetFileSize( m_hLogFile, NULL ) >= MAX_TRACE_FILE_SIZE )
		{
			SetFilePointer( m_hLogFile, 0L, NULL, FILE_BEGIN ); 
			SetEndOfFile( m_hLogFile ); 
		}

		// 再次检查文件大小
		if( GetFileSize( m_hLogFile, NULL ) == 0 )
		{
			DWORD Bytes = 0; 
			::WriteFile( m_hLogFile, UNICODE_BOM, 2, &Bytes, NULL ); 	
			//		::FlushFileBuffers( hFile ); 
		}

		// 输出日志到文件
		DWORD dwBytesWritten = 0; 

		if( !WriteFile( 	m_hLogFile, 
			wszLog,  
		    nLogWLen * sizeof(wchar_t),
			&dwBytesWritten,  
			NULL ) )
		{
			return; 
		}
	}
}

/// <summary> 
/// 从文件路径中提取文件名
/// </summary>
KTN_COMMON_STDEXT const WCHAR* GetOnlyFileName( const WCHAR* FilePath  ); 

/// <summary> 
/// 从文件路径中提取文件名
/// </summary>
const WCHAR* GetOnlyFileName( const WCHAR* FilePath  )
{
	if( FilePath == NULL )
		return L"";

	// 提取文件名
	const WCHAR* p = wcsrchr( FilePath,L'\\' );
	if( p == NULL )return FilePath;

	// 跳过最后的分隔符
	p++;
	if( p == NULL )
		return L"";
	else
		return p;
}

/// <summary> 
///		输出信息到指定文件中CDECL
/// </summary>
/// <param name="_TraceHelper">
///		日志输出类
/// </param>
/// <param name="lpszFormat">
///		日志输出信息
/// </param>
void  SvcTrace( CHW104_Log& _Helper , wchar_t* args ) 
{
	/// 输出缓冲区，加上日期输出32个字符
	WCHAR	wszBuffer[  MAX_TRACE_BUFFER_LEN + 32  ] = {0} ; 

	size_t cbBuffer = _countof( wszBuffer ); 

	int nPos = 0; 
	int nRet = 0; 

	// 记录时间
	SYSTEMTIME	localTime = { 0 }; 
	GetLocalTime( &localTime ); 

	nRet = _snwprintf_s( 
		wszBuffer , 
		cbBuffer,
		cbBuffer,
		WSTR( "[  %05d-%05d  ] %4d-%02d-%02d %02d:%02d:%02d:%03d " ), 
		GetCurrentProcessId( ), 
		GetCurrentThreadId( ),  
		localTime.wYear ,  
		localTime.wMonth ,  
		localTime.wDay ,  
		localTime.wHour ,  
		localTime.wMinute,  
		localTime.wSecond,  
		localTime.wMilliseconds ); 

	if( nRet < 0 ) return;  // error
	nPos += nRet; 

	// 格式化字符串
	nRet =  swprintf_s( wszBuffer+nPos , cbBuffer - nPos - 4,  L"%s ",  args ); 

	if( nRet < 0 ) return ;  // error
	nPos += nRet; 

	/// 正确处理结尾的'\r''\n'
	int nDest = nPos; 
	if( wszBuffer[  nPos-2  ] == WSTR( '\r' ) || wszBuffer[  nPos-2  ] == WSTR( '\n' ) )
		nPos -= 2; 
	else if( wszBuffer[  nPos-1  ] == WSTR( '\r' ) || wszBuffer[  nPos-1  ] == WSTR( '\n' ) )
		nPos -= 1; 
	wszBuffer[  nPos  ]   = WSTR( '\r' ); 
	wszBuffer[  nPos+1  ] = WSTR( '\n' ); 
	wszBuffer[  nPos+2  ] = WSTR( '\0' ); 
	nPos+=2; 

	_Helper.WriteLog( wszBuffer , nPos );
}



/// <summary>
///		日志输出信息。
/// </summary>
/// <param name="Category">
///		日志输出类型
/// </param>
/// <param name="ClientInfor">
///		Client端信息
/// </param>
/// <param name="File">
///		文件名
/// </param>
/// <param name="Line">
///		行号
/// </param>
/// <param name="Msg">
///		日志信息
/// </param>
/// <param name="WinErrCode">
///		Windows错误信息
/// </param>
KTN_COMMON_STDEXT void hw104f_LogMessage( IN CHW104_Log&			 _logHelper,
					 IN const std::wstring&  Category,
					 IN const std::wstring&	 File, 
					 IN const DWORD			 Line, 
					 IN std::wstring		 Msg
					 )
{
	CString strLog; 
	//去掉多余信息的输出, 2010.11.1 栾述强修改
	/*DWORD dwError = ::GetLastError();
	if ( dwError == 0 )
	{
		strLog.Format( TEXT( "Category:%s  Msg:%s   %s[  %d  ].\n" ), Category.c_str() , Msg.c_str( ),  File.c_str( ), Line ); 
	}
	else
	{
		std::wstring wstrError;
		GetWinErrMsg( dwError, wstrError );
		strLog.Format( TEXT( "%s :Msg:%s .%s[  %d  ]\n	[  WinERR%d  ] %s\n" ), Category.c_str() , Msg.c_str( ), File.c_str( ), Line, dwError, wstrError.c_str() ); 
	}*/
	strLog.Format( TEXT("Msg: %s    ---[%s].\n"), Msg.c_str(), Category.c_str() );

	SvcTrace( _logHelper, strLog.GetBuffer( ) ); 
	strLog.ReleaseBuffer( ); 
}


//用于输出接收及发送的buf的数据
KTN_COMMON_STDEXT void hw104f_LogStream(   IN KTNCOMMON::CHW104_Log&		 _TraceHelper, 		
										   IN std::wstring					 Msg
										   )
{
	CString strLog; 
	strLog.Format( TEXT( "%s\n" ), Msg.c_str() ); 
	
	SvcTrace( _TraceHelper, strLog.GetBuffer() ); 
	strLog.ReleaseBuffer( ); 
}