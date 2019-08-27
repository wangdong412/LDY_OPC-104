//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: HW104Help.h
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
///			V0.10 : ���� 2009-10-17 11:24 �����ļ�
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
	/// 	��ȡ�����ļ�������·��
	/// </summary>
	static void GetConfigIniPath( std::wstring& wstrIniPath );

	/// <summary>
	/// 	�ж�һ���ļ��Ƿ����(��Ŀ¼)
	/// </summary>
	static bool IsFileExist( const wchar_t* wstrPath );

	/// <summary>
	/// 	��һ�����ֽ��ַ�ת����Ϊһ��std::wstring
	/// </summary>
	static bool ConvertToMultiBytes( const wchar_t* , std::string& );

	/// <summary>
	///   ��һ��˫�ַ�ת����Ϊ���ֽ�
	/// </summary>
	static bool ConvertToWideBytes( const char*, std::wstring& );

	/// <summary>
	/// 	ת��std::wstring�е�Сд��ĸΪ��д��ĸ
	/// </summary>
	static void Upper( std::wstring& );

	/// <summary>
	/// 	ת��std::string�е�Сд��ĸΪ��д��ĸ
	/// </summary>
	static void Upper( std::string& );

	/// <summary>
	///		�ѻ�����ת����ΪIP��ַ
	/// </summary>
	static bool   GetIPAddressByNameA( const char* nodeName, std::vector< std::string >& ipArray );

	//�ж�����ķ����Ƿ�Ϊ����
	//�ǺϷ��������򷵻�true������false
	static bool IsLegalNumber(CString &strInput);  //2010-1-29, ����ǿ���
};

//========================================================================
#endif #define __HW104Help_H_INCLUDED__

