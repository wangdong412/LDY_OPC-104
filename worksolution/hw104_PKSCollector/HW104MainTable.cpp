//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	104Trans
///		文件名: HW104MainTable.cpp
///		作  者: 王勃
///	</file>
///	<copyright>
/// 	本代码所有版权为所有，任何个人或者组织不得以任何方式复制、转载以挪作
/// 他用，否则将构成对的产权知识侵权。	
///	</copyright>
///	<versions>
///		<version>
///			V0.10 : 王勃 [11/4/2009] 创建文件
///		</version>
///	</versions>
//========================================================================
#include "StdAfx.h"
#include "HW104MainTable.h"
#include "KVADORecordset.h"

/// <summary>
/// 	全局日志
/// </summary>
extern  CHW104_Log  g_HW104Log;


CHW104MainTable::CHW104MainTable(void)
{
}

CHW104MainTable::~CHW104MainTable(void)
{
}

/// <summary>
/// 	加载HW104总表
/// </summary>
BOOL CHW104MainTable::LoadTable()
{
	std::wstring wstrIniPath;
	//获取配置文件路径
	CHW104Help::GetConfigIniPath( wstrIniPath );

	wchar_t wstrFilePath[ MAX_PATH_LEN ] = {0};
	//加载主表配置文件
	GetPrivateProfileString(
		TABLESECTIONNAME,
		FILEPATHKEY,
		L"WRONG",
		wstrFilePath,
		MAX_PATH_LEN,
		wstrIniPath.c_str()
		);

	if ( wcscmp( wstrFilePath, L"WRONG" ) == 0 )
	{
		HW104PKSLog( L"CHW104MainTable", L" Load MainTable File Path failed " );
		return FALSE;
	}
	//判断文件是否存在
	if( !CHW104Help::IsFileExist( wstrFilePath ))
	{
		CString szError;
		szError.Format( L"%s is not exist ",wstrFilePath );
		HW104PKSLog( L"CHW104MainTable", (LPCWSTR)szError );
		return FALSE;
	}

	//连接Excel
	KVADOConnection mainTableCon;

	if ( !mainTableCon.CreateInstance() )
	{
		CString szError;
		mainTableCon.GetLastError(szError);
		HW104PKSLog( L"CHW104MainTable", L"mainTableCon.CreateInstance() failed");
		HW104PKSLog( L"CHW104MainTable", (LPCWSTR)szError );
		return FALSE;
	}

	CString conString = L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=";
	conString += wstrFilePath;
	conString += L";Extended Properties=\"Excel 8.0;HDR=No;IMEX=1\"";

	if ( !mainTableCon.Open( (LPCWSTR)conString ) )
	{
		HW104PKSLog( L"CHW104MainTable", L"mainTableCon.Open failed" );
		return FALSE;
	}

	//创建记录集
	KVADORecordset adoRecord;

	if ( !adoRecord.CreateInstance() )
	{
		HW104PKSLog( L"CHW104MainTable", L"adoRecord.CreateInstance()" );
		return FALSE;
	}

	CString wstrSql = L"select * from[";
	wstrSql += MTABLESHEETNAME;
	wstrSql += L"$]";
	
	adoRecord = mainTableCon.Execute( (LPCWSTR)wstrSql );

	if ( !adoRecord.IsValid() )
	{
		CString szError;
		mainTableCon.GetLastError( szError );
		szError += L" [mainTableCon.Execute failed] ";
		HW104PKSLog( L"CHW104MainTable", (LPCWSTR)szError );
		return FALSE;
	}

	long lRowsCounts = adoRecord.GetRecordCount();
	
	long lFieldsCounts = adoRecord.GetFieldsCount();

	if ( 0 == lFieldsCounts )
	{
		return FALSE;
	}

	long lRegAddrIndex  = -1;
	long lDevIndex		= -1;
	long lIecAddrIndex  = -1;

	try
	{
		adoRecord.MoveFirst();

		//读取列索引
		for ( long i = 0; i < lFieldsCounts ; ++i )
		{
			COleVariant vtTemp = adoRecord( i );

			if ( VT_BSTR == vtTemp.vt )
			{
				if ( wcscmp( vtTemp.bstrVal, REGADDRCOLNAME ) == 0 )
				{
					lRegAddrIndex = i;
				}
				else if ( wcscmp( vtTemp.bstrVal, IECADDRCOLNAME ) == 0 )
				{
					lIecAddrIndex = i;
				}
				else if ( wcscmp( vtTemp.bstrVal, DEVCOLNAME ) == 0 )
				{
					lDevIndex = i;
				}
				else if ( wcscmp( vtTemp.bstrVal, L"" ) == 0 )
				{
					break;
				}
			}
			else
			{
				break;
			}

		}
	}
	catch ( _com_error &Error )
	{
		CString szError = (wchar_t*)Error.Description();
		HW104PKSLog( L"CHW104MainTable", (LPCWSTR)szError );
		return FALSE;
	}

	//校验列号
	if ( -1 == lRegAddrIndex )
	{
		HW104PKSLog( L"CHW104MainTable", L" 继存器地址 cloumn is not exist " );
		return FALSE;
	}

	if ( -1 == lDevIndex )
	{
		HW104PKSLog( L"CHW104MainTable", L" 数据来源 cloumn is not exist " );
		return FALSE;
	}

	if ( -1 == lIecAddrIndex )
	{
		HW104PKSLog( L"CHW104MainTable", L" IEC地址 cloumn is not exist " );
		return FALSE;
	}

	try
	{
		//读取表中的各个字段的信息
		for ( long i = 1; i < lRowsCounts; ++i  )
		{
			adoRecord.MoveNext();

			//读取寄存器地址
			std::wstring		  wstrRegAddr;
			COleVariant vtTemp = adoRecord( lRegAddrIndex );

			if ( vtTemp.vt == VT_BSTR )
			{
				wstrRegAddr.assign( vtTemp.bstrVal );
			}
			else
			{
				continue;
			}

			if ( true == wstrRegAddr.empty() )
			{
				CString szError;
				szError.Format( L" %d rows %d column is empty ", i, lRegAddrIndex );
				HW104PKSLog( L"CHW104MainTable", (LPCWSTR)szError );
				continue;
			}

			//读取Iec地址信息
			int			  nIecAddr = 0;

			COleVariant vtTemp2 = adoRecord( lIecAddrIndex );

			if ( vtTemp2.vt == VT_BSTR )
			{
				swscanf_s( vtTemp2.bstrVal, L"%d",&nIecAddr );
			}
			else
			{
				CString szError;
				szError.Format( L" %d rows %d column iecAddress value is incorrect ", i, lRegAddrIndex );
				HW104PKSLog( L"CHW104MainTable", (LPCWSTR)szError );
				continue;
			}

			if ( nIecAddr <= 0 || nIecAddr > 0xffffff )
			{	
				CString szError;
				szError.Format( L" %d rows %d column iecAddress value is incorrect ", i, lRegAddrIndex );
				HW104PKSLog( L"CHW104MainTable", (LPCWSTR)szError );
				continue;
			}

			//读取设备类型信息
			COleVariant vtTemp3 = adoRecord( lDevIndex );

			if ( vtTemp3.vt != VT_BSTR )
			{
				CString szError;
				szError.Format( L" %d rows %d column device type value is incorrect ", i, lRegAddrIndex );
				HW104PKSLog( L"CHW104MainTable", (LPCWSTR)szError );
				continue;
			}

			std::wstring wstrDeviceType( vtTemp3.bstrVal );

			CHW104Help::Upper( wstrRegAddr );				//把所有的寄存器地址也转换成为大写字母

			//查找检查有没有这种设备类型
			std::pair< RegisterIecMapIt, bool > insertPair2;
			DeviceRegisterIecMapIt it = m_DeviceRegisterIecMap.find( wstrDeviceType );
			if ( m_DeviceRegisterIecMap.end() == it )
			{
				//没有这种设备类型,则新建这种设备类型
				std::pair< DeviceRegisterIecMapIt, bool > insertPair = 
					m_DeviceRegisterIecMap.insert( std::make_pair( wstrDeviceType, RegisterIecMap() ) );

				//向内部map中插入
				insertPair2 =
					(insertPair.first)->second.insert( std::make_pair( wstrRegAddr, nIecAddr ) );
			}
			else
			{//拥有这种设备类型
				insertPair2 =
					it->second.insert( std::make_pair( wstrRegAddr, nIecAddr ) );
			}

			if ( false == insertPair2.second )
			{//寄存器类型重复,写入日志
				CString szError;
				szError.Format( L" %d rows Resigter is already exist ", i, lDevIndex );
				HW104PKSLog( L"CHW104MainTable", (LPCWSTR)szError );
				continue;
			}

		}
	}
	catch ( _com_error &Error )
	{
		CString szError = (wchar_t*)Error.Description();
		HW104PKSLog( L"CHW104MainTable", (LPCWSTR)szError );
		return FALSE;
	}
	
	return TRUE;
}

/// <summary>
/// 	根据寄存器地址,设备类型获取iec地址
/// </summary>
BOOL CHW104MainTable::GetIecAddress( 
				   const std::wstring& registerAddr, 
				   const CString& deviceType,
				   int& nIecAddress )
{
	DeviceRegisterIecMapIt it = m_DeviceRegisterIecMap.find( std::wstring( LPCTSTR(deviceType) ) );

	if ( m_DeviceRegisterIecMap.end() == it )
	{//设备类型不存在
		return FALSE;
	}

	RegisterIecMapIt it4Iec = it->second.find( registerAddr );

	if ( it4Iec == it->second.end() )
	{
		return FALSE;
	}

	nIecAddress = it4Iec->second;
	return TRUE;
}

/// <summary>
/// 	枚举所有的设备类型
/// </summary>
void CHW104MainTable::EnumDeviceType(
									 std::set< CString >& deviceTypeArray )
{
	deviceTypeArray.clear();

	for ( DeviceRegisterIecMapIt it = m_DeviceRegisterIecMap.begin();
		  it != m_DeviceRegisterIecMap.end();
		  ++it   )
	{
		deviceTypeArray.insert( CString( it->first.c_str() ) );
	}
}