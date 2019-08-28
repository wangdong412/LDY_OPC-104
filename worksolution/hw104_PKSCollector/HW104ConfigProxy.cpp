//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	KingModel
///		文件名: HW104ConfigInfo.cpp
///		作  者: 王勃
///	</file>
///	<copyright>
/// 	本代码所有版权为所有，任何个人或者组织不得以任何方式复制、转载以挪作
/// 他用，否则将构成对的产权知识侵权。
///	
///		
///	</copyright>
///	<versions>
///		<version>
///			V0.10 : 王勃 2009-10-15 16:45 创建文件
///		</version>
///	</versions>
//========================================================================
#include "StdAfx.h"
#include "HW104ConfigProxy.h"
#include "HW104PKSCollector.h"
#include "HW104ProtcolHelp.h"
#include "HW104MainTable.h"
#include "KVADORecordset.h"

//全局采集器
extern CHW104PKSCollector g_PKSCollector;
//全局日志模块
extern CHW104_Log		  g_HW104Log;

#define STATIONFILEPATHKEY		L"Path2"				//路径key
#define POINTCOLUMN				L"POINT"				//点名称
#define RegisterAddrCOLUMN		L"REGISTERADDR"			//寄存器地址
#define ASDUCOLUMN				L"ASDU"					//ASDU列名称

CHW104ConfigProxy::CHW104ConfigProxy(void)
{

}

CHW104ConfigProxy::~CHW104ConfigProxy(void)
{
	for( std::list< CHW104Subscription* >::iterator it = m_SubscriptionList.begin();
		 it != m_SubscriptionList.end();
		 ++it
		)
	{
		//停止变化上送
		(*it)->End();
		//删除指针
		delete *it;
	}

}

/// <summary>
/// 	加载配置信息
/// </summary>
/// <returns>
///		加载成功返回KTN_OK,失败返回错误码
/// </returns>
/// <remarks>
///     1. 加载配置文件
///     2. 把所有信息发送给数据库采集器
/// </remarks>
KTN_RET CHW104ConfigProxy::LoadKTNConfig()
{
	g_HW104Log.Open( PKSLOGNAME );

	//加载主表
	CHW104MainTable mTable;

	if ( mTable.LoadTable() == FALSE )
	{
		return FALSE;
	}

	//加载站点表
	std::wstring wstrIniPath;
	//获取配置文件路径
	CHW104Help::GetConfigIniPath( wstrIniPath );

	wchar_t wstrFilePath[ MAX_PATH_LEN ] = {0};
	//加载主表配置文件
	GetPrivateProfileString(
		TABLESECTIONNAME,
		STATIONFILEPATHKEY,
		L"WRONG",
		wstrFilePath,
		MAX_PATH_LEN,
		wstrIniPath.c_str()
		);

	if ( wcscmp( wstrFilePath, L"WRONG" ) == 0 )
	{
		HW104PKSLog( L"CHW104ConfigProxy", L" Load Station Table File Path failed " );
		return FALSE;
	}
	
	//判断文件是否存在
	if( !CHW104Help::IsFileExist( wstrFilePath ))
	{
		CString szError;
		szError.Format( L"%s is not exist ",wstrFilePath );
		HW104PKSLog( L"CHW104ConfigProxy", (LPCWSTR)szError );
		return FALSE;
	}

	//连接Excel
	KVADOConnection stationTableCon;

	if ( !stationTableCon.CreateInstance() )
	{
		HW104PKSLog( L"CHW104ConfigProxy", L"stationTableCon.CreateInstance() failed" );
		return FALSE;
	}

	CString conString = L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=";
	conString += wstrFilePath;
	conString += L";Extended Properties=\"Excel 8.0;HDR=No;IMEX=1\"";

	if ( !stationTableCon.Open( (LPCWSTR)conString ) )
	{
		HW104PKSLog( L"CHW104ConfigProxy", L"stationTableCon.Open failed" );
		return FALSE;
	}

	//创建记录集
	KVADORecordset adoRecord;

	if ( !adoRecord.CreateInstance() )
	{
		HW104PKSLog( L"CHW104ConfigProxy", L"adoRecord.CreateInstance()" );
		return FALSE;
	}

	//枚举所有的表名称
	std::set< CString > sheetNameSet;
	mTable.EnumDeviceType( sheetNameSet );

	std::vector< KTNPKSPoint > pointArray;

	for ( std::set< CString >::iterator it4SheetName = sheetNameSet.begin();
		  it4SheetName != sheetNameSet.end();
		  ++it4SheetName  )
	{
		CString wstrSql = L"select * from[";
		wstrSql += *it4SheetName;
		wstrSql += L"$]";

		adoRecord = stationTableCon.Execute( (LPCWSTR)wstrSql );

		if ( !adoRecord.IsValid() )
		{
			CString szError;
			stationTableCon.GetLastError( szError );
			szError += L" [stationTableCon.Execute failed]  sheet name: ";
			szError += *it4SheetName;
			HW104PKSLog( L"CHW104ConfigProxy", (LPCWSTR)szError );
			continue;
		}
		else
		{
			CString szError;
			szError += L" stationTableCon.Execute OK sheet name:  ";
			szError += *it4SheetName;
			HW104PKSLog( L"CHW104ConfigProxy", (LPCWSTR)szError );			
		}


		//获取行数
		long lRowsCounts = adoRecord.GetRecordCount();
		long lFieldCounts = adoRecord.GetFieldsCount();
		if ( 0 == lFieldCounts )
		{
			TRACE( "adoRecord.GetFieldsCount() failed" );
			return FALSE;
		}

		long lPointsIndex = -1;
		long lRegAddrIndex = -1;
		long lAsduIndex = -1;

		try
		{
			adoRecord.MoveFirst();

			//读取列索引
			for ( long i = 0;i<lFieldCounts; ++i )
			{
				COleVariant vtTemp = adoRecord( i );

				if ( vtTemp.vt == VT_BSTR )
				{
					CString wstrColumnNameTemp( vtTemp.bstrVal );

					if ( wstrColumnNameTemp.CompareNoCase( RegisterAddrCOLUMN ) == 0 )
					{
						// 当出现重复的列时,以第一列为准 [11/27/2009 pks]
						if ( -1 == lRegAddrIndex )
						{
							lRegAddrIndex = i;						
						}
						else
						{
							HW104PKSLog( L"CHW104ConfigProxy",L"REGISTERADDR column repeat, ingore" );
						}

					}
					else if( wstrColumnNameTemp.CompareNoCase( POINTCOLUMN ) == 0 )
					{
						// 当出现重复的列时,以第一列为准 [11/27/2009 pks]
						if ( -1 == lPointsIndex )
						{
							lPointsIndex = i;						
						}
						else
						{
							HW104PKSLog( L"CHW104ConfigProxy",L"Point column repeat, ingore" );
						}
					}
					else if ( wstrColumnNameTemp.CompareNoCase( ASDUCOLUMN ) == 0 )
					{
						// 当出现重复的列时,以第一列为准 [11/27/2009 pks]
						if ( -1 == lAsduIndex )
						{
							lAsduIndex = i;						
						}
						else
						{
							HW104PKSLog( L"CHW104ConfigProxy",L"Asdu column repeat, ingore" );
						}
					}
					else if ( wcscmp( vtTemp.bstrVal, L"" ) == 0  )
					{
						break;
					}
				}
				
			}
		}
		catch ( _com_error &Error )
		{
			CString szError = (wchar_t*)Error.Description();
			HW104PKSLog( L"CHW104ConfigProxy", (LPCWSTR)szError );
			continue;
		}

		//校验列号
		if ( -1 == lRegAddrIndex )
		{
			HW104PKSLog( L"CHW104ConfigProxy", L" RegisterAddr cloumn is not exist " );
			continue;
		}

		if ( -1 == lPointsIndex )
		{
			HW104PKSLog( L"CHW104ConfigProxy", L" Point cloumn is not exist " );
			continue;
		}

		if ( -1 == lAsduIndex )
		{
			HW104PKSLog( L"CHW104ConfigProxy", L" ASDU cloumn is not exist " );
			continue;
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

				if ( vtTemp.vt != VT_BSTR )
				{
					continue;
				}

				wstrRegAddr.assign( vtTemp.bstrVal );

				if ( true == wstrRegAddr.empty() )
				{
					CString szError;
					szError.Format( L" %d rows %d colum is empty ", i, lRegAddrIndex );
					HW104PKSLog( L"CHW104ConfigProxy", (LPCWSTR)szError );
					continue;
				}

				//读取Point 信息
				KTNPKSPoint ktnPoint;

				COleVariant vtTemp3 = adoRecord( lPointsIndex );

				if ( vtTemp3.vt != VT_BSTR )
				{
					continue;
				}

				CHW104Help::ConvertToMultiBytes( vtTemp3.bstrVal, ktnPoint.point );

				if ( true == ktnPoint.point.empty() )
				{
					CString szError;
					szError.Format( L" %d rows %d column is empty ", i, lPointsIndex  );
					HW104PKSLog( L"CHW104ConfigProxy", (LPCWSTR)szError );
					continue;
				}

				//读取IEC地址
				KTNIECAddress iecAddrTmp;
				int nIecAddress = 0;

				if ( FALSE == mTable.GetIecAddress( 
					wstrRegAddr , 
					*it4SheetName, 
					nIecAddress ) )
				{
					CString szError;
					szError.Format( L" %d rows %d column Register %s,cann't find in MainTable ", i, lRegAddrIndex, wstrRegAddr.c_str() );
					HW104PKSLog( L"CHW104ConfigProxy", (LPCWSTR)szError );
					continue;
				}

				iecAddrTmp.pos1 = *(char*)(&nIecAddress);
				iecAddrTmp.pos2 = *( (char*)(&nIecAddress) + 1 );
				iecAddrTmp.pos3 = *( (char*)(&nIecAddress) + 2 );
 
				//读取ASDU
				int	nASDU = 0;

				COleVariant vtTemp2 = adoRecord( lAsduIndex );

				if ( vtTemp2.vt != VT_BSTR )
				{
					continue;
				}

				swscanf_s( vtTemp2.bstrVal, L"%d",&nASDU );

				if ( nASDU == 1 || 13 == nASDU )
				{
					ktnPoint.parameter= "PV" ;
					iecAddrTmp.asduType = (KTNASDUType)nASDU;

					m_KTNPksIecMap.insert( std::make_pair( ktnPoint, iecAddrTmp ) );
					pointArray.push_back( ktnPoint );
				}
				else if ( 45 == nASDU )
				{
					//所有O点都是可读写的点
					//所以所有的写点都有读属性
					ktnPoint.parameter = "OP";
					iecAddrTmp.asduType = (KTNASDUType)nASDU;
					
					m_KTNIecPksMap.insert( std::make_pair( iecAddrTmp,ktnPoint ) );

					//读属性的点,
					ktnPoint.parameter = "PV";
					iecAddrTmp.asduType = M_SP_NA_1;

					m_KTNPksIecMap.insert( std::make_pair( ktnPoint, iecAddrTmp ) );
					pointArray.push_back( ktnPoint );
				}
				else if ( 50 == nASDU )
				{
					ktnPoint.parameter = "SP";
					iecAddrTmp.asduType = (KTNASDUType)nASDU;

					m_KTNIecPksMap.insert( std::make_pair( iecAddrTmp,ktnPoint ) );
				
					//读属性的点
					ktnPoint.parameter = "PV";
					iecAddrTmp.asduType = M_ME_NC_1;

					m_KTNPksIecMap.insert( std::make_pair( ktnPoint , iecAddrTmp ) );
					pointArray.push_back( ktnPoint );
				}
				else
				{
					CString szError;
					szError.Format( L" %d rows %d colum ASDU %d is wrong ", i, lPointsIndex ,nASDU );
					HW104PKSLog( L"CHW104ConfigProxy", (LPCWSTR)szError );
					continue;
				}

			}
		}
		catch ( _com_error &Error )
		{
			CString szError = (wchar_t*)Error.Description();
			HW104PKSLog( L"CHW104ConfigProxy", (LPCWSTR)szError );
			continue;
		}

	}//-for

	//下载到采集器
	g_PKSCollector.AddPoint( pointArray );

	return KTN_OK;
}

/// <summary>
/// 	读取所有的数据库点
/// </summary>
/// <param name="iecArray">
///		out 信息对象地址
/// </param>
/// <param name="valueArray">
///		out  数据数组
/// </param>
/// <returns>
///		加载成功返回KTN_OK,失败返回错误码
/// </returns>
KTN_RET CHW104ConfigProxy::ReadAllPoint(
							 std::vector< KTNIECAddress >& iecArray,
							 std::vector< KTNValue >&	  valueArray
							 )
{


	iecArray.clear();
	valueArray.clear();

	std::vector< KTNPKSPoint > pointArrayTemp;

	for ( 	KTNPksIecMapCIt c_it  = m_KTNPksIecMap.begin();
		    c_it != m_KTNPksIecMap.end();
			++c_it )
	{
		//读表不可能存在写的ASDU
		ASSERT( c_it->second.asduType < C_SC_NA_1 );

		pointArrayTemp.push_back( c_it->first );
		iecArray.push_back( c_it->second );
	}

	KTN_RET ret = g_PKSCollector.ReadPoint( pointArrayTemp, valueArray );

	if ( KTNERR(ret) )
	{
		wchar_t wstrError[ MAX_ERROR_LEN ] = {0};
		swprintf_s( wstrError, MAX_ERROR_LEN, L"g_PKSCollector.ReadPoint error %d",ret );
		HW104PKSLog( L"CHW104ConfigProxy", wstrError );
		iecArray.clear();
		valueArray.clear();
		return ret;
	}

	HW104PKSLog( L"HW104ConfigProxy::",L"ReadAllPoint OK" );

	return KTN_OK;
}

/// <summary>
/// 	变化回调
/// </summary>
/// <param name="pksPointArray">
///		数据库点信息数组
/// </param>
/// <param name="valueArray">
///		数据库数值数组
/// </param>
/// <returns>
///		加载成功返回KTN_OK,失败返回错误码
/// </returns>
KTN_RET CHW104ConfigProxy::OnChange( 
						 const std::vector< KTNPKSPoint >&	pksPointArray,
						 const std::vector< KTNValue >&		valueArray
						 )
{
	ASSERT( pksPointArray.size() == valueArray.size() );

	SYSTEMTIME t;
	GetSystemTime( &t );

	std::vector< KTNHW104_InfoUnit > infoUTArray;
	infoUTArray.reserve( valueArray.size() );

	for( unsigned int i = 0; i<pksPointArray.size(); ++i )
	{
		KTNPksIecMapCIt c_it2 = m_KTNPksIecMap.find( pksPointArray[i] );
		ASSERT( c_it2 != m_KTNPksIecMap.end() );

		KTNHW104_InfoUnit infoUt;

		KTN_RET ret = CHW104ProtcolHelp::GetInfoUnit( 
							c_it2->second,
							valueArray[i],
							infoUt,
							true );

		if ( KTNERR( ret ) )
		{
			continue;
		}

		infoUTArray.push_back( infoUt );
	}

	AutoLock auLock( &m_SubListLock );

	for ( std::list< CHW104Subscription* >::iterator it3 = m_SubscriptionList.begin();
		  m_SubscriptionList.end() != it3;
		  ++it3 )
	{
		(*it3)->OnChange( infoUTArray );
	}

	return KTN_OK;
}

/// <summary>
/// 	写入IEC地址对应的数据
/// </summary>
/// <param name="iecAddress">
///		KTNIECAddress 
/// </param>
/// <param name="ktnValue">
///		KTNValue
/// </param>
/// <returns>
///		加载成功返回KTN_OK,失败返回错误码
/// </returns>
KTN_RET CHW104ConfigProxy::WriteIecData(
							 const KTNIECAddress&   iecAddress,
							 const KTNValue&		   ktnValue
							 )
{
	wchar_t wstrError[ MAX_ERROR_LEN ] = {0};

	KTNIecPksMapCIt c_it = m_KTNIecPksMap.find( iecAddress );

	if ( m_KTNIecPksMap.end() == c_it )
	{// can not find pkspint
		swprintf_s( wstrError, MAX_ERROR_LEN, 
			L"g_PKSCollector.WriteIecData %d.%d.%d error KTN_COLLECTOR_NO_IECADDRESS",
			iecAddress.pos1,iecAddress.pos2,iecAddress.pos3 );
		HW104PKSLog( L"CHW104ConfigProxy", wstrError );
		return KTN_COLLECTOR_NO_IECADDRESS;
	}

	//write error
	KTN_RET ret = g_PKSCollector.WriteSinglePoint( c_it->second, ktnValue );

	if ( KTNERR(ret) )
	{		
		swprintf_s( wstrError, MAX_ERROR_LEN, L"g_PKSCollector.WriteIecData %d.%d.%d error %d",
			iecAddress.pos1,iecAddress.pos2,iecAddress.pos3,ret );
		HW104PKSLog( L"CHW104ConfigProxy", wstrError );
		return ret;
	}
	
	//write ok
	swprintf_s( wstrError, MAX_ERROR_LEN, L"g_PKSCollector.WriteIecData %d.%d.%d OK",
		iecAddress.pos1,iecAddress.pos2,iecAddress.pos3 );
	HW104PKSLog( L"CHW104ConfigProxy", wstrError );

	return KTN_OK;
}

/// <summary>
/// 	注册回调接口
/// </summary>
/// <param name="pIHW104CallBack">
///		in 回调接口
/// </param>
/// <returns>
///		加载成功返回KTN_OK,失败返回错误码
/// </returns>
KTN_RET CHW104ConfigProxy::RegsiterCallBack(
			  IHW104CollectorCallBack* pIHW104CallBack
								 )
{
	if ( NULL == pIHW104CallBack )
	{
		return KTN_OK;
	}

	AutoLock auLock( &m_SubListLock );
	
	CHW104Subscription* pSubscription = NULL;

	if ( m_SubscriptionList.empty() )
	{//链表为空
		pSubscription = new CHW104Subscription;
		pSubscription->AddCallBackInterface( pIHW104CallBack );
		m_SubscriptionList.push_back( pSubscription );
		pSubscription->Start();
	}
	else
	{
		pSubscription = m_SubscriptionList.back();
		
		bool bRet = pSubscription->AddCallBackInterface( pIHW104CallBack );

		if ( false == bRet )
		{//如果加入失败,则说明需要申请新指针
			pSubscription = new CHW104Subscription;
			pSubscription->AddCallBackInterface( pIHW104CallBack );
			m_SubscriptionList.push_back( pSubscription );
			pSubscription->Start();
		}
	}


	return KTN_OK;
}

/// <summary>
/// 	获得数量
/// </summary>
int CHW104ConfigProxy::GetCount()
{
	return m_KTNPksIecMap.size();
}

/// <summary>
/// 	冗余状态变化回调
/// </summary>
/// <param name="redunStatus">
///		冗余状态
/// </param>
KTN_RET CHW104ConfigProxy::OnRedunStatusChange(
							KTNRedunStatus& redunStatus
							)
{
	AutoLock auLock( &m_SubListLock );

	for ( std::list< CHW104Subscription* >::iterator it3 = m_SubscriptionList.begin();
		m_SubscriptionList.end() != it3;
		++it3 )
	{
		(*it3)->OnRedunStatusChange( redunStatus );
	}

	return KTN_OK;
}

/// <summary>
///   停止变化上送
/// </summary>
void CHW104ConfigProxy::End()
{
	AutoLock auLock( &m_SubListLock );

	for ( std::list< CHW104Subscription* >::iterator it3 = m_SubscriptionList.begin();
		m_SubscriptionList.end() != it3;
		++it3 )
	{
		(*it3)->End();
		delete *it3;
	}

}