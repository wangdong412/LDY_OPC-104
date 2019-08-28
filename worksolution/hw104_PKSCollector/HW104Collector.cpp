//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	KingModel
///		文件名: HW104Collector.cpp
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
///			V0.10 : 王勃 2009-10-15 15:03 创建文件
///		</version>
///	</versions>
//========================================================================
#include "StdAfx.h"
#include "HW104ConfigAdapter.h"
#include "HW104Collector.h"

/// <summary>
/// 	配置管理器
/// </summary>
extern NKTNCollector::CHW104ConfigAdapter g_HW104ConfigAdapter;

CHW104Collector::CHW104Collector(void)
{
}

CHW104Collector::~CHW104Collector(void)
{
}

/// <summary>
/// 	销毁采集器
/// </summary>
/// <returns>
///		成功返回KTN_OK,失败返回错误码
/// </returns>
/// <remarks>
///     释放采集器所占有的资源,销毁采集器
/// </remarks>
KTN_RET	CHW104Collector::DelCollector()
{
	delete this;

	return KTN_OK;
}

/// <summary>
/// 	总召唤所有的asdu数组
/// </summary>
/// <param name="infoUnitArray">
///		总召唤所有的信息对象
/// </param>
/// <returns>
///		成功返回KTN_OK,失败返回错误码
/// </returns>
//为多份点表的新功能, 增加输入参数strClientIP,根据IP确定使用哪个点表. 2010-6-8,栾述强修改
KTN_RET CHW104Collector::ReadAllRealData( const std::string* pStrClientIP,
						std::vector< KTNHW104_InfoUnit >& infoUnitArray
						)
{
	std::vector< KTNIECAddress > iecAddressArray;
	std::vector< KTNValue > valueArray;
	KTN_RET ret = g_HW104ConfigAdapter.ReadAllPoint( pStrClientIP, iecAddressArray, valueArray );

	if ( KTNERR(ret) )
	{
		return ret;
	}

	infoUnitArray.clear();
	infoUnitArray.reserve( iecAddressArray.size() );
	for ( unsigned int i = 0; i < iecAddressArray.size(); ++i )
	{//转换成为IEC地址
		KTNHW104_InfoUnit hw104_InfoUnit;
		KTN_RET ret = CHW104ProtcolHelp::GetInfoUnit( 
			iecAddressArray[i],
			valueArray[i],
			hw104_InfoUnit
			);

		if ( KTNERR(ret) )
		{
			continue;
		}

		infoUnitArray.push_back( hw104_InfoUnit );
	}

	return KTN_OK;
}

/// <summary>
/// 	写入指定的数值
/// </summary>
/// <param name="infoUnit">
///		in asdu内容
/// </param>
/// <returns>
///		成功返回KTN_OK,失败返回错误码
/// </returns>
KTN_RET CHW104Collector::WriteInfoUnit(
							  const std::string& ip,
							  const KTNHW104_InfoUnit& infoUnit 
							  ) 
{
	KTNValue ktnValue;
	KTNIECAddress ktnIecAddr;

	KTN_RET ret = CHW104ProtcolHelp::GetIecAddressAndKTNValue( 
					infoUnit,
					ktnValue,
					ktnIecAddr );

	if ( KTNERR(ret) )
	{
		if ( KTN_COLLECTROR_SELECTCOMMAND == ret )
		{
			return KTN_OK;
		}
		return ret;
	}

	ret = g_HW104ConfigAdapter.WriteIecData( ip,ktnIecAddr, ktnValue );

	if ( KTNERR(ret) )
	{
		return ret;
	}

	return KTN_OK;
}


/// <summary>
/// 	注册变化上送函数
/// </summary>
/// <param name="pIHW104CallBack">
///		回调接口
/// </param>
KTN_RET CHW104Collector::RegsiterCallBack( std::string strClientIP,
							  IHW104CollectorCallBack* pIHW104CallBack
							  )
{
	return 	g_HW104ConfigAdapter.RegsiterCallBack(strClientIP, pIHW104CallBack );;
}

/// <summary>
/// 	获得数量
/// </summary>
int CHW104Collector::GetCount()
{
	return g_HW104ConfigAdapter.GetCount();
}

KTN_RET NKTNCollector::CHW104Collector::UpdateLinkErr( KTNCollectorTpye clientType, BOOL bConnect )
{
	return g_HW104ConfigAdapter.WriteLinkErr(clientType, bConnect);
}