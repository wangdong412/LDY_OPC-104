///========================================================================
/// <summary>
///	    ���ݿ�������KVADOConnection��ʵ��	
/// </summary>
///	<file>
///		��Ŀ��:	�Զ������Կ��KVAutoTestFrame
///		�ļ���: KVADOConnection.h
///		��  ��: Ƥ����
///	</file>
///	<copyright>
/// 	���������а�ȨΪ���У��κθ��˻�����֯�������κη�ʽ���ơ�ת����Ų��
/// ���ã����򽫹��ɶԵ�֪ʶ��Ȩ��Ȩ��
///	</copyright>
///	<versions>
///		<version>
///			V1.0 : Ƥ���� 2008-3-18   14:21 �����ļ�
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
	/// ��ʼ��COM��
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

	/// ж��COM��
	CoUninitialize();
}

BOOL KVADOConnection::CreateInstance()
{
	try
	{
		/// ����Connection����ʵ��
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
		/// �������ݿ�
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
	/// ������ɺ���Ӱ�������
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
		/// �����й�����Դ��������Ϣ
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