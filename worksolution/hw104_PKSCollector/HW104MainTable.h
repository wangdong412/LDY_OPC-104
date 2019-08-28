//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	104Trans
///		文件名: HW104MainTable.h
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
///			V0.10 : 王勃 [11/4/2009] 创建文件
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
/// 	列名
/// </summary>
#define REGADDRCOLNAME		L"继存器地址"
#define	IECADDRCOLNAME		L"IEC地址"
#define DEVCOLNAME			L"数据来源"

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
	/// 	加载HW104总表
	/// </summary>
	BOOL LoadTable();

	/// <summary>
	/// 	根据寄存器地址,设备类型获取iec地址
	/// </summary>
	BOOL GetIecAddress( 
		const std::wstring& registerAddr, 
		const CString& deviceType,
		int& nIecAddress );

	/// <summary>
	/// 	枚举所有的设备类型
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

