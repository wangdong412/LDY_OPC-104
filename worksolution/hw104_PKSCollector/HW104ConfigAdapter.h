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
#include "HW104MainTable.h"

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
typedef std::map< KTNIECAddress, int >  KTNIecPksMap;
typedef KTNIecPksMap::iterator					KTNIecPksMapIt;
typedef KTNIecPksMap::const_iterator			KTNIecPksMapCIt;

namespace NKTNCollector
{
	
	class COPCCollector;

	class CHW104ConfigAdapter
	{
	public:
		CHW104ConfigAdapter(void);
		~CHW104ConfigAdapter(void);

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
		
		void CollectThread(COPCCollector* p);

		//���ص��ݵ��
		KTN_RET Load104IOList(const wchar_t* wstrFilePath, CHW104MainTable& mTable,
							  KTNPksIecMap& mapPksToIEC, KTNIecPksMap& mapIECtoPKS,
							  std::vector<KTNPKSPoint>& pointArray);

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
		//Ϊ��ݵ����¹���, �����������strClientIP,����IPȷ��ʹ���ĸ����. 2010-6-8,����ǿ�޸�
		KTN_RET ReadAllPoint( const std::string* pStrClientIP,
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
			const std::vector< int >&	indexArray,
			const std::vector< KTNValue >&		valueArray
			);

		/// <summary>
		/// 	����״̬�仯�ص�
		/// </summary>
		/// <param name="redunStatus">
		///		����״̬
		/// </param>
		/*KTN_RET OnRedunStatusChange(
			KTNRedunStatus& redunStatus
			);*/

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
			const std::string& ip,
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
		KTN_RET RegsiterCallBack( std::string strClientIP,
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
		///   ��ʼ�ɼ�
		/// </summary>
		KTN_RET StartCollect();


		/// <summary>
		///   ֹͣ�ɼ�
		/// </summary>
		void EndCollect();

		//����������¼��master��������������ݵ�
		KTN_RET WriteLinkErr(KTNCollectorTpye clientType, BOOL bConnect);

		std::vector<KTNPKSPoint> m_R_OPCVector;
		int m_current_write_index;
		KTNValue m_write_val;
	protected:
		
		//client ip
		//std::wstring	m_OPCClientIP;
		std::vector<COPCCollector *> m_OPCArray;
		std::vector<HANDLE>	m_ThreadArray;

		/// <summary>
		/// 	ʵʱ�����ݱ���
		/// </summary>
		NkxLatchLock::XDBLatch m_ReadDataLock;

		/// <summary>
		/// 	ʵʱд���ݱ���
		/// </summary>
		NkxLatchLock::XDBLatch m_IECOPCMapLock;

		volatile bool   m_ThreadStop;

		HANDLE m_WriteEvent;
		HANDLE m_hMutexCollectThread;
		std::map<  std::string, KTNIecPksMap > m_mapIPToKTNIecPksMap;

		/************************************************************************/
		/* ��Ȼ�漰�����߳�,�����������˵�����ʱ,�����һ�� loadconfig
		/* ��ʹ�ù�����,�������޸������������ݽṹ.����ѹ��������,
		/************************************************************************/		
		/// <summary>
		/// 	Point��< point, iec > ( ���� ) //�Ը��ݵ��Ļ��ܱ�, ���пɶ���
		/// </summary>
		KTNPksIecMap				 m_KTNPksIecMap;

		///ÿһ��IP��Ӧһ�ݵ��(����,��Կɶ���)
		std::map<std::string, KTNPksIecMap> m_mapIPToPointTable;
		

		/// <summary>
		///		�ص�ָ���б�
		/// </summary>
		//std::list< CHW104Subscription* > m_SubscriptionList;
		std::map<std::string, CHW104Subscription*> m_mapIPToSubscribe;

		/// <summary>
		/// 	�ص�ע���б���
		/// </summary>
		CBaseLock	m_SubListLock;
	};

}

//========================================================================
#endif #define __HW104ConfigInfo_H_INCLUDED__


