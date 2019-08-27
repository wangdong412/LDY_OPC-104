//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	KingModel
///		文件名: HW104Help.cpp
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
///			V0.10 : 王勃 2009-10-17 11:25 创建文件
///		</version>
///	</versions>
//========================================================================
#include "StdAfx.h"
#include "HW104Help.h"
#include "ws2tcpip.h"

CHW104Help::CHW104Help(void)
{
}

CHW104Help::~CHW104Help(void)
{
}


/// <summary>
/// 	获取配置文件的完整路径
/// </summary>
void CHW104Help::GetConfigIniPath( std::wstring& wstrIniPath )
{
	wchar_t wstrFileName[ MAX_PATH_LEN ] = {0}; 

	DWORD dwRet = GetModuleFileName(  NULL,  //模块地址
									  wstrFileName,
									  MAX_PATH_LEN );
	
	if (  0 == dwRet )
	{
		return;
	}

	for ( DWORD i = dwRet-1; i>0; --i )
	{
		if ( wstrFileName[ i ] == L'\\' )
		{
			wstrFileName[ i+1 ] = 0;
			break;
		}
	}

	wstrIniPath.assign( wstrFileName );
	wstrIniPath += CONFIGFILENAME;
}

/// <summary>
/// 	判断一个文件是否存在(非目录)
/// </summary>
bool CHW104Help::IsFileExist( const wchar_t* wstrPath )
{
	// Data structure for FindFirstFile
	WIN32_FIND_DATA findData;

	// Clear find structure
	ZeroMemory(&findData, sizeof(findData));

	// Search the file
	HANDLE hFind = FindFirstFile( wstrPath, &findData );
	if ( hFind == INVALID_HANDLE_VALUE )
	{
		// File not found
		return false;
	}

	// File found

	// Release find handle
	FindClose( hFind );
	hFind = NULL;

	// The file exists
	return true;

}

/// <summary>
/// 	把一个单字节字符转换成为一个std::wstring
/// </summary>
bool CHW104Help::ConvertToMultiBytes( const wchar_t* pWStr, std::string& str )
{
	DWORD dwNum = WideCharToMultiByte( 
		CP_UTF8,
		0,
		pWStr,
		-1,
		NULL,
		0,
		NULL,
		FALSE
		);

	 char* pStr = new char[ dwNum+1 ];
	 ZeroMemory( pStr, dwNum+1 );

	 WideCharToMultiByte( 
		 CP_UTF8,
		 0,
		 pWStr,
		 -1,
		 pStr,
		 dwNum+1,
		 NULL,
		 FALSE
		 );

	str.assign( pStr );

	 delete []pStr;

	 return true;
}

/// <summary>
///   把一个双字符转换成为单字节
/// </summary>
bool CHW104Help::ConvertToWideBytes( const char* pChar, std::wstring& pwstrOut)
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
		TRACE( "ConvertToWideBytes failed\n" );
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
	delete []pTemp;
	return true;
}

/// <summary>
/// 	转换std::wstring中的小写字母为大写字母
/// </summary>
void CHW104Help::Upper( std::wstring& wstr )
{
	for ( UINT i = 0 ; i < wstr.size(); ++i )
	{
		if ( ( wstr[ i ] <= L'z' ) && ( wstr[ i ] >= L'a' ) )
		{
			wstr[ i ] += L'A' - L'a';
		}
	}
}

/// <summary>
/// 	转换std::wstring中的小写字母为大写字母
/// </summary>
void CHW104Help::Upper( std::string& str )
{
	for ( UINT i = 0 ; i < str.size(); ++i )
	{
		if ( ( str[ i ] <= L'z' ) && ( str[ i ] >= L'a' ) )
		{
			str[ i ] += L'A' - L'a';
		}
	}
}

/// <summary>
///		把机器名转换成为IP地址
/// </summary>
bool  CHW104Help::GetIPAddressByNameA( const char* nodeName, std::vector< std::string >& ipArray )
{
	ASSERT( nodeName != NULL );
	WSADATA wsaData;

	if ( 0 != WSAStartup( MAKEWORD(2,2), &wsaData) )
	{
		return false;
	}
	//获得IP链表
	addrinfo* ipList = NULL;
	int nRet = getaddrinfo( nodeName, 
							NULL,
							NULL,
							&ipList );

	if ( 0!=nRet )
	{
		return false;
	}

	//返回容器初始化
	ipArray.clear();

	addrinfo* ipListIt = ipList;
	do 
	{
		if ( ipListIt != NULL )
		{//正常处理
			std::string strIpTemp( inet_ntoa( (( sockaddr_in* )( ipListIt->ai_addr ))->sin_addr ) );
			ipArray.push_back( strIpTemp );
			ipListIt = ipListIt->ai_next;
		}
		else
		{//第一个就是null
			break;
		}

	} while ( NULL != ipListIt );

	//释放地址
	freeaddrinfo( ipList );

	return true;
}

//判断输入的符号是否为数字
//是合法的数字则返回true，否则false
bool CHW104Help::IsLegalNumber(CString &strInput)
{
	ASSERT( strInput != _T("") );

	strInput.TrimLeft();
	strInput.TrimRight();
	if(strInput.IsEmpty())
		return false;

	int len = strInput.GetLength();
	TCHAR buffer[MAX_PATH + 1] = {0};   //整数部分数字
	int len1, len2;
	//判断是否有小数点
	if(strInput.Find( _T('.') ) != -1)
	{
		//存在小数点
		TCHAR bufferDot[MAX_PATH + 1] = {0};    //小数部分数字
		//匹配数字和小数点
		_stscanf(strInput, _T("%[0-9].%[0-9]"), buffer, bufferDot);
		//判断buffer和bufferDot是否为空且长度之和是否为总长度-1
		len1= _tcslen(buffer);
		len2 = _tcslen(bufferDot);
		if(len1 == 0 || len2 == 0)  //小数点的两侧，都要有数字
			return false;
		if( len1+len2 != len-1 )
			return false;
		//去掉整数部分最左边的0
		strInput.Format( _T("%d.%s"), _ttoi(buffer), bufferDot);
	}
	else
	{
		//没有小数点
		//匹配数字
		_stscanf(strInput, _T("%[0-9]"), buffer);
		len1 = _tcslen(buffer);
		if(len1 == 0)
			return false;
		if(len1 != len)
			return false;
		//去掉整数部分最左边的0
		strInput.Format(_T("%d"), _ttoi(buffer));
	}

	return true;
}

