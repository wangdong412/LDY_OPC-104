//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	KingModel
///		文件名: HW104_TCPServer.h
///		作  者: 栾述强
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
		SOCKET       m_hListenSocket;	//监听socket 
		WSAEVENT     m_wsaEvent;    //监听socket关联事件
		HANDLE	     m_hListenThread;  //监听线程句柄
		BOOL		 m_bQuitListenThread;//监听线程退出标志	
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
	//server端相关
	WORD m_listenPort;								//监听端口，2404	
	CListenSocketData m_listenSocketData;			//监听socket相关数据 
	unsigned int m_dwTimeOutForListenSocket;		//监听线程中等待socket事件的超时值

	//完成端口
	HANDLE m_hCompletionPort;						//完成端口句柄
	HANDLE m_hThreadMonitorIoCompletionPort;		//完成端口处理线程句柄
	BOOL m_bQuitThreadMonitorIoCompletionPort;		//完成端口处理线程退出标志	

	//客户端相关
	BYTE m_IPNum;											//IP数
	std::map<std::string, CHW104_Slave*> m_mapHW104_Slave;  //客户端数据	

	//日志
	CString m_strLog;
};

//========================================================================
#endif #define __HW104_TCPServer_H_INCLUDED__


