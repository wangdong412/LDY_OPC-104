//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	KingModel
///		文件名: HW104CollectorDataType.h
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
///			V0.10 : 王勃 2009-10-14 13:43 创建文件
///		</version>
///	</versions>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __HW104CollectorDataType_H_INCLUDED__
#define __HW104CollectorDataType_H_INCLUDED__
//========================================================================

namespace NKTNCollector
{	
	enum KTNRedunStatus
	{
		KTN_PRIMARY = 0,		//主机状态
		KTN_BACKUP	= 1			//从机状态
	};

	enum KTNLocalStatus
	{
		KTNOffline = 0,		//本机处于离线状态
		KTNPrimary = 1,		//冗余系统中本机处于主机状态
		KTNBackUp  = 2		//冗余系统中本机处于从机状态
	};

	#define MAX_INFOSTREAM 20

	/// <summary>
	/// 	信息单元数据流
	/// </summary>
	struct KTNHW104_InfoUnit
	{
		KTNInfoUintType	infoType;								//信息对象/信息元素
		KTNASDUType		asduType;								//ASDU类型标识
		char			infoUnitStream[ MAX_INFOSTREAM ];		//数据流
		int				streamLen;								//数据流长度
		int				reversed1;								//保留1
		int				reversed2;								//保留2

		KTNHW104_InfoUnit():infoType( InfoObject ),
						 asduType( MC_UNKNOWN ),
						 reversed1( 0 ),
						 reversed2( 0 ),
						 streamLen( 0 )
		{
			ZeroMemory( infoUnitStream , MAX_INFOSTREAM );
		}

		KTNHW104_InfoUnit( const KTNHW104_InfoUnit& obj )
		{
			infoType = obj.infoType;
			asduType = obj.asduType;
			memcpy( infoUnitStream , obj.infoUnitStream, obj.streamLen );
			streamLen = obj.streamLen;
			reversed1 = obj.reversed1;
			reversed2 = obj.reversed2;
		}

		KTNHW104_InfoUnit& operator=( const KTNHW104_InfoUnit& obj )
		{
			if ( &obj != this  )
			{
				infoType = obj.infoType;
				asduType = obj.asduType;
				memcpy( infoUnitStream , obj.infoUnitStream, obj.streamLen );
				streamLen = obj.streamLen;
				reversed1 = obj.reversed1;
				reversed2 = obj.reversed2;
			}

			return *this;
		}
	};

	/// <summary>
	/// 	ASDU组
	/// </summary>
	struct HW104_ASDUGroup
	{
		unsigned int					groupID;			//组号
		std::vector< KTNHW104_InfoUnit >	infoUnitArray;		//组数据流
		int								reversed;			//保留

		HW104_ASDUGroup():groupID( INVALID_GOURPID )
		{
		}
	};

	/************************************************************************/
	/* Collector 错误码                                                                     
	/************************************************************************/
#define KTN_COLLECTOR_ERROR_ASDUTYPE			(KTN_HWCOLLECTOR_ERROR_START + 1)   //ASDU类型标识不匹配
#define KTN_COLLECTOR_UNSUPPORT_ASDUTYPE		(KTN_HWCOLLECTOR_ERROR_START + 2)   //不支持的ASDU类型
#define KTN_COLLECTOR_UNSUPPORT_INFOUNITTYPE	(KTN_HWCOLLECTOR_ERROR_START + 3)   //不支持的信息单元类型
#define KTN_COLLECTOR_ERROR_CONTROL				(KTN_HWCOLLECTOR_ERROR_START + 4)   //采集控制器没有启动
#define KTN_COLLECTOR_ERROR_INITIAL				(KTN_HWCOLLECTOR_ERROR_START + 5)   //采集器没有被初始化
#define KTN_COLLECTOR_ERROR_LOADCONFIG			(KTN_HWCOLLECTOR_ERROR_START + 6)   //加载配置文件失败
#define KTN_COLLECTOR_UNSUPPORT_COLLECTORTYPE   (KTN_HWCOLLECTOR_ERROR_START + 7)   //不支持的采集器类型
#define	KTN_PKSCOLLECTOR_NOT_START				(KTN_HWCOLLECTOR_ERROR_START + 8)   //采集器没有启动
#define KTN_COLLECTOR_UNSUPPORT_DATATYPE		(KTN_HWCOLLECTOR_ERROR_START + 9)   //不支持的数据类型
#define KTN_COLLECTOR_NO_IECADDRESS				(KTN_HWCOLLECTOR_ERROR_START + 9)   //没有找到IEC地址
#define KTN_COLLECTROR_SELECTCOMMAND			( KTN_HWCOLLECTOR_ERROR_START+ 10 ) //选择指令
#define KTN_POINT_NOT_EXIST						( KTN_HWCOLLECTOR_ERROR_START+ 11 ) //数据库中点不存在
#define KTN_PARMATER_NOT_EXIST				    ( KTN_HWCOLLECTOR_ERROR_START+ 11 ) //数据库中参数不存在
	 
}

//========================================================================
#endif #define __HW104CollectorDataType_H_INCLUDED__