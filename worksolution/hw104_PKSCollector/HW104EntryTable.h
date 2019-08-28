//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: HW104EntryTable.h
///		��  ��: ����
///	</file>
///	<copyright>
/// 	���������а�ȨΪ�ǿؿƼ����У��κθ��˻�����֯�������κη�ʽ���ơ�ת����Ų��
/// ���ã����򽫹��ɶ��ǿؿƼ��Ĳ�Ȩ֪ʶ��Ȩ��
///	
///		��ϵ: http://www.kingview.com/
///	</copyright>
///	<versions>
///		<version>
///			V0.10 : ���� 2009-11-2 19:59 �����ļ�
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
	/// 	��ʼ��
	/// </summary>
	bool Initial(); 

	/// <summary>
	/// 	���Ҷ�Ӧ��IEC
	/// </summary>
	/// <param name="registerName">
	///		�Ĵ�������
	/// </param>
	/// <param name="ktnPoint">
	///		out ���ݵ�����
	/// </param>
	/// <param name="ktnIecAddress">
	///		out IEC��ַ��
	/// </param>
	bool GetIecAddressAndPKSPoint( 
		const std::wstring& registerName,
		KTNPKSPoint&		ktnPoint,
		KTNIECAddress&		ktnIecAddress
		);
};

//========================================================================
#endif #define __HW104EntryTable_H_INCLUDED__


