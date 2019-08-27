//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	KingModel
///		文件名: HW104Collector.h
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

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "hw104_PKSCollector/HW104Collector_ExtInc.h"
#include "HW104ProtcolHelp.h"

#ifndef __HW104Collector_H_INCLUDED__
#define __HW104Collector_H_INCLUDED__
//========================================================================

namespace NKTNCollector
{

	class CHW104Collector:public IHW104Collector
	{
	public:
		CHW104Collector(void);
		~CHW104Collector(void);

	public:
		/************************************************************************/
		/* 采集器接口                                                                     
		/************************************************************************/
		/// <summary>
		/// 	销毁采集器
		/// </summary>
		/// <returns>
		///		成功返回KTN_OK,失败返回错误码
		/// </returns>
		/// <remarks>
		///     释放采集器所占有的资源,销毁采集器
		/// </remarks>
		virtual KTN_RET	DelCollector();

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
		virtual KTN_RET ReadAllRealData( const std::string* pStrClientIP,
			std::vector< KTNHW104_InfoUnit >& infoUnitArray
			);

		/// <summary>
		/// 	获得数量
		/// </summary>
		virtual int GetCount();

		/// <summary>
		/// 	写入指定的数值
		/// </summary>
		/// <param name="infoUnit">
		///		in asdu内容
		/// </param>
		/// <returns>
		///		成功返回KTN_OK,失败返回错误码
		/// </returns>
		virtual KTN_RET WriteInfoUnit(
			const std::string& ip,
			const KTNHW104_InfoUnit& infoUnit 
			);

		/// <summary>
		/// 	注册变化上送函数
		/// </summary>
		/// <param name="pIHW104CallBack">
		///		回调接口
		/// </param>
		virtual KTN_RET RegsiterCallBack( std::string strClientIP,
			IHW104CollectorCallBack* pIHW104CallBack
			);

		//更新用来记录与master端连接情况的数据点
		virtual KTN_RET UpdateLinkErr(KTNCollectorTpye clientType, BOOL bConnect);
	};

}


//========================================================================
#endif #define __HW104Collector_H_INCLUDED__

