//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	KingModel
///		文件名: HW104CollectorFactory.h
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
///			V0.10 : 王勃 2009-10-15 10:09 创建文件
///		</version>
///	</versions>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "hw104_PKSCollector/HW104Collector_ExtInc.h"

#ifndef __HW104CollectorFactory_H_INCLUDED__
#define __HW104CollectorFactory_H_INCLUDED__
//========================================================================

namespace NKTNCollector
{

	class CHW104CollectorFactory:public IHW104CollectorFactory
	{
	public:
		CHW104CollectorFactory(void);
		~CHW104CollectorFactory(void);

	public:
		/// <summary>
		/// 	创建采集控制器
		/// </summary>
		/// <param name="pIHW104CollectorControl">
		///		out 采集控制器
		/// </param>
		/// <returns>
		///		成功返回KTN_OK,失败返回错误码
		/// </returns>
		virtual KTN_RET	CreateIHW104CollectorControl(
			IHW104CollectorControl*& pIHW104CollectorControl
			);

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
		virtual KTN_RET CreateIHW104Collector(
			IHW104Collector*& pIHW104Collector
			);
	};

}



//========================================================================
#endif #define __HW104CollectorFactory_H_INCLUDED__


