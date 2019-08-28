///========================================================================
/// <summary>
///	    ADO记录集类KVADORecordset的声明	
/// </summary>
///	<file>
///		项目名:	自动化测试框架KVAutoTestFrame
///		文件名: KVADORecordset.h
///		作  者: 皮敏捷
///	</file>
///	<copyright>
/// 	本代码所有版权为所有，任何个人或者组织不得以任何方式复制、转载以挪作
/// 他用，否则将构成对的知识产权侵权。
///	
///		
///	</copyright>
///	<versions>
///		<version>
///			V1.0 : 皮敏捷 2008-3-18   16:21 创建文件
///		</version>
///	</versions>
///========================================================================

#pragma  once

#ifndef __KV__ADO__RECORDSET__H__INCLUDED__
#define __KV__ADO__RECORDSET__H__INCLUDED__

#include "KVADOConnection.h"
#include <vector>

///========================================================================
/// ADO记录集类
///========================================================================
class KVADORecordset  
{
public:
	KVADORecordset();
	virtual ~KVADORecordset();
	void operator=(_RecordsetPtr pRecordset);

	/// <summary> 
	///		创建Recordset对象
	/// </summary> 
	BOOL CreateInstance();

	/// <summary> 
	///		判断记录集对象指针是否是合法的
	/// </summary>
	BOOL IsValid();

	/// <summary> 
	///		直接用Recordset对象进行查询取得记录集
	/// </summary> 
	/// <param name="bstrCommandString">
	///		数据查询字符串，一般是一条SQL语句，比如SELECT * FROM TestForm
	/// </param>
	/// <param name="pAdoConn">
	///		已经建立好的连接对象
	/// </param>
	/// <param name="CursorType">
	///		光标类型，它可以是以下值之一
	///     - adOpenUnspecified：不作特别指定  
	///     - adOpenForwardOnly：前滚静态光标。这种光标只能向前浏览记录集，比如用MoveNext向前滚动,这种方式可以提高浏览速度
	///     - adOpenKeyset：采用这种光标的记录集看不到其它用户的新增、删除操作，但对于更新原有记录的操作对你是可见的
	///     - adOpenDynamic：动态光标。所有数据库的操作都会立即在各用户记录集上反应出来
	///     - adOpenStatic：静态光标。它为你的记录集产生一个静态备份，但其它用户的新增、删除、更新操作对你的记录集来说是不可见的
	/// </param>
	/// <param name="LockType">
	///		锁定类型，它可以是以下值之一
	///     - adLockUnspecified：未指定
	///     - adLockReadOnly：只读记录集
	///     - adLockPessimistic：悲观锁定方式。数据在更新时锁定其它所有动作，这是最安全的锁定机制
	///     - adLockOptimistic：乐观锁定方式。只有在你调用Update方法时才锁定记录。在此之前仍然可以做数据的更新、插入、删除等动作
	///     - adLockBatchOptimistic：乐观分批更新。编辑时记录不会锁定，更改、插入及删除是在批处理模式下完成
	/// </param>
	/// <param name="nOptions">
	///     - adCmdText：表明bstrCommandText是文本命令
	///     - adCmdTable：表明bstrCommandText是一个表名
	///     - adCmdStoredProc：表明bstrCommandText是一个存储过程
	///     - adCmdUnknown：未知
	/// </param>
	BOOL Open(_bstr_t			bstrCommandString, 
			  KVADOConnection	*pAdoConn,
			  CursorTypeEnum	CursorType = adOpenStatic,
			  adoLockTypeEnum	LockType = adLockOptimistic,
			  long				nOptions = adCmdText);

	/// <summary> 
	///		关闭已打开的记录集对象
	/// </summary> 
	bool Close();

///========================================================================
/// 接口说明：以下操作为记录集的遍历、更新
///========================================================================
public:
	/// <summary> 
	///		读取字段的值
	/// </summary> 
	_variant_t operator()(_variant_t vtFieldName);

	/// <summary> 
	///		设置字段的值
	/// </summary> 
	void operator()(_variant_t vtFieldName, _variant_t vtValue);

	/// <summary> 
	///		增加记录
	/// </summary> 
	bool AddNew();

	/// <summary> 
	///		更新记录
	/// </summary> 
	bool Update();

	/// <summary> 
	///		删除当前记录
	/// </summary>
	bool Delete();

	/// <summary> 
	///		获取记录的条数
	/// </summary> 
	long GetRecordCount();

	/// <summary>
	///   获得表的列数
	/// </summary>
	long GetFieldsCount();

	/// <summary> 
	///		移动到第一条记录
	/// </summary> 
	void MoveFirst();

	/// <summary> 
	///		移动到最后一条记录
	/// </summary> 
	void MoveLast();

	/// <summary> 
	///		移动到下一条记录
	/// </summary> 
	void MoveNext();

	/// <summary> 
	///		移动到上一条记录
	/// </summary> 
	void MovePrev();

	/// <summary> 
	///		枚举表名
	/// </summary> 
	void EnumTableName(std::vector<CString> &vecTableName);

	/// <summary> 
	///		获得上一次的错误信息
	/// </summary> 
	void GetLastError(CString &strLastErrorMsg);

private:
	_RecordsetPtr	m_pRs;			/// 记录集对象指针
	CString			m_strLastError;	/// 上一次错误的信息
};

#endif /// __KV__ADO__RECORDSET__H__INCLUDED__