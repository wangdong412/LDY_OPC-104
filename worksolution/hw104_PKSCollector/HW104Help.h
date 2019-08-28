//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	KingModel
///		文件名: HW104Help.h
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
///			V0.10 : 王勃 2009-10-17 11:24 创建文件
///		</version>
///	</versions>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HW104CollectorCore_Inc.h"

#ifndef __HW104Help_H_INCLUDED__
#define __HW104Help_H_INCLUDED__
//========================================================================

class CHW104Help
{
public:
	CHW104Help(void);
	~CHW104Help(void);

public:
	/// <summary>
	/// 	获取配置文件的完整路径
	/// </summary>
	static void GetConfigIniPath( std::wstring& wstrIniPath );

	/// <summary>
	/// 	判断一个文件是否存在(非目录)
	/// </summary>
	static bool IsFileExist( const wchar_t* wstrPath );

	/// <summary>
	/// 	把一个单字节字符转换成为一个std::wstring
	/// </summary>
	static bool ConvertToMultiBytes( const wchar_t* , std::string& );

	/// <summary>
	///   把一个双字符转换成为单字节
	/// </summary>
	static bool ConvertToWideBytes( const char*, std::wstring& );

	/// <summary>
	/// 	转换std::wstring中的小写字母为大写字母
	/// </summary>
	static void Upper( std::wstring& );

	/// <summary>
	/// 	转换std::string中的小写字母为大写字母
	/// </summary>
	static void Upper( std::string& );

	/// <summary>
	///		把机器名转换成为IP地址
	/// </summary>
	static bool   GetIPAddressByNameA( const char* nodeName, std::vector< std::string >& ipArray );

	//判断输入的符号是否为数字
	//是合法的数字则返回true，否则false
	static bool IsLegalNumber(CString &strInput);  //2010-1-29, 栾述强添加
};

//========================================================================
#endif #define __HW104Help_H_INCLUDED__

