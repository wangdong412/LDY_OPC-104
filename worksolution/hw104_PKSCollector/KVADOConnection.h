///========================================================================
/// <summary>
///	    ADO连接类KVADOConnection声明	
/// </summary>
///	<file>
///		项目名:	自动化测试框架KVAutoTestFrame
///		文件名: KVADOConnection.h
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

#ifndef __KV__ADO__CONNECTION__H__INCLUDED__
#define __KV__ADO__CONNECTION__H__INCLUDED__

#pragma warning(disable:4146)  /// 禁止4146警告
/// 引入ADO对象动态链接库
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace \
	rename("EOF", "adoEOF") \
	rename("LockTypeEnum", "adoLockTypeEnum") \
	rename("DataTypeEnum", "adoDataTypeEnum") \
	rename("FieldAttributeEnum", "adoFieldAttributeEnum") \
	rename("EditModeEnum", "adoEditModeEnum") \
	rename("RecordStatusEnum", "adoRecordStatusEnum") \
	rename("ParameterDirectionEnum", "adoParameterDirectionEnum") 

#pragma warning(default:4146) /// 恢复4146警告

///========================================================================
/// ADO连接类
///========================================================================
class KVADOConnection  
{
public:
	/// <summary> 
	///		构造函数
	/// </summary>
	KVADOConnection();

	/// <summary> 
	///		析构函数
	/// </summary>
	virtual ~KVADOConnection();

	/// <summary> 
	///		创建Connection对象
	/// </summary> 
	BOOL CreateInstance();

	/// <summary> 
	///		判断连接对象指针是否是合法的
	/// </summary>
	BOOL IsValid();

	/// <summary> 
	///		获取连接对象
	/// </summary>
	_ConnectionPtr GetConnection();

	/// <summary> 
	///		连接数据库
	/// </summary> 
	/// <param name="bstrConnectionString">
	///		连接字串
	/// </param>
	/// <param name="nOptions">
	///		连接选项，用于指定Connection对象对数据的更新许可权，可以是如下几个常量
	///     - adModeUnknown：缺省，当前的许可权未设置
	///     - adModeRead：只读 
	///     - adModeWrite：只写 
	///     - adModeReadWrite：可以读写
	///     - adModeShareDenyRead：阻止其它Connection对象以读权限打开连接
	///     - adModeShareDenyWrite：阻止其它Connection对象以写权限打开连接  
	///     - adModeShareExclusive：阻止其它Connection对象打开连接  
	///     - adModeShareDenyNone：允许其它程序或对象以任何权限建立连接
	/// </param>
	BOOL Open(_bstr_t bstrConnectionString, long nOptions = adModeUnknown);

	/// <summary> 
	///		关闭连接对象，断开连接对象与数据源的联系  
	/// </summary> 
	void Close();   

	/// <summary> 
	///		利用Connection对象的Execute方法执行SQL命令
	/// </summary> 
	/// <param name="bstrCommandText">
	///		命令字符串，通常是SQL语句
	///     创建表：CREATE TABLE TestForm(UserID INTEGER, UserName TEXT(20), PRIMARY KEY(UserID))
	///     删除表：DROP TABLE TestForm
	///     往表里添记录：INSERT INTO TestForm VALUES(1, "KINGVIEW")
	/// </param>
	/// <param name="nOptions">
	///		bstrCommandText中内容的类型，可以取如下值之一：
	///     - adCmdText：表明bstrCommandText是文本命令
	///     - adCmdTable：表明bstrCommandText是一个表名
	///     - adCmdStoredProc：表明bstrCommandText是一个存储过程
	///     - adCmdUnknown：未知
	/// </param>
	_RecordsetPtr Execute(_bstr_t bstrCommandText, long nOptions = adCmdText);

	/// <summary> 
	///		返回有关数据源的自述信息，如数据源中包含的表、表中的列和支持的数据类型
	/// </summary> 
	_RecordsetPtr OpenSchema(SchemaEnum Schema = adSchemaTables);

	/// <summary> 
	///		获得上一次的错误信息
	/// </summary> 
	void GetLastError(CString &strLastErrorMsg);

private:
	_ConnectionPtr	m_pConn;		/// 连接对象指针
	CString			m_strLastError;	/// 上一次错误的信息
};

#endif /// __KV__ADO__CONNECTION__H__INCLUDED__