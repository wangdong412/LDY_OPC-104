//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: HW104CollectControl.h
///		��  ��: ����
///	</file>
///	<copyright>
/// 	���������а�ȨΪ���У��κθ��˻�����֯�������κη�ʽ���ơ�ת����Ų��
/// ���ã����򽫹��ɶԵĲ�Ȩ֪ʶ��Ȩ��
///	
///		
///	</copyright>
///	<versions>
///		<version>
///			V0.10 : ���� 2009-10-15 10:41 �����ļ�
///		</version>
///	</versions>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "hw104_PKSCollector/HW104Collector_ExtInc.h"

#ifndef __HW104CollectControl_H_INCLUDED__
#define __HW104CollectControl_H_INCLUDED__
//========================================================================

class CHW104CollectControl:public IHW104CollectorControl
{
public:
	CHW104CollectControl(void);
	~CHW104CollectControl(void);

public:
	/************************************************************************/
	/* �ӿ�ʵ��                                                                     
	/************************************************************************/
	/// <summary>
	/// 	�����ɼ�
	/// </summary>
	/// <returns>
	///		�ɹ�����KTN_OK,ʧ�ܷ��ش�����
	/// </returns>
	/// <remarks>
	///     ÿ������֮ǰ����ͨ���������֪ͨ�ɼ�����ʼ�ɼ�
	/// </remarks>
	virtual KTN_RET	 StartCollect();

	/// <summary>
	/// 	ֹͣ�ɼ�
	/// </summary>
	/// <returns>
	///		�ɹ�����KTN_OK,ʧ�ܷ��ش�����
	/// </returns>
	/// <remarks>
	///     ÿ���˳�ʱ����ͨ���������ֹͣ�ɼ����ͷ������Դ
	/// </remarks>
	virtual KTN_RET	EndCollect();

	/// <summary>
	/// 	��ñ���������״̬
	/// </summary>
	/// <param name="redunStatus">
	///		����������״̬
	/// </param>
	virtual KTN_RET GetRedunStatus(
		KTNRedunStatus& redunStatus );
};


//========================================================================
#endif #define __HW104CollectControl_H_INCLUDED__

