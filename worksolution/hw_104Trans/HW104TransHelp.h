//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	104Trans
///		�ļ���: HW104TransHelp.h
///		
///	</file>
///	<versions>
///		<version>
///			V0.10 :  [11/9/2009] �����ļ�
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
	/// 	��������ļ���·��
	/// </summary>
	static void GetIniPath( std::wstring&  );
	static void GetIniPath( std::string& );

	///�ж�һ���ļ��Ƿ����
	static bool IsFileExist( const wchar_t* wstrPath );
	///��DWORD��36���Ʊ������ʽ��ʾ
	static CString Code36_DWORD(DWORD dwVal);
};
