//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: IHW104Collector.h
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
///			V0.10 : ���� 2009-10-14 13:33 �����ļ�
///		</version>
///	</versions>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __IHW104Collector_H_INCLUDED__
#define __IHW104Collector_H_INCLUDED__
//========================================================================

namespace NKTNCollector
{
	class IHW104Collector
	{
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
		virtual KTN_RET	DelCollector() = 0;

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
			) = 0;

		/// <summary>
		/// 	���asdu��Ԫ������
		/// </summary>
		virtual int GetCount() = 0;

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
			) = 0;

		/// <summary>
		/// 	ע��仯���ͺ���
		/// </summary>
		/// <param name="pIHW104CallBack">
		///		�ص��ӿ�
		/// </param>
		//�����������strClientIP, 2010-6-8 ����ǿ
		virtual KTN_RET RegsiterCallBack( std::string strClientIP,
			IHW104CollectorCallBack* pIHW104CallBack
			) = 0;

		//����������¼��master��������������ݵ�
		virtual KTN_RET UpdateLinkErr(KTNCollectorTpye clientType, BOOL bConnect) = 0;
	};
}

//========================================================================
#endif #define __IHW104Collector_H_INCLUDED__