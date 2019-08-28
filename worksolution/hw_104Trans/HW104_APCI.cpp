//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	104Trans
///		�ļ���: HW104_APCI.cpp
///		��  ��: ����ǿ
///	</file>
//========================================================================
#include "StdAfx.h"
#include "HW104_APCI.h"
#include "HW104TransHelp.h"

#define APCINAME  L"CHW104_APCI"
extern CHW104_Log global_HW104Log;

CHW104_APCI::CHW104_APCI():m_bInitial( false )   //�Ƿ񱻳�ʼ��
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

	m_bNeedUpdateSeq = false;  //20091113ΪTelvent�����������

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

	//��ȡ�����ļ�
	std::string strIniPath;
	CHW104TransHelp::GetIniPath(strIniPath);
	strIniPath += CONFIGFILENAME;

	//�������ļ���ȡt1
	WORD theT1 = GetPrivateProfileIntA( 
		HW104SECTION,
		"t1",
		30,
		strIniPath.c_str()
		);
	if ( theT1 <= 10 )
	{
		HW104TransLog( APCINAME, L"Initial, t1<=0 is wrong!" );		
		theT1 = 30; // ���ô������Ĭ����ֵ [12/4/2009 pks]
	}

	//�������ļ��ж�ȡt3
	WORD theT3 = GetPrivateProfileIntA( 
		HW104SECTION,
		"t3",
		8,
		strIniPath.c_str()
		);
	if ( theT3 <= 0 )
	{
		HW104TransLog( APCINAME, L"Initial, t3<=0 is wrong!" );		
		theT3 = 8; // ���ô������Ĭ��ֵ [12/4/2009 pks]
	}

	//��ȡK
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
	//20091113ӦTelvent�����������
	//��Telvent�����󣬽��ռ�������Ų�����0��ʼ��������Telvent���͹����ĵ�һ��I֡
	//���������Ľ��ռ��������Ϊ��ʼ���
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

	//ֻ��I��S
	if(I_TYPE == frameTyp)
	{
		//���master�ķ�����ţ�����slave�Ľ������
		//if (NS_Master != m_NR_Slave)
		//{
		//	//����		
		//	return SeqError;
		//}
		//���������ȷ����������ż�1
		//��ʾ��Χ0~32767, ��������¼���
		m_NR_Slave = (m_NR_Slave+1) % 32768;
	}

	//���master�Ľ�����ţ�����slave�Ľ�����ؼ����ͼ�ʱ
	if (NR_Master > m_NS_Slave)
	{			
		if ((NR_Master <= m_lastNR_Master+m_NoResponseINum)
			&&(NR_Master >= m_lastNR_Master))
		{
			//��ȷ���
			m_NoResponseINum -= NR_Master - m_lastNR_Master;
		}
		else
		{
			//slave��û���ģ��ѱ�master�������ȷ��			
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
			//��ȷ���
			m_NoResponseINum -= 32768-m_lastNR_Master+NR_Master;
		}
		else
		{
			//֮ǰ�Ѿ���ȷ�Ϲ�			
			//return SeqError;
			m_NoResponseINum = 0;
			m_NS_Slave = NR_Master;
			HW104TransLog( L"seq fixed", L" number of no response reset to 0!" );
		}
	}
	else
	{
		//���������ȷ������δ��ȷ�ϵ�I֡����
		m_NoResponseINum -= NR_Master - m_lastNR_Master;			
	}

	if (m_NoResponseINum < m_K)
	{		
		if ( bSetEvent )//��Ҫ��һ���¼�
		{
			//���ڽ����δȷ�ϵ�I�ﵽKʱ�Է��͵�����
			SetEvent(m_hWaitSendEvent);
		}

	}
	//���¶�I��ȷ��ʱ�䣬����T1���ж�
	m_lastResponseITime = CTime::GetCurrentTime();
	//�ѱ�ȷ�ϵ�I֡���
	m_lastNR_Master = NR_Master;
	//����֡�Ľ���ʱ����£�����T3���ж�
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

	//����ʱ�����
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
	if ( bIsTelvent ) //telvent���⴦��, 2010.11.1, ����ǿ�޸�, ƽ��ɽ����
	{
		if ( FALSE == m_bTestCon ) //��δ��ȷ�ϵ�������
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
			if ((curTime.GetTime() - m_lastSendTestTime.GetTime()) >= m_T3) //������·�Ƿ����
			{
				return T3Out;
			}
		}
	}
	else
	{
		if ( m_NoResponseINum != 0 ) //��δ��ȷ�ϵ�I
		{	
			if ( (curTime.GetTime()-m_lastSendITime.GetTime()) >= m_T1)				
			{
				if ((curTime.GetTime()-m_lastResponseITime.GetTime()) >= m_T1)
				{
					return T1Out;
				}
			}
		}
		else if ( FALSE == m_bTestCon ) //��δ��ȷ�ϵ�������
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
			if ((curTime.GetTime() - m_lastRecvTime.GetTime()) >= m_T3) //��·����ʱ��
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
	{//�Ƿ񱻳�ʼ��
		return NoInitial;
	}

	bool bNeedWait = false;

	{
		AutoLock realLock(&m_seqLock);
		
		if ( m_NoResponseINum == m_K )
		{//��Ҫ�ȴ��������¼���
			bNeedWait = true;
			ResetEvent( m_hWaitSendEvent );
		}
	}

	if ( true == bNeedWait )
	{
		DWORD dwRet = WaitForSingleObject( m_hWaitSendEvent, m_T1*1000 );
		//t1��ʱ
		if ( WAIT_TIMEOUT == dwRet )
		{
			return T1Out;
		}
	}

	AutoLock realLock(&m_seqLock);

	switch(frameTyp)
	{
	case I_TYPE:		
		//�������		
		pBuf[2] = LOBYTE(m_NS_Slave<<1);
		pBuf[3] = HIBYTE(m_NS_Slave<<1);
		//�������		
		pBuf[4] = LOBYTE(m_NR_Slave<<1);
		pBuf[5] = HIBYTE(m_NR_Slave<<1);
		break;
	case S_TYPE:
		pBuf[2] = 0x01;
		pBuf[3] = 0x0;
		//�������
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