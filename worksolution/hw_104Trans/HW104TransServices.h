//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	104Trans
///		文件名: HW104TransServices.h
///		
///	</file>
///	<versions>
///		<version>
///			V0.10 :  [11/27/2009] 创建文件
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
	///   停止事件
	/// </summary>
	HANDLE m_hStopEvent;

	/// <summary>
	///   线程句柄
	/// </summary>
	HANDLE m_hWorkThread;

	/// <summary>
	///   线程ID
	/// </summary>
	unsigned m_dwWorkThreadID;
};	

#endif #define _HW104TRANSSERVICES_H_INCLUDED__

