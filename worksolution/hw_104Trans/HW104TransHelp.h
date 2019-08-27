//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	104Trans
///		文件名: HW104TransHelp.h
///		
///	</file>
///	<versions>
///		<version>
///			V0.10 :  [11/9/2009] 创建文件
///		</version>
///	</versions>
//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _HW104TRANSHELP_H_INCLUDED__
#define _HW104TRANSHELP_H_INCLUDED__

#endif #define _HW104TRANSHELP_H_INCLUDED__

class CHW104TransHelp
{
public:
	CHW104TransHelp(void);
	~CHW104TransHelp(void);

public:
	/// <summary>
	/// 	获得配置文件的路径
	/// </summary>
	static void GetIniPath( std::wstring&  );
	static void GetIniPath( std::string& );

	///判断一个文件是否存在
	static bool IsFileExist( const wchar_t* wstrPath );
	///将DWORD以36进制编码的形式表示
	static CString Code36_DWORD(DWORD dwVal);
};
