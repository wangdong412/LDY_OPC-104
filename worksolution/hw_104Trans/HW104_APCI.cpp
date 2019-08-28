//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	104Trans
///		文件名: HW104_APCI.cpp
///		作  者: 栾述强
///	</file>
//========================================================================
#include "StdAfx.h"
#include "HW104_APCI.h"
#include "HW104TransHelp.h"

#define APCINAME  L"CHW104_APCI"
extern CHW104_Log global_HW104Log;

CHW104_APCI::CHW104_APCI():m_bInitial( false )   //是否被初始化
{
	m_lastSendITime = CTime::GetCurrentTime();
	m_lastSendTestTime = CTime::GetCurrentTime();
	m_lastRecvTime = CTime::GetCurrentTime();
	m_lastTestConTime = CTime::GetCurrentTime();
	m_lastResponseITime = CTime::GetCurrentTime();
	m_NS_Slave = 0;
	m_NR_Slave = 0;
	m_NoResponseINum = 0;
	m_lastNR_Master = 0;
	m_bTestCon = TRUE;
	
	m_K = 12;	
	m_hWaitSendEvent = NULL;	
	m_T1 = 15;
	m_T3 = 10;	
}

CHW104_APCI::~CHW104_APCI(void)
{
	if (m_hWaitSendEvent != NULL)
	{
		CloseHandle(m_hWaitSendEvent);
		m_hWaitSendEvent = NULL;
	}
}

bool CHW104_APCI::Initial()
{
	AutoLock realLock(&m_seqLock);

	m_bNeedUpdateSeq = false;  //20091113为Telvent重连机制添加

	m_bInitial = true;	

	m_lastSendITime = CTime::GetCurrentTime();
	m_lastSendTestTime = CTime::GetCurrentTime();
	m_lastRecvTime = CTime::GetCurrentTime();
	m_lastTestConTime = CTime::GetCurrentTime();
	m_lastResponseITime = CTime::GetCurrentTime();
	m_NS_Slave = 0;
	m_NR_Slave = 0;
	m_NoResponseINum = 0;
	m_lastNR_Master = 0;	
	m_bTestCon = TRUE;

	if ( m_hWaitSendEvent == NULL )
	{
		m_hWaitSendEvent = CreateEvent(
			NULL,						//security des
			FALSE,						//auto - reset
			FALSE,						//no single
			NULL	);
	}
	else
	{
		PulseEvent( m_hWaitSendEvent );
	}

	if(m_hWaitSendEvent == NULL)
	{
		return false;
	}

	//读取配置文件
	std::string strIniPath;
	CHW104TransHelp::GetIniPath(strIniPath);
	strIniPath += CONFIGFILENAME;

	//从配置文件读取t1
	WORD theT1 = GetPrivateProfileIntA( 
		HW104SECTION,
		"t1",
		30,
		strIniPath.c_str()
		);
	if ( theT1 <= 10 )
	{
		HW104TransLog( APCINAME, L"Initial, t1<=0 is wrong!" );		
		theT1 = 30; // 设置错误采用默认数值 [12/4/2009 pks]
	}

	//从配置文件中读取t3
	WORD theT3 = GetPrivateProfileIntA( 
		HW104SECTION,
		"t3",
		8,
		strIniPath.c_str()
		);
	if ( theT3 <= 0 )
	{
		HW104TransLog( APCINAME, L"Initial, t3<=0 is wrong!" );		
		theT3 = 8; // 设置错误采用默认值 [12/4/2009 pks]
	}

	//读取K
	WORD theK = GetPrivateProfileIntA( 
		HW104SECTION,
		"K",
		12,
		strIniPath.c_str()
		);
	if (theK <= 0)
	{
		HW104TransLog( APCINAME, L"Initial, K<=0 is wrong!" );
		theK = 12;
	}

	m_T1 = theT1;
	m_T3 = theT3;
	m_K = theK;

	return true;
}

HW104_APCICODE CHW104_APCI::UpdateRecvInfo(WORD NS_Master, WORD NR_Master, FRAMETYPE frameTyp)
{
	AutoLock realLock(&m_seqLock);

	if( !m_bInitial )
	{
		return NoInitial;
	}
	//20091113应Telvent重连机制添加
	//与Telvent重连后，接收及发送序号并不从0开始，而是以Telvent发送过来的第一条I帧
	//中所包含的接收及发送序号为初始序号
	if ( m_bNeedUpdateSeq )
	{
		m_NR_Slave = NS_Master;
		m_NS_Slave = NR_Master;
		m_lastNR_Master = NR_Master;
		m_NoResponseINum = 0;
		m_bNeedUpdateSeq = false;
	}
		
	bool bSetEvent = false ;

	ASSERT( m_NoResponseINum <= m_K );
	if ( m_NoResponseINum == m_K )
	{
		bSetEvent = true;
	}

	//只有I、S
	if(I_TYPE == frameTyp)
	{
		//检查master的发送序号，更新slave的接收序号
		//if (NS_Master != m_NR_Slave)
		//{
		//	//错序		
		//	return SeqError;
		//}
		//发送序号正确，将接收序号加1
		//表示范围0~32767, 溢出后重新计数
		m_NR_Slave = (m_NR_Slave+1) % 32768;
	}

	//检查master的接收序号，更新slave的接收相关计数和计时
	if (NR_Master > m_NS_Slave)
	{			
		if ((NR_Master <= m_lastNR_Master+m_NoResponseINum)
			&&(NR_Master >= m_lastNR_Master))
		{
			//正确情况
			m_NoResponseINum -= NR_Master - m_lastNR_Master;
		}
		else
		{
			//slave还没发的，已被master接收序号确认			
			//return SeqError;
			m_NoResponseINum = 0;
			m_NS_Slave = NR_Master;
			HW104TransLog( L"seq fixed", L" number of response reset to 0!" );
		}			
	}
	else if (NR_Master < m_lastNR_Master)
	{
		if (NR_Master < m_NoResponseINum)
		{
			//正确情况
			m_NoResponseINum -= 32768-m_lastNR_Master+NR_Master;
		}
		else
		{
			//之前已经被确认过			
			//return SeqError;
			m_NoResponseINum = 0;
			m_NS_Slave = NR_Master;
			HW104TransLog( L"seq fixed", L" number of no response reset to 0!" );
		}
	}
	else
	{
		//接收序号正确，更新未被确认的I帧计数
		m_NoResponseINum -= NR_Master - m_lastNR_Master;			
	}

	if (m_NoResponseINum < m_K)
	{		
		if ( bSetEvent )//需要置一下事件
		{
			//用于解除当未确认的I达到K时对发送的阻塞
			SetEvent(m_hWaitSendEvent);
		}

	}
	//更新对I的确认时间，用于T1的判断
	m_lastResponseITime = CTime::GetCurrentTime();
	//已被确认的I帧序号
	m_lastNR_Master = NR_Master;
	//所有帧的接收时间更新，用于T3的判断
	m_lastRecvTime = CTime::GetCurrentTime();

	return Normal;
}

HW104_APCICODE CHW104_APCI::UpdateSndInfo()
{
	AutoLock realLock(&m_seqLock);
	if( !m_bInitial )
	{
		return NoInitial;
	}

	m_NS_Slave = (m_NS_Slave+1)%32768;
	m_lastSendITime = CTime::GetCurrentTime();
	m_NoResponseINum++;

	return Normal;
}

HW104_APCICODE CHW104_APCI::UpdateLastRecvTime()
{
	AutoLock realLock(&m_seqLock);
	if( !m_bInitial )
	{
		return NoInitial;
	}

	//接收时间更新
	m_lastRecvTime = CTime::GetCurrentTime();

	return Normal;
}

HW104_APCICODE CHW104_APCI::UpdateSendTest()
{
	AutoLock realLock(&m_seqLock);	
	if( !m_bInitial )
	{
		return NoInitial;
	}

	m_lastSendTestTime = CTime::GetCurrentTime();
	m_bTestCon = FALSE;

	return Normal;
}

HW104_APCICODE CHW104_APCI::UpdateLastSendITime()
{
	AutoLock realLock(&m_seqLock);	
	if( !m_bInitial )
	{
		return NoInitial;
	}
	m_lastSendITime = CTime::GetCurrentTime();

	return Normal;
}

HW104_APCICODE CHW104_APCI::UpdateLastResponseITime()
{
	AutoLock realLock(&m_seqLock);
	if( !m_bInitial )
	{
		return NoInitial;
	}
	m_lastResponseITime = CTime::GetCurrentTime();

	return Normal;
}

HW104_APCICODE CHW104_APCI::UpdateTestCon()
{
	AutoLock realLock(&m_seqLock);
	if( !m_bInitial )
	{
		return NoInitial;
	}
	m_bTestCon = TRUE;
	m_lastTestConTime = CTime::GetCurrentTime();
	m_lastRecvTime = CTime::GetCurrentTime();

	return Normal;
}

HW104_APCICODE CHW104_APCI::UpdateNeedUpdateSeq()
{
	AutoLock realLock(&m_seqLock);
	if( !m_bInitial )
	{
		return NoInitial;
	}
	
	m_bNeedUpdateSeq = true;

	return Normal;
}

void CHW104_APCI::Clear()
{	
	AutoLock realLock(&m_seqLock);

	PulseEvent( m_hWaitSendEvent );
	m_bNeedUpdateSeq = false;
	m_bInitial = false;

	return;
}

bool CHW104_APCI::IsInitial( void )
{
	AutoLock realLock(&m_seqLock);

	return m_bInitial;
}

HW104_APCICODE CHW104_APCI::CheckTimeOut(bool bIsTelvent)
{
	AutoLock realLock(&m_seqLock);	

	if( !m_bInitial )
	{
		return NoInitial;
	}

	CTime curTime = CTime::GetCurrentTime();
	if ( bIsTelvent ) //telvent特殊处理, 2010.11.1, 栾述强修改, 平顶山故障
	{
		if ( FALSE == m_bTestCon ) //有未被确认的心跳包
		{
			if ( (curTime.GetTime()-m_lastSendTestTime.GetTime()) >= m_T1)				
			{
				if ((curTime.GetTime()-m_lastTestConTime.GetTime()) >= m_T1)
				{
					return T1Out;
				}
			}
		}
		else
		{
			if ((curTime.GetTime() - m_lastSendTestTime.GetTime()) >= m_T3) //不管链路是否空闲
			{
				return T3Out;
			}
		}
	}
	else
	{
		if ( m_NoResponseINum != 0 ) //有未被确认的I
		{	
			if ( (curTime.GetTime()-m_lastSendITime.GetTime()) >= m_T1)				
			{
				if ((curTime.GetTime()-m_lastResponseITime.GetTime()) >= m_T1)
				{
					return T1Out;
				}
			}
		}
		else if ( FALSE == m_bTestCon ) //有未被确认的心跳包
		{
			if ( (curTime.GetTime()-m_lastSendTestTime.GetTime()) >= m_T1)				
			{
				if ((curTime.GetTime()-m_lastTestConTime.GetTime()) >= m_T1)
				{
					return T1Out;
				}
			}
		}
		else
		{
			if ((curTime.GetTime() - m_lastRecvTime.GetTime()) >= m_T3) //链路空闲时间
			{
				return T3Out;
			}
		}
	}

	return Normal;
}

HW104_APCICODE CHW104_APCI::FillAPCI(BYTE* pBuf, const int& nLen, FRAMETYPE frameTyp)
{
	ASSERT( pBuf != NULL && nLen >= 6 );

	if ( !m_bInitial )
	{//是否被初始化
		return NoInitial;
	}

	bool bNeedWait = false;

	{
		AutoLock realLock(&m_seqLock);
		
		if ( m_NoResponseINum == m_K )
		{//需要等待，重置事件。
			bNeedWait = true;
			ResetEvent( m_hWaitSendEvent );
		}
	}

	if ( true == bNeedWait )
	{
		DWORD dwRet = WaitForSingleObject( m_hWaitSendEvent, m_T1*1000 );
		//t1超时
		if ( WAIT_TIMEOUT == dwRet )
		{
			return T1Out;
		}
	}

	AutoLock realLock(&m_seqLock);

	switch(frameTyp)
	{
	case I_TYPE:		
		//发送序号		
		pBuf[2] = LOBYTE(m_NS_Slave<<1);
		pBuf[3] = HIBYTE(m_NS_Slave<<1);
		//接收序号		
		pBuf[4] = LOBYTE(m_NR_Slave<<1);
		pBuf[5] = HIBYTE(m_NR_Slave<<1);
		break;
	case S_TYPE:
		pBuf[2] = 0x01;
		pBuf[3] = 0x0;
		//接收序号
		pBuf[4] = LOBYTE(m_NR_Slave<<1);
		pBuf[5] = HIBYTE(m_NR_Slave<<1);
		break;
	case U_TYPE:
		break;
	default:
		return OtherError;
		break;
	}

	return Normal;
}