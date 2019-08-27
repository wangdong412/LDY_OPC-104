//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	KingModel
///		文件名: ICollectorCallBack.h
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
///			V0.10 : 王勃 2009-10-14 21:58 创建文件
///		</version>
///	</versions>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __ICollectorCallBack_H_INCLUDED__
#define __ICollectorCallBack_H_INCLUDED__
//========================================================================

namespace NKTNCollector
{
	class IHW104CollectorCallBack
	{
	public:
		/// <summary>
		/// 	信息单元变化回调函数
		/// </summary>
		/// <param name="changeUnitArray">
		///		变化的信息单元数据
		/// </param>
		virtual void OnInfoUnitChange( 
			const std::vector< KTNHW104_InfoUnit >& changeUnitArray 
			) = 0;		
	};
}

//========================================================================
#endif #define __ICollectorCallBack_H_INCLUDED__
