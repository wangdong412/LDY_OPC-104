//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	KingModel
///		文件名: HW104ConfigInfo.h
///		作  者: 王勃
///	</file>
///	<copyright>
/// 	本代码所有版权为所有，任何个人或者组织不得以任何方式复制、转载以挪作
/// 他用，否则将构成对的产权知识侵权。
///	
///		
///	</copyright>
///	<versions>
///		<version>
///			V0.10 : 王勃 2009-10-15 16:45 创建文件
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
/// 	<信息对象地址, 数据库点>
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
		/// 	加载配置信息
		/// </summary>
		/// <returns>
		///		加载成功返回KTN_OK,失败返回错误码
		/// </returns>
		/// <remarks>
		///     1. 加载配置文件
		///     2. 把所有信息发送给数据库采集器
		/// </remarks>
		KTN_RET LoadKTNConfig();
		
		void CollectThread(COPCCollector* p);

		//加载单份点表
		KTN_RET Load104IOList(const wchar_t* wstrFilePath, CHW104MainTable& mTable,
							  KTNPksIecMap& mapPksToIEC, KTNIecPksMap& mapIECtoPKS,
							  std::vector<KTNPKSPoint>& pointArray);

		/// <summary>
		/// 	读取所有的数据库点
		/// </summary>
		/// <param name="iecArray">
		///		out 信息对象地址
		/// </param>
		/// <param name="valueArray">
		///		out  数据数组
		/// </param>
		/// <returns>
		///		加载成功返回KTN_OK,失败返回错误码
		/// </returns>
		//为多份点表的新功能, 增加输入参数strClientIP,根据IP确定使用哪个点表. 2010-6-8,栾述强修改
		KTN_RET ReadAllPoint( const std::string* pStrClientIP,
			std::vector< KTNIECAddress >& iecArray,
			std::vector< KTNValue >&	  valueArray
			);

		/// <summary>
		/// 	变化回调
		/// </summary>
		/// <param name="pksPointArray">
		///		数据库点信息数组
		/// </param>
		/// <param name="valueArray">
		///		数据库数值数组
		/// </param>
		/// <returns>
		///		加载成功返回KTN_OK,失败返回错误码
		/// </returns>
		KTN_RET OnChange( 
			const std::vector< int >&	indexArray,
			const std::vector< KTNValue >&		valueArray
			);

		/// <summary>
		/// 	冗余状态变化回调
		/// </summary>
		/// <param name="redunStatus">
		///		冗余状态
		/// </param>
		/*KTN_RET OnRedunStatusChange(
			KTNRedunStatus& redunStatus
			);*/

		/// <summary>
		/// 	写入IEC地址对应的数据
		/// </summary>
		/// <param name="iecAddress">
		///		KTNIECAddress 
		/// </param>
		/// <param name="ktnValue">
		///		KTNValue
		/// </param>
		/// <returns>
		///		加载成功返回KTN_OK,失败返回错误码
		/// </returns>
		KTN_RET WriteIecData(
			const std::string& ip,
			const KTNIECAddress&   iecAddress,
			const KTNValue&		   ktnValue
			);

		/// <summary>
		/// 	注册回调接口
		/// </summary>
		/// <param name="pIHW104CallBack">
		///		in 回调接口
		/// </param>
		/// <returns>
		///		加载成功返回KTN_OK,失败返回错误码
		/// </returns>
		KTN_RET RegsiterCallBack( std::string strClientIP,
			  IHW104CollectorCallBack* pIHW104CallBack
			);

		/// <summary>
		/// 	获得数量
		/// </summary>
		int GetCount();

		/// <summary>
		/// 	获得point的数据类型
		/// </summary>
		inline KTNASDUType GetPointType( const KTNPKSPoint& ktnPoint )
		{
			KTNPksIecMapIt it = m_KTNPksIecMap.find( ktnPoint );
			return it->second.asduType;
		}

		/// <summary>
		///   开始采集
		/// </summary>
		KTN_RET StartCollect();


		/// <summary>
		///   停止采集
		/// </summary>
		void EndCollect();

		//更新用来记录与master端连接情况的数据点
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
		/// 	实时读数据表锁
		/// </summary>
		NkxLatchLock::XDBLatch m_ReadDataLock;

		/// <summary>
		/// 	实时写数据表锁
		/// </summary>
		NkxLatchLock::XDBLatch m_IECOPCMapLock;

		volatile bool   m_ThreadStop;

		HANDLE m_WriteEvent;
		HANDLE m_hMutexCollectThread;
		std::map<  std::string, KTNIecPksMap > m_mapIPToKTNIecPksMap;

		/************************************************************************/
		/* 虽然涉及到多线程,但是这个类除了当创建时,会调用一下 loadconfig
		/* 在使用过程中,并不会修改以下两个数据结构.所以压根不用锁,
		/************************************************************************/		
		/// <summary>
		/// 	Point组< point, iec > ( 读表 ) //对各份点表的汇总表, 所有可读点
		/// </summary>
		KTNPksIecMap				 m_KTNPksIecMap;

		///每一个IP对应一份点表(读表,针对可读点)
		std::map<std::string, KTNPksIecMap> m_mapIPToPointTable;
		

		/// <summary>
		///		回调指针列表
		/// </summary>
		//std::list< CHW104Subscription* > m_SubscriptionList;
		std::map<std::string, CHW104Subscription*> m_mapIPToSubscribe;

		/// <summary>
		/// 	回调注册列表锁
		/// </summary>
		CBaseLock	m_SubListLock;
	};

}

//========================================================================
#endif #define __HW104ConfigInfo_H_INCLUDED__


