//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: HW104_TCPServer.h
///		��  ��: ����ǿ
///	</file>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "hw104_Common/hw104Common_Ext_inc.h"
#include "HW104_Slave.h"

#ifndef __HW104_TCPServer_H_INCLUDED__
#define __HW104_TCPServer_H_INCLUDED__
//========================================================================

class CListenSocketData
{
	public:
		CListenSocketData();
		~CListenSocketData();
	public:
		SOCKET       m_hListenSocket;	//����socket 
		WSAEVENT     m_wsaEvent;    //����socket�����¼�
		HANDLE	     m_hListenThread;  //�����߳̾��
		BOOL		 m_bQuitListenThread;//�����߳��˳���־	
};

class CHW104_TCPServer
{
public:
	CHW104_TCPServer();
	~CHW104_TCPServer();

	BOOL Initial();
	BOOL ServerStart();
	BOOL ServerEnd();
    void ListenSocket();
	void MonitorIoCompletionPort();		

private:	
	LRESULT OnClientAccept(WPARAM wParam, LPARAM lParam);
	void OnClientClose(std::string strIP);

public:
	//server�����
	WORD m_listenPort;								//�����˿ڣ�2404	
	CListenSocketData m_listenSocketData;			//����socket������� 
	unsigned int m_dwTimeOutForListenSocket;		//�����߳��еȴ�socket�¼��ĳ�ʱֵ

	//��ɶ˿�
	HANDLE m_hCompletionPort;						//��ɶ˿ھ��
	HANDLE m_hThreadMonitorIoCompletionPort;		//��ɶ˿ڴ����߳̾��
	BOOL m_bQuitThreadMonitorIoCompletionPort;		//��ɶ˿ڴ����߳��˳���־	

	//�ͻ������
	BYTE m_IPNum;											//IP��
	std::map<std::string, CHW104_Slave*> m_mapHW104_Slave;  //�ͻ�������	

	//��־
	CString m_strLog;
};

//========================================================================
#endif #define __HW104_TCPServer_H_INCLUDED__


