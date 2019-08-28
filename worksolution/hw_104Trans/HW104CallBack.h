//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	104Trans.exe
///		文件名: HW104CallBack.h
///		作  者: shuqiang.luan
///	</file>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "hw104_PKSCollector/HW104Collector_ExtInc.h"

#ifndef __HW104CallBack_H_INCLUDED__
#define __HW104CallBack_H_INCLUDED__
//========================================================================

class CHW104_Slave;

class CHW104CallBack:public IHW104CollectorCallBack
{
public:
	CHW104CallBack( CHW104_Slave* pSlave );
	~CHW104CallBack(void);

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

private:
	CHW104_Slave* m_pSlave;
};

//========================================================================
#endif #define __HW104CallBack_H_INCLUDED__

