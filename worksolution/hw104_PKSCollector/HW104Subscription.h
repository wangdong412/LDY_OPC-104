//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	104Trans
///		�ļ���: HW104Subscription.h
///		��  ��: ����
///	</file>
///	<versions>
///		<version>
///			V0.10 : ���� [11/11/2009] �����ļ�
///		</version>
///	</versions>
//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif

#include "HW104CollectorCore_Inc.h"

#ifndef _HW104SUBSCRIPTION_H_INCLUDED__
#define _HW104SUBSCRIPTION_H_INCLUDED__

//���ķ������е����ص��ӿ�����
#define MAX_CALLBACK_NUM 1

class CHW104Subscription
{
public:
	CHW104Subscription(void);
	~CHW104Subscription(void);

public:
	/// <summary>
	/// 	���ӱ仯�ӿ�
	/// </summary>
	/// <remarks>
	///     ��ӳɹ�����true
	///     ���ʧ�ܷ���false
	/// </remarks>
	bool AddCallBackInterface( IHW104CollectorCallBack* pHW104CallBack );

	/// <summary>
	/// 	���ӱ仯����
	/// </summary>
	void OnChange( 
				  const std::vector< KTNHW104_InfoUnit >& changeUnitArray );

	/// <summary>
	/// 	�仯�ص������߳�
	/// </summary>	
	void ThreadMoniterChange( );

	/// <summary>
	/// 	�����仯����
	/// </summary>
	bool Start();

	/// <summary>
	/// 	ֹͣ�仯����
	/// </summary>
	void End();	

private:
	/// <summary
	/// 	�ص��ӿ�����
	/// </summary>
	std::list< IHW104CollectorCallBack* > m_CallBackList;

	/// <summary>
	/// 	�ص��ӿ�������
	/// </summary>
	CBaseLock   m_CallBackListLock;

	/// <summary>
	/// 	�仯ASDU����
	/// </summary>
	std::list< std::vector< KTNHW104_InfoUnit >* > m_ChangeList;

	/// <summary>
	/// 	�仯������
	/// </summary>
	CBaseLock			m_ChangeListLock;

	/************************************************************************/
	/* �ص��߳�                                                                    
	/************************************************************************/
	HANDLE		m_hCallBackThread;

	/// <summary>
	/// 	�߳̿��Ʊ�־λ
	/// </summary>
	bool		m_bThreadControl;

	/// <summary>
	/// 	�߳�ID
	/// </summary>
	unsigned int	m_unThreadID;

	/// <summary>
	/// 	��ռ�õ�����ڴ���ֵ
	/// </summary>
	static unsigned int	m_nMaxMemory;
};

#endif #define _HW104SUBSCRIPTION_H_INCLUDED__


