//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	KingModel
///		文件名: LogTest.cpp
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
///			V0.10 : 王勃 2009-10-31 16:33 创建文件
///		</version>
///	</versions>
//========================================================================
#include "StdAfx.h"
#include "LogTest.h"

CLogTest::CLogTest(void)
{
}

CLogTest::~CLogTest(void)
{
}

void CLogTest::Run()
{
	m_104Log.Open( L"logTest.log" );

	for ( int i = 0; i<50;++i )
	{
		HW104LOG( m_104Log, L"tst", L"TestMsg" );
	}
}
