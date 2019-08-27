//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: HW104Collector_ExtInc.h
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
///			V0.10 : ���� 2009-10-14 13:41 �����ļ�
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
/// ���뵼�������
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
/* �����ⲿͷ�ļ�                                                                    
/************************************************************************/
#include "hw104_Common/hw104Common_Ext_inc.h"

/************************************************************************/
/* �����ڲ�ͷ�ļ�                                                                     
/************************************************************************/
#include "hw104_PKSCollector/HW104CollectorDataType.h"
#include "hw104_PKSCollector/IHW104CollectorCallBack.h"
#include "hw104_PKSCollector/IHW104Collector.h"
#include "hw104_PKSCollector/IHW104CollectControl.h"
#include "hw104_PKSCollector/IHW104CollectorFactory.h"

/************************************************************************/
/* ���������ռ�                                                                     
/************************************************************************/
using namespace NKTNCollector;

/************************************************************************/
/* ��������                                                                     
/************************************************************************/
/// <summary>
/// 	ȫ�ֵ�������,�����೧
/// </summary>
XKTN_COLLECTOR_STDEXT IHW104CollectorFactory* ktn_GetHW104CollectorFactory();

//========================================================================
#endif #define __HW104Collector_ExtInc_H_INCLUDED__