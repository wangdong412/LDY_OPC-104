//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: LogTest.cpp
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
///			V0.10 : ���� 2009-10-31 16:33 �����ļ�
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
