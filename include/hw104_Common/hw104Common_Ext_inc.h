//========================================================================
/// <summary>
///		HoneyWell转发程序
/// </summary>
///	<file>
///		项目名:	KingModel
///		文件名: hw104Common_Ext_inc.h
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
///			V0.10 : 王勃 2009-10-14 13:57 创建文件
///		</version>
///	</versions>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __hw104Common_Ext_inc_H_INCLUDED__
#define __hw104Common_Ext_inc_H_INCLUDED__
//========================================================================

//==============================================================================
/// 导入导出定义宏
#ifdef _AFXDLL

#ifdef KTNCOMMON_DLL_FLAG
#define KTN_COMMON_CLASSEXT		_declspec(dllexport)
#define KTN_COMMON_STDEXT		_declspec(dllexport)
#else
#define KTN_COMMON_CLASSEXT		_declspec(dllimport)
#define KTN_COMMON_STDEXT		_declspec(dllimport)
#endif
#else
#define KTN_COMMON_CLASSEXT
#define KTN_COMMON_STDEXT
#endif

//////////////////////////////////////////////////////////////////////////
/// 常用宏定义
#define WSTR(x)			L##x
#define HI32( n )		((unsigned long)( (n) >> 32 ) )
#define LO32( n )		((unsigned long)( n ) )

// 位操作
#define GET_BIT( op , n )			( ( ( op ) >> ( n ) ) & 0x00000001 )
#define SET_BIT( op , n )			( ( op ) |= ( 1 << ( n ) ) )
#define CLR_BIT( op , n )			( ( op ) &= ( ~( 1 << ( n ) ) ) )

#define KTNOK(ret)   ( KTN_OK == ret )
#define KTNERR(ret)  ( KTN_OK != ret )

#define ARRAYLEN( arr ) (sizeof(arr))/sizeof(arr[0])

//数据同步锁
#define HW104_MT_INCR(mtx, x)	_InterlockedIncrement(&x)
#define HW104_MT_DECR(mtx, x)	_InterlockedDecrement(&x)
#define HW104_MT_CMPX(x, y, z)	_InterlockedCompareExchange(&x, y, z)

/// <summary>
/// 	默认的自旋锁尝试时间(次数)
/// </summary>
/// <remarks>
///     该值使用于自旋锁定义类CkxSpinLock中，当系统中存在多CPU时，需要在调用
/// Sleep前进行线程抢占尝试，这里定义的缺省的尝试次数。
/// </remarks>
#define DEFAULT_SPIN_COUNT			500

#ifndef __WFILE__
#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)
#endif

//==============================================================================
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <tchar.h>
#include <process.h>
#include <winsock2.h>

//==============================================================================
// 
// 标准C++头文件
// 
//==============================================================================
#include <string>
#include <vector>
#include <map>
#include <list>
#include <queue>
#include <set>
#include <iostream>
#include <algorithm>
#include <functional>
#include <sstream>
#include <fstream>
#include <float.h>
#include <math.h>
#include <afxtempl.h>

/************************************************************************/
/* 公共类型定义                                                                    
/************************************************************************/
#define INVALID_GOURPID	(UINT)(-1)

/// <summary>
/// 	采集类型
/// </summary>
enum KTNCollectorTpye
{
	KTNCollector_Unknown = 0,	//未知采集类型
	KTN_HoneyWell = 1,
	KTN_Cegelec   = 2,
	KTN_Telvent   = 3
};

/// <summary>
/// 	需要实现的ASDU类型
/// </summary>
enum KTNASDUType
{
	MC_UNKNOWN = 0,					//未知类型

	//监视方向的过程信息
	M_SP_NA_1  = 1,					//单点信息
	M_DP_NA_1  = 3,                 //双点信息
	M_ME_NA_1  = 9,                 //测量值, 归一化值
	M_ME_NB_1  = 11,                //测量值，标度化值
	M_ME_NC_1  = 13,				//测量值, 短浮点数
	M_IT_NA_1  = 15,				//计数量
	M_SP_TB_1  = 30,				//带CP56TIME2A时标的单点信息
	M_DP_TB_1  = 31,				//带CP56TIME2A时标的双点信息
	M_ME_TD_1  = 34,				//带CP56TIME2A时标的测量值，归一化值
	M_ME_TE_1  = 35,				//带CP56TIME2A时标的测量值，标度化值
	M_ME_TF_1  = 36,				//带时标CP56Time2a的测量值，短浮点数
	M_IT_TB_1  = 37,			    //带CP56TIME2A时标的计数量

	//控制方向的过程信息
	C_SC_NA_1  = 45,				//单点命令
	C_DC_NA_1  = 46,				//双点命令
	C_SE_NA_1  = 48,				//设定值命令，归一化值
	C_SE_NB_1  = 49,				//设定值命令，标度化值
	C_SE_NC_1  = 50,				//设定值命令，短浮点数
	C_SC_TA_1  = 58,				//带CP56TIME2A时标的单点命令
	C_DC_TA_1  = 59,				//带CP56TIME2A时标的双点命令
	C_SE_TA_1  = 61,				//带CP56TIME2A时标的设定值命令，归一化值
	C_SE_TB_1  = 62,				//带CP56TIME2A时标的设定值命令，标度化值
	C_SE_TC_1  = 63,			    //带CP56Time2a时标的设点命令，短浮点数

	//在监视方向的系统命令
	M_EI_NA_1  = 70,			    //初始化结束

	//在控制方向的系统命令
	C_IC_NA_1  = 100,			    //总召唤命令
	C_CI_NA_1  = 101,			    //计数量召唤命令
	C_RD_NA_1  = 102,			    //读命令
	C_CS_NA_1  = 103,			    //时钟同步命令
	C_RP_NC_1  = 105,			    //复位进程命令	
};

/// <summary>
/// 	信息对象/信息元素
/// </summary>
/// <remarks>
///     我们在这个项目中只实现信息对象
/// </remarks>
enum KTNInfoUintType			
{
	Info_Unknown = 0,			//未知类型
	InfoObject   = 1,			//信息对象
	InfoElement  = 2			//信息元素
};

/************************************************************************/
/* 定义返回码类型                                                                   
/************************************************************************/
typedef long KTN_RET;
 
//=============================================================================
/// 定义一些状态返回码
#define KTN_OK			0			/// 成功状态

#define KTN_ERRORSTART			0x20008000	/// 错误代码起始编码

/************************************************************************/
/// 通用错误码定义
/// 范围:0x01 - 0x3F
#define KTN_FAILED				(KTN_ERRORSTART+0x01) /// 一般性失败
#define KTN_INVALIDARGS			(KTN_ERRORSTART+0x02) /// 参数无效
#define KTN_NOTIMPLEMENT		(KTN_ERRORSTART+0x03) /// 未实现
#define KTN_ITEMNOTFOUND		(KTN_ERRORSTART+0x04) /// 项目未找到
#define KTN_INVALID_OPERATION	(KTN_ERRORSTART+0x05) /// 非法操作
#define KTN_USER_CANCELED		(KTN_ERRORSTART+0x06) /// 用户取消操作
#define KTN_ADDINEXIST			(KTN_ERRORSTART+0x07) /// 插件已经存在  
#define KTN_OUT_OF_MEMORY       (KTN_ERRORSTART+0x08) /// 内存错误
/************************************************************************/
/// 模块错误码起始值及范围定义

/// HWCollector模块错误码定义
/// 区间:0x040 - 0x06F
#define KTN_HWCOLLECTOR_ERROR_START				(KTN_ERRORSTART+0x40)

/// HWTrans模块错误码定义
/// 区间:0x070 - 0x08F
#define KTN_HWTRANS_ERROR_START					(KTN_ERRORSTART+0x070)
//=============================================================================

/// <summary>
/// 	为锁定义的宏
/// </summary>
#define KXASSERT ASSERT
#define KXSTATUS_SUCCESS KTN_OK
#define KXSTATUS_FAILED KTN_FAILED

/************************************************************************/
/* 内部头文件引用                                                                     
/************************************************************************/
#include "hw104_Common/XDBLatch.h"				  //锁头文件 
#include "hw104_Common/BaseLock.h"
#include "hw104_Common/HW_104Log.h"


/************************************************************************/
/* 命名空间引用                                                                     
/************************************************************************/
using namespace KTNCOMMON;
using namespace NkxLatchLock;

//========================================================================
#endif #define __hw104Common_Ext_inc_H_INCLUDED__