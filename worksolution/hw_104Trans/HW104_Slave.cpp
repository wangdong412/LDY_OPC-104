//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	104Trans
///		�ļ���: HW104_Slave.cpp
///		��  ��: ����ǿ
///	</file>
//========================================================================
#include "stdafx.h"
#include "HW104_Slave.h"

//������־���
extern CHW104_Log global_HW104Log;
#define SLAVENAME L"CHW104_Slave"

//add 2010.8.12, ����ǿ
//����2010.8.5�ͻ�������: ���������ͨѶ�����������жϵ������¼��PKS����ָ�������ݵ�
volatile int g_hwLinkNum = 0; //���ڼ�¼��Honeywell��master�˵�����������, ��Ϊ0, ��ʾ��Honeywell��master�������ж�
volatile int g_teLinkNum = 0; //���ڼ�¼��Telvent��master�˵�����������
volatile int g_ceLinkNum = 0; //���ڼ�¼��Cegelec��master�˵�����������
CBaseLock g_linkNumLock; //����������������

/// <summary>
/// 	�ɼ����೧
/// </summary>
extern IHW104CollectorFactory* g_phw104Factory;

extern IHW104CollectorControl* g_phwCollectorControl;

unsigned int _stdcall ThreadHook( LPVOID p )
{
	ASSERT( NULL != p );
	
	CHW104_Slave* pSlave = (CHW104_Slave*)p;
	pSlave->MonitorTimeOut();

	return 0;
}


CHW104_Slave::CHW104_Slave():m_bInitialInfoTrans( false ),			//client���Ƿ��ʼ����ϣ�startup��
							 m_pIHW104Collector( NULL ),			//�ɼ����ӿ�
							 m_pCallBack( NULL ),					//�ص��ӿ�
							 m_bFirstCall( false ),					//�״����ٻ�
							 m_CA(0),								//CA��ַ
							 m_bQuitTimeOutThread( true ),			//��ʱ����߳��˳���־λ
							 m_maxFrameLen( 255 ),					//����֡��󳤶�
							 m_clientType( KTNCollector_Unknown ),	//�ͻ�������
							 m_bHandleIntrogen( FALSE ),			//�Ƿ����ڴ������ٻ�
							 m_bTermIntrogen( FALSE ),				//�Ƿ���ֹ���ٻ�
							 m_bFirstIFrame( false ),               //20091113ӦTelvent�����������
							 m_bInitial( false )					//Slave�Ƿ��Ѿ���ʼ���ɹ�							
{		
	memset(m_byRecvBuf, 0, sizeof(m_byRecvBuf));	
	ZeroMemory( (void*)(&m_overlapped), sizeof( m_overlapped ) );	
	ZeroMemory(m_pWcharIP, sizeof(m_pWcharIP));
}

CHW104_Slave::~CHW104_Slave()
{	
	if ( m_pIHW104Collector != NULL )
	{
		m_pIHW104Collector->DelCollector();
	}

	//�˳���ʱ����߳�
	m_bQuitTimeOutThread = true;
	if(m_hTimOutThread != NULL)
	{
		DWORD dwRet = WaitForSingleObject( m_hTimOutThread , 10000 );
		if ( WAIT_OBJECT_0 != dwRet )
		{
			TerminateThread( m_hTimOutThread, 1 );
			CloseHandle( m_hTimOutThread );
			m_hTimOutThread = NULL;
		}
	}
}

//������Ӧ��slave
BOOL CHW104_Slave::Initial( const SOCKET sock )
{
	ASSERT( sock != INVALID_SOCKET );

	m_APCI.Initial();

	m_mySocket.Initial(sock);

	//��ʼ����־λ
	m_bInitialInfoTrans = false;

	//
	if ( m_pCallBack == NULL )
	{
		m_pCallBack = new CHW104CallBack( this );
	}

	/// ��ʼ���ӿ�
	if ( m_pIHW104Collector == NULL )
	{
		KTN_RET ret = g_phw104Factory->CreateIHW104Collector( m_pIHW104Collector );

		if ( KTNERR(ret) )
		{//����ʧ��
			return FALSE;
		}

		ret = m_pIHW104Collector->RegsiterCallBack( m_clientIp,
													(IHW104CollectorCallBack*)m_pCallBack	);
	}	
	
	/************************************************************************/
	/* ������鳬ʱ�߳�                                                                     
	///************************************************************************/
	if ( true == m_bQuitTimeOutThread )
	{
		m_bQuitTimeOutThread = false;
		m_hTimOutThread = (HANDLE) _beginthreadex( 
									NULL,   
									0,
									ThreadHook,
									this,
									0,
									NULL );

		if ( NULL == m_hTimOutThread )
		{
			m_bQuitTimeOutThread = true;
			return FALSE;
		}
	}

	//��ʼ����־�࣬������ռ����͵�������
	CString strLog;
	strLog.Format(L"IP%sStream.log", m_pWcharIP);
	m_streamLog.Open((LPCWSTR)strLog);
	hw104f_LogStream( m_streamLog, L"*********Start*********" );

	//�����ӽ���, �������Ӽ�¼��
	AddLinkNum();

	//��ʼ�����
	m_bInitial = true;

	return TRUE;
}

BOOL CHW104_Slave::isInitial()
{
	AutoLock auLock( &m_controlLock );

	return m_bInitial;
}

BOOL CHW104_Slave::Clear()
{
	m_APCI.Clear();
	m_mySocket.Close();
	ReduceLinkNum();

	AutoLock auLock( &m_controlLock );	

	m_bInitialInfoTrans = false;
	m_bFirstIFrame = false;
	m_bFirstCall = false;
	m_bHandleIntrogen = false;
	m_bTermIntrogen   = false;
	m_bInitial = false;

	return TRUE;
}

BOOL CHW104_Slave::End()
{
	Clear();

	m_bQuitTimeOutThread = true;

	DWORD dwRet = WaitForSingleObject( m_hTimOutThread, 10000 );

	if ( WAIT_OBJECT_0 != dwRet )
	{
		TerminateThread( m_hTimOutThread, 0 );
	}

	if ( m_pIHW104Collector != NULL )
	{
		m_pIHW104Collector->DelCollector();
	}

	return true;
}

void CHW104_Slave::MonitorTimeOut()
{	
	while( !m_bQuitTimeOutThread )
	{	
		//  [11/11/2009 Administrator]
		HW104_APCICODE timeOut = m_APCI.CheckTimeOut( m_clientType == KTN_Telvent );//2010.11.1, ����ǿ�޸�, ƽ��ɽ����

		if ( NoInitial != timeOut )  //APCIδ��clear, ������ת
		{
			if ( T1Out == timeOut )
			{
				//t1��ʱ���ر�TCP����
				hw104f_LogStream( m_streamLog, L"\r\n\tMonitorTimeOut, T1 time out!!!" ); //���ռ�����buf��־
				CString strLog;   //����trans�ĳ���������־
				strLog.Format( L"\r\n\tClient(IP: %s) T1 time out in MonitorTimeOut!", m_pWcharIP );
				HW104TransLog( SLAVENAME, (LPCWSTR)strLog );
				TRACE( "MonitorTimeOut, T1 Time Out\n" );
				Clear();	
			}			
			else if( T3Out == timeOut )
			{				
				//t3��ʱ, ��������
				//TRACE( "T3 Time Out\n" );
				BYTE buf[BUF_MAX_SIZE] = {0};
				BYTE nLen = 0;

				//����Ϊ�������ɹ��������ȴ�ȷ��
				m_APCI.UpdateSendTest();

				if ( !FillFrame_U(buf, nLen, TestAct) )
				{
					//��������ʧ�ܲ��˳���ʱ�߳�
					TRACE("FillFrame_U failed!\n");
				}				
			}			 
		}		

		Sleep(30);
	}

	return;
}

BOOL CHW104_Slave::OnRecv(BYTE* pBuf, BYTE nLen)
{
	ASSERT(pBuf != NULL);

	LogStream( pBuf, nLen, true );

	if( !PreHandleFrame( pBuf, nLen ) )
	{
		CString strLog;
		strLog.Format( L"\r\n\tClient(IP: %s), PreHandleFrame failed!", m_pWcharIP );
		HW104TransLog( SLAVENAME, (LPCWSTR)strLog );	
		TRACE( "PreHandleFrame failed!\n" );
		Clear();
		return FALSE;
	}

	FuncReadFile();

	int nHead = 0;
	while(nHead < nLen)
	{		
		BYTE nSingleLen = pBuf[nHead+1];
		BYTE singleBuf[BUF_MAX_SIZE] = {0};
		memcpy( singleBuf, pBuf+nHead, nSingleLen+2 );
		if( !HandleFrame_I( singleBuf, nSingleLen+2 ) )
		{
			CString strLog;
			strLog.Format( L"\r\n\tClient(IP: %s), HandleFrame_I failed!", m_pWcharIP );
			HW104TransLog( SLAVENAME, (LPCWSTR)strLog );	
			TRACE( "HandleFrame_I failed!\n" );
			Clear();
			return FALSE;
		}
		nHead += nSingleLen+2;
	}		
	
	return TRUE;
}

BOOL CHW104_Slave::PreHandleFrame(BYTE* pBuf, BYTE nLen)
{
	ASSERT( pBuf != NULL );

	int nHead = 0;
	while(nHead < nLen)
	{			
		if (pBuf[nHead] != 0x68)
		{	
			CString strLog;
			strLog.Format( L"\r\n\tClient(IP: %s), frame head is wrong!", m_pWcharIP );
			HW104TransLog( SLAVENAME, (LPCWSTR)strLog );	
			TRACE( "frame head is wrong!\n" );
			return FALSE;
		}
		BYTE nSingleLen = pBuf[nHead+1];
		if ( nLen < nHead+nSingleLen+2 )
		{
			CString strLog;
			strLog.Format( L"\r\n\tClient(IP: %s), frame length is wrong!", m_pWcharIP );
			HW104TransLog( SLAVENAME, (LPCWSTR)strLog );	
			TRACE( "frame length is wrong!\n" );
			return FALSE;
		}
		BYTE singleBuf[BUF_MAX_SIZE] = {0};
		memcpy( singleBuf, pBuf+nHead, nSingleLen+2 );
		if( !HandleFrame( singleBuf, nSingleLen+2 ) )
		{
			CString strLog;
			strLog.Format( L"\r\n\tClient(IP: %s), HandleFrame failed!", m_pWcharIP );
			HW104TransLog( SLAVENAME, (LPCWSTR)strLog );	
			TRACE( "HandleFrame failed!\n" );
			return FALSE;
		}
		nHead += nSingleLen+2;
	}

	return TRUE;
}

BOOL CHW104_Slave::HandleFrame(BYTE* pBuf, BYTE nLen)
{
	ASSERT(pBuf != NULL);
	//����֡�ĸ�ʽ
	BYTE firstBit = (pBuf[2] & 0x01);	
	BYTE secondBit = (pBuf[2] & 0x02);
	if (0x0 == firstBit)
	{
		//I��ʽ֡
		//ȡ���������
		WORD NS_Master = 0;  //�������
		NS_Master = pBuf[3];
		NS_Master = (((NS_Master<<8)|pBuf[2])>>1);
		//ȡ���������
		WORD NR_Master = 0;  //�������
		NR_Master = pBuf[5];
		NR_Master = (((NR_Master<<8)|pBuf[4])>>1);

		//20091113ӦTelvent�����������	
		if ( KTN_Telvent == m_clientType )
		{
			if ( false == m_bFirstIFrame )
			{
				if ( m_APCI.UpdateNeedUpdateSeq() != Normal )
				{
					return FALSE;
				}
				else
				{
					m_bFirstIFrame = true;
				}
			}
		}					

		HW104_APCICODE apciCode = m_APCI.UpdateRecvInfo(NS_Master, NR_Master, I_TYPE);
		if ( apciCode == SeqError )
		{
			hw104f_LogStream( m_streamLog, L"\tHandleFrame, I Seq Error!!!" );
			CString strLog;
			strLog.Format( L"\r\n\tClient(IP: %s) I Seq Error in HandleFrame!", m_pWcharIP );
			HW104TransLog( SLAVENAME, (LPCWSTR)strLog );
			TRACE( "HandleFrame, I Seq Error\n" );
			return FALSE;
		}
		else if( apciCode != Normal )
		{
			return FALSE;
		}			
	}
	else
	{
		if (0x0 == secondBit)
		{
			//S��ʽ֡
			BOOL bRet = HandleFrame_S(pBuf, nLen);
			return bRet;			
		}
		else
		{
			//U��ʽ֡
			BOOL bRet = HandleFrame_U(pBuf, nLen);
			return bRet;	
		}
	}

	return TRUE;
}

void CHW104_Slave::FuncReadFile()
{
	memset( &m_overlapped, 0, sizeof(OVERLAPPED) );
	ReadFile( (HANDLE)(m_mySocket.GetMySocket()),
			m_byRecvBuf,
			BUF_MAX_SIZE,
			NULL,
			&m_overlapped );
}

//
BOOL CHW104_Slave::HandleFrame_I(BYTE* pBuf, BYTE nLen)
{
	ASSERT(pBuf != NULL);	

	//ֻ��I֡�Ŵ���
	BYTE firstBit = (pBuf[2] & 0x01);	
	if (0x0 != firstBit)
	{
		return TRUE;		
	}
	
	//У��COT
	COT theCOT = (COT)pBuf[8];
	if ((theCOT != Act)&&(theCOT != Deact))
	{
		CString strLog;
		strLog.Format( L"\r\n\tClient(IP: %s) UnknownCOT in HandleFrame_I!", m_pWcharIP );
		HW104TransLog( SLAVENAME, (LPCWSTR)strLog );
		return SendMirrorFrame(pBuf, nLen, UnknownCOT, FALSE);		
	}

	if ( KTN_Cegelec != m_clientType )  //Cegelec��CA����,����У��
	{
		//У��CA
		WORD CA_temp = pBuf[11];
		CA_temp = ((CA_temp<<8)|pBuf[10]);
		if ( CA_temp != m_CA )
		{
			CString strLog;
			strLog.Format( L"\r\n\tClient(IP: %s) UnknownCA in HandleFrame_I!", m_pWcharIP );
			HW104TransLog( SLAVENAME, (LPCWSTR)strLog );
			return SendMirrorFrame(pBuf, nLen, UnknownCA, FALSE);
		}
	}	

	//Cegelec�������ʼ��
	if ( KTN_Cegelec == m_clientType )
	{
		//���Cegelec�������෽������Զ��������״̬
		DWORD dwIOA = pBuf[14];
		dwIOA = (dwIOA<<8)|pBuf[13];
		dwIOA = (dwIOA<<8)|pBuf[12];

		if ((dwIOA == (((DWORD)32)<<16)))
		{
			if( !SendCegelecInitialDevExist() )
			{
				return FALSE;
			}
			Sleep(20);

			return SendCegelecInitial32( TRUE );
		}
		else if(dwIOA == (((DWORD)36)<<16))
		{
			return SendCegelecInitial36( TRUE );   //ֻ���͵�ǰΪ��
		}		
	}

	//**//���δStartDT, ����ӦCegelec��I�����ʱ��ŵ�APCI��Ϣ�ѱ����������Cegelec�����෽���޸�
	//����ǿ��2010-1-15
	if ( KTN_Cegelec == m_clientType )
	{
		if ( !m_bInitialInfoTrans )   //�����������ֵ�ĸı���Զ�ǵ��̵߳ģ������ؼ���
		{
			return TRUE;
		}
	}

	//�������ͱ�ʶ
	KTNASDUType TYP_temp = (KTNASDUType)pBuf[6];
	switch(TYP_temp)
	{
	case C_IC_NA_1: //���ٻ�
		{				
			BOOL bRet = HandleFrame_C_IC_NA_1(pBuf, nLen);	
			return bRet;
		}
		break;
	case C_CI_NA_1: //�������ٻ�
		{
			BOOL bRet = HandleFrame_C_CI_NA_1(pBuf, nLen);
			return bRet;
		}
		break;
	case C_CS_NA_1: //ʱ��ͬ��
		{
			BOOL bRet = HandleFrame_C_CS_NA_1(pBuf, nLen);		
			return bRet;
		}
		break;
	case C_SC_NA_1:  //���,����
		{
			BOOL bRet = HandleFrame_C_SC_NA_1(pBuf, nLen);		
			return bRet;
		}
		break;	
	case C_SE_NB_1:  //�趨ֵ����Ȼ�ֵ
		{
			BOOL bRet = HandleFrame_C_SE_NB_1(pBuf, nLen);
			return bRet;
		}
		break;	
	case C_SE_NC_1:  //�趨ֵ���̸�����
		{
			BOOL bRet = HandleFrame_C_SE_NC_1(pBuf, nLen);
			return bRet;
		}
		break;	
	default: //δ֪�����ͱ�ʶ
		{	
			CString strLog;
			strLog.Format( L"\r\n\tClient(IP: %s) UnknownTYP in HandleFrame_I!", m_pWcharIP );
			HW104TransLog( SLAVENAME, (LPCWSTR)strLog );
			return SendMirrorFrame(pBuf, nLen, UnknownTYP, FALSE);
		}		
	}

	return TRUE;
}

BOOL CHW104_Slave::HandleFrame_C_SC_NA_1(BYTE* pBuf, BYTE nLen)
{
	ASSERT(pBuf != NULL);	
	if( nLen < 16)
	{
		CString strLog;
		strLog.Format( L"\r\n\tClient(IP: %s) nLen < 16 in HandleFrame_C_SC_NA_1!", m_pWcharIP );
		HW104TransLog( SLAVENAME, (LPCWSTR)strLog );
		return FALSE;
	}		
	AutoLock realLock(&m_controlLock);
	
	COT COT_temp = (COT)pBuf[8];
	switch(COT_temp)
	{
	case Act:
		{
			//��ȷ��		
			if (!SendMirrorFrame(pBuf, nLen, ActCon))
			{
				return FALSE;
			}

			//д
			struct KTNHW104_InfoUnit infoUnit_t;
			infoUnit_t.asduType = (KTNASDUType)C_SC_NA_1;
			infoUnit_t.streamLen = 4;
			memcpy(infoUnit_t.infoUnitStream, pBuf+12, 4);
			if(m_pIHW104Collector->WriteInfoUnit(m_clientIp,infoUnit_t) != KTN_OK)
			{
				return SendMirrorFrame(pBuf, nLen, ActTerm, FALSE);
			}
			else
			{
				return SendMirrorFrame(pBuf, nLen, ActTerm);
			}
		}		
		break;
	case Deact:
		{
			CString strLog;
			strLog.Format( L"\r\n\tClient(IP: %s) UnknownCOT in HandleFrame_C_SC_NA_1!", m_pWcharIP );
			HW104TransLog( SLAVENAME, (LPCWSTR)strLog );
			return SendMirrorFrame(pBuf, nLen, UnknownCOT, FALSE);
		}
	default:		
		break;
	}

	return TRUE;
}

BOOL CHW104_Slave::HandleFrame_C_SE_NB_1(BYTE* pBuf, BYTE nLen)
{
	ASSERT(pBuf != NULL);	
	if( nLen < 18)
	{
		CString strLog;
		strLog.Format( L"\r\n\tClient(IP: %s) nLen < 18 in HandleFrame_C_SE_NB_1!", m_pWcharIP );
		HW104TransLog( SLAVENAME, (LPCWSTR)strLog );
		return FALSE;
	}	
	AutoLock realLock(&m_controlLock);

	COT COT_temp = (COT)pBuf[8];
	switch(COT_temp)
	{
	case Act:
		{
			//��ȷ��		
			if (!SendMirrorFrame(pBuf, nLen, ActCon))
			{
				return FALSE;
			}

			//д
			struct KTNHW104_InfoUnit infoUnit_t;
			infoUnit_t.asduType = (KTNASDUType)C_SE_NB_1;
			infoUnit_t.streamLen = 6;
			memcpy(infoUnit_t.infoUnitStream, pBuf+12, 6);
			if(m_pIHW104Collector->WriteInfoUnit(m_clientIp,infoUnit_t) != KTN_OK)
			{
				return SendMirrorFrame(pBuf, nLen, ActTerm, FALSE);
			}
			else
			{
				return SendMirrorFrame(pBuf, nLen, ActTerm);
			}
		}		
		break;	
	default:		
		break;
	}

	return TRUE;
}

BOOL CHW104_Slave::HandleFrame_C_SE_NC_1(BYTE* pBuf, BYTE nLen)
{
	ASSERT(pBuf != NULL);
	if( nLen < 20)
	{
		CString strLog;
		strLog.Format( L"\r\n\tClient(IP: %s) nLen < 20 in HandleFrame_C_SE_NC_1!", m_pWcharIP );
		HW104TransLog( SLAVENAME, (LPCWSTR)strLog );		
		return FALSE;
	}	
	AutoLock realLock(&m_controlLock);

	COT COT_temp = (COT)pBuf[8];
	switch(COT_temp)
	{
	case Act:
		{
			//��ȷ��		
			if (!SendMirrorFrame(pBuf, nLen, ActCon))
			{
				return FALSE;
			}

			//д
			struct KTNHW104_InfoUnit infoUnit_t;
			infoUnit_t.asduType = (KTNASDUType)C_SE_NC_1;
			infoUnit_t.streamLen = 8;
			memcpy(infoUnit_t.infoUnitStream, pBuf+12, 8);
			if(m_pIHW104Collector->WriteInfoUnit(m_clientIp,infoUnit_t) != KTN_OK)
			{
				return SendMirrorFrame(pBuf, nLen, ActTerm, FALSE);
			}
			else
			{
				return SendMirrorFrame(pBuf, nLen, ActTerm);
			}
		}		
		break;	
	default:		
		break;
	}

	return TRUE;
}

BOOL CHW104_Slave::HandleFrame_S(BYTE* pBuf, BYTE nLen)
{
	ASSERT(pBuf != NULL);

	if ( KTN_Telvent == m_clientType )
	{
		if ( false == m_bFirstIFrame )
		{
			return TRUE;
		}
	}
		
	//У��������
	WORD NR_Master = 0;  //�������
	NR_Master = pBuf[5];
	NR_Master = (((NR_Master<<8)|pBuf[4])>>1);
	HW104_APCICODE apciCode = m_APCI.UpdateRecvInfo(0, NR_Master, S_TYPE);
	if ( apciCode == SeqError )
	{
		hw104f_LogStream( m_streamLog, L"\tHandleFrame_S, S Seq Error!!!" );
		CString strLog;
		strLog.Format( L"\r\n\tClient(IP: %s) S Seq Error in HandleFrame_S!", m_pWcharIP );
		HW104TransLog( SLAVENAME, (LPCWSTR)strLog );		
		TRACE( "HandleFrame_S, S Seq Error\n" );
		return FALSE;
	}
	else if ( apciCode != Normal )
	{
		return FALSE;
	}

	if ( KTN_Telvent == m_clientType )  //Telvent������Ҫ��ÿ�յ�һ��S֡����һ��������
	{
		BYTE buf[BUF_MAX_SIZE] = {0};
		BYTE nLen = 0;
		//U��ʽ������
		if ( !FillFrame_U(buf, nLen, TestAct) )
		{
			return FALSE;
		}

		//�������ɹ��������ȴ�ȷ��
		if ( m_APCI.UpdateSendTest() != Normal )
		{
			return FALSE;
		}
	}
		
	return TRUE;
}

BOOL CHW104_Slave::HandleFrame_U(BYTE* pBuf, BYTE nLen)
{//U��ʽ�����κ���
	ASSERT(pBuf != NULL);		

	U_CODE funCode = (U_CODE)pBuf[2];
	BYTE sndBuf[BUF_MAX_SIZE] = {0};
	BYTE nSndLen = 0;

    /* Added by wangdong begin */
	char bufTemp[BUF_MAX_SIZE] = { 0 };
	BYTE nBufLen = 0;
	BYTE theVSQ = 0;
	KTNASDUType asduType = MC_UNKNOWN;
	COT theCOT = Unuse;
	/* Added by wangdong end */

	switch(funCode)
	{
	case StartAct:
		if ( KTN_Telvent == m_clientType )  //20091208�޸ģ�����֮�����յ�������Ͽ�����
		{
			if (m_bFirstCall || m_bHandleIntrogen)
			{
				CString strLog;
				strLog.Format( L"\r\n\tIP: %s recv StartDT after Introgen, connection is closed!", m_pWcharIP );
				HW104TransLog( SLAVENAME, (LPCWSTR)strLog );
				return Clear();
			}
		}
		m_bInitialInfoTrans = true;
		//���ڶ���·����T3���ж�
		if( m_APCI.UpdateLastRecvTime() != Normal )
		{
			return FALSE;
		}
		//����־
		if ( KTN_Cegelec == m_clientType )
		{
			CString strLog;
			strLog.Format( L"\r\n\tIP: %s recv StartDT!", m_pWcharIP );
			HW104TransLog( SLAVENAME, (LPCWSTR)strLog );
		}

		/* Changed by wangdong begin */
#if 0
		//��ȷ��
		return FillFrame_U(sndBuf, nSndLen, StartCon);
#else
        if( FillFrame_U(sndBuf, nSndLen, StartCon))
		{
			Sleep( 50 );

			memset( &sndBuf[0], 0, sizeof(BYTE)*(BUF_MAX_SIZE));
			asduType = M_EI_NA_1;  /* ���ͱ�ʶ����ʼ������ */
			theVSQ = 0x01;         /* �ɱ�ṹ�޶��ʣ������������ݸ���Ϊ1 */
			theCOT = Init;         /* ����ԭ�򣺳�ʼ�� */

			/* ���ݣ�00 00 00 80 */
			bufTemp[3] = 0x80;
			nBufLen = 4;

			FillASDU( sndBuf, nSndLen, bufTemp, nBufLen, asduType, theCOT, theVSQ );			
			return OnSend( sndBuf, nSndLen, I_TYPE );
		}
		else
		{
			return false;
		}
#endif

		/* Changed by wangdong end */
	case StopAct:		
		m_bInitialInfoTrans = false;
		//���ڶ���·����T3���ж�
		if( m_APCI.UpdateLastRecvTime() != Normal )
		{
			return FALSE;
		}
		//����־
		if ( KTN_Cegelec == m_clientType )
		{
			CString strLog;
			strLog.Format( L"\r\n\tIP: %s recv StopAct!", m_pWcharIP );
			HW104TransLog( SLAVENAME, (LPCWSTR)strLog );
		}
		//��ȷ��
		return FillFrame_U(sndBuf, nSndLen, StopCon);		
	case TestAct:	
		//���ڶ���·����T3���ж�
		if( m_APCI.UpdateLastRecvTime() != Normal )
		{
			return FALSE;
		}
		//��ȷ��
		return FillFrame_U(sndBuf, nSndLen, TestCon);
	case TestCon:		
		if( m_APCI.UpdateTestCon() != Normal )
		{
			return FALSE;
		}
		return TRUE;
	default:
		{
			//�����U֡
			CString strLog;
			strLog.Format( L"\r\n\tClient(IP: %s) U frame is wrong in HandleFrame_U!", m_pWcharIP );
			HW104TransLog( SLAVENAME, (LPCWSTR)strLog );
			return FALSE;
		}		
	}

	return TRUE;
}

BOOL CHW104_Slave::FillFrame_U(BYTE* pBuf, BYTE& nLen, U_CODE funCode)
{
	ASSERT(pBuf != NULL);	

	pBuf[0] = 0x68;
	pBuf[1] = 4;
	pBuf[2] = funCode;
	pBuf[3] =0;
	pBuf[4] =0;
	pBuf[5] =0;	
	nLen = 6;

	bool bRet = m_mySocket.Send( (char*)pBuf, nLen );
	if ( bRet )
	{
		LogStream( pBuf, nLen, false );
	}
	return bRet;
}

BOOL CHW104_Slave::SendMirrorFrame(BYTE* pBuf, BYTE nLen, COT theCot, BOOL bConfirm, BOOL bTest)
{	
	ASSERT(pBuf != NULL);	

	BYTE sndBuf_temp[BUF_MAX_SIZE] = {0};	
	memcpy(sndBuf_temp, pBuf, nLen);
	sndBuf_temp[8] = (BYTE)theCot;
	if (!bConfirm)
	{
		sndBuf_temp[8] = sndBuf_temp[8]|0x40;  //�񶨱�־
	}
	if (bTest)
	{
		sndBuf_temp[8] = sndBuf_temp[8]|0x80; //����
	}
	
	if (!OnSend(sndBuf_temp, nLen, I_TYPE))
	{			
		return FALSE;
	}

	return TRUE;
}

BOOL CHW104_Slave::HandleFrame_C_IC_NA_1(BYTE* pBuf, BYTE nLen)
{
	ASSERT(pBuf != NULL);	
	if( nLen < 16 )
	{
		CString strLog;
		strLog.Format( L"\r\n\tClient(IP: %s) nLen < 16 in HandleFrame_C_IC_NA_1!", m_pWcharIP );
		HW104TransLog( SLAVENAME, (LPCWSTR)strLog );
		return FALSE;
	}	
	
	//COT
	COT COT_temp = (COT)pBuf[8];
	switch(COT_temp)
	{
	case Act:
		{
			{				
				if(m_bHandleIntrogen)
				{
					CString strLog;
					strLog.Format( L"\r\n\tClient(IP: %s) the last Introgen is handling in HandleFrame_C_IC_NA_1!", m_pWcharIP );
					HW104TransLog( SLAVENAME, (LPCWSTR)strLog );
					TRACE("The last Introgen is handling!");
					return TRUE;
				}
				else
				{
					m_bHandleIntrogen = TRUE;
				}
			}

			BOOL bRet = FALSE;
			if ( KTN_Cegelec == m_clientType )  //Cegelec�����ٲ������ٻ��޶��ʣ�20091201�޸�
			{
				bRet = HandleIntrogen_Cegelec( pBuf, nLen );
			}
			else
			{
				bRet = HandleIntrogen( pBuf, nLen );
			}			
			m_bHandleIntrogen = FALSE;
			m_bTermIntrogen = FALSE;
			return bRet;			
		}		
		break;
	case Deact:
		if( m_bHandleIntrogen )
		{	
			m_bTermIntrogen = TRUE;			
		}		
		break;
	default:		
		break;
	}

	return TRUE;
}

BOOL CHW104_Slave::HandleIntrogen(BYTE* pBuf, BYTE nLen)
{
	ASSERT(pBuf != NULL);
	TRACE( L"%s: Enter HandleIntrogen\n", m_pWcharIP );
	
	std::vector<KTNHW104_InfoUnit> infoUnitArray;	
	// Ԥ���涨������ [11/11/2009 Administrator]
	infoUnitArray.reserve( 6000 );
	//QOI
	COT theQOI = (COT)pBuf[15];
	switch(theQOI)
	{
	case Introgen:
		{
			TRACE( L"%s Begin Introgen\n", m_pWcharIP);

			//C_IC ACTCON
			{
				AutoLock realLock(&m_controlLock);
				if (!SendMirrorFrame(pBuf, nLen, ActCon))
				{					
					return FALSE;
				}
			}
						
			//��������
			m_pIHW104Collector->ReadAllRealData(&m_clientIp, infoUnitArray);
			BYTE sndBuf[BUF_MAX_SIZE] = {0};
			BYTE nSndLen = 0;
			std::vector<KTNHW104_InfoUnit>::iterator itr;
			char bufTemp[BUF_MAX_SIZE] = {0};
			BYTE nBufLen = 0;
			BYTE theVSQ = 0;
			KTNASDUType asduType = MC_UNKNOWN;

			itr = infoUnitArray.begin();			
			while(itr != infoUnitArray.end())
			{
				//�Ƿ�ֹͣ����ʱ����ϡ��˳���
				if ( m_bTermIntrogen )
				{
					//��ֹͣ����ȷ��
					return SendMirrorFrame(pBuf, nLen, DeactCon);
				}
				//��������ѹر�, ��ʱ�˳�
				if ( false == m_APCI.IsInitial() )
				{
					return TRUE;
				}

				if ( KTN_Telvent == m_clientType )
				{
					Sleep( 20 );
				}
				else
				{
					Sleep( 10 );
				}

				AutoLock realLock(&m_controlLock);
				asduType = itr->asduType;
				memcpy(bufTemp, itr->infoUnitStream, itr->streamLen);
				nBufLen = itr->streamLen;
				theVSQ = 1;
				itr++;
				while(itr != infoUnitArray.end())
				{
					if (itr->asduType == asduType)
					{
						if (nBufLen+itr->streamLen <= m_maxFrameLen-12)
						{
							memcpy(bufTemp+nBufLen, itr->infoUnitStream, itr->streamLen);
							nBufLen += itr->streamLen;
							theVSQ++;
							itr++;
						}
						else //�Ѵﵽ���֡��
						{
							FillASDU(sndBuf, nSndLen, bufTemp, nBufLen,	asduType, 
								Introgen, theVSQ);			
							if (!OnSend(sndBuf, nSndLen, I_TYPE))
							{					
								return FALSE;
							}
							break;
						}
					}
					else  //ASDU��ͬ
					{
						FillASDU(sndBuf, nSndLen, bufTemp, nBufLen,	asduType, 
							Introgen, theVSQ);			
						if (!OnSend(sndBuf, nSndLen, I_TYPE))
						{					
							return FALSE;
						}
						break;
					}
				}
				if (itr == infoUnitArray.end())
				{
					FillASDU(sndBuf, nSndLen, bufTemp, nBufLen,	asduType, 
						Introgen, theVSQ);			
					if (!OnSend(sndBuf, nSndLen, I_TYPE))
					{					
						return FALSE;
					}
					break;
				}
			}
			Sleep(5);
			{
				AutoLock realLock(&m_controlLock);
				//���ٽ���C_IC ACTTERM
				if (!SendMirrorFrame(pBuf, nLen, ActTerm))
				{
					return FALSE;
				}
			}			
		}
		break;	
	default:
		{
			AutoLock realLock(&m_controlLock);
			//��ȷ��
			if (!SendMirrorFrame(pBuf, nLen, ActCon, FALSE))
			{
				return FALSE;
			}
		}
		break;
	}

	TRACE( L"%s End Introgen\n",m_pWcharIP );
	m_bFirstCall = true;

	return TRUE;
}

//cegelec�����ٲ������ٻ��޶��ʣ�20091201�޸�
BOOL CHW104_Slave::HandleIntrogen_Cegelec(BYTE* pBuf, BYTE nLen)
{
	ASSERT(pBuf != NULL);
	TRACE( L"%s: Enter Cegelec HandleIntrogen\n", m_pWcharIP );

	std::vector<KTNHW104_InfoUnit> infoUnitArray;	
	// Ԥ���涨������ [11/11/2009 Administrator]
	infoUnitArray.reserve( 6000 );

	//C_IC ACTCON
	{
		AutoLock realLock(&m_controlLock);
		if (!SendMirrorFrame(pBuf, nLen, ActCon))
		{					
			return FALSE;
		}
	}

	//��������
	m_pIHW104Collector->ReadAllRealData(&m_clientIp, infoUnitArray);
	BYTE sndBuf[BUF_MAX_SIZE] = {0};
	BYTE nSndLen = 0;
	std::vector<KTNHW104_InfoUnit>::iterator itr;
	char bufTemp[BUF_MAX_SIZE] = {0};
	BYTE nBufLen = 0;
	BYTE theVSQ = 0;
	KTNASDUType asduType = MC_UNKNOWN;

	itr = infoUnitArray.begin();			
	while(itr != infoUnitArray.end())
	{
		//�Ƿ�ֹͣ����ʱ����ϡ��˳���
		if ( m_bTermIntrogen )
		{
			//��ֹͣ����ȷ��
			return SendMirrorFrame(pBuf, nLen, DeactCon);
		}
		//��������ѹر�, ��ʱ�˳�
		if ( false == m_APCI.IsInitial() )
		{
			return TRUE;
		}
		Sleep(10);

		AutoLock realLock(&m_controlLock);
		asduType = itr->asduType;
		memcpy(bufTemp, itr->infoUnitStream, itr->streamLen);
		nBufLen = itr->streamLen;
		theVSQ = 1;
		itr++;
		while(itr != infoUnitArray.end())
		{
			if (itr->asduType == asduType)
			{
				if (nBufLen+itr->streamLen <= m_maxFrameLen-12)
				{
					memcpy(bufTemp+nBufLen, itr->infoUnitStream, itr->streamLen);
					nBufLen += itr->streamLen;
					theVSQ++;
					itr++;
				}
				else //�Ѵﵽ���֡��
				{
					FillASDU(sndBuf, nSndLen, bufTemp, nBufLen,	asduType, 
						Introgen, theVSQ);			
					if (!OnSend(sndBuf, nSndLen, I_TYPE))
					{					
						return FALSE;
					}
					break;
				}
			}
			else  //ASDU��ͬ
			{
				FillASDU(sndBuf, nSndLen, bufTemp, nBufLen,	asduType, 
					Introgen, theVSQ);			
				if (!OnSend(sndBuf, nSndLen, I_TYPE))
				{					
					return FALSE;
				}
				break;
			}
		}
		if (itr == infoUnitArray.end())
		{
			FillASDU(sndBuf, nSndLen, bufTemp, nBufLen,	asduType, 
				Introgen, theVSQ);			
			if (!OnSend(sndBuf, nSndLen, I_TYPE))
			{					
				return FALSE;
			}
			break;
		}
	}
	Sleep(5);

	{
		AutoLock realLock(&m_controlLock);
		//���ٽ���C_IC ACTTERM
		if (!SendMirrorFrame(pBuf, nLen, ActTerm))
		{
			return FALSE;
		}
	}	

	TRACE( L"%s End Introgen\n",m_pWcharIP );
	m_bFirstCall = true;

	return TRUE;
}

//�������ٻ�
BOOL CHW104_Slave::HandleFrame_C_CI_NA_1(BYTE* pBuf, BYTE nLen)
{
	ASSERT(pBuf != NULL);
	AutoLock realLock(&m_controlLock);	

	COT COT_temp = (COT)pBuf[8];

	if (!SendMirrorFrame(pBuf, nLen, ActCon))
	{
		return FALSE;
	}

	return SendMirrorFrame(pBuf, nLen, ActTerm);			
}

//ʱ��ͬ��
BOOL CHW104_Slave::HandleFrame_C_CS_NA_1(BYTE* pBuf, BYTE nLen)
{
	ASSERT(pBuf != NULL);	
	if (nLen <22)
	{
		CString strLog;
		strLog.Format( L"\r\n\tClient(IP: %s) nLen < 22 in HandleFrame_C_CS_NA_1!", m_pWcharIP );
		HW104TransLog( SLAVENAME, (LPCWSTR)strLog );
		return FALSE;
	}	
	AutoLock realLock(&m_controlLock);

	COT theCOT = (COT)pBuf[8];
	switch(theCOT)
	{
	case Act:
		{
			SYSTEMTIME locTime;
			SYSTEMTIME newTime;
			GetLocalTime(&locTime);

			WORD wMilliseconds = pBuf[16];
			wMilliseconds = (wMilliseconds<<8)|pBuf[15];
			newTime.wMilliseconds = wMilliseconds % 1000;
			newTime.wSecond = wMilliseconds/1000;
			newTime.wMinute = pBuf[17] & 0x3F;
			newTime.wHour = pBuf[18] & 0x1F;
			newTime.wDay = pBuf[19] & 0x1F;
			newTime.wDayOfWeek = pBuf[19] & 0xE0;
			newTime.wMonth = pBuf[20] & 0x0F;
			newTime.wYear = pBuf[21] & 0x7F;
			if (!SetLocalTime(&newTime))
			{
				return SendMirrorFrame(pBuf, nLen, ActCon, FALSE);
			}
			else
			{
				pBuf[15] = LOBYTE(locTime.wSecond*1000 + locTime.wMilliseconds);
				pBuf[16] = HIBYTE(locTime.wSecond*1000 + locTime.wMilliseconds);
				pBuf[17] = (BYTE)locTime.wMinute;
				pBuf[18] = (BYTE)locTime.wHour;
				pBuf[19] = (BYTE)((locTime.wDayOfWeek<<5)|locTime.wDay);
				pBuf[20] = (BYTE)locTime.wMonth;
				pBuf[21] = (BYTE)locTime.wYear;
				pBuf[8] = ActCon;
				return OnSend(pBuf, nLen, I_TYPE);
			}
		}
		break;
	default:
		{
			CString strLog;
			strLog.Format( L"\r\n\tClient(IP: %s) UnknownCOT in HandleFrame_C_CS_NA_1!", m_pWcharIP );
			HW104TransLog( SLAVENAME, (LPCWSTR)strLog );
			return SendMirrorFrame(pBuf, nLen, UnknownCOT, FALSE);
		}
	}

	return TRUE;
}

BOOL CHW104_Slave::FillASDU(BYTE* pBuf, BYTE& nLen, const char* infoUnitStream,
					int nStreamLen, KTNASDUType asduType, COT theCOT, BYTE theVSQ)
{
	ASSERT(pBuf != NULL);	
	memset(pBuf, 0, sizeof(pBuf));

	pBuf[0] = 0x68;
	pBuf[2] = 0;
	pBuf[3] = 0;
	pBuf[4] = 0;
	pBuf[5] = 0;
	pBuf[6] = (BYTE)asduType;
	pBuf[7] = theVSQ;
	pBuf[8] = theCOT;
	pBuf[9] = 0;
	pBuf[10] = LOBYTE(m_CA);
	pBuf[11] = HIBYTE(m_CA);
	memcpy(pBuf+12, infoUnitStream, nStreamLen);	
	nLen = 12+nStreamLen; //���ͳ���
	pBuf[1] = nLen-2;  //APDU����

	return TRUE;
}

BOOL CHW104_Slave::OnSend(BYTE* pBuf, BYTE nLen, FRAMETYPE frameTyp)
{
	ASSERT(pBuf != NULL);	
	//����
	AutoLock realLock(&m_OnSendLock);	

	HW104_APCICODE apciCode = m_APCI.FillAPCI(pBuf, nLen, frameTyp);
	if( T1Out == apciCode )
	{
		hw104f_LogStream( m_streamLog, L"\tOnSend, T1 time out!!!" );
		CString strLog;
		strLog.Format( L"\r\n\tClient(IP: %s) T1 time out in OnSend!", m_pWcharIP );
		HW104TransLog( SLAVENAME, (LPCWSTR)strLog );
		TRACE( "OnSend(), T1 time out\n" );
		return FALSE;
	}
	else if( Normal != apciCode )
	{
		return FALSE;
	}
	
	//����Ϊ���ͳɹ�, �������
	if (frameTyp == I_TYPE)
	{
		if( m_APCI.UpdateSndInfo() != Normal )
		{
			return FALSE;
		}
	}
	else if (frameTyp == U_TYPE)
	{
		//t1���жϣ�ֻ����ѷ���I�������������͵Ķ�Master��U֡��ȷ�ϻظ��������ʱ��. 
		//���������ķ���ʱ�����ڳ�ʱ�߳��и���		
	}
	else if (frameTyp == S_TYPE)
	{
		//�����Ƕ�t1����t3���жϣ�����S֡������Ҫ����ʱ��		
	}

	if( m_mySocket.Send((char*)pBuf, nLen) == false )
	{
		return FALSE;
	}	

	LogStream( pBuf, nLen, false );
	
	return TRUE;
}

void CHW104_Slave::LogStream( BYTE *pBuf, WORD nLen, bool bRecv )
{
	ASSERT(pBuf != NULL);

	CString bufStr = L"\r\n  ";
	CString strMark = L"";
	if ( bRecv )
	{
		bufStr += L"Recv:\t";
		strMark = L"--->";
	}
	else
	{
		bufStr += L"Send:\t";
		strMark = L"<---";
	}	

	for(int i=0; i<nLen; i++)
	{
		bufStr.Format( bufStr + L"%02x ", (unsigned char)(*(pBuf+i)));
	}

	BYTE firstBit = (pBuf[2] & 0x01);	
	BYTE secondBit = (pBuf[2] & 0x02);
	if (0x0 == firstBit)  //I
	{
		KTNASDUType TYP = (KTNASDUType)pBuf[6];
		CString strTYP;
		switch(TYP)
		{
		case C_IC_NA_1: //���ٻ�
			strTYP = L"C_IC_NA_1";
			break;
		case C_CI_NA_1: //�������ٻ�
			strTYP = L"C_CI_NA_1";
			break;
		case C_CS_NA_1: //ʱ��ͬ��
			strTYP = L"C_CS_NA_1";
			break;
		case C_SC_NA_1:  //���,����
			strTYP = L"C_SC_NA_1";
			break;	
		case C_SE_NB_1:  //�趨ֵ����Ȼ�ֵ
			strTYP = L"C_SE_NB_1";
			break;	
		case C_SE_NC_1:  //�趨ֵ���̸�����
			strTYP = L"C_SE_NC_1";
			break;	
		default: //δ֪�����ͱ�ʶ
			if ( bRecv )
			{
				strTYP = L"Unknown TYP";
			}
			else
			{
				strTYP = L"";
			}			
			break;
		}

		COT theCOT = (COT)(pBuf[8]&0x3F);
		CString strCOT;
		switch( theCOT )
		{
		case Act:
			strCOT = L"Act";
			break;
		case ActCon:
			strCOT = L"ActCon";
			break;
		case Deact:
			strCOT = L"Deact";
			break;
		case DeactCon:
			strCOT = L"DeactCon";
			break;
		case ActTerm:
			strCOT = L"ActTerm";
			break;
		default:
			if ( bRecv )
			{
				strCOT = L"Unknown COT";
			}
			else
			{
				strCOT = L"";
			}		
			break;
		}

		bufStr.Format( bufStr + L"\r\n  \t" + strMark + L"I: NS=%d, NR=%d\t" + strTYP + L"\t" + strCOT, (((((WORD)pBuf[3])<<8)|pBuf[2])>>1), (((((WORD)pBuf[5])<<8)|pBuf[4])>>1) );
	}
	else
	{
		if (0x0 == secondBit)  //S
		{
			bufStr.Format( bufStr + L"\r\n  \t" + strMark + L"S: NR=%d", (((((WORD)pBuf[5])<<8)|pBuf[4])>>1) );			
		}
		else //U
		{
			U_CODE uCode = (U_CODE)pBuf[2];
			CString strUCode;
			switch( uCode )
			{
			case StartAct:
				strUCode = L"StartAct";
				break;
			case StartCon:		
				strUCode = L"StartCon";
				break;
			case StopAct:
				strUCode = L"StopAct";
				break;	
			case StopCon:		
				strUCode = L"StopCon";
				break;
			case TestAct:	
				strUCode = L"TestAct";
				break;
			case TestCon:		
				strUCode = L"TestCon";
				break;
			default:
				strUCode = L"U Frame Error!";
				break;	
			}
			bufStr.Format( bufStr + L"\r\n  \t" + strMark + L"U: " + strUCode );
		}
	}	
	hw104f_LogStream( m_streamLog, (LPCWSTR)bufStr );
}

//Cegelec��ʼ��ר�ã�����״̬
BOOL CHW104_Slave::SendCegelecInitial36( BOOL bPrimary )
{	
	//ȡ����IP�����һ��
	int ip1, ip2, ip3, ip4;
	int nRet = sscanf( m_localIP.c_str(), "%d.%d.%d.%d", &ip1, &ip2, &ip3, &ip4 );
	if ( nRet != 4 )
	{
		return FALSE;
	}
	if ( (ip4<0)||(ip4>255) )
	{
		return FALSE;
	}

	BYTE bufCegelec[BUF_MAX_SIZE] = {0};
	BYTE nLenCegelec = 0;

	bufCegelec[0] = 0x68;
	bufCegelec[2] = 0;
	bufCegelec[3] = 0;
	bufCegelec[4] = 0;
	bufCegelec[5] = 0;
	bufCegelec[6] = (BYTE)M_SP_NA_1;
	bufCegelec[7] = 1;
	bufCegelec[8] = (BYTE)Spont;
	bufCegelec[9] = 0;
	bufCegelec[10] = 0;
	bufCegelec[11] = HIBYTE(m_CA);	
	bufCegelec[12] = (BYTE)ip4;  //IP����һ��
	bufCegelec[13] = 0;   //ר��IOA
	bufCegelec[14] = 15;  //ר��IOA
	bufCegelec[15] = (BYTE)bPrimary;  //1��ǰSlaveΪ����0Ϊ��
	bufCegelec[1] = 14;
	nLenCegelec = 16;
	if (!OnSend(bufCegelec, nLenCegelec, I_TYPE))
	{
		return FALSE;
	}		

	return TRUE;
}

//Cegelec��ʼ��ר�ã��豸����״̬
BOOL CHW104_Slave::SendCegelecInitial32( BOOL bPrimary )
{
	BYTE bufCegelec[BUF_MAX_SIZE] = {0};
	BYTE nLenCegelec = 0;	

	bufCegelec[0] = 0x68;
	bufCegelec[2] = 0;
	bufCegelec[3] = 0;
	bufCegelec[4] = 0;
	bufCegelec[5] = 0;
	bufCegelec[6] = (BYTE)M_SP_NA_1;
	bufCegelec[7] = 1;
	bufCegelec[8] = (BYTE)Spont;
	bufCegelec[9] = 0;
	bufCegelec[10] = 0;
	bufCegelec[11] = HIBYTE(m_CA);	
	bufCegelec[12] = LOBYTE(m_CA);
	bufCegelec[13] = HIBYTE(m_CA);
	bufCegelec[14] = 1;  //ר��IOA
	bufCegelec[15] = (BYTE)bPrimary;  //1�豸����������0������
	bufCegelec[1] = 14;
	nLenCegelec = 16;
	if (!OnSend(bufCegelec, nLenCegelec, I_TYPE))
	{
		return FALSE;
	}

	return TRUE;
}

//���Cegelec�����෽�������豸�Ƿ���ڵĻظ�֡������ǿ 2010-1-18
//Cegelec��ʼ��ר�ã��豸�Ƿ����
BOOL CHW104_Slave::SendCegelecInitialDevExist()
{
	BYTE bufCegelec[BUF_MAX_SIZE] = {0};
	BYTE nLenCegelec = 0;	

	bufCegelec[0] = 0x68;
	bufCegelec[2] = 0;
	bufCegelec[3] = 0;
	bufCegelec[4] = 0;
	bufCegelec[5] = 0;
	bufCegelec[6] = (BYTE)M_SP_NA_1;
	bufCegelec[7] = 1;
	bufCegelec[8] = (BYTE)Spont;
	bufCegelec[9] = 0;
	bufCegelec[10] = 0;
	bufCegelec[11] = HIBYTE(m_CA);	
	bufCegelec[12] = LOBYTE(m_CA);
	bufCegelec[13] = HIBYTE(m_CA);
	bufCegelec[14] = 27;  //ר��IOA
	bufCegelec[15] = 0;  //0��ʾ�豸����
	bufCegelec[1] = 14;
	nLenCegelec = 16;
	if (!OnSend(bufCegelec, nLenCegelec, I_TYPE))
	{
		return FALSE;
	}

	return TRUE;
}

///�µ����ӽ�����, ����������͵�master�˵����Ӽ���
BOOL CHW104_Slave::AddLinkNum()
{
	{
		AutoLock realLock(&g_linkNumLock);
		switch (m_clientType)
		{
		case KTN_HoneyWell:
			++g_hwLinkNum;		
			break;
		case KTN_Telvent:
			++g_teLinkNum;		
			break;
		case KTN_Cegelec:
			++g_ceLinkNum;		
			break;
		}
	}

	if ( KTN_OK != m_pIHW104Collector->UpdateLinkErr(m_clientType, TRUE) )
	{
		CString strLog;
		strLog.Format( L"\r\n\tFail to UpdateLinkErr, in AddLinkNum!" );
		HW104TransLog( SLAVENAME, (LPCWSTR)strLog );
		return FALSE;
	}
	
	return TRUE;
}

BOOL CHW104_Slave::ReduceLinkNum()
{
	AutoLock realLock(&g_linkNumLock);
	
	switch (m_clientType)
	{
	case KTN_HoneyWell:
		//��������1
		--g_hwLinkNum;
		//��1֮���ֵ
		if (0 == g_hwLinkNum)
		{
			//������Ϊ0, �������master������״̬��¼
			if ( KTN_OK != m_pIHW104Collector->UpdateLinkErr(KTN_HoneyWell, FALSE) )
			{
				CString strLog;
				strLog.Format( L"\r\n\tFail to UpdateLinkErr!" );
				HW104TransLog( SLAVENAME, (LPCWSTR)strLog );
				return FALSE;
			}
		}
		else if (g_hwLinkNum < 0)//g_hwLinkNum��СΪ0
		{
			g_hwLinkNum = 0;
		}
		break;
	case KTN_Telvent:		
		--g_teLinkNum;		
		if (0 == g_teLinkNum)
		{			
			if ( KTN_OK != m_pIHW104Collector->UpdateLinkErr(KTN_Telvent, FALSE) )
			{
				CString strLog;
				strLog.Format( L"\r\n\tFail to UpdateLinkErr!" );
				HW104TransLog( SLAVENAME, (LPCWSTR)strLog );
				return FALSE;
			}
		}
		else if (g_teLinkNum < 0)
		{
			g_teLinkNum = 0;
		}		
		break;
	case KTN_Cegelec:
		--g_ceLinkNum;		
		if (0 == g_ceLinkNum)
		{			
			if ( KTN_OK != m_pIHW104Collector->UpdateLinkErr(KTN_Cegelec, FALSE) )
			{
				CString strLog;
				strLog.Format( L"\r\n\tFail to UpdateLinkErr!" );
				HW104TransLog( SLAVENAME, (LPCWSTR)strLog );
				return FALSE;
			}
		}
		else if (g_ceLinkNum < 0)
		{
			g_ceLinkNum = 0;
		}				
		break;
	}	

	return TRUE;
}