//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: HW104Help.cpp
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
///			V0.10 : ���� 2009-10-17 11:25 �����ļ�
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
/// 	��ȡ�����ļ�������·��
/// </summary>
void CHW104Help::GetConfigIniPath( std::wstring& wstrIniPath )
{
	wchar_t wstrFileName[ MAX_PATH_LEN ] = {0}; 

	DWORD dwRet = GetModuleFileName(  NULL,  //ģ���ַ
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
/// 	�ж�һ���ļ��Ƿ����(��Ŀ¼)
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
/// 	��һ�����ֽ��ַ�ת����Ϊһ��std::wstring
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
///   ��һ��˫�ַ�ת����Ϊ���ֽ�
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
/// 	ת��std::wstring�е�Сд��ĸΪ��д��ĸ
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
/// 	ת��std::wstring�е�Сд��ĸΪ��д��ĸ
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
///		�ѻ�����ת����ΪIP��ַ
/// </summary>
bool  CHW104Help::GetIPAddressByNameA( const char* nodeName, std::vector< std::string >& ipArray )
{
	ASSERT( nodeName != NULL );
	WSADATA wsaData;

	if ( 0 != WSAStartup( MAKEWORD(2,2), &wsaData) )
	{
		return false;
	}
	//���IP����
	addrinfo* ipList = NULL;
	int nRet = getaddrinfo( nodeName, 
							NULL,
							NULL,
							&ipList );

	if ( 0!=nRet )
	{
		return false;
	}

	//����������ʼ��
	ipArray.clear();

	addrinfo* ipListIt = ipList;
	do 
	{
		if ( ipListIt != NULL )
		{//��������
			std::string strIpTemp( inet_ntoa( (( sockaddr_in* )( ipListIt->ai_addr ))->sin_addr ) );
			ipArray.push_back( strIpTemp );
			ipListIt = ipListIt->ai_next;
		}
		else
		{//��һ������null
			break;
		}

	} while ( NULL != ipListIt );

	//�ͷŵ�ַ
	freeaddrinfo( ipList );

	return true;
}

//�ж�����ķ����Ƿ�Ϊ����
//�ǺϷ��������򷵻�true������false
bool CHW104Help::IsLegalNumber(CString &strInput)
{
	ASSERT( strInput != _T("") );

	strInput.TrimLeft();
	strInput.TrimRight();
	if(strInput.IsEmpty())
		return false;

	int len = strInput.GetLength();
	TCHAR buffer[MAX_PATH + 1] = {0};   //������������
	int len1, len2;
	//�ж��Ƿ���С����
	if(strInput.Find( _T('.') ) != -1)
	{
		//����С����
		TCHAR bufferDot[MAX_PATH + 1] = {0};    //С����������
		//ƥ�����ֺ�С����
		_stscanf(strInput, _T("%[0-9].%[0-9]"), buffer, bufferDot);
		//�ж�buffer��bufferDot�Ƿ�Ϊ���ҳ���֮���Ƿ�Ϊ�ܳ���-1
		len1= _tcslen(buffer);
		len2 = _tcslen(bufferDot);
		if(len1 == 0 || len2 == 0)  //С��������࣬��Ҫ������
			return false;
		if( len1+len2 != len-1 )
			return false;
		//ȥ��������������ߵ�0
		strInput.Format( _T("%d.%s"), _ttoi(buffer), bufferDot);
	}
	else
	{
		//û��С����
		//ƥ������
		_stscanf(strInput, _T("%[0-9]"), buffer);
		len1 = _tcslen(buffer);
		if(len1 == 0)
			return false;
		if(len1 != len)
			return false;
		//ȥ��������������ߵ�0
		strInput.Format(_T("%d"), _ttoi(buffer));
	}

	return true;
}

