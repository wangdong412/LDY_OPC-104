//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	104Trans.exe
///		文件名: HW104_Slave.h
///		作  者: shuqiang.luan
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

	BOOL PreHandleFrame(BYTE* pBuf, BYTE nLen);			//预处理函数
	BOOL HandleFrame(BYTE* pBuf, BYTE nLen);			//只是记录是否有总召唤数据帧需要处理
	BOOL HandleFrame_I(BYTE* pBuf, BYTE nLen);
	BOOL HandleFrame_S(BYTE* pBuf, BYTE nLen);
	BOOL HandleFrame_U(BYTE* pBuf, BYTE nLen);

	BOOL HandleFrame_C_IC_NA_1(BYTE* pBuf, BYTE nLen);
	BOOL HandleFrame_C_CI_NA_1(BYTE* pBuf, BYTE nLen);
	BOOL HandleFrame_C_CS_NA_1(BYTE* pBuf, BYTE nLen);
	BOOL HandleFrame_C_SC_NA_1(BYTE* pBuf, BYTE nLen);
	BOOL HandleFrame_C_SE_NB_1(BYTE* pBuf, BYTE nLen);
	BOOL HandleFrame_C_SE_NC_1(BYTE* pBuf, BYTE nLen);

	BOOL HandleIntrogen(BYTE* pBuf, BYTE nLen);  //处理总召唤
	BOOL HandleIntrogen_Cegelec(BYTE* pBuf, BYTE nLen);  //cegelec的总召不区分召唤限定词，20091201修改

	BOOL FillASDU(BYTE* pBuf, BYTE& nLen,const char* infoUnitStream, int nStreamLen,
		KTNASDUType asduType, COT theCOT, BYTE theVSQ);	//I格式组帧
	BOOL FillFrame_U(BYTE* pBuf, BYTE& nLen, U_CODE funCode); //U格式组帧
	//BOOL FillFrame_S(BYTE* pBuf, BYTE& nLen);	
	BOOL SendMirrorFrame(BYTE* pBuf, BYTE nLen, COT theCot, BOOL bConfirm = TRUE, BOOL bTest = FALSE); //发送镜像报文

	void MonitorTimeOut();  //超时检查	

	BOOL SendCegelecInitial32( BOOL bPrimary );  //Cegelec初始化专用，设备工作状态
	BOOL SendCegelecInitial36( BOOL bPrimary );  //Cegelec初始化专用，主备状态
	//针对Cegelec新冗余方案增加设备是否存在的回复帧，栾述强 2010-1-18	
	BOOL SendCegelecInitialDevExist();  //Cegelec初始化专用，设备是否存在
private:
	void FuncReadFile();  //封装ReadFile函数，发起一次IO
	void LogStream( BYTE *pBuf, WORD nLen, bool bRecv );  //输出收发数据帧的日志

	//add 2010.8.12, 栾述强
	//用于2010.8.5客户新需求: 将与各中心通讯连接正常或中断的情况记录到PKS库中指定的数据点
	///新的连接建立后, 增加与该类型的master端的链接计数
	BOOL AddLinkNum();
	///连接关闭, 减少与该类型的master端的链接计数
	BOOL ReduceLinkNum();

public:
	/************************************************************************/
	/* Socket相关                                                                     
	/************************************************************************/
	KTNCollectorTpye	m_clientType;				//客户端类型	
	WORD				m_CA;						//CA地址
	std::string			m_clientIp;					//客户端IP	
	CMySocket			m_mySocket;					//客户端socket
	BYTE				m_byRecvBuf[BUF_MAX_SIZE];  //接收缓冲区	
	OVERLAPPED			m_overlapped;				//重叠I/O
	std::string         m_localIP;   //本地IP

	/************************************************************************/
	/* 数据流控制逻辑                                                                    
	/************************************************************************/
	/// <summary>
	/// 	数据流控制锁
	/// </summary>
	CBaseLock m_controlLock;

	/// <summary>
	/// 	Slave初始化标志位
	/// </summary>
	bool m_bInitial;

	//是否正在处理总召唤
	BOOL m_bHandleIntrogen;
	//是否终止总召唤
	BOOL m_bTermIntrogen;

	//APCI控制类
	CHW104_APCI m_APCI;

	//最大数据帧长度
	BYTE m_maxFrameLen;

	/// <summary>
	/// 	发送锁
	/// </summary>
	CBaseLock m_OnSendLock;

	/// <summary>
	/// 	Master是否已启动本站的数据传输（StartDT）
	/// </summary>
	/// <remarks>
	///     只针对Cegelec，而且仍以第一次总召唤做为初始化完成标志，若第一次总召唤未完成，
	///     不会变化上送数据
	/// </remarks>
	bool m_bInitialInfoTrans;  //监视方向信息传输启停控制位

	/// <summary>
	/// 	第一次总召唤是否完成
	/// </summary>
	/// <remarks>
	///     以第一次总召唤做为初始化
	///     完成标志    
	/// </remarks>
	bool m_bFirstCall;

	bool m_bFirstIFrame; //20091113应Telvent重连机制添加	

	/************************************************************************/
	/* 超时线程                                                                     
	/************************************************************************/
	/// <summary>
	/// 	线程处理句柄
	/// </summary>
	HANDLE m_hTimOutThread;

	/// <summary>
	/// 	线程退出标志位
	/// </summary>
	bool   m_bQuitTimeOutThread; //超时检查线程退出标志

	/************************************************************************/
	/* 采集器句柄                                                                     
	/************************************************************************/
	/// <summary>
	/// 	采集器句柄
	/// </summary>
	IHW104Collector* m_pIHW104Collector;

	/// <summary>
	/// 	回调实现句柄
	/// </summary>
	CHW104CallBack*  m_pCallBack;	

	//用于输出接收及发送的数据帧
	CHW104_Log m_streamLog;
	WCHAR m_pWcharIP[32];	
};

//========================================================================
#endif #define __HW104_Slave_H_INCLUDED__
