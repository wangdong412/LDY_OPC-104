//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	104Trans
///		�ļ���: HW104MainTable.cpp
///		��  ��: ����
///	</file>
///	<copyright>
/// 	���������а�ȨΪ���У��κθ��˻�����֯�������κη�ʽ���ơ�ת����Ų��
/// ���ã����򽫹��ɶԵĲ�Ȩ֪ʶ��Ȩ��	
///	</copyright>
///	<versions>
///		<version>
///			V0.10 : ���� [11/4/2009] �����ļ�
///		</version>
///	</versions>
//========================================================================
#include "StdAfx.h"
#include "HW104MainTable.h"
#include "KVADORecordset.h"

/// <summary>
/// 	ȫ����־
/// </summary>
extern  CHW104_Log  g_HW104Log;


CHW104MainTable::CHW104MainTable(void)
{
}

CHW104MainTable::~CHW104MainTable(void)
{
}

/// <summary>
/// 	����HW104�ܱ�
/// </summary>
BOOL CHW104MainTable::LoadTable()
{
	std::wstring wstrIniPath;
	//��ȡ�����ļ�·��
	CHW104Help::GetConfigIniPath( wstrIniPath );

	wchar_t wstrFilePath[ MAX_PATH_LEN ] = {0};
	//�������������ļ�
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
	//�ж��ļ��Ƿ����
	if( !CHW104Help::IsFileExist( wstrFilePath ))
	{
		CString szError;
		szError.Format( L"%s is not exist ",wstrFilePath );
		HW104PKSLog( L"CHW104MainTable", (LPCWSTR)szError );
		return FALSE;
	}

	//����Excel
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

	//������¼��
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

		//��ȡ������
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

	//У���к�
	if ( -1 == lRegAddrIndex )
	{
		HW104PKSLog( L"CHW104MainTable", L" �̴�����ַ cloumn is not exist " );
		return FALSE;
	}

	if ( -1 == lDevIndex )
	{
		HW104PKSLog( L"CHW104MainTable", L" ������Դ cloumn is not exist " );
		return FALSE;
	}

	if ( -1 == lIecAddrIndex )
	{
		HW104PKSLog( L"CHW104MainTable", L" IEC��ַ cloumn is not exist " );
		return FALSE;
	}

	try
	{
		//��ȡ���еĸ����ֶε���Ϣ
		for ( long i = 1; i < lRowsCounts; ++i  )
		{
			adoRecord.MoveNext();

			//��ȡ�Ĵ�����ַ
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

			//��ȡIec��ַ��Ϣ
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

			//��ȡ�豸������Ϣ
			COleVariant vtTemp3 = adoRecord( lDevIndex );

			if ( vtTemp3.vt != VT_BSTR )
			{
				CString szError;
				szError.Format( L" %d rows %d column device type value is incorrect ", i, lRegAddrIndex );
				HW104PKSLog( L"CHW104MainTable", (LPCWSTR)szError );
				continue;
			}

			std::wstring wstrDeviceType( vtTemp3.bstrVal );

			CHW104Help::Upper( wstrRegAddr );				//�����еļĴ�����ַҲת����Ϊ��д��ĸ

			//���Ҽ����û�������豸����
			std::pair< RegisterIecMapIt, bool > insertPair2;
			DeviceRegisterIecMapIt it = m_DeviceRegisterIecMap.find( wstrDeviceType );
			if ( m_DeviceRegisterIecMap.end() == it )
			{
				//û�������豸����,���½������豸����
				std::pair< DeviceRegisterIecMapIt, bool > insertPair = 
					m_DeviceRegisterIecMap.insert( std::make_pair( wstrDeviceType, RegisterIecMap() ) );

				//���ڲ�map�в���
				insertPair2 =
					(insertPair.first)->second.insert( std::make_pair( wstrRegAddr, nIecAddr ) );
			}
			else
			{//ӵ�������豸����
				insertPair2 =
					it->second.insert( std::make_pair( wstrRegAddr, nIecAddr ) );
			}

			if ( false == insertPair2.second )
			{//�Ĵ��������ظ�,д����־
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
/// 	���ݼĴ�����ַ,�豸���ͻ�ȡiec��ַ
/// </summary>
BOOL CHW104MainTable::GetIecAddress( 
				   const std::wstring& registerAddr, 
				   const CString& deviceType,
				   int& nIecAddress )
{
	DeviceRegisterIecMapIt it = m_DeviceRegisterIecMap.find( std::wstring( LPCTSTR(deviceType) ) );

	if ( m_DeviceRegisterIecMap.end() == it )
	{//�豸���Ͳ�����
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
/// 	ö�����е��豸����
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