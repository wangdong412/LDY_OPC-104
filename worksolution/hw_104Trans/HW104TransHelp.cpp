//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	104Trans
///		文件名: HW104TransHelp.cpp
///	</file>
///	<versions>
///		<version>
///			V0.10 :  [11/9/2009] 创建文件
///		</version>
///	</versions>
//========================================================================
#include "StdAfx.h"
#include "HW104TransHelp.h"
#include "DataType.h"

CHW104TransHelp::CHW104TransHelp(void)
{
}

CHW104TransHelp::~CHW104TransHelp(void)
{
}

/// <summary>
/// 	获得配置文件的路径
/// </summary>
void CHW104TransHelp::GetIniPath( std::string& strIniPath )
{
	char strFileName[ 256 ] = {0}; 

	DWORD dwRet = GetModuleFileNameA(  NULL,  //模块地址
		strFileName,
		256 );

	for ( DWORD i = dwRet-1; i>0; --i )
	{
		if ( strFileName[ i ] == '\\' )
		{
			strFileName[ i+1 ] = 0;
			break;
		}
	}
	//配置文件路径
	strIniPath.assign( strFileName );	
}

/// <summary>
/// 	获得配置文件的路径
/// </summary>
void CHW104TransHelp::GetIniPath( std::wstring& wstrIniPath )
{
	wchar_t wstrFileName[ 256 ] = {0}; 

	DWORD dwRet = GetModuleFileNameW(  NULL,  //模块地址
		wstrFileName,
		256 );

	for ( DWORD i = dwRet-1; i>0; --i )
	{
		if ( wstrFileName[ i ] == L'\\' )
		{
			wstrFileName[ i+1 ] = 0;
			break;
		}
	}
	//配置文件路径
	wstrIniPath.assign( wstrFileName );	
}

///判断一个文件是否存在
bool CHW104TransHelp::IsFileExist( const wchar_t* wstrPath )
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

///将DWORD以36进制编码的形式表示
CString CHW104TransHelp::Code36_DWORD(DWORD dwVal)
{
	TCHAR cCode36[36] = { _T('0'), _T('1'), _T('2'), _T('3'), _T('4'), _T('5'), _T('6'),
		_T('7'), _T('8'), _T('9'), _T('A'), _T('B'), _T('C'), _T('D'),
		_T('E'), _T('F'), _T('G'), _T('H'), _T('I'), _T('J'), _T('K'),
		_T('L'), _T('M'), _T('N'), _T('O'), _T('P'), _T('Q'), _T('R'),
		_T('S'), _T('T'), _T('U'), _T('V'), _T('W'), _T('X'), _T('Y'),
		_T('Z') };

	CString strCode36 = _T("");
	int nTemp = 0;
	for (int i=0; i<6; i++) //取6位36进制编码
	{
		nTemp =	dwVal % 36;
		strCode36.Insert(0, cCode36[nTemp]);
		dwVal = dwVal / 36;
	}

	return strCode36;
}