//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	104Trans.exe
///		�ļ���: HW104CallBack.h
///		��  ��: shuqiang.luan
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
	/// 	��Ϣ��Ԫ�仯�ص�����
	/// </summary>
	/// <param name="changeUnitArray">
	///		�仯����Ϣ��Ԫ����
	/// </param>
	virtual void OnInfoUnitChange( 
		const std::vector< KTNHW104_InfoUnit >& changeUnitArray 
		);

private:
	CHW104_Slave* m_pSlave;
};

//========================================================================
#endif #define __HW104CallBack_H_INCLUDED__

