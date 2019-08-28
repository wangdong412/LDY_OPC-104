//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: BaseLock.h
///		��  ��: ����
///	</file>
///	<copyright>
/// 	���������а�ȨΪ���У��κθ��˻�����֯�������κη�ʽ���ơ�ת����Ų��
/// ���ã����򽫹��ɶԵĲ�Ȩ֪ʶ��Ȩ��
///	
///		
///	</copyright>
///	<versions>
///		<version>
///			V0.10 : ���� 2009-10-19 18:57 �����ļ�
///		</version>
///	</versions>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __BaseLock_H_INCLUDED__
#define __BaseLock_H_INCLUDED__
//========================================================================

namespace KTNCOMMON
{
	class CBaseLock
	{
	public:
		CBaseLock()
		{
			InitializeCriticalSection(&m_C_S);
		}

		~CBaseLock()
		{
			DeleteCriticalSection(&m_C_S);
		}

	public:
		void Lock()
		{
			EnterCriticalSection(&m_C_S);
		}

		void unLock()
		{
			LeaveCriticalSection(&m_C_S);
		}

	private:
		CRITICAL_SECTION m_C_S;
	};


	class AutoLock
	{
	public:
		explicit AutoLock(CBaseLock* pCBaseLock)
		{
			m_pCBaseLock = pCBaseLock;
			m_pCBaseLock->Lock();
		}

		~AutoLock()
		{
			m_pCBaseLock->unLock();
		}

	private:
		CBaseLock* m_pCBaseLock;
	};
}



//========================================================================
#endif #define __BaseLock_H_INCLUDED__


