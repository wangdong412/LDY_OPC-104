//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	KingModel
///		文件名: LogTest.h
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
///			V0.10 : 王勃 2009-10-31 16:32 创建文件
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

