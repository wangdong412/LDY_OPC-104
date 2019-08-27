///========================================================================
/// <summary>
///	    ADO��¼����KVADORecordset������	
/// </summary>
///	<file>
///		��Ŀ��:	�Զ������Կ��KVAutoTestFrame
///		�ļ���: KVADORecordset.h
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

#ifndef __KV__ADO__RECORDSET__H__INCLUDED__
#define __KV__ADO__RECORDSET__H__INCLUDED__

#include "KVADOConnection.h"
#include <vector>

///========================================================================
/// ADO��¼����
///========================================================================
class KVADORecordset  
{
public:
	KVADORecordset();
	virtual ~KVADORecordset();
	void operator=(_RecordsetPtr pRecordset);

	/// <summary> 
	///		����Recordset����
	/// </summary> 
	BOOL CreateInstance();

	/// <summary> 
	///		�жϼ�¼������ָ���Ƿ��ǺϷ���
	/// </summary>
	BOOL IsValid();

	/// <summary> 
	///		ֱ����Recordset������в�ѯȡ�ü�¼��
	/// </summary> 
	/// <param name="bstrCommandString">
	///		���ݲ�ѯ�ַ�����һ����һ��SQL��䣬����SELECT * FROM TestForm
	/// </param>
	/// <param name="pAdoConn">
	///		�Ѿ������õ����Ӷ���
	/// </param>
	/// <param name="CursorType">
	///		������ͣ�������������ֵ֮һ
	///     - adOpenUnspecified�������ر�ָ��  
	///     - adOpenForwardOnly��ǰ����̬��ꡣ���ֹ��ֻ����ǰ�����¼����������MoveNext��ǰ����,���ַ�ʽ�����������ٶ�
	///     - adOpenKeyset���������ֹ��ļ�¼�������������û���������ɾ�������������ڸ���ԭ�м�¼�Ĳ��������ǿɼ���
	///     - adOpenDynamic����̬��ꡣ�������ݿ�Ĳ������������ڸ��û���¼���Ϸ�Ӧ����
	///     - adOpenStatic����̬��ꡣ��Ϊ��ļ�¼������һ����̬���ݣ��������û���������ɾ�������²�������ļ�¼����˵�ǲ��ɼ���
	/// </param>
	/// <param name="LockType">
	///		�������ͣ�������������ֵ֮һ
	///     - adLockUnspecified��δָ��
	///     - adLockReadOnly��ֻ����¼��
	///     - adLockPessimistic������������ʽ�������ڸ���ʱ�����������ж����������ȫ����������
	///     - adLockOptimistic���ֹ�������ʽ��ֻ���������Update����ʱ��������¼���ڴ�֮ǰ��Ȼ���������ݵĸ��¡����롢ɾ���ȶ���
	///     - adLockBatchOptimistic���ֹ۷������¡��༭ʱ��¼�������������ġ����뼰ɾ������������ģʽ�����
	/// </param>
	/// <param name="nOptions">
	///     - adCmdText������bstrCommandText���ı�����
	///     - adCmdTable������bstrCommandText��һ������
	///     - adCmdStoredProc������bstrCommandText��һ���洢����
	///     - adCmdUnknown��δ֪
	/// </param>
	BOOL Open(_bstr_t			bstrCommandString, 
			  KVADOConnection	*pAdoConn,
			  CursorTypeEnum	CursorType = adOpenStatic,
			  adoLockTypeEnum	LockType = adLockOptimistic,
			  long				nOptions = adCmdText);

	/// <summary> 
	///		�ر��Ѵ򿪵ļ�¼������
	/// </summary> 
	bool Close();

///========================================================================
/// �ӿ�˵�������²���Ϊ��¼���ı���������
///========================================================================
public:
	/// <summary> 
	///		��ȡ�ֶε�ֵ
	/// </summary> 
	_variant_t operator()(_variant_t vtFieldName);

	/// <summary> 
	///		�����ֶε�ֵ
	/// </summary> 
	void operator()(_variant_t vtFieldName, _variant_t vtValue);

	/// <summary> 
	///		���Ӽ�¼
	/// </summary> 
	bool AddNew();

	/// <summary> 
	///		���¼�¼
	/// </summary> 
	bool Update();

	/// <summary> 
	///		ɾ����ǰ��¼
	/// </summary>
	bool Delete();

	/// <summary> 
	///		��ȡ��¼������
	/// </summary> 
	long GetRecordCount();

	/// <summary>
	///   ��ñ������
	/// </summary>
	long GetFieldsCount();

	/// <summary> 
	///		�ƶ�����һ����¼
	/// </summary> 
	void MoveFirst();

	/// <summary> 
	///		�ƶ������һ����¼
	/// </summary> 
	void MoveLast();

	/// <summary> 
	///		�ƶ�����һ����¼
	/// </summary> 
	void MoveNext();

	/// <summary> 
	///		�ƶ�����һ����¼
	/// </summary> 
	void MovePrev();

	/// <summary> 
	///		ö�ٱ���
	/// </summary> 
	void EnumTableName(std::vector<CString> &vecTableName);

	/// <summary> 
	///		�����һ�εĴ�����Ϣ
	/// </summary> 
	void GetLastError(CString &strLastErrorMsg);

private:
	_RecordsetPtr	m_pRs;			/// ��¼������ָ��
	CString			m_strLastError;	/// ��һ�δ������Ϣ
};

#endif /// __KV__ADO__RECORDSET__H__INCLUDED__