//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: HW104Collector.h
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
///			V0.10 : ���� 2009-10-15 15:03 �����ļ�
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
		/* �ɼ����ӿ�                                                                     
		/************************************************************************/
		/// <summary>
		/// 	���ٲɼ���
		/// </summary>
		/// <returns>
		///		�ɹ�����KTN_OK,ʧ�ܷ��ش�����
		/// </returns>
		/// <remarks>
		///     �ͷŲɼ�����ռ�е���Դ,���ٲɼ���
		/// </remarks>
		virtual KTN_RET	DelCollector();

		/// <summary>
		/// 	���ٻ����е�asdu����
		/// </summary>
		/// <param name="infoUnitArray">
		///		���ٻ����е���Ϣ����
		/// </param>
		/// <returns>
		///		�ɹ�����KTN_OK,ʧ�ܷ��ش�����
		/// </returns>
		//Ϊ��ݵ����¹���, �����������strClientIP,����IPȷ��ʹ���ĸ����. 2010-6-8,����ǿ�޸�
		virtual KTN_RET ReadAllRealData( const std::string* pStrClientIP,
			std::vector< KTNHW104_InfoUnit >& infoUnitArray
			);

		/// <summary>
		/// 	�������
		/// </summary>
		virtual int GetCount();

		/// <summary>
		/// 	д��ָ������ֵ
		/// </summary>
		/// <param name="infoUnit">
		///		in asdu����
		/// </param>
		/// <returns>
		///		�ɹ�����KTN_OK,ʧ�ܷ��ش�����
		/// </returns>
		virtual KTN_RET WriteInfoUnit(
			const std::string& ip,
			const KTNHW104_InfoUnit& infoUnit 
			);

		/// <summary>
		/// 	ע��仯���ͺ���
		/// </summary>
		/// <param name="pIHW104CallBack">
		///		�ص��ӿ�
		/// </param>
		virtual KTN_RET RegsiterCallBack( std::string strClientIP,
			IHW104CollectorCallBack* pIHW104CallBack
			);

		//����������¼��master��������������ݵ�
		virtual KTN_RET UpdateLinkErr(KTNCollectorTpye clientType, BOOL bConnect);
	};

}


//========================================================================
#endif #define __HW104Collector_H_INCLUDED__

