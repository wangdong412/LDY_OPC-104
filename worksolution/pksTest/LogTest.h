//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: LogTest.h
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
///			V0.10 : ���� 2009-10-31 16:32 �����ļ�
///		</version>
///	</versions>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "hw104_Common/hw104Common_Ext_inc.h"

#ifndef __LogTest_H_INCLUDED__
#define __LogTest_H_INCLUDED__
//========================================================================

class CLogTest
{
public:
	CLogTest(void);
	~CLogTest(void);

public:
	void Run();

private:
	CHW104_Log m_104Log;
};


//========================================================================
#endif #define __LogTest_H_INCLUDED__

