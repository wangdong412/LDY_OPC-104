//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	104Trans
///		文件名: HW104Subscription.h
///		作  者: 王勃
///	</file>
///	<versions>
///		<version>
///			V0.10 : 王勃 [11/11/2009] 创建文件
///		</version>
///	</versions>
//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif

#include "HW104CollectorCore_Inc.h"

#ifndef _HW104SUBSCRIPTION_H_INCLUDED__
#define _HW104SUBSCRIPTION_H_INCLUDED__

//订阅发布类中的最大回调接口数量
#define MAX_CALLBACK_NUM 1

class CHW104Subscription
{
public:
	CHW104Subscription(void);
	~CHW104Subscription(void);

public:
	/// <summary>
	/// 	增加变化接口
	/// </summary>
	/// <remarks>
	///     添加成功返回true
	///     添加失败返回false
	/// </remarks>
	bool AddCallBackInterface( IHW104CollectorCallBack* pHW104CallBack );

	/// <summary>
	/// 	增加变化队列
	/// </summary>
	void OnChange( 
				  const std::vector< KTNHW104_InfoUnit >& changeUnitArray );

	/// <summary>
	/// 	变化回调处理线程
	/// </summary>	
	void ThreadMoniterChange( );

	/// <summary>
	/// 	启动变化上送
	/// </summary>
	bool Start();

	/// <summary>
	/// 	停止变化上送
	/// </summary>
	void End();	

private:
	/// <summary
	/// 	回调接口链表
	/// </summary>
	std::list< IHW104CollectorCallBack* > m_CallBackList;

	/// <summary>
	/// 	回调接口链表锁
	/// </summary>
	CBaseLock   m_CallBackListLock;

	/// <summary>
	/// 	变化ASDU队列
	/// </summary>
	std::list< std::vector< KTNHW104_InfoUnit >* > m_ChangeList;

	/// <summary>
	/// 	变化队列锁
	/// </summary>
	CBaseLock			m_ChangeListLock;

	/************************************************************************/
	/* 回调线程                                                                    
	/************************************************************************/
	HANDLE		m_hCallBackThread;

	/// <summary>
	/// 	线程控制标志位
	/// </summary>
	bool		m_bThreadControl;

	/// <summary>
	/// 	线程ID
	/// </summary>
	unsigned int	m_unThreadID;

	/// <summary>
	/// 	所占用的最大内存数值
	/// </summary>
	static unsigned int	m_nMaxMemory;
};

#endif #define _HW104SUBSCRIPTION_H_INCLUDED__


