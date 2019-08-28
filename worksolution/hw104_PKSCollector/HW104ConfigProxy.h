//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: HW104ConfigInfo.h
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
///			V0.10 : ���� 2009-10-15 16:45 �����ļ�
///		</version>
///	</versions>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "hw104_PKSCollector/HW104Collector_ExtInc.h"
#include "HW104CollectorCore_Datatype.h"
#include "HW104Subscription.h"

#ifndef __HW104ConfigInfo_H_INCLUDED__
#define __HW104ConfigInfo_H_INCLUDED__
//========================================================================

/// <summary>
/// 	<��Ϣ�����ַ, ���ݿ��>
/// </summary>
typedef std::map< KTNPKSPoint, KTNIECAddress > KTNPksIecMap;
typedef KTNPksIecMap::iterator				   KTNPksIecMapIt;
typedef KTNPksIecMap::const_iterator		   KTNPksIecMapCIt;

/// <summary>
/// 	< address ,  point >
/// </summary>
typedef std::map< KTNIECAddress, KTNPKSPoint >  KTNIecPksMap;
typedef KTNIecPksMap::iterator					KTNIecPksMapIt;
typedef KTNIecPksMap::const_iterator			KTNIecPksMapCIt;

namespace NKTNCollector
{

	class CHW104ConfigProxy
	{
	public:
		CHW104ConfigProxy(void);
		~CHW104ConfigProxy(void);

	public:
		/// <summary>
		/// 	����������Ϣ
		/// </summary>
		/// <returns>
		///		���سɹ�����KTN_OK,ʧ�ܷ��ش�����
		/// </returns>
		/// <remarks>
		///     1. ���������ļ�
		///     2. ��������Ϣ���͸����ݿ�ɼ���
		/// </remarks>
		KTN_RET LoadKTNConfig();

		/// <summary>
		/// 	��ȡ���е����ݿ��
		/// </summary>
		/// <param name="iecArray">
		///		out ��Ϣ�����ַ
		/// </param>
		/// <param name="valueArray">
		///		out  ��������
		/// </param>
		/// <returns>
		///		���سɹ�����KTN_OK,ʧ�ܷ��ش�����
		/// </returns>
		KTN_RET ReadAllPoint(
			std::vector< KTNIECAddress >& iecArray,
			std::vector< KTNValue >&	  valueArray
			);

		/// <summary>
		/// 	�仯�ص�
		/// </summary>
		/// <param name="pksPointArray">
		///		���ݿ����Ϣ����
		/// </param>
		/// <param name="valueArray">
		///		���ݿ���ֵ����
		/// </param>
		/// <returns>
		///		���سɹ�����KTN_OK,ʧ�ܷ��ش�����
		/// </returns>
		KTN_RET OnChange( 
			const std::vector< KTNPKSPoint >&	pksPointArray,
			const std::vector< KTNValue >&		valueArray
			);

		/// <summary>
		/// 	����״̬�仯�ص�
		/// </summary>
		/// <param name="redunStatus">
		///		����״̬
		/// </param>
		KTN_RET OnRedunStatusChange(
			KTNRedunStatus& redunStatus
			);

		/// <summary>
		/// 	д��IEC��ַ��Ӧ������
		/// </summary>
		/// <param name="iecAddress">
		///		KTNIECAddress 
		/// </param>
		/// <param name="ktnValue">
		///		KTNValue
		/// </param>
		/// <returns>
		///		���سɹ�����KTN_OK,ʧ�ܷ��ش�����
		/// </returns>
		KTN_RET WriteIecData(
			const KTNIECAddress&   iecAddress,
			const KTNValue&		   ktnValue
			);

		/// <summary>
		/// 	ע��ص��ӿ�
		/// </summary>
		/// <param name="pIHW104CallBack">
		///		in �ص��ӿ�
		/// </param>
		/// <returns>
		///		���سɹ�����KTN_OK,ʧ�ܷ��ش�����
		/// </returns>
		KTN_RET RegsiterCallBack(
			  IHW104CollectorCallBack* pIHW104CallBack
			);

		/// <summary>
		/// 	�������
		/// </summary>
		int GetCount();

		/// <summary>
		/// 	���point����������
		/// </summary>
		inline KTNASDUType GetPointType( const KTNPKSPoint& ktnPoint )
		{
			KTNPksIecMapIt it = m_KTNPksIecMap.find( ktnPoint );
			return it->second.asduType;
		}

		/// <summary>
		///   ֹͣ�仯����
		/// </summary>
		void End();

	protected:
		/************************************************************************/
		/* ��Ȼ�漰�����߳�,�����������˵�����ʱ,�����һ�� loadconfig
		/* ��ʹ�ù�����,�������޸������������ݽṹ.����ѹ��������,
		/************************************************************************/
		/// <summary>
		/// 	ӳ����,�±꼴�����
		/// </summary>
		std::map< int, KTNPKSPoint > m_KTNPksGroup;

		/// <summary>
		/// 	Point��< point, iec > ( ���� )
		/// </summary>
		KTNPksIecMap				 m_KTNPksIecMap;

		/// <summary>
		/// 	iecӳ����    < iec, point > ( д�� )
		/// </summary>
		KTNIecPksMap				 m_KTNIecPksMap;

		/// <summary>
		///		�ص�ָ���б�
		/// </summary>
		std::list< CHW104Subscription* > m_SubscriptionList;

		/// <summary>
		/// 	�ص�ע���б���
		/// </summary>
		CBaseLock	m_SubListLock;
	};

}

//========================================================================
#endif #define __HW104ConfigInfo_H_INCLUDED__


