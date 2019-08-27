//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: HW_104Log.cpp
///		��  ��: ����
///	</file>
///	<copyright>
/// 	���������а�ȨΪ���У��κθ��˻�����֯�������κη�ʽ���ơ�ת����Ų��
/// ���ã����򽫹��ɶԵĲ�Ȩ֪ʶ��Ȩ��
///	
///		
///	</copyright>
///	<versions>
///		<version>
///			V0.10 : ���� 2009-10-30 15:42 �����ļ�
///		</version>
///	</versions>
//========================================================================
#include "stdafx.h"
#include "hw104_Common/hw104Common_Ext_inc.h"

std::wstring& GetWinErrMsg(DWORD dwErrorCode,std::wstring& strMsg);

/************************************************************************/
/* ������������                                                                     
/************************************************************************/
CHW104_Log::CHW104_Log()
{
	GetModuleFileNameW(NULL,m_wszTraceFileDirectory,MAX_PATH_LEN );

	LPWSTR pwszEsc = wcsrchr( m_wszTraceFileDirectory, L'\\' );
	if( NULL != pwszEsc )
		ZeroMemory(pwszEsc+1,wcslen(pwszEsc)-1);
	wcscat_s( m_wszTraceFileDirectory, MAX_PATH_LEN, L"log\\" );	//��־�����log�ļ�����
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

		// ���ļ�
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

		// ����ļ���С
		if( GetFileSize( m_hLogFile, NULL ) >= MAX_TRACE_FILE_SIZE )
		{
			SetFilePointer( m_hLogFile, 0L, NULL, FILE_BEGIN ); 
			SetEndOfFile( m_hLogFile ); 
		}

		// �ٴμ���ļ���С
		if( GetFileSize( m_hLogFile, NULL ) == 0 )
		{
			DWORD Bytes = 0; 
			::WriteFile( m_hLogFile, UNICODE_BOM, 2, &Bytes, NULL ); 	
			//		::FlushFileBuffers( hFile ); 
		}

		// �����־���ļ�
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
/// ���ļ�·������ȡ�ļ���
/// </summary>
KTN_COMMON_STDEXT const WCHAR* GetOnlyFileName( const WCHAR* FilePath  ); 

/// <summary> 
/// ���ļ�·������ȡ�ļ���
/// </summary>
const WCHAR* GetOnlyFileName( const WCHAR* FilePath  )
{
	if( FilePath == NULL )
		return L"";

	// ��ȡ�ļ���
	const WCHAR* p = wcsrchr( FilePath,L'\\' );
	if( p == NULL )return FilePath;

	// �������ķָ���
	p++;
	if( p == NULL )
		return L"";
	else
		return p;
}

/// <summary> 
///		�����Ϣ��ָ���ļ���CDECL
/// </summary>
/// <param name="_TraceHelper">
///		��־�����
/// </param>
/// <param name="lpszFormat">
///		��־�����Ϣ
/// </param>
void  SvcTrace( CHW104_Log& _Helper , wchar_t* args ) 
{
	/// ����������������������32���ַ�
	WCHAR	wszBuffer[  MAX_TRACE_BUFFER_LEN + 32  ] = {0} ; 

	size_t cbBuffer = _countof( wszBuffer ); 

	int nPos = 0; 
	int nRet = 0; 

	// ��¼ʱ��
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

	// ��ʽ���ַ���
	nRet =  swprintf_s( wszBuffer+nPos , cbBuffer - nPos - 4,  L"%s ",  args ); 

	if( nRet < 0 ) return ;  // error
	nPos += nRet; 

	/// ��ȷ�����β��'\r''\n'
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
///		��־�����Ϣ��
/// </summary>
/// <param name="Category">
///		��־�������
/// </param>
/// <param name="ClientInfor">
///		Client����Ϣ
/// </param>
/// <param name="File">
///		�ļ���
/// </param>
/// <param name="Line">
///		�к�
/// </param>
/// <param name="Msg">
///		��־��Ϣ
/// </param>
/// <param name="WinErrCode">
///		Windows������Ϣ
/// </param>
KTN_COMMON_STDEXT void hw104f_LogMessage( IN CHW104_Log&			 _logHelper,
					 IN const std::wstring&  Category,
					 IN const std::wstring&	 File, 
					 IN const DWORD			 Line, 
					 IN std::wstring		 Msg
					 )
{
	CString strLog; 
	//ȥ��������Ϣ�����, 2010.11.1 ����ǿ�޸�
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


//����������ռ����͵�buf������
KTN_COMMON_STDEXT void hw104f_LogStream(   IN KTNCOMMON::CHW104_Log&		 _TraceHelper, 		
										   IN std::wstring					 Msg
										   )
{
	CString strLog; 
	strLog.Format( TEXT( "%s\n" ), Msg.c_str() ); 
	
	SvcTrace( _TraceHelper, strLog.GetBuffer() ); 
	strLog.ReleaseBuffer( ); 
}