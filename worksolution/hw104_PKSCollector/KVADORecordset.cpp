///========================================================================
/// <summary>
///	    数据库记录集类KVADORecordset的实现	
/// </summary>
///	<file>
///		项目名:	自动化测试框架KVAutoTestFrame
///		文件名: KVADORecordset.h
///		作  者: 皮敏捷
///	</file>
///	<copyright>
/// 	本代码所有版权为所有，任何个人或者组织不得以任何方式复制、转载以挪作
/// 他用，否则将构成对的知识产权侵权。
///	</copyright>
///	<versions>
///		<version>
///			V1.0 : 皮敏捷 2008-3-18   14:21 创建文件
///		</version>
///	</versions>
///========================================================================

#include "stdafx.h"
#include "KVADORecordset.h"

///========================================================================
/// Implementation of the class KVADORecordset 
///========================================================================
KVADORecordset::KVADORecordset()
{
	m_pRs = NULL;
	m_strLastError = _T("");
}

KVADORecordset::~KVADORecordset()
{
	if (IsValid())
	{
		if(m_pRs->State == adStateOpen) 
		{
			m_pRs->Close();
		}
		m_pRs.Release();
	}
	m_pRs = NULL;
}

void KVADORecordset::operator=(_RecordsetPtr pRecordset)
{
	if (IsValid())
	{
		m_pRs.Release();
	}

	m_pRs = pRecordset;
}

BOOL KVADORecordset::CreateInstance()
{
	if (IsValid())
	{
		m_pRs.Release();
	}

	try
	{
		/// 创建Recordset对象的实例
		HRESULT hr = m_pRs.CreateInstance(__uuidof(Recordset));
		if (FAILED(hr))
		{
			m_strLastError.Format(_T("_RecordsetPtr.CreateInstance Failed! ReturnCode: %08X"), hr);
			return FALSE;
		}
	}
	catch (_com_error &Error)
	{
		m_strLastError = (TCHAR*)Error.Description();
		TRACE(_T("%s"), (LPCTSTR)m_strLastError);
		return FALSE;
	}

	return TRUE;
}

BOOL KVADORecordset::IsValid()
{
	if (m_pRs == NULL)
	{
		return FALSE;
	}
	if (m_pRs.GetInterfacePtr() == NULL)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL KVADORecordset::Open(_bstr_t			bstrCommandString, 
						  KVADOConnection	*pAdoConn,
						  CursorTypeEnum	CursorType,
						  adoLockTypeEnum	LockType,
						  long              nOptions)
{
	ASSERT(pAdoConn != NULL);

	if (!IsValid())
	{
		return FALSE;
	}

	try
	{
		_ConnectionPtr pConn = pAdoConn->GetConnection();
		if (pConn == NULL || pConn.GetInterfacePtr() == NULL)
		{
			return FALSE;
		}

		_variant_t vtConn(pConn.GetInterfacePtr());
		HRESULT hr = m_pRs->Open(bstrCommandString, vtConn, CursorType, LockType, nOptions);
		if (FAILED(hr))
		{
			m_strLastError.Format(_T("_RecordsetPtr.Open Failed! ReturnCode: %08X"), hr);
			return FALSE;
		}
	}
	catch (_com_error &Error)
	{
		m_strLastError = (TCHAR*)Error.Description();
		TRACE(_T("%s"), (LPCTSTR)m_strLastError);
		return FALSE;
	}

	return TRUE;
}

bool KVADORecordset::Close()
{
	if (IsValid())
	{
		if(m_pRs->State == adStateOpen)
		{
			try
			{
				HRESULT hRes = m_pRs->Close();
				if (SUCCEEDED(hRes))
				{
					return true;
				}
			}
			catch (_com_error &Error)
			{
				m_strLastError = (TCHAR*)Error.Description();
				TRACE(_T("%s"), (LPCTSTR)m_strLastError);	
			}
		}
	}

	return false;
}

_variant_t KVADORecordset::operator()(_variant_t vtFieldName)
{
	_variant_t vtTemp(L"");

	if (IsValid())
	{
		if (m_pRs->State == adStateOpen)
		{
			try
			{
				vtTemp = m_pRs->GetCollect(vtFieldName);
			}
			catch (_com_error &Error)
			{
				//TRACE(_T("%s"), (TCHAR*)Error.Description());	
			}
		}
	}

	return vtTemp;
}

void KVADORecordset::operator()(_variant_t vtFieldName, _variant_t vtValue)
{
	if (IsValid())
	{
		if (m_pRs->State == adStateOpen)
		{
			m_pRs->PutCollect(vtFieldName, vtValue);
		}
	}
}

bool KVADORecordset::AddNew()
{
	if (IsValid())
	{
		if (m_pRs->State == adStateOpen)
		{
			try
			{
				HRESULT hRes = m_pRs->AddNew();
				if (SUCCEEDED(hRes))
				{
					return true;
				}
			}
			catch (_com_error &Error)
			{
				m_strLastError = (TCHAR*)Error.Description();
				TRACE(_T("%s"), (LPCTSTR)m_strLastError);	
			}	
		}
	}

	return false;
}

bool KVADORecordset::Update()
{
	if (IsValid())
	{
		if (m_pRs->State == adStateOpen)
		{
			try
			{
				HRESULT hRes = m_pRs->Update();
				if (SUCCEEDED(hRes))
				{
					return true;
				}
			}
			catch (_com_error &Error)
			{
				m_strLastError = (TCHAR*)Error.Description();
				TRACE(_T("%s"), (LPCTSTR)m_strLastError);	
			}
		}
	}

	return false;
}

bool KVADORecordset::Delete()
{
	if (IsValid())
	{
		if (m_pRs->State == adStateOpen)
		{
			try
			{
				HRESULT hRes = m_pRs->Delete(adAffectCurrent);
				if (SUCCEEDED(hRes))
				{
					return true;
				}
			}
			catch (_com_error &Error)
			{
				m_strLastError = (TCHAR*)Error.Description();
				TRACE(_T("%s"), (LPCTSTR)m_strLastError);	
			}
		}
	}

	return false;
}

long KVADORecordset::GetRecordCount()
{
	if (IsValid())
	{
		try
		{
			if (m_pRs->State == adStateOpen)
			{
				return m_pRs->RecordCount;
			}
		}
		catch (_com_error &Error)
		{
			TRACE(_T("%s"), (TCHAR*)Error.Description());	
			return 0;
		}
	}

	return 0;
}

/// <summary>
///   获得表的列数
/// </summary>
long KVADORecordset::GetFieldsCount()
{
	if (IsValid())
	{
		try
		{
			if (m_pRs->State == adStateOpen)
			{
				return m_pRs->Fields->GetCount();	
			}
		}
		catch (_com_error &Error)
		{
			TRACE(_T("%s"), (TCHAR*)Error.Description());	
			return 0;
		}
	}

	return 0;
}

void KVADORecordset::MoveFirst()
{
	if (IsValid())
	{
		if (m_pRs->State == adStateOpen)
		{
			m_pRs->MoveFirst();
		}
	}
}

void KVADORecordset::MoveLast()
{
	if (IsValid())
	{
		if (m_pRs->State == adStateOpen)
		{
			m_pRs->MoveLast();
		}
	}
}

void KVADORecordset::MovePrev()
{
	if (IsValid())
	{
		if (m_pRs->State == adStateOpen)
		{
			m_pRs->MovePrevious();
			if (m_pRs->BOF)
			{
				m_pRs->MoveFirst();
			}
		}
	}
}

void KVADORecordset::MoveNext()
{
	if (IsValid())
	{
		if (m_pRs->State == adStateOpen)
		{
			m_pRs->MoveNext();
			if (m_pRs->adoEOF)
			{
				m_pRs->MoveLast();
			}
		}
	}
}

void KVADORecordset::EnumTableName(std::vector<CString> &vecTableName)
{
	if (!IsValid())
	{
		return;
	}

	while (!(m_pRs->adoEOF))
	{
		/// 获取表的名称
		_bstr_t table_name = m_pRs->Fields->GetItem(_T("TABLE_NAME"))->Value;
		/// 获取表的类型
		_bstr_t table_type = m_pRs->Fields->GetItem(_T("TABLE_TYPE"))->Value;

		if (_tcscmp((LPCTSTR)table_type, _T("TABLE")) == 0)
		{
			/// 保存表的名称
			vecTableName.push_back((LPCTSTR)table_name);
		}

		m_pRs->MoveNext();
	}
}

void KVADORecordset::GetLastError(CString &strLastErrorMsg)
{
	strLastErrorMsg = m_strLastError;
}