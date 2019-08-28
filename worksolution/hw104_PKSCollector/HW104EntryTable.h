//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	KingModel
///		文件名: HW104EntryTable.h
///		作  者: 王勃
///	</file>
///	<copyright>
/// 	本代码所有版权为亚控科技所有，任何个人或者组织不得以任何方式复制、转载以挪作
/// 他用，否则将构成对亚控科技的产权知识侵权。
///	
///		联系: http://www.kingview.com/
///	</copyright>
///	<versions>
///		<version>
///			V0.10 : 王勃 2009-11-2 19:59 创建文件
///		</version>
///	</versions>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HW104CollectorCore_Inc.h"

#ifndef __HW104EntryTable_H_INCLUDED__
#define __HW104EntryTable_H_INCLUDED__
//========================================================================

class CHW104EntryTable
{
public:
	CHW104EntryTable(void);
	~CHW104EntryTable(void);

public:
	/// <summary>
	/// 	初始化
	/// </summary>
	bool Initial(); 

	/// <summary>
	/// 	查找对应的IEC
	/// </summary>
	/// <param name="registerName">
	///		寄存器名称
	/// </param>
	/// <param name="ktnPoint">
	///		out 数据点名称
	/// </param>
	/// <param name="ktnIecAddress">
	///		out IEC地址表
	/// </param>
	bool GetIecAddressAndPKSPoint( 
		const std::wstring& registerName,
		KTNPKSPoint&		ktnPoint,
		KTNIECAddress&		ktnIecAddress
		);
};

//========================================================================
#endif #define __HW104EntryTable_H_INCLUDED__


