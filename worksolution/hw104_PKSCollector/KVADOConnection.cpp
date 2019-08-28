///========================================================================
/// <summary>
///	    数据库连接类KVADOConnection的实现	
/// </summary>
///	<file>
///		项目名:	自动化测试框架KVAutoTestFrame
///		文件名: KVADOConnection.h
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
#include "KVADOConnection.h"


///========================================================================
/// Implementation of the class KVADOConnection 
///========================================================================
KVADOConnection::KVADOConnection()
{
	/// 初始化COM库
	CoInitialize(NULL);

	m_pConn = NULL;
	m_strLastError = _T("");
}

KVADOConnection::~KVADOConnection()
{
	if (IsValid())
	{
		if (m_pConn->State)
		{
			m_pConn->Close();
		}
		m_pConn.Release();
	}
	m_pConn = NULL;

	/// 卸载COM库
	CoUninitialize();
}

BOOL KVADOConnection::CreateInstance()
{
	try
	{
		/// 创建Connection对象实例
		HRESULT hr = m_pConn.CreateInstance(__uuidof(Connection));
		if (FAILED(hr))
		{
			m_strLastError.Format(_T("_ConnectionPtr.CreateInstance Failed! ReturnCode: %08X"), hr);
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

BOOL KVADOConnection::IsValid()
{
	if (m_pConn == NULL)
	{
		return FALSE;
	}
	if (m_pConn.GetInterfacePtr() == NULL)
	{
		return FALSE;
	}

	return TRUE;
}

_ConnectionPtr KVADOConnection::GetConnection()
{
	if (IsValid())
	{
		return m_pConn;
	}

	return NULL;
}

BOOL KVADOConnection::Open(_bstr_t bstrConnectionString, long nOptions)
{
	if (!IsValid())
	{
		return FALSE;
	}

	m_pConn->CursorLocation = adUseClient;
	try
	{
		/// 连接数据库
		HRESULT hr = m_pConn->Open(bstrConnectionString, _T(""), _T(""), nOptions);
		if (FAILED(hr))
		{
			m_strLastError.Format(_T("_ConnectionPtr.Open Failed! ReturnCode: %08X"), hr);
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

void KVADOConnection::Close()
{
	if (IsValid())
	{
		if (m_pConn->State)
		{
			m_pConn->Close();
		}
	}
}

_RecordsetPtr KVADOConnection::Execute(_bstr_t bstrCommandText, long nOptions)
{
	if (!IsValid())
	{
		return NULL;
	}

	_RecordsetPtr pRecordset;
	/// 操作完成后所影响的行数
	_variant_t vtRecordsAffected;

	try
	{
		pRecordset = m_pConn->Execute(bstrCommandText, &vtRecordsAffected, nOptions);
	}
	catch (_com_error &Error)
	{
		m_strLastError = (TCHAR*)Error.Description();
		TRACE(_T("%s"), (LPCTSTR)m_strLastError);
		return NULL;
	}

	return pRecordset;
}

_RecordsetPtr KVADOConnection::OpenSchema(SchemaEnum Schema)
{
	if (!IsValid())
	{
		return NULL;
	}

	_RecordsetPtr pRecordset;

	try
	{
		/// 返回有关数据源的自述信息
		pRecordset = m_pConn->OpenSchema(Schema);
	}
	catch (_com_error &Error)
	{
		m_strLastError = (TCHAR*)Error.Description();
		TRACE(_T("%s"), (LPCTSTR)m_strLastError);
		return NULL;
	}

	return pRecordset;
}

void KVADOConnection::GetLastError(CString &strLastErrorMsg)
{
	strLastErrorMsg = m_strLastError;
}