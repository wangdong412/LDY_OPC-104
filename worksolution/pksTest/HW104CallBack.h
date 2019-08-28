//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	104Trans
///		�ļ���: HW104CallBack.h
///		
///	</file>
///	<versions>
///		<version>
///			V0.10 :  [10/23/2009] �����ļ�
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
	/// 	��Ϣ��Ԫ�仯�ص�����
	/// </summary>
	/// <param name="changeUnitArray">
	///		�仯����Ϣ��Ԫ����
	/// </param>
	virtual void OnInfoUnitChange( 
		const std::vector< KTNHW104_InfoUnit >& changeUnitArray 
		);
};


#endif #define _HW104CALLBACK_H_INCLUDED__

