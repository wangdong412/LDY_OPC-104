//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	104Trans.exe
///		�ļ���: HW104_APCI.h
///		��  ��: shuqiang.luan
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

	HW104_APCICODE UpdateNeedUpdateSeq(); //20091113ӦTelvent�����������
	
	HW104_APCICODE CheckTimeOut(bool bIsTelvent);

	HW104_APCICODE FillAPCI(BYTE* pBuf, const int& nBufLen , FRAMETYPE frameTyp);

private:	
	/************************************************************************/
	/* ��ʱ���                                                            */
	/************************************************************************/
	CTime m_lastSendITime;				//���һ�η���I��ʽ����֡��ʱ��
	CTime m_lastResponseITime;			//�����Iȷ�ϣ�I/S����ʱ��
	CTime m_lastSendTestTime;			//���һ�η���Test����֡��ʱ��
	CTime m_lastRecvTime;				//��һ�ν���ʱ��,������и�ʽ����Ч֡
	CTime m_lastTestConTime;			//������������Ļظ�ʱ��

	WORD m_T1;							//����APDU�����֡�ĳ�ʱ,��λs
	WORD m_T3;							//��·���г�ʱ,��λs

	/************************************************************************/
	/*���������������                                                    */
	/************************************************************************/
	WORD	m_K;							//��ӦЭ���е�K, 1~32767
	HANDLE	m_hWaitSendEvent;				//ȷ���¼�
	WORD	m_NS_Slave;						//�������
	WORD	m_NR_Slave;						//�������
	BYTE	m_NoResponseINum;				//���ͼ������ѷ��͵�δ��ȷ�ϵ�I֡
	WORD	m_lastNR_Master;				//���һ�α�ȷ�ϵ�I֡��ţ������һ�ε�Master�Ľ������
	bool	m_bTestCon;						//�Ƿ��ѻظ�������

	//ACPI��
	CBaseLock m_seqLock;

	bool    m_bInitial;						//ACPI�Ƿ񱻳�ʼ��

	bool m_bNeedUpdateSeq;      //20091113ӦTelvent�����������
};

//========================================================================
#endif #define __HW104_APCI_H_INCLUDED__


