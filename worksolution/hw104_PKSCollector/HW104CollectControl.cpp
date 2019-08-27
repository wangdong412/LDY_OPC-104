//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: HW104CollectControl.cpp
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
///			V0.10 : ���� 2009-10-15 10:41 �����ļ�
///		</version>
///	</versions>
//========================================================================
#include "StdAfx.h"
#include "HW104ConfigAdapter.h"
#include "HW104CollectControl.h"
//#include "HW104PKSCollector.h"
//#include "OPCCollector.h"

/// <summary>
/// 	1.���ݿ�ɼ���
///     2.������Ϣ������
/// </summary>
//extern CHW104PKSCollector g_PKSCollector;
extern CHW104ConfigAdapter g_HW104ConfigAdapter;

extern CHW104_Log        g_HW104Log;
extern KTNThresHold		 g_HW104DefaultThresHold;

CHW104CollectControl::CHW104CollectControl(void)
{
}

CHW104CollectControl::~CHW104CollectControl(void)
{
}

/// <summary>
/// 	�����ɼ�
/// </summary>
/// <returns>
///		�ɹ�����KTN_OK,ʧ�ܷ��ش�����
/// </returns>
/// <remarks>
///     ÿ������֮ǰ����ͨ���������֪ͨ�ɼ�����ʼ�ɼ�
/// </remarks>
KTN_RET	 CHW104CollectControl::StartCollect()
{
	g_HW104Log.Open( PKSLOGNAME );
	KTN_RET ret;

	///************************************************************************/
	///* 1. �������е�������Ϣ(��Ҫ��Ϊ���·����ݿ������)
	///* 2. �����ɼ�
	///************************************************************************/
	////��ȡ�����ļ�·��
	//std::wstring wstrIniPath;
	//CHW104Help::GetConfigIniPath( wstrIniPath );

	//wchar_t wstrThresHold[ MAX_PATH_LEN ] = {0};

	//GetPrivateProfileString( SECTIONNAME,		  //section name		
	//	THRESHOLD,								  //key name
	//	L"0.1",									  //default value
	//	wstrThresHold,		  
	//	MAX_PATH_LEN,
	//	wstrIniPath.c_str() );

	////��ʽ����ֵ
	//swscanf_s( wstrThresHold, L"%f", &( g_HW104DefaultThresHold.thresHold ) );

	//if ( g_HW104DefaultThresHold.thresHold < 0 || g_HW104DefaultThresHold.thresHold > 100 )
	//{
	//	g_HW104DefaultThresHold.thresHold = (float)DEFAULT_THRESHOLD;
	//}

	//swprintf_s( wstrThresHold, MAX_PATH_LEN, L"%f PercentTage",g_HW104DefaultThresHold.thresHold  );
	//HW104PKSLog( L"CHW104PKSCollector g_HW104DefaultThresHold",wstrThresHold );

	////���زɼ��������Ϣ
	//KTN_RET ret = g_PKSCollector.LoadConfigInfo();

	//if ( KTNERR( ret ) )
	//{
	//	HW104PKSLog( L"CHW104PKSCollector",L"LoadCollectorConfigInfo error" );
	//	return ret;
	//}

	//����104������Ϣ�������Ϣ
	ret = g_HW104ConfigAdapter.LoadKTNConfig();

	if ( KTNERR(ret) )
	{
		HW104PKSLog( L"CHW104PKSCollector",L"LoadProxyConfigInfo error" );
		return ret;
	}

	//�����ɼ�
	ret = g_HW104ConfigAdapter.StartCollect();
	if ( KTNERR(ret) )
	{
		return ret;
	}

	HW104PKSLog( L"CHW104PKSCollector",L"StartCollect OK" );
	return KTN_OK;
}

/// <summary>
/// 	ֹͣ�ɼ�
/// </summary>
/// <returns>
///		�ɹ�����KTN_OK,ʧ�ܷ��ش�����
/// </returns>
/// <remarks>
///     ÿ���˳�ʱ����ͨ���������ֹͣ�ɼ����ͷ������Դ
/// </remarks>
KTN_RET	CHW104CollectControl::EndCollect()
{
	g_HW104ConfigAdapter.EndCollect();

	return KTN_OK;
}

/// <summary>
/// 	��ñ���������״̬
/// </summary>
/// <param name="redunStatus">
///		����������״̬
/// </param>
KTN_RET CHW104CollectControl::GetRedunStatus(
							    KTNRedunStatus& redunStatus )
{
	return KTN_OK;
}