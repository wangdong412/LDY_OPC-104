//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	104Trans
///		�ļ���: HW104TransServices.h
///		
///	</file>
///	<versions>
///		<version>
///			V0.10 :  [11/27/2009] �����ļ�
///		</version>
///	</versions>
//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif

#include "ntservice.h"

#ifndef _HW104TRANSSERVICES_H_INCLUDED__
#define _HW104TRANSSERVICES_H_INCLUDED__

class CHW104TransServices :
	public NTService
{
public:
	CHW104TransServices(void);
	~CHW104TransServices(void);

public:
	void		Run(DWORD argc, LPTSTR* argv);
	void		Stop(); 
	BOOL		InstallService();

private:
	/// <summary>
	///   ֹͣ�¼�
	/// </summary>
	HANDLE m_hStopEvent;

	/// <summary>
	///   �߳̾��
	/// </summary>
	HANDLE m_hWorkThread;

	/// <summary>
	///   �߳�ID
	/// </summary>
	unsigned m_dwWorkThreadID;
};	

#endif #define _HW104TRANSSERVICES_H_INCLUDED__

