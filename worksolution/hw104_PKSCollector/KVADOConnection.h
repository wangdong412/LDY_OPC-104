///========================================================================
/// <summary>
///	    ADO������KVADOConnection����	
/// </summary>
///	<file>
///		��Ŀ��:	�Զ������Կ��KVAutoTestFrame
///		�ļ���: KVADOConnection.h
///		��  ��: Ƥ����
///	</file>
///	<copyright>
/// 	���������а�ȨΪ���У��κθ��˻�����֯�������κη�ʽ���ơ�ת����Ų��
/// ���ã����򽫹��ɶԵ�֪ʶ��Ȩ��Ȩ��
///	
///		
///	</copyright>
///	<versions>
///		<version>
///			V1.0 : Ƥ���� 2008-3-18   16:21 �����ļ�
///		</version>
///	</versions>
///========================================================================

#pragma  once

#ifndef __KV__ADO__CONNECTION__H__INCLUDED__
#define __KV__ADO__CONNECTION__H__INCLUDED__

#pragma warning(disable:4146)  /// ��ֹ4146����
/// ����ADO����̬���ӿ�
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace \
	rename("EOF", "adoEOF") \
	rename("LockTypeEnum", "adoLockTypeEnum") \
	rename("DataTypeEnum", "adoDataTypeEnum") \
	rename("FieldAttributeEnum", "adoFieldAttributeEnum") \
	rename("EditModeEnum", "adoEditModeEnum") \
	rename("RecordStatusEnum", "adoRecordStatusEnum") \
	rename("ParameterDirectionEnum", "adoParameterDirectionEnum") 

#pragma warning(default:4146) /// �ָ�4146����

///========================================================================
/// ADO������
///========================================================================
class KVADOConnection  
{
public:
	/// <summary> 
	///		���캯��
	/// </summary>
	KVADOConnection();

	/// <summary> 
	///		��������
	/// </summary>
	virtual ~KVADOConnection();

	/// <summary> 
	///		����Connection����
	/// </summary> 
	BOOL CreateInstance();

	/// <summary> 
	///		�ж����Ӷ���ָ���Ƿ��ǺϷ���
	/// </summary>
	BOOL IsValid();

	/// <summary> 
	///		��ȡ���Ӷ���
	/// </summary>
	_ConnectionPtr GetConnection();

	/// <summary> 
	///		�������ݿ�
	/// </summary> 
	/// <param name="bstrConnectionString">
	///		�����ִ�
	/// </param>
	/// <param name="nOptions">
	///		����ѡ�����ָ��Connection��������ݵĸ������Ȩ�����������¼�������
	///     - adModeUnknown��ȱʡ����ǰ�����Ȩδ����
	///     - adModeRead��ֻ�� 
	///     - adModeWrite��ֻд 
	///     - adModeReadWrite�����Զ�д
	///     - adModeShareDenyRead����ֹ����Connection�����Զ�Ȩ�޴�����
	///     - adModeShareDenyWrite����ֹ����Connection������дȨ�޴�����  
	///     - adModeShareExclusive����ֹ����Connection���������  
	///     - adModeShareDenyNone���������������������κ�Ȩ�޽�������
	/// </param>
	BOOL Open(_bstr_t bstrConnectionString, long nOptions = adModeUnknown);

	/// <summary> 
	///		�ر����Ӷ��󣬶Ͽ����Ӷ���������Դ����ϵ  
	/// </summary> 
	void Close();   

	/// <summary> 
	///		����Connection�����Execute����ִ��SQL����
	/// </summary> 
	/// <param name="bstrCommandText">
	///		�����ַ�����ͨ����SQL���
	///     ������CREATE TABLE TestForm(UserID INTEGER, UserName TEXT(20), PRIMARY KEY(UserID))
	///     ɾ����DROP TABLE TestForm
	///     ���������¼��INSERT INTO TestForm VALUES(1, "KINGVIEW")
	/// </param>
	/// <param name="nOptions">
	///		bstrCommandText�����ݵ����ͣ�����ȡ����ֵ֮һ��
	///     - adCmdText������bstrCommandText���ı�����
	///     - adCmdTable������bstrCommandText��һ������
	///     - adCmdStoredProc������bstrCommandText��һ���洢����
	///     - adCmdUnknown��δ֪
	/// </param>
	_RecordsetPtr Execute(_bstr_t bstrCommandText, long nOptions = adCmdText);

	/// <summary> 
	///		�����й�����Դ��������Ϣ��������Դ�а����ı����е��к�֧�ֵ���������
	/// </summary> 
	_RecordsetPtr OpenSchema(SchemaEnum Schema = adSchemaTables);

	/// <summary> 
	///		�����һ�εĴ�����Ϣ
	/// </summary> 
	void GetLastError(CString &strLastErrorMsg);

private:
	_ConnectionPtr	m_pConn;		/// ���Ӷ���ָ��
	CString			m_strLastError;	/// ��һ�δ������Ϣ
};

#endif /// __KV__ADO__CONNECTION__H__INCLUDED__