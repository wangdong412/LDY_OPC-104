//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	104Trans
///		文件名: HW104CallBack.h
///		
///	</file>
///	<versions>
///		<version>
///			V0.10 :  [10/23/2009] 创建文件
///		</version>
///	</versions>
//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif

#include "hw104_PKSCollector/HW104Collector_ExtInc.h"

#ifndef _HW104CALLBACK_H_INCLUDED__
#define _HW104CALLBACK_H_INCLUDED__

class CHW104CallBack:public IHW104CollectorCallBack
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
		);
};


#endif #define _HW104CALLBACK_H_INCLUDED__

