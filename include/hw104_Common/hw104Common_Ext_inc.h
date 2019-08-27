//========================================================================
/// <summary>
///		HoneyWellת������
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: hw104Common_Ext_inc.h
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
///			V0.10 : ���� 2009-10-14 13:57 �����ļ�
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
/// ���뵼�������
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
/// ���ú궨��
#define WSTR(x)			L##x
#define HI32( n )		((unsigned long)( (n) >> 32 ) )
#define LO32( n )		((unsigned long)( n ) )

// λ����
#define GET_BIT( op , n )			( ( ( op ) >> ( n ) ) & 0x00000001 )
#define SET_BIT( op , n )			( ( op ) |= ( 1 << ( n ) ) )
#define CLR_BIT( op , n )			( ( op ) &= ( ~( 1 << ( n ) ) ) )

#define KTNOK(ret)   ( KTN_OK == ret )
#define KTNERR(ret)  ( KTN_OK != ret )

#define ARRAYLEN( arr ) (sizeof(arr))/sizeof(arr[0])

//����ͬ����
#define HW104_MT_INCR(mtx, x)	_InterlockedIncrement(&x)
#define HW104_MT_DECR(mtx, x)	_InterlockedDecrement(&x)
#define HW104_MT_CMPX(x, y, z)	_InterlockedCompareExchange(&x, y, z)

/// <summary>
/// 	Ĭ�ϵ�����������ʱ��(����)
/// </summary>
/// <remarks>
///     ��ֵʹ����������������CkxSpinLock�У���ϵͳ�д��ڶ�CPUʱ����Ҫ�ڵ���
/// Sleepǰ�����߳���ռ���ԣ����ﶨ���ȱʡ�ĳ��Դ�����
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
// ��׼C++ͷ�ļ�
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
/* �������Ͷ���                                                                    
/************************************************************************/
#define INVALID_GOURPID	(UINT)(-1)

/// <summary>
/// 	�ɼ�����
/// </summary>
enum KTNCollectorTpye
{
	KTNCollector_Unknown = 0,	//δ֪�ɼ�����
	KTN_HoneyWell = 1,
	KTN_Cegelec   = 2,
	KTN_Telvent   = 3
};

/// <summary>
/// 	��Ҫʵ�ֵ�ASDU����
/// </summary>
enum KTNASDUType
{
	MC_UNKNOWN = 0,					//δ֪����

	//���ӷ���Ĺ�����Ϣ
	M_SP_NA_1  = 1,					//������Ϣ
	M_DP_NA_1  = 3,                 //˫����Ϣ
	M_ME_NA_1  = 9,                 //����ֵ, ��һ��ֵ
	M_ME_NB_1  = 11,                //����ֵ����Ȼ�ֵ
	M_ME_NC_1  = 13,				//����ֵ, �̸�����
	M_IT_NA_1  = 15,				//������
	M_SP_TB_1  = 30,				//��CP56TIME2Aʱ��ĵ�����Ϣ
	M_DP_TB_1  = 31,				//��CP56TIME2Aʱ���˫����Ϣ
	M_ME_TD_1  = 34,				//��CP56TIME2Aʱ��Ĳ���ֵ����һ��ֵ
	M_ME_TE_1  = 35,				//��CP56TIME2Aʱ��Ĳ���ֵ����Ȼ�ֵ
	M_ME_TF_1  = 36,				//��ʱ��CP56Time2a�Ĳ���ֵ���̸�����
	M_IT_TB_1  = 37,			    //��CP56TIME2Aʱ��ļ�����

	//���Ʒ���Ĺ�����Ϣ
	C_SC_NA_1  = 45,				//��������
	C_DC_NA_1  = 46,				//˫������
	C_SE_NA_1  = 48,				//�趨ֵ�����һ��ֵ
	C_SE_NB_1  = 49,				//�趨ֵ�����Ȼ�ֵ
	C_SE_NC_1  = 50,				//�趨ֵ����̸�����
	C_SC_TA_1  = 58,				//��CP56TIME2Aʱ��ĵ�������
	C_DC_TA_1  = 59,				//��CP56TIME2Aʱ���˫������
	C_SE_TA_1  = 61,				//��CP56TIME2Aʱ����趨ֵ�����һ��ֵ
	C_SE_TB_1  = 62,				//��CP56TIME2Aʱ����趨ֵ�����Ȼ�ֵ
	C_SE_TC_1  = 63,			    //��CP56Time2aʱ����������̸�����

	//�ڼ��ӷ����ϵͳ����
	M_EI_NA_1  = 70,			    //��ʼ������

	//�ڿ��Ʒ����ϵͳ����
	C_IC_NA_1  = 100,			    //���ٻ�����
	C_CI_NA_1  = 101,			    //�������ٻ�����
	C_RD_NA_1  = 102,			    //������
	C_CS_NA_1  = 103,			    //ʱ��ͬ������
	C_RP_NC_1  = 105,			    //��λ��������	
};

/// <summary>
/// 	��Ϣ����/��ϢԪ��
/// </summary>
/// <remarks>
///     �����������Ŀ��ֻʵ����Ϣ����
/// </remarks>
enum KTNInfoUintType			
{
	Info_Unknown = 0,			//δ֪����
	InfoObject   = 1,			//��Ϣ����
	InfoElement  = 2			//��ϢԪ��
};

/************************************************************************/
/* ���巵��������                                                                   
/************************************************************************/
typedef long KTN_RET;
 
//=============================================================================
/// ����һЩ״̬������
#define KTN_OK			0			/// �ɹ�״̬

#define KTN_ERRORSTART			0x20008000	/// ���������ʼ����

/************************************************************************/
/// ͨ�ô����붨��
/// ��Χ:0x01 - 0x3F
#define KTN_FAILED				(KTN_ERRORSTART+0x01) /// һ����ʧ��
#define KTN_INVALIDARGS			(KTN_ERRORSTART+0x02) /// ������Ч
#define KTN_NOTIMPLEMENT		(KTN_ERRORSTART+0x03) /// δʵ��
#define KTN_ITEMNOTFOUND		(KTN_ERRORSTART+0x04) /// ��Ŀδ�ҵ�
#define KTN_INVALID_OPERATION	(KTN_ERRORSTART+0x05) /// �Ƿ�����
#define KTN_USER_CANCELED		(KTN_ERRORSTART+0x06) /// �û�ȡ������
#define KTN_ADDINEXIST			(KTN_ERRORSTART+0x07) /// ����Ѿ�����  
#define KTN_OUT_OF_MEMORY       (KTN_ERRORSTART+0x08) /// �ڴ����
/************************************************************************/
/// ģ���������ʼֵ����Χ����

/// HWCollectorģ������붨��
/// ����:0x040 - 0x06F
#define KTN_HWCOLLECTOR_ERROR_START				(KTN_ERRORSTART+0x40)

/// HWTransģ������붨��
/// ����:0x070 - 0x08F
#define KTN_HWTRANS_ERROR_START					(KTN_ERRORSTART+0x070)
//=============================================================================

/// <summary>
/// 	Ϊ������ĺ�
/// </summary>
#define KXASSERT ASSERT
#define KXSTATUS_SUCCESS KTN_OK
#define KXSTATUS_FAILED KTN_FAILED

/************************************************************************/
/* �ڲ�ͷ�ļ�����                                                                     
/************************************************************************/
#include "hw104_Common/XDBLatch.h"				  //��ͷ�ļ� 
#include "hw104_Common/BaseLock.h"
#include "hw104_Common/HW_104Log.h"


/************************************************************************/
/* �����ռ�����                                                                     
/************************************************************************/
using namespace KTNCOMMON;
using namespace NkxLatchLock;

//========================================================================
#endif #define __hw104Common_Ext_inc_H_INCLUDED__