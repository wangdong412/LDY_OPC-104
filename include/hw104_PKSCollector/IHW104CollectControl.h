//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	KingModel
///		文件名: IHW104CollectControl.h
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
///			V0.10 : 王勃 2009-10-14 15:08 创建文件
///		</version>
///	</versions>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __IHW104CollectControl_H_INCLUDED__
#define __IHW104CollectControl_H_INCLUDED__
//========================================================================

namespace NKTNCollector
{
	class IHW104CollectorControl
	{
	public:
		/// <summary>
		/// 	启动采集
		/// </summary>
		/// <returns>
		///		成功返回KTN_OK,失败返回错误码
		/// </returns>
		/// <remarks>
		///     每次启动之前必须通过这个函数通知采集器开始采集
		///     只能在主线程中进行调用
		/// </remarks>
		virtual KTN_RET	 StartCollect() = 0;

		/// <summary>
		/// 	停止采集
		/// </summary>
		/// <returns>
		///		成功返回KTN_OK,失败返回错误码
		/// </returns>
		/// <remarks>
		///     每次退出时必须通过这个函数停止采集并释放相关资源
		/// </remarks>
		virtual KTN_RET	EndCollect() = 0;

		/// <summary>
		/// 	获得本机的冗余状态
		/// </summary>
		/// <param name="redunStatus">
		///		本机的冗余状态
		/// </param>
		virtual KTN_RET GetRedunStatus(
			KTNRedunStatus& redunStatus ) = 0;
	};
}

//========================================================================
#endif #define __IHW104CollectControl_H_INCLUDED__