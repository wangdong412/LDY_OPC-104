//========================================================================
/// <summary>
///		内部总括头文件
/// </summary>
///	<file>
///		项目名:	KingModel
///		文件名: HW104CollectorCore_Inc.h
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
///			V0.10 : 王勃 2009-10-16 10:47 创建文件
///		</version>
///	</versions>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __HW104CollectorCore_Inc_H_INCLUDED__
#define __HW104CollectorCore_Inc_H_INCLUDED__
//========================================================================

/************************************************************************/
/* INI 配置文件内容                                                                     
/************************************************************************/
#define CONFIGFILENAME		L"104Trans.ini"
//采集器配置信息
#define SECTIONNAME			L"PKSCOLLECTOR"
#define CYCLESTIMES			L"CollectCycles"
#define COLLECTORTIMEOUT	L"CollectTimeOut"
#define THRESHOLD			L"ThresHold"
#define MEMORYBUFFER		L"MemoryBuffer"
//PKSServer配置
#define PKSSERVERSECTION    L"PKSServer"
#define SERVERNumber		L"ServerNum"
#define SERVERNAME			L"Server"

/************************************************************************/
/* 日志文件相关                                                                     
/************************************************************************/
#define PKSLOGNAME L"opcda_collector.log"
#define MAX_ERROR_LEN 256


#include "hw104_PKSCollector/HW104Collector_ExtInc.h"

/************************************************************************/
/* 常用宏定义                                                                     
/************************************************************************/


/************************************************************************/
/* 内部使用头文件                                                                     
/************************************************************************/
#include "HW104CollectorCore_Datatype.h"
#include "HW104Help.h"

#define HW104PKSLog(Category, msg)	\
HW104LOG( g_HW104Log, Category, msg )

//========================================================================
#endif #define __HW104CollectorCore_Inc_H_INCLUDED__