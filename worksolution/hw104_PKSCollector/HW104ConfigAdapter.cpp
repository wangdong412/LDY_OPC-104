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
#include "HW104ConfigAdapter.h"
#include "OPCCollector.h"
#include "HW104ProtcolHelp.h"
#include "KVADORecordset.h"

//全局采集器
//extern CHW104PKSCollector g_PKSCollector;
//全局日志模块
extern CHW104_Log		  g_HW104Log;
//全局默认阈值
extern KTNThresHold		  g_HW104DefaultThresHold;

#define STATIONFILEPATHKEY		L"IOListPath"           //路径key   //L"Path2"		
#define POINTCOLUMN				L"POINT"				//点名称
#define RegisterAddrCOLUMN		L"VirRegAddr"			//寄存器地址
#define ASDUCOLUMN				L"ASDU"					//ASDU列名称
#define THRESHOLDCOLUMN			L"AbsThreshold"			//阈值列名

#define WIPLISTSECTION    L"IPLIST"
#define WIPNUMKEY		 L"IPNum"
#define WIPADDRESSKEY	 L"ip"

#define WOPCSECTION			L"OPCServer"
#define WOPCSERVERNUM		L"ServerNum"
#define WOPCADDRESSKEY		L"ServerAddress"
#define WPROGIDKEY			L"ProgID"
#define WUPDATERATE			L"UpdateRate"

#define DEFAULT_UPDATE_RATE		1000		//ms

#define MAX_POINT_NUM	50000

static unsigned __stdcall ThreadHook(void *p){
	COPCCollector *opc=(COPCCollector*)p;
	CHW104ConfigAdapter *adapter=opc->m_Adpater;
	adapter->CollectThread(opc);
	return 0;
}

static bool OverThreshold(const KTNValue& old,const KTNValue &neww,const KTNThresHold& threshold){
	if(old.GetDataType()!=neww.GetDataType())
		return true;

	if( threshold.thresHoldType == KTN_ABSOLUTE ){
		switch(old.GetDataType()){
			case KTNVDT_I1:
				{
					if(abs( old.asInt8()-neww.asInt8() )<threshold.thresHold)
						return false;
					if( old.asInt8()-neww.asInt8() == 0.0 && old.GetQuality() == neww.GetQuality() )	return false;
				}
				break;
			case KTNVDT_UI1:
				{
					if(abs( old.asUInt8()-neww.asUInt8() )<threshold.thresHold)
						return false;
					if( old.asUInt8()-neww.asUInt8() == 0.0 && old.GetQuality() == neww.GetQuality() )	return false;
				}
				break;
			case KTNVDT_I2:
				{
					if(abs( old.asInt16()-neww.asInt16() )<threshold.thresHold)
						return false;
					if( old.asInt16()-neww.asInt16() == 0.0 && old.GetQuality() == neww.GetQuality() )	return false;
				}
				break;
			case KTNVDT_UI2:
				{ 
					if(abs( old.asUInt16()-neww.asUInt16() )<threshold.thresHold)
						return false;
					if( old.asUInt16()-neww.asUInt16() == 0.0 && old.GetQuality() == neww.GetQuality() )	return false;
				}
				break;
			case KTNVDT_I4:
				{
					if(abs( old.asInt32()-neww.asInt32() )<threshold.thresHold)
						return false;
					if( old.asInt32()-neww.asInt32() == 0.0 && old.GetQuality() == neww.GetQuality() )	return false;
				}
				break;
			case KTNVDT_UI4:
				{
					if(abs( (float)(old.asUInt32()-neww.asUInt32()) )<threshold.thresHold)
						return false;
					if( (float)(old.asUInt32()-neww.asUInt32()) == 0.0 && old.GetQuality() == neww.GetQuality() )	return false;
				}
				break;
			case KTNVDT_R4:
				{
					if(abs( old.asFloat()-neww.asFloat() )<threshold.thresHold)
						return false;
					if( old.asFloat()-neww.asFloat() == 0.0 && old.GetQuality() == neww.GetQuality() )	return false;
				}
				break;
			default:
				break;
		}
	}

	return true;
}

CHW104ConfigAdapter::CHW104ConfigAdapter(void):m_ThreadStop(1),m_WriteEvent(INVALID_HANDLE_VALUE),m_current_write_index(-1)
{
	m_hMutexCollectThread = CreateMutex(NULL, FALSE, L"OPCMutexCollectThread");
}

CHW104ConfigAdapter::~CHW104ConfigAdapter(void)
{
	EndCollect();
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
KTN_RET CHW104ConfigAdapter::LoadKTNConfig()
{
	HW104PKSLog( L"CHW104ConfigAdapter", L" Start to LoadKTNConfig " );
	//加载主表
	CHW104MainTable mTable;
	if ( mTable.LoadTable() == FALSE )
	{
		return FALSE;
	}

	//加载站点表	
	//获取配置文件路径
	std::wstring wstrIniPath;
	CHW104Help::GetConfigIniPath( wstrIniPath );

	//获取各点表路径, 每个IP都有一个点表, 针对多份点表的需求, 2010-6-9 栾述强修改
	//获得IP个数
	int nIpNum = GetPrivateProfileIntW( 
		WIPLISTSECTION,
		WIPNUMKEY,
		6,
		wstrIniPath.c_str()
		);
	if ( nIpNum <= 0 )
	{
		HW104PKSLog( L"CHW104ConfigAdapter", L"IPNum<=0 is wrong in the ini file!" );
		return FALSE;
	}	
	//读取各点表
	std::vector< KTNPKSPoint > pointArray;
	for ( int i = 1; i<=nIpNum; ++i )
	{
		wchar_t szSection[ 10 ] = {0};
		swprintf_s( szSection, 10, L"IP%d", i );

		//获取IP
		wchar_t szIP[50] = {0};
		GetPrivateProfileStringW( 
			szSection,
			WIPADDRESSKEY,
			L"127.0.0.1",
			szIP,
			50,
			wstrIniPath.c_str()
			);	
		std::string strIP;
		CHW104Help::ConvertToMultiBytes(szIP, strIP);

		//去掉重复IP
		if ( m_mapIPToPointTable.end() != m_mapIPToPointTable.find(strIP) )
		{
			continue;
		}

		//得到点表路径
		wchar_t wstrFilePath[ MAX_PATH_LEN ] = {0};	
		GetPrivateProfileString(
			szSection,
			STATIONFILEPATHKEY,
			L"WRONG",
			wstrFilePath,
			MAX_PATH_LEN,
			wstrIniPath.c_str()
			);
		if ( wcscmp( wstrFilePath, L"WRONG" ) == 0 )
		{
			HW104PKSLog( L"CHW104ConfigAdapter", L" Load 104IO List File Path failed " );
			return FALSE;
		}

		//读取该点表
		KTNPksIecMap mapPksToIEC;
		KTNIecPksMap mapIECToPKS;
		if ( KTN_OK != Load104IOList(wstrFilePath, mTable, mapPksToIEC, mapIECToPKS,pointArray) )
		{
			return FALSE;
		}

		//保存IP和点表
		m_mapIPToPointTable.insert( std::make_pair(strIP, mapPksToIEC) );

		m_mapIPToKTNIecPksMap.insert( std::make_pair(strIP,mapIECToPKS) );
	}	

	m_R_OPCVector = pointArray;

	unsigned long update_rate=(unsigned long)GetPrivateProfileIntW( 
		WOPCSECTION,
		WUPDATERATE,
		DEFAULT_UPDATE_RATE,
		wstrIniPath.c_str()
		);

	//获得客户端个数
	int opc_num;
	opc_num = GetPrivateProfileIntW( 
						WOPCSECTION,
						WOPCSERVERNUM,
						-1,
						wstrIniPath.c_str()
						);
	if( opc_num<=0 && opc_num>8 ){
		HW104PKSLog( L"Config Adapter", L"wrong "WOPCSERVERNUM );
		return KTN_FAILED;
	}

	for(int i=0;i<opc_num;i++){
		wchar_t szProgID[100]={0};
		wchar_t prog_key[50];
		_snwprintf(prog_key,sizeof(prog_key),WPROGIDKEY L"%d",i+1);
		GetPrivateProfileStringW( 
			WOPCSECTION,
			prog_key,
			NULL,
			szProgID,
			100,
			wstrIniPath.c_str()
			);

		if(szProgID[0]==0){
			HW104PKSLog( L"Config Adapter", L"wrong "WPROGIDKEY );
			return KTN_FAILED;
		}

		wchar_t server_key[50];
		_snwprintf(server_key,sizeof(server_key),WOPCADDRESSKEY L"%d",i+1);

		wchar_t szOPCIP[50] = {0};
		GetPrivateProfileStringW( 
			WOPCSECTION,
			server_key,
			L"127.0.0.1",
			szOPCIP,
			50,
			wstrIniPath.c_str()
			);

		//if( szOPCIP[0]==0 ){
		//	HW104PKSLog( L"Config Adapter", L"wrong "WOPCADDRESSKEY );
		//	return KTN_FAILED;
		//}

		CString szmsg;
		szmsg.Format(L"Load %dst :ip %s, progID %s",i,szOPCIP,szProgID);
		HW104PKSLog(L"CHW104ConfigAdapter",(LPCWSTR)szmsg);

		COPCCollector *opc=new COPCCollector(szProgID,szOPCIP,update_rate,this);
		m_OPCArray.push_back(opc);
	}

	HW104PKSLog( L"CHW104ConfigAdapter", L" End LoadKTNConfig " );

	return KTN_OK;
}

//加载单份点表
KTN_RET NKTNCollector::CHW104ConfigAdapter::Load104IOList( const wchar_t* wstrFilePath,
														   CHW104MainTable& mTable,
														   KTNPksIecMap& mapPksToIEC,
														   KTNIecPksMap& mapIECtoPKS,
														   std::vector<KTNPKSPoint>& pointArray )
{
	//判断文件是否存在
	if( !CHW104Help::IsFileExist( wstrFilePath ))
	{
		CString szError;
		szError.Format( L"%s is not exist ",wstrFilePath );
		HW104PKSLog( L"CHW104ConfigAdapter", (LPCWSTR)szError );
		return FALSE;
	}

	//连接Excel
	KVADOConnection stationTableCon;
	if ( !stationTableCon.CreateInstance() )
	{
		HW104PKSLog( L"CHW104ConfigAdapter", L"Load 104IO List failed" );
		return FALSE;
	}

	CString conString = L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=";
	conString += wstrFilePath;
	conString += L";Extended Properties=\"Excel 8.0;HDR=No;IMEX=1\"";
	if ( !stationTableCon.Open( (LPCWSTR)conString ) )
	{
		HW104PKSLog( L"CHW104ConfigAdapter", L"stationTableCon.Open failed" );
		return FALSE;
	}

	//创建记录集
	KVADORecordset adoRecord;
	if ( !adoRecord.CreateInstance() )
	{
		HW104PKSLog( L"CHW104ConfigAdapter", L"Fail to adoRecord.CreateInstance()" );
		return FALSE;
	}

	//枚举所有的表名称
	std::set< CString > sheetNameSet;
	mTable.EnumDeviceType( sheetNameSet );	
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
			HW104PKSLog( L"CHW104ConfigAdapter", (LPCWSTR)szError );
			continue;
		}
		else
		{
			CString szError;
			szError += L" Start to load sheet name:  ";
			szError += *it4SheetName;
			HW104PKSLog( L"CHW104ConfigAdapter", (LPCWSTR)szError );			
		}

		//获取行数
		long lRowsCounts = adoRecord.GetRecordCount();
		long lFieldCounts = adoRecord.GetFieldsCount();
		if ( 0 == lFieldCounts )
		{
			TRACE( "adoRecord.GetFieldsCount() failed" );
			return FALSE;
		}

		//如果这三列没有,则退出程序
		long lPointsIndex = -1;
		long lRegAddrIndex = -1;
		long lAsduIndex = -1;
		//如果这一列没有,则使用默认阈值
		long lThresholdIndex = -1;

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
							HW104PKSLog( L"CHW104ConfigAdapter",L"VirRegAddr column repeat, ingore" );
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
							HW104PKSLog( L"CHW104ConfigAdapter",L"Point column repeat, ingore" );
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
							HW104PKSLog( L"CHW104ConfigAdapter",L"Asdu column repeat, ingore" );
						}
					}
					else if ( wstrColumnNameTemp.CompareNoCase( THRESHOLDCOLUMN ) == 0 )
					{
						if ( -1 == lThresholdIndex )
						{
							lThresholdIndex = i;
						}
						else
						{
							HW104PKSLog( L"CHW104ConfigAdapter",L"Threshlod column repeat, ingore" );
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
			HW104PKSLog( L"CHW104ConfigAdapter", (LPCWSTR)szError );
			continue;
		}

		//校验列号
		if ( -1 == lRegAddrIndex )
		{
			HW104PKSLog( L"CHW104ConfigAdapter", L" VirRegAddr cloumn is not exist " );
			continue;
		}
		if ( -1 == lPointsIndex )
		{
			HW104PKSLog( L"CHW104ConfigAdapter", L" Point cloumn is not exist " );
			continue;
		}
		if ( -1 == lAsduIndex )
		{
			HW104PKSLog( L"CHW104ConfigAdapter", L" ASDU cloumn is not exist " );
			continue;
		}
		if ( -1 == lThresholdIndex )
		{
			HW104PKSLog( L"CHW104ConfigAdapter", L"AbsThreshold cloumn is not exist " );
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
					CString szError;
					szError.Format( L" %d rows %d colum: VirRegAddr is wrong ", i, lRegAddrIndex );
					HW104PKSLog( L"CHW104ConfigAdapter", (LPCWSTR)szError );
					continue;
				}
				wstrRegAddr.assign( vtTemp.bstrVal );
				if ( true == wstrRegAddr.empty() )
				{
					CString szError;
					szError.Format( L" %d rows %d colum: VirRegAddr is empty ", i, lRegAddrIndex );
					HW104PKSLog( L"CHW104ConfigAdapter", (LPCWSTR)szError );
					continue;
				}

				//读取Point 信息
				KTNPKSPoint ktnPoint;
				COleVariant vtTemp3 = adoRecord( lPointsIndex );
				if ( vtTemp3.vt != VT_BSTR )
				{
					CString szError;
					szError.Format( L" %d rows %d column: point is wrong ", i, lPointsIndex  );
					HW104PKSLog( L"CHW104ConfigAdapter", (LPCWSTR)szError );
					continue;
				}

				ktnPoint.point = (wchar_t*)vtTemp3.bstrVal;
				if ( true == ktnPoint.point.empty() )
				{
					CString szError;
					szError.Format( L" %d rows %d column: point is empty ", i, lPointsIndex  );
					HW104PKSLog( L"CHW104ConfigAdapter", (LPCWSTR)szError );
					continue;
				}
				if ( 60 < ktnPoint.point.length() )
				{//名称长度超限
					CString szError;
					szError.Format( L" %d rows %d column: point is  invalid : Name Length Overflow  ", i, lPointsIndex  );
					HW104PKSLog( L"CHW104ConfigAdapter", (LPCWSTR)szError );
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
					szError.Format( L" %d rows %d column VirRegAddr %s,cann't find in MainTable ", i, lRegAddrIndex, wstrRegAddr.c_str() );
					HW104PKSLog( L"CHW104ConfigAdapter", (LPCWSTR)szError );
					continue;
				}
				iecAddrTmp.pos1 = *(char*)(&nIecAddress);
				iecAddrTmp.pos2 = *( (char*)(&nIecAddress) + 1 );
				iecAddrTmp.pos3 = *( (char*)(&nIecAddress) + 2 );

				//读取ASDU
				int	nASDU = 0;
				//ASDU数值
				COleVariant vtTemp2 = adoRecord( lAsduIndex );
				if ( vtTemp2.vt != VT_BSTR )
				{
					CString szError;
					szError.Format( L" %d rows %d colum ASDU %d is wrong ", i, lPointsIndex ,nASDU );
					HW104PKSLog( L"CHW104ConfigAdapter", (LPCWSTR)szError );
					continue;
				}
				swscanf_s( vtTemp2.bstrVal, L"%d",&nASDU );

				//根据ASDU区别处理读点和写点
				if ( nASDU == 1  )  //读点, 单点信息
				{
					iecAddrTmp.asduType = (KTNASDUType)nASDU;

					//ktnPoint.point += L".PV";  /* Deleted by wangdong */

					//所有点表的汇总表, 用于和PKS交互
					if ( m_KTNPksIecMap.find(ktnPoint) == m_KTNPksIecMap.end() )
					{
						ktnPoint.cookie = pointArray.size();
						m_KTNPksIecMap.insert( std::make_pair( ktnPoint, iecAddrTmp ) );
						pointArray.push_back( ktnPoint );
					}					

					//单份点表(读点), 每份针对一个IP
					mapPksToIEC.insert( std::make_pair(ktnPoint, iecAddrTmp) );
				}
				else if ( 13 == nASDU ) //读点, 浮点数
				{
					iecAddrTmp.asduType = (KTNASDUType)nASDU;
					//ktnPoint.point += L".PV";  /* Deleted by wangdong */
					ktnPoint.thresHold = g_HW104DefaultThresHold;
					//模拟量需要设置阈值
					if ( -1 != lThresholdIndex )
					{//如果有这一列,则需要根据实际赋值
						COleVariant vtThresHold = adoRecord( lThresholdIndex );

						//1. 如果为空,则还是按照默认值
						//2. 如果不为空,则根据需要设置阈值
						if ( ( vtThresHold.vt == VT_BSTR ) && ( vtThresHold.bstrVal != L"" ) )
						{
							//增加对是否为有效数字的判断，2010-1-29，栾述强修改
							CString strThreshold( vtThresHold.bstrVal ); //将BSTR转为CString
							if ( CHW104Help::IsLegalNumber(strThreshold) ) //读取的是有效数字
							{								
								ktnPoint.thresHold.thresHoldType = KTN_ABSOLUTE;
								_stscanf( strThreshold, _T("%f"), &(ktnPoint.thresHold.thresHold) );
								//swscanf_s( vtThresHold.bstrVal, L"%f",&( ktnPoint.thresHold.thresHold ) );								
							}							
						}
					}

					//所有点表的汇总表, 用于和PKS交互
					if ( m_KTNPksIecMap.find(ktnPoint) == m_KTNPksIecMap.end() )
					{
						ktnPoint.cookie = pointArray.size();
						m_KTNPksIecMap.insert( std::make_pair( ktnPoint, iecAddrTmp ) );
						pointArray.push_back( ktnPoint );
					}

					//单份点表(读点), 每份针对一个IP
					mapPksToIEC.insert( std::make_pair(ktnPoint, iecAddrTmp) );
				}
				else if ( 45 == nASDU ) //写点, 所有写点也可读, 单点信息
				{			
					std::wstring name = ktnPoint.point;
					//读属性的点,
					iecAddrTmp.asduType = M_SP_NA_1;

					//ktnPoint.point = name + L".PV";  /* Deleted by wangdong */

					//所有点表的汇总表, 用于和PKS交互
					if ( m_KTNPksIecMap.find(ktnPoint) == m_KTNPksIecMap.end() )
					{
						ktnPoint.cookie = pointArray.size();
						m_KTNPksIecMap.insert( std::make_pair( ktnPoint, iecAddrTmp ) );
						pointArray.push_back( ktnPoint );
					}

					//单份点表(读点), 每份针对一个IP
					mapPksToIEC.insert( std::make_pair(ktnPoint, iecAddrTmp) );

					//ktnPoint.point = name + L".OP";  /* Deleted by wangdong */

					iecAddrTmp.asduType = (KTNASDUType)nASDU;

					//所有点表的汇总表, 用于和PKS交互
					if ( m_KTNPksIecMap.find(ktnPoint) == m_KTNPksIecMap.end() )
					{
						ktnPoint.cookie = pointArray.size();
						m_KTNPksIecMap.insert( std::make_pair( ktnPoint, iecAddrTmp ) );
						pointArray.push_back( ktnPoint );
					}

					mapIECtoPKS.insert( std::make_pair( iecAddrTmp,ktnPoint.cookie ) );
				}
				else if ( 50 == nASDU ) //写点, 所有写点也可读, 浮点数
				{
					std::wstring name = ktnPoint.point;
					//读属性的点
					ktnPoint.thresHold = g_HW104DefaultThresHold;

					//模拟量需要设置阈值
					if ( -1 != lThresholdIndex )
					{//如果有这一列,则需要根据实际赋值
						COleVariant vtThresHold = adoRecord( lThresholdIndex );

						//1. 如果为空,则还是按照默认值
						//2. 如果不为空,则根据需要设置阈值
						if ( ( vtThresHold.vt == VT_BSTR ) && ( vtThresHold.bstrVal != L"" ) )
						{
							//增加对是否为有效数字的判断，2010-1-29，栾述强修改
							CString strThreshold( vtThresHold.bstrVal ); //将BSTR转为CString
							if ( CHW104Help::IsLegalNumber(strThreshold) ) //读取的是有效数字
							{								
								ktnPoint.thresHold.thresHoldType = KTN_ABSOLUTE;
								_stscanf( strThreshold, _T("%f"), &(ktnPoint.thresHold.thresHold) );
								//swscanf_s( vtThresHold.bstrVal, L"%f",&( ktnPoint.thresHold.thresHold ) );								
							}		
						}
					}

					//ktnPoint.point = name + L".PV";  /* Deleted by wangdong */
					iecAddrTmp.asduType = M_ME_NC_1;

					//所有点表的汇总表, 用于和PKS交互
					if ( m_KTNPksIecMap.find(ktnPoint) == m_KTNPksIecMap.end() )
					{
						ktnPoint.cookie = pointArray.size();
						m_KTNPksIecMap.insert( std::make_pair( ktnPoint , iecAddrTmp ) );
						pointArray.push_back( ktnPoint );
					}

					//单份点表(读点), 每份针对一个IP
					mapPksToIEC.insert( std::make_pair(ktnPoint, iecAddrTmp) );
					//ktnPoint.point = name + L".SP";  /* Deleted by wangdong */
					iecAddrTmp.asduType = (KTNASDUType)nASDU;

					//所有点表的汇总表, 用于和PKS交互
					if ( m_KTNPksIecMap.find(ktnPoint) == m_KTNPksIecMap.end() )
					{
						ktnPoint.cookie = pointArray.size();
						m_KTNPksIecMap.insert( std::make_pair( ktnPoint , iecAddrTmp ) );
						pointArray.push_back( ktnPoint );
					}

					mapIECtoPKS.insert( std::make_pair( iecAddrTmp,ktnPoint.cookie ) );
				}
				else
				{
					CString szError;
					szError.Format( L" %d rows %d colum ASDU %d error ", i, lPointsIndex ,nASDU );
					HW104PKSLog( L"CHW104ConfigAdapter", (LPCWSTR)szError );
					continue;
				}

			}
		}
		catch ( _com_error &Error )
		{
			CString szError = (wchar_t*)Error.Description();
			HW104PKSLog( L"CHW104ConfigAdapter", (LPCWSTR)szError );
			continue;
		}

	}//-for

	return KTN_OK;
}

KTN_RET NKTNCollector::CHW104ConfigAdapter::WriteLinkErr( KTNCollectorTpye clientType, BOOL bConnect )
{
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
//为多份点表的新功能, 增加输入参数strClientIP,根据IP确定使用哪个点表. 2010-6-8,栾述强修改
KTN_RET CHW104ConfigAdapter::ReadAllPoint( const std::string* pStrClientIP,
							 std::vector< KTNIECAddress >& iecArray,
							 std::vector< KTNValue >&	  valueArray
							 )
{
	iecArray.clear();
	valueArray.clear();

	//确定使用哪个点表
	std::map<std::string, KTNPksIecMap>::iterator itIPToTable = m_mapIPToPointTable.find(*pStrClientIP);
	if (m_mapIPToPointTable.end() == itIPToTable)
	{
		return KTN_ITEMNOTFOUND;
	}

	NkxLatchLock::XDBAutoLatch readDataMapLock( m_ReadDataLock, LATCH_SH );

	for ( 	KTNPksIecMapCIt c_it  = itIPToTable->second.begin();
		    c_it != itIPToTable->second.end();
			++c_it )
	{
		//读表不可能存在写的ASDU
		ASSERT( c_it->second.asduType < C_SC_NA_1 );

		iecArray.push_back( c_it->second );
		valueArray.push_back( m_R_OPCVector[ c_it->first.cookie ] .val );
	}


	HW104PKSLog( L"HW104ConfigAdapter::",L"ReadAllPoint OK" );

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
KTN_RET CHW104ConfigAdapter::OnChange( 
						 const std::vector< int >&	indexArray,
						 const std::vector< KTNValue >&		valueArray
						 )
{
	ASSERT( indexArray.size() == valueArray.size() );
	
	std::vector< KTNHW104_InfoUnit > infoUTArray;
	infoUTArray.reserve( valueArray.size() );

	//对各个点表做判断
	std::map<std::string, KTNPksIecMap>::iterator itIPToTable;
	for (itIPToTable=m_mapIPToPointTable.begin(); itIPToTable!=m_mapIPToPointTable.end(); ++itIPToTable)
	{				
		infoUTArray.clear();

		//找出属于当前点表的点
		for( unsigned int i = 0; i<indexArray.size(); ++i )
		{
			if( indexArray[i] >= m_R_OPCVector.size() || indexArray[i]<0 ){
				continue;
			}

			KTNPksIecMapCIt c_it2 = itIPToTable->second.find( m_R_OPCVector[ indexArray[i] ] );
			if( c_it2 == itIPToTable->second.end() )
			{
				continue;
			}

			if(!OverThreshold(m_R_OPCVector[ indexArray[i] ].val,valueArray[i],m_R_OPCVector[i].thresHold))
			{

				continue;
			}

			//找到一个, 转化其值并存入infoUTArray
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

			m_R_OPCVector[ indexArray[i] ].val = valueArray[i];

			infoUTArray.push_back( infoUt );			
		}		
		if ( infoUTArray.size() == 0 )
		{
			continue;
		}

		//找到使用当前点表的订阅者
		AutoLock auLock( &m_SubListLock );
		std::map<std::string, CHW104Subscription*>::iterator itSubscribe;
		itSubscribe = m_mapIPToSubscribe.find(itIPToTable->first);
		if (m_mapIPToSubscribe.end() == itSubscribe)
		{
			continue;
		}

		itSubscribe->second->OnChange(infoUTArray);			
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
KTN_RET CHW104ConfigAdapter::WriteIecData(
							 const std::string& ip,
							 const KTNIECAddress&   iecAddress,
							 const KTNValue&		   ktnValue
							 )
{
	wchar_t wstrError[ MAX_ERROR_LEN ] = {0};

	std::map<std::string, KTNIecPksMap>::iterator map_it = m_mapIPToKTNIecPksMap.find( ip );

	if ( m_mapIPToKTNIecPksMap.end() == map_it )
	{// can not find pkspint
		swprintf_s( wstrError, MAX_ERROR_LEN, 
			L"g_PKSCollector.WriteIecData %d.%d.%d error KTN_COLLECTOR_NO_IPADDRESS",
			iecAddress.pos1,iecAddress.pos2,iecAddress.pos3 );
		HW104PKSLog( L"CHW104ConfigAdapter", wstrError );
		return KTN_COLLECTOR_NO_IECADDRESS;
	}

	//write error
	//KTN_RET ret = g_PKSCollector.WriteSinglePoint( c_it->second, ktnValue );
 
	KTNIecPksMapCIt c_it = map_it->second.find(iecAddress);

	if ( map_it->second.end() == c_it )
	{// can not find pkspint
		swprintf_s( wstrError, MAX_ERROR_LEN, 
			L"g_PKSCollector.WriteIecData %d.%d.%d error KTN_COLLECTOR_NO_IECADDRESS",
			iecAddress.pos1,iecAddress.pos2,iecAddress.pos3 );
		HW104PKSLog( L"CHW104ConfigAdapter", wstrError );
		return KTN_COLLECTOR_NO_IECADDRESS;
	}

	ASSERT( c_it->second >=0 && c_it->second< m_R_OPCVector.size() );

	if( m_R_OPCVector[c_it->second].is_valid == false ){
		swprintf_s( wstrError, MAX_ERROR_LEN, L"g_PKSCollector.WriteIecData %d.%d.%d invalid point",
			iecAddress.pos1,iecAddress.pos2,iecAddress.pos3 );
		HW104PKSLog( L"CHW104ConfigAdapter", wstrError );
		return KTN_COLLECTOR_NO_IECADDRESS;
	}
	{
		NkxLatchLock::XDBAutoLatch readDataMapLock( m_IECOPCMapLock, LATCH_EX );

		m_current_write_index = c_it->second;
		m_write_val = ktnValue;
	}

	SetEvent( m_WriteEvent );
	
	//write ok
	swprintf_s( wstrError, MAX_ERROR_LEN, L"g_PKSCollector.WriteIecData %d.%d.%d OK",
		iecAddress.pos1,iecAddress.pos2,iecAddress.pos3 );
	HW104PKSLog( L"CHW104ConfigAdapter", wstrError );

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
KTN_RET CHW104ConfigAdapter::RegsiterCallBack( std::string strClientIP,
			  IHW104CollectorCallBack* pIHW104CallBack
								 )
{
	if ( NULL == pIHW104CallBack )
	{
		return KTN_OK;
	}

	AutoLock auLock( &m_SubListLock );

	if (m_mapIPToSubscribe.find(strClientIP) != m_mapIPToSubscribe.end())
	{
		return KTN_OK;
	}
	
	CHW104Subscription* pSubscription = NULL;

	{
		pSubscription = new CHW104Subscription;
		pSubscription->AddCallBackInterface( pIHW104CallBack );
		m_mapIPToSubscribe.insert(std::make_pair(strClientIP, pSubscription));
		pSubscription->Start();
	}

	return KTN_OK;
}

/// <summary>
/// 	获得数量
/// </summary>
int CHW104ConfigAdapter::GetCount()
{
	return m_KTNPksIecMap.size();
}

/// <summary>
///   停止变化上送
/// </summary>
//void CHW104ConfigAdapter::End()
//{
//	AutoLock auLock( &m_SubListLock );
//
//	std::map<std::string, CHW104Subscription*>::iterator it;
//	for( it = m_mapIPToSubscribe.begin(); it != m_mapIPToSubscribe.end(); ++it )
//	{
//		//停止变化上送
//		it->second->End();
//		//删除指针
//		delete it->second;
//	}	
//}

KTN_RET CHW104ConfigAdapter::StartCollect()
{
	m_ThreadStop=0;

	if(m_WriteEvent==INVALID_HANDLE_VALUE){
		m_WriteEvent=CreateEvent(
			NULL,
			FALSE,
			FALSE,
			NULL);
	}

	int i;
	for(i=0;i<m_OPCArray.size();i++){
		HANDLE collect_thread=(HANDLE)_beginthreadex(NULL,
		0,
		ThreadHook,
		m_OPCArray.at(i),
		0,
		NULL);

		m_ThreadArray.push_back(collect_thread);
	}

	return KTN_OK;
}

void CHW104ConfigAdapter::EndCollect()
{
	if( m_ThreadArray.size()!=0 ){
		m_ThreadStop=1;

		int i;
		for(i=0;i<m_ThreadArray.size();i++){
			HANDLE thread=m_ThreadArray.at(i);
	
			WaitForSingleObject(thread,INFINITE);
			CloseHandle(thread);
		}
	}

	if( m_OPCArray.size()!=0 ){
		int i;
		for(i=0;i<m_OPCArray.size();i++){
			COPCCollector *opc=m_OPCArray.at(i);

			delete opc;
		}
	}

	if(m_WriteEvent!=INVALID_HANDLE_VALUE){
		CloseHandle(m_WriteEvent);
		m_WriteEvent=INVALID_HANDLE_VALUE;
	}

	HW104PKSLog( L"Config Adapter", L"End Collect" );
}

void CHW104ConfigAdapter::CollectThread(COPCCollector* opc){
	CoInitialize(NULL);
	CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_NONE, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
	int failed_count = 0;

	while (!m_ThreadStop){
		if (!opc->Connect()){
			opc->DisConnect();
			Sleep(2000);
			continue;
		}
		if (opc->GetStatus())
		{
			DWORD r = WaitForSingleObject(m_hMutexCollectThread, 2000);

			//获得令牌    返回值为WAIT_OBJECT_0
			if (r == WAIT_OBJECT_0)
			{
				failed_count = 0;
				{
					NkxLatchLock::XDBAutoLatch readDataMapLock(m_ReadDataLock, LATCH_EX);

					if (!opc->ReadAll(m_R_OPCVector)){
						ReleaseMutex(m_hMutexCollectThread);
						opc->DisConnect();
						continue;
					}
				}
				do
				{
					DWORD count = 1;
					DWORD ret = MsgWaitForMultipleObjects(count, &m_WriteEvent, FALSE, 500, QS_ALLEVENTS);
					switch (ret)
					{
					case (WAIT_OBJECT_0 + 1):
					{
						MSG msg;
						if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)){
							TranslateMessage(&msg);
							DispatchMessage(&msg);
						}
					}
						break;
					case WAIT_OBJECT_0:
					{
						KTNPKSPoint point;
						
						{
							NkxLatchLock::XDBAutoLatch readDataMapLock( m_IECOPCMapLock, LATCH_EX );
							ASSERT( m_current_write_index>=0 && m_current_write_index<m_R_OPCVector.size() );
							point = m_R_OPCVector[m_current_write_index];
							point.val = m_write_val;
						}
						CString szError;
						szError.Format(L"start to write to point %s",point.point.c_str());
						HW104PKSLog(L"OPCCollector", (LPCWSTR)szError);

						opc->WriteItems( point );
					}
						break;
					case WAIT_TIMEOUT:
					{}
						break;
					default:
						break;
					}
				} while (opc->GetStatus() && !m_ThreadStop);
				//释放令牌
				ReleaseMutex(m_hMutexCollectThread);
				opc->DisConnect();
			}
		}
		else
		{
			opc->DisConnect();
		}
	}
	opc->DisConnect();
}
