//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	104Trans.exe
///		�ļ���: HW104_Slave.h
///		��  ��: shuqiang.luan
///	</file>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "hw104_PKSCollector/HW104Collector_ExtInc.h"
#include "DataType.h"
#include "HW104CallBack.h"
#include "HW104_APCI.h"

#ifndef __HW104_Slave_H_INCLUDED__
#define __HW104_Slave_H_INCLUDED__
//========================================================================
class CHW104_APCI;

class CMySocket
{
public:
	CMySocket():m_hSocket(INVALID_SOCKET)
	{
	}
	
	~CMySocket()
	{
		Clear();
	}

	void Initial(SOCKET hSocket)
	{
		AutoLock autoLock( &m_SocketLock ); 

		Clear();

		m_hSocket = hSocket;
	}

	bool Send( const char* pBuf, int nLen )
	{
		AutoLock autoLock( &m_SocketLock ); 

		if (INVALID_SOCKET != m_hSocket)
		{
			if(send(m_hSocket, pBuf, nLen, 0) == SOCKET_ERROR)
			{
				Clear();
				return false;
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	bool IsVaild()
	{
		AutoLock autoLock( &m_SocketLock ); 

		if ( m_hSocket == INVALID_SOCKET )
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	SOCKET GetMySocket()
	{
		AutoLock autoLock( &m_SocketLock ); 

		return m_hSocket;
	}

	void Close()
	{
		AutoLock autoLock( &m_SocketLock ); 

		Clear();
	}

private:
	void Clear()
	{
		if ( INVALID_SOCKET != m_hSocket )
		{
			shutdown(m_hSocket, SD_BOTH);
			closesocket( m_hSocket );
			m_hSocket = INVALID_SOCKET;
		}
	}

private:
	SOCKET m_hSocket;
	CBaseLock m_SocketLock;
};

class CHW104_Slave
{
public:
	CHW104_Slave();
	~CHW104_Slave();

	BOOL Initial( const SOCKET sock );	
	BOOL Clear();	
	BOOL isInitial();
	BOOL End();
public:
	BOOL OnSend(BYTE* pBuf, BYTE nLen, FRAMETYPE frameTyp);
	BOOL OnRecv(BYTE* pBuf, BYTE nLen);

	BOOL PreHandleFrame(BYTE* pBuf, BYTE nLen);			//Ԥ������
	BOOL HandleFrame(BYTE* pBuf, BYTE nLen);			//ֻ�Ǽ�¼�Ƿ������ٻ�����֡��Ҫ����
	BOOL HandleFrame_I(BYTE* pBuf, BYTE nLen);
	BOOL HandleFrame_S(BYTE* pBuf, BYTE nLen);
	BOOL HandleFrame_U(BYTE* pBuf, BYTE nLen);

	BOOL HandleFrame_C_IC_NA_1(BYTE* pBuf, BYTE nLen);
	BOOL HandleFrame_C_CI_NA_1(BYTE* pBuf, BYTE nLen);
	BOOL HandleFrame_C_CS_NA_1(BYTE* pBuf, BYTE nLen);
	BOOL HandleFrame_C_SC_NA_1(BYTE* pBuf, BYTE nLen);
	BOOL HandleFrame_C_SE_NB_1(BYTE* pBuf, BYTE nLen);
	BOOL HandleFrame_C_SE_NC_1(BYTE* pBuf, BYTE nLen);

	BOOL HandleIntrogen(BYTE* pBuf, BYTE nLen);  //�������ٻ�
	BOOL HandleIntrogen_Cegelec(BYTE* pBuf, BYTE nLen);  //cegelec�����ٲ������ٻ��޶��ʣ�20091201�޸�

	BOOL FillASDU(BYTE* pBuf, BYTE& nLen,const char* infoUnitStream, int nStreamLen,
		KTNASDUType asduType, COT theCOT, BYTE theVSQ);	//I��ʽ��֡
	BOOL FillFrame_U(BYTE* pBuf, BYTE& nLen, U_CODE funCode); //U��ʽ��֡
	//BOOL FillFrame_S(BYTE* pBuf, BYTE& nLen);	
	BOOL SendMirrorFrame(BYTE* pBuf, BYTE nLen, COT theCot, BOOL bConfirm = TRUE, BOOL bTest = FALSE); //���;�����

	void MonitorTimeOut();  //��ʱ���	

	BOOL SendCegelecInitial32( BOOL bPrimary );  //Cegelec��ʼ��ר�ã��豸����״̬
	BOOL SendCegelecInitial36( BOOL bPrimary );  //Cegelec��ʼ��ר�ã�����״̬
	//���Cegelec�����෽�������豸�Ƿ���ڵĻظ�֡������ǿ 2010-1-18	
	BOOL SendCegelecInitialDevExist();  //Cegelec��ʼ��ר�ã��豸�Ƿ����
private:
	void FuncReadFile();  //��װReadFile����������һ��IO
	void LogStream( BYTE *pBuf, WORD nLen, bool bRecv );  //����շ�����֡����־

	//add 2010.8.12, ����ǿ
	//����2010.8.5�ͻ�������: ���������ͨѶ�����������жϵ������¼��PKS����ָ�������ݵ�
	///�µ����ӽ�����, ����������͵�master�˵����Ӽ���
	BOOL AddLinkNum();
	///���ӹر�, ����������͵�master�˵����Ӽ���
	BOOL ReduceLinkNum();

public:
	/************************************************************************/
	/* Socket���                                                                     
	/************************************************************************/
	KTNCollectorTpye	m_clientType;				//�ͻ�������	
	WORD				m_CA;						//CA��ַ
	std::string			m_clientIp;					//�ͻ���IP	
	CMySocket			m_mySocket;					//�ͻ���socket
	BYTE				m_byRecvBuf[BUF_MAX_SIZE];  //���ջ�����	
	OVERLAPPED			m_overlapped;				//�ص�I/O
	std::string         m_localIP;   //����IP

	/************************************************************************/
	/* �����������߼�                                                                    
	/************************************************************************/
	/// <summary>
	/// 	������������
	/// </summary>
	CBaseLock m_controlLock;

	/// <summary>
	/// 	Slave��ʼ����־λ
	/// </summary>
	bool m_bInitial;

	//�Ƿ����ڴ������ٻ�
	BOOL m_bHandleIntrogen;
	//�Ƿ���ֹ���ٻ�
	BOOL m_bTermIntrogen;

	//APCI������
	CHW104_APCI m_APCI;

	//�������֡����
	BYTE m_maxFrameLen;

	/// <summary>
	/// 	������
	/// </summary>
	CBaseLock m_OnSendLock;

	/// <summary>
	/// 	Master�Ƿ���������վ�����ݴ��䣨StartDT��
	/// </summary>
	/// <remarks>
	///     ֻ���Cegelec���������Ե�һ�����ٻ���Ϊ��ʼ����ɱ�־������һ�����ٻ�δ��ɣ�
	///     ����仯��������
	/// </remarks>
	bool m_bInitialInfoTrans;  //���ӷ�����Ϣ������ͣ����λ

	/// <summary>
	/// 	��һ�����ٻ��Ƿ����
	/// </summary>
	/// <remarks>
	///     �Ե�һ�����ٻ���Ϊ��ʼ��
	///     ��ɱ�־    
	/// </remarks>
	bool m_bFirstCall;

	bool m_bFirstIFrame; //20091113ӦTelvent�����������	

	/************************************************************************/
	/* ��ʱ�߳�                                                                     
	/************************************************************************/
	/// <summary>
	/// 	�̴߳�����
	/// </summary>
	HANDLE m_hTimOutThread;

	/// <summary>
	/// 	�߳��˳���־λ
	/// </summary>
	bool   m_bQuitTimeOutThread; //��ʱ����߳��˳���־

	/************************************************************************/
	/* �ɼ������                                                                     
	/************************************************************************/
	/// <summary>
	/// 	�ɼ������
	/// </summary>
	IHW104Collector* m_pIHW104Collector;

	/// <summary>
	/// 	�ص�ʵ�־��
	/// </summary>
	CHW104CallBack*  m_pCallBack;	

	//����������ռ����͵�����֡
	CHW104_Log m_streamLog;
	WCHAR m_pWcharIP[32];	
};

//========================================================================
#endif #define __HW104_Slave_H_INCLUDED__
