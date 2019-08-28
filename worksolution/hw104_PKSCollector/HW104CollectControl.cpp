//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	KingModel
///		文件名: HW104CollectControl.cpp
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
///			V0.10 : 王勃 2009-10-15 10:41 创建文件
///		</version>
///	</versions>
//========================================================================
#include "StdAfx.h"
#include "HW104ConfigAdapter.h"
#include "HW104CollectControl.h"
//#include "HW104PKSCollector.h"
//#include "OPCCollector.h"

/// <summary>
/// 	1.数据库采集器
///     2.配置信息加载器
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
/// 	启动采集
/// </summary>
/// <returns>
///		成功返回KTN_OK,失败返回错误码
/// </returns>
/// <remarks>
///     每次启动之前必须通过这个函数通知采集器开始采集
/// </remarks>
KTN_RET	 CHW104CollectControl::StartCollect()
{
	g_HW104Log.Open( PKSLOGNAME );
	KTN_RET ret;

	///************************************************************************/
	///* 1. 加载所有的配置信息(主要是为了下发数据库变量点)
	///* 2. 启动采集
	///************************************************************************/
	////获取配置文件路径
	//std::wstring wstrIniPath;
	//CHW104Help::GetConfigIniPath( wstrIniPath );

	//wchar_t wstrThresHold[ MAX_PATH_LEN ] = {0};

	//GetPrivateProfileString( SECTIONNAME,		  //section name		
	//	THRESHOLD,								  //key name
	//	L"0.1",									  //default value
	//	wstrThresHold,		  
	//	MAX_PATH_LEN,
	//	wstrIniPath.c_str() );

	////格式化阈值
	//swscanf_s( wstrThresHold, L"%f", &( g_HW104DefaultThresHold.thresHold ) );

	//if ( g_HW104DefaultThresHold.thresHold < 0 || g_HW104DefaultThresHold.thresHold > 100 )
	//{
	//	g_HW104DefaultThresHold.thresHold = (float)DEFAULT_THRESHOLD;
	//}

	//swprintf_s( wstrThresHold, MAX_PATH_LEN, L"%f PercentTage",g_HW104DefaultThresHold.thresHold  );
	//HW104PKSLog( L"CHW104PKSCollector g_HW104DefaultThresHold",wstrThresHold );

	////加载采集器相关信息
	//KTN_RET ret = g_PKSCollector.LoadConfigInfo();

	//if ( KTNERR( ret ) )
	//{
	//	HW104PKSLog( L"CHW104PKSCollector",L"LoadCollectorConfigInfo error" );
	//	return ret;
	//}

	//加载104配置信息的相关信息
	ret = g_HW104ConfigAdapter.LoadKTNConfig();

	if ( KTNERR(ret) )
	{
		HW104PKSLog( L"CHW104PKSCollector",L"LoadProxyConfigInfo error" );
		return ret;
	}

	//启动采集
	ret = g_HW104ConfigAdapter.StartCollect();
	if ( KTNERR(ret) )
	{
		return ret;
	}

	HW104PKSLog( L"CHW104PKSCollector",L"StartCollect OK" );
	return KTN_OK;
}

/// <summary>
/// 	停止采集
/// </summary>
/// <returns>
///		成功返回KTN_OK,失败返回错误码
/// </returns>
/// <remarks>
///     每次退出时必须通过这个函数停止采集并释放相关资源
/// </remarks>
KTN_RET	CHW104CollectControl::EndCollect()
{
	g_HW104ConfigAdapter.EndCollect();

	return KTN_OK;
}

/// <summary>
/// 	获得本机的冗余状态
/// </summary>
/// <param name="redunStatus">
///		本机的冗余状态
/// </param>
KTN_RET CHW104CollectControl::GetRedunStatus(
							    KTNRedunStatus& redunStatus )
{
	return KTN_OK;
}