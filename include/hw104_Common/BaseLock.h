//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	KingModel
///		文件名: BaseLock.h
///		作  者: 王勃
///	</file>
///	<copyright>
/// 	本代码所有版权为所有，任何个人或者组织不得以任何方式复制、转载以挪作
/// 他用，否则将构成对的产权知识侵权。
///	
///		
///	</copyright>
///	<versions>
///		<version>
///			V0.10 : 王勃 2009-10-19 18:57 创建文件
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


