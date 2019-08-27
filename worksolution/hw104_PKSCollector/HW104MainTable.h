//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	104Trans
///		�ļ���: HW104MainTable.h
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
///			V0.10 : ���� [11/4/2009] �����ļ�
///		</version>
///	</versions>
//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif

#include "HW104CollectorCore_Inc.h"

#include <map>
#include <string>

#ifndef _HW104MAINTABLE_H_INCLUDED__
#define _HW104MAINTABLE_H_INCLUDED__

#define TABLESECTIONNAME	L"HW104Excel"
#define FILEPATHKEY			L"Path1"
#define MTABLESHEETNAME		L"104Addr Structure"
/// <summary>
/// 	����
/// </summary>
#define REGADDRCOLNAME		L"�̴�����ַ"
#define	IECADDRCOLNAME		L"IEC��ַ"
#define DEVCOLNAME			L"������Դ"

/// <summary>
/// 	key = register name
///     value = iec address
/// </summary>
typedef std::map< std::wstring, int >	RegisterIecMap;
typedef RegisterIecMap::iterator		RegisterIecMapIt;
typedef RegisterIecMap::const_iterator	RegisterIecMapCIt;

/// <summary>
/// 	key		= device type && sheet name
///     value   = registerIecMap
/// </summary>
typedef std::map< std::wstring, RegisterIecMap > DeviceRegisterIecMap;
typedef DeviceRegisterIecMap::iterator		  DeviceRegisterIecMapIt;
typedef DeviceRegisterIecMap::const_iterator  DeviceRegisterIecMapCIt;


class CHW104MainTable
{
public:
	CHW104MainTable(void);
	~CHW104MainTable(void);

public:
	/// <summary>
	/// 	����HW104�ܱ�
	/// </summary>
	BOOL LoadTable();

	/// <summary>
	/// 	���ݼĴ�����ַ,�豸���ͻ�ȡiec��ַ
	/// </summary>
	BOOL GetIecAddress( 
		const std::wstring& registerAddr, 
		const CString& deviceType,
		int& nIecAddress );

	/// <summary>
	/// 	ö�����е��豸����
	/// </summary>
	void EnumDeviceType(
		std::set< CString >& );

private:
	/// <remarks>
	///     index = sheetname
	/// </remarks>
	DeviceRegisterIecMap m_DeviceRegisterIecMap;
};


#endif #define _HW104MAINTABLE_H_INCLUDED__

