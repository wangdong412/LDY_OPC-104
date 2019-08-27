//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	104Trans
///		文件名: HW104CallBack.cpp
///		作  者: shuqiang.luan
///	</file>
//========================================================================
#include "StdAfx.h"
#include "HW104_Slave.h"
#include "HW104CallBack.h"

CHW104CallBack::CHW104CallBack(CHW104_Slave* pSlave):m_pSlave( pSlave )
{
}

CHW104CallBack::~CHW104CallBack(void)
{
}

/// <summary>
/// 	信息单元变化回调函数
/// </summary>
/// <param name="changeUnitArray">
///		变化的信息单元数据
/// </param>
void CHW104CallBack::OnInfoUnitChange( 
							  const std::vector< KTNHW104_InfoUnit >& changeUnitArray 
							  )
{
	BYTE sndBuf[BUF_MAX_SIZE] = {0};
	BYTE nSndLen = 0;
	std::vector<KTNHW104_InfoUnit>::const_iterator itr;	
	char bufTemp[BUF_MAX_SIZE] = {0};
	BYTE nBufLen = 0;
	BYTE theVSQ = 0;
	KTNASDUType asduType = MC_UNKNOWN;

	itr = changeUnitArray.begin();			
	while(itr != changeUnitArray.end())
	{		
		//是否停止、超时、打断、退出等
		if ( !m_pSlave->isInitial() )
		{
			return;
		}
		//针对Cegelec新冗余修改，栾述强，2010-1-15
		if ( KTN_Cegelec == m_pSlave->m_clientType )
		{
			if ( !m_pSlave->m_bInitialInfoTrans )
			{
				return;
			}
		}		
		if (!m_pSlave->m_bFirstCall)
		{
			return;
		}
		if ( m_pSlave->m_bHandleIntrogen )
		{					
			return;
		}
		Sleep(2);

		AutoLock realLock(&(m_pSlave->m_controlLock));
		asduType = itr->asduType;
		memcpy(bufTemp, itr->infoUnitStream, itr->streamLen);
		nBufLen = itr->streamLen;
		theVSQ = 1;
		itr++;
		while(itr != changeUnitArray.end())
		{			
			if (itr->asduType == asduType)
			{
				if (nBufLen+itr->streamLen <= m_pSlave->m_maxFrameLen-12)
				{
					memcpy(bufTemp+nBufLen, itr->infoUnitStream, itr->streamLen);
					nBufLen += itr->streamLen;
					theVSQ++;
					itr++;
				}
				else //已达到最大帧长
				{
					m_pSlave->FillASDU(sndBuf, nSndLen, bufTemp, nBufLen, asduType, 
						               Spont, theVSQ);			
					m_pSlave->OnSend(sndBuf, nSndLen, I_TYPE);
					break;
				}
			}
			else  //ASDU不同
			{
				m_pSlave->FillASDU(sndBuf, nSndLen, bufTemp, nBufLen, asduType, 
					               Spont, theVSQ);			
				m_pSlave->OnSend(sndBuf, nSndLen, I_TYPE);				
				break;
			}
		}
		if (itr == changeUnitArray.end())
		{
			m_pSlave->FillASDU(sndBuf, nSndLen, bufTemp, nBufLen, asduType, 
				Spont, theVSQ);			
			m_pSlave->OnSend(sndBuf, nSndLen, I_TYPE);
			break;
		}
	}
	
	return;
}