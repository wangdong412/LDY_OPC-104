#pragma once

// 加入自动锁 [12/8/2008 bo.wang]
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
// END [12/8/2008 bo.wang]