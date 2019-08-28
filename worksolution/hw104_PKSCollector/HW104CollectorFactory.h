//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: HW104CollectorFactory.h
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
///			V0.10 : ���� 2009-10-15 10:09 �����ļ�
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
		/// 	�����ɼ�������
		/// </summary>
		/// <param name="pIHW104CollectorControl">
		///		out �ɼ�������
		/// </param>
		/// <returns>
		///		�ɹ�����KTN_OK,ʧ�ܷ��ش�����
		/// </returns>
		virtual KTN_RET	CreateIHW104CollectorControl(
			IHW104CollectorControl*& pIHW104CollectorControl
			);

		/// <summary>
		/// 	����104�ɼ���ʵ��
		/// </summary>
		/// <param name="collectorType">
		///		in �ɼ�������
		/// </param>
		/// <param name="pIHW104Collector">
		///		out �ɼ���
		/// </param>
		/// <returns>
		///		�ɹ�����KTN_OK,ʧ�ܷ��ش�����
		/// </returns>
		virtual KTN_RET CreateIHW104Collector(
			IHW104Collector*& pIHW104Collector
			);
	};

}



//========================================================================
#endif #define __HW104CollectorFactory_H_INCLUDED__


