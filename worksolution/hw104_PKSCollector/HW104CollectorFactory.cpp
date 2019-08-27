//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	KingModel
///		文件名: HW104CollectorFactory.cpp
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
///			V0.10 : 王勃 2009-10-15 10:11 创建文件
///		</version>
///	</versions>
//========================================================================
#include "StdAfx.h"
#include "HW104CollectorFactory.h"
#include "HW104Collector.h"
#include "HW104CollectControl.h"

CHW104CollectorFactory g_HW104CollectorFactory;

/// <summary>
/// 	全局导出函数,导出类厂
/// </summary>
IHW104CollectorFactory* ktn_GetHW104CollectorFactory()
{
	return (IHW104CollectorFactory*)(&g_HW104CollectorFactory);
}

CHW104CollectorFactory::CHW104CollectorFactory(void)
{
}

CHW104CollectorFactory::~CHW104CollectorFactory(void)
{
}

/// <summary>
/// 	创建采集控制器
/// </summary>
/// <param name="pIHW104CollectorControl">
///		out 采集控制器
/// </param>
/// <returns>
///		成功返回KTN_OK,失败返回错误码
/// </returns>
KTN_RET	CHW104CollectorFactory::CreateIHW104CollectorControl(
	IHW104CollectorControl*& pIHW104CollectorControl
	)
{
	pIHW104CollectorControl = dynamic_cast< IHW104CollectorControl* >( new CHW104CollectControl );

	ASSERT( NULL != pIHW104CollectorControl );

	if ( NULL == pIHW104CollectorControl )
	{
		return KTN_OUT_OF_MEMORY;
	}

	return KTN_OK;
}

/// <summary>
/// 	创建104采集器实例
/// </summary>
/// <param name="collectorType">
///		in 采集器类型
/// </param>
/// <param name="pIHW104Collector">
///		out 采集器
/// </param>
/// <returns>
///		成功返回KTN_OK,失败返回错误码
/// </returns>
KTN_RET CHW104CollectorFactory::CreateIHW104Collector( 
									  IHW104Collector*& pIHW104Collector
									  )
{
	pIHW104Collector = dynamic_cast< IHW104Collector* >( new CHW104Collector );

	ASSERT( pIHW104Collector != NULL );
	if ( NULL == pIHW104Collector )
	{
		return KTN_OUT_OF_MEMORY;
	}

	return KTN_OK;
}

