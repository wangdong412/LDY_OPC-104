//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	KingModel
///		文件名: HW104Collector_ExtInc.h
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
///			V0.10 : 王勃 2009-10-14 13:41 创建文件
///		</version>
///	</versions>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __HW104Collector_ExtInc_H_INCLUDED__
#define __HW104Collector_ExtInc_H_INCLUDED__
//========================================================================

//==============================================================================
/// 导入导出定义宏
//==============================================================================

#ifdef _AFXDLL

#ifdef KTN_COLLECTOR_DLL_FLAG
#define XKTN_COLLECTOR_CLASSEXT				_declspec(dllexport)
#define XKTN_COLLECTOR_STDEXT				_declspec(dllexport)
#else
#define XKTN_COLLECTOR_CLASSEXT				_declspec(dllimport)
#define XKTN_COLLECTOR_STDEXT				_declspec(dllimport)
#endif
#else
#define XKTN_COLLECTOR_CLASSEXT
#define XKTN_COLLECTOR_STDEXT
#endif

/************************************************************************/
/* 导入外部头文件                                                                    
/************************************************************************/
#include "hw104_Common/hw104Common_Ext_inc.h"

/************************************************************************/
/* 导入内部头文件                                                                     
/************************************************************************/
#include "hw104_PKSCollector/HW104CollectorDataType.h"
#include "hw104_PKSCollector/IHW104CollectorCallBack.h"
#include "hw104_PKSCollector/IHW104Collector.h"
#include "hw104_PKSCollector/IHW104CollectControl.h"
#include "hw104_PKSCollector/IHW104CollectorFactory.h"

/************************************************************************/
/* 引入命名空间                                                                     
/************************************************************************/
using namespace NKTNCollector;

/************************************************************************/
/* 导出函数                                                                     
/************************************************************************/
/// <summary>
/// 	全局导出函数,导出类厂
/// </summary>
XKTN_COLLECTOR_STDEXT IHW104CollectorFactory* ktn_GetHW104CollectorFactory();

//========================================================================
#endif #define __HW104Collector_ExtInc_H_INCLUDED__