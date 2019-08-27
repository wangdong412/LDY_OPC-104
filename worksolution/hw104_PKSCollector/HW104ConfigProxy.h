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
		KTN_RET ReadAllPoint(
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
			const std::vector< KTNPKSPoint >&	pksPointArray,
			const std::vector< KTNValue >&		valueArray
			);

		/// <summary>
		/// 	冗余状态变化回调
		/// </summary>
		/// <param name="redunStatus">
		///		冗余状态
		/// </param>
		KTN_RET OnRedunStatusChange(
			KTNRedunStatus& redunStatus
			);

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
		KTN_RET RegsiterCallBack(
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
		///   停止变化上送
		/// </summary>
		void End();

	protected:
		/************************************************************************/
		/* 虽然涉及到多线程,但是这个类除了当创建时,会调用一下 loadconfig
		/* 在使用过程中,并不会修改以下两个数据结构.所以压根不用锁,
		/************************************************************************/
		/// <summary>
		/// 	映射组,下标即是组号
		/// </summary>
		std::map< int, KTNPKSPoint > m_KTNPksGroup;

		/// <summary>
		/// 	Point组< point, iec > ( 读表 )
		/// </summary>
		KTNPksIecMap				 m_KTNPksIecMap;

		/// <summary>
		/// 	iec映射组    < iec, point > ( 写表 )
		/// </summary>
		KTNIecPksMap				 m_KTNIecPksMap;

		/// <summary>
		///		回调指针列表
		/// </summary>
		std::list< CHW104Subscription* > m_SubscriptionList;

		/// <summary>
		/// 	回调注册列表锁
		/// </summary>
		CBaseLock	m_SubListLock;
	};

}

//========================================================================
#endif #define __HW104ConfigInfo_H_INCLUDED__


