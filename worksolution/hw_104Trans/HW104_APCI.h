//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	104Trans.exe
///		文件名: HW104_APCI.h
///		作  者: shuqiang.luan
///	</file>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "hw104_Common/hw104Common_Ext_inc.h"
#include "DataType.h"

#ifndef __HW104_APCI_H_INCLUDED__
#define __HW104_APCI_H_INCLUDED__
//========================================================================

class CHW104_APCI
{
public:
	CHW104_APCI(void);
	~CHW104_APCI(void);

	bool Initial();
	void Clear();
	bool IsInitial( void );
		
	HW104_APCICODE UpdateRecvInfo(WORD NS_Master, WORD NR_Master, FRAMETYPE frameTyp);
	HW104_APCICODE UpdateSndInfo();
	HW104_APCICODE UpdateLastRecvTime();
	HW104_APCICODE UpdateSendTest();
	HW104_APCICODE UpdateLastSendITime();	
	HW104_APCICODE UpdateLastResponseITime();
	HW104_APCICODE UpdateTestCon();

	HW104_APCICODE UpdateNeedUpdateSeq(); //20091113应Telvent重连机制添加
	
	HW104_APCICODE CheckTimeOut(bool bIsTelvent);

	HW104_APCICODE FillAPCI(BYTE* pBuf, const int& nBufLen , FRAMETYPE frameTyp);

private:	
	/************************************************************************/
	/* 超时相关                                                            */
	/************************************************************************/
	CTime m_lastSendITime;				//最后一次发送I格式数据帧的时间
	CTime m_lastResponseITime;			//最近对I确认（I/S）的时间
	CTime m_lastSendTestTime;			//最后一次发送Test数据帧的时间
	CTime m_lastRecvTime;				//上一次接收时间,针对所有格式的有效帧
	CTime m_lastTestConTime;			//最近对心跳包的回复时间

	WORD m_T1;							//发送APDU或测试帧的超时,单位s
	WORD m_T3;							//链路空闲超时,单位s

	/************************************************************************/
	/*控制域控制序号相关                                                    */
	/************************************************************************/
	WORD	m_K;							//对应协议中的K, 1~32767
	HANDLE	m_hWaitSendEvent;				//确认事件
	WORD	m_NS_Slave;						//发送序号
	WORD	m_NR_Slave;						//接收序号
	BYTE	m_NoResponseINum;				//发送计数，已发送但未被确认的I帧
	WORD	m_lastNR_Master;				//最近一次被确认的I帧序号，即最近一次的Master的接收序号
	bool	m_bTestCon;						//是否已回复心跳包

	//ACPI锁
	CBaseLock m_seqLock;

	bool    m_bInitial;						//ACPI是否被初始化

	bool m_bNeedUpdateSeq;      //20091113应Telvent重连机制添加
};

//========================================================================
#endif #define __HW104_APCI_H_INCLUDED__


