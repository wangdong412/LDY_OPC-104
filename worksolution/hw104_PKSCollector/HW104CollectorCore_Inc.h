//========================================================================
/// <summary>
///		�ڲ�����ͷ�ļ�
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: HW104CollectorCore_Inc.h
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
///			V0.10 : ���� 2009-10-16 10:47 �����ļ�
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
/* INI �����ļ�����                                                                     
/************************************************************************/
#define CONFIGFILENAME		L"104Trans.ini"
//�ɼ���������Ϣ
#define SECTIONNAME			L"PKSCOLLECTOR"
#define CYCLESTIMES			L"CollectCycles"
#define COLLECTORTIMEOUT	L"CollectTimeOut"
#define THRESHOLD			L"ThresHold"
#define MEMORYBUFFER		L"MemoryBuffer"
//PKSServer����
#define PKSSERVERSECTION    L"PKSServer"
#define SERVERNumber		L"ServerNum"
#define SERVERNAME			L"Server"

/************************************************************************/
/* ��־�ļ����                                                                     
/************************************************************************/
#define PKSLOGNAME L"opcda_collector.log"
#define MAX_ERROR_LEN 256


#include "hw104_PKSCollector/HW104Collector_ExtInc.h"

/************************************************************************/
/* ���ú궨��                                                                     
/************************************************************************/


/************************************************************************/
/* �ڲ�ʹ��ͷ�ļ�                                                                     
/************************************************************************/
#include "HW104CollectorCore_Datatype.h"
#include "HW104Help.h"

#define HW104PKSLog(Category, msg)	\
HW104LOG( g_HW104Log, Category, msg )

//========================================================================
#endif #define __HW104CollectorCore_Inc_H_INCLUDED__