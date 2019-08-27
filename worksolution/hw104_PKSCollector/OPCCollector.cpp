#include "StdAfx.h"

#include "opcda.h"
#include "opccomn.h"
#include "opcerrors.h"
#include "opcprops.h"
#include "opcquality.h"

#include "comcat.h"
#include "OPCCollector.h"
#include "HW104ConfigAdapter.h"

//全局日志模块
extern CHW104_Log		  g_HW104Log;

static inline bool ktnValue2Variant(const KTNValue& point,VARIANT *var){
	if( point.GetDataType() == KTNVDT_I4 ){
		switch(var->vt){
		case VT_BOOL:
			{
				var->boolVal = point.asInt32();
			}
			break;
		case VT_UI1:
			{
				var->bVal=(BYTE)point.asInt32();
			}
			break;
		case VT_I1:
			{
				var->cVal=(signed char)point.asInt32();
			}
			break;
		case VT_UI2:
			{
				var->uiVal=(unsigned short)point.asInt32();
			}
			break;
		case VT_I2:
			{
				var->iVal=(short)point.asInt32();
			}
			break;
		case VT_I4:
			{
				var->lVal=(int)point.asInt32();
			}
			break;
		default:
			return false;
		}
		return true;
	}
	else if( point.GetDataType()  == KTNVDT_R4 ){
		switch(var->vt){
		case VT_UI1:
			{
				var->bVal=(BYTE)point.asFloat();
			}
			break;
		case VT_I1:
			{
				var->cVal=(signed char)point.asFloat();
			}
			break;
		case VT_UI2:
			{
				var->uiVal=(unsigned short)point.asFloat();
			}
			break;
		case VT_I2:
			{
				var->iVal=(short)point.asFloat();
			}
			break;
		case VT_I4:
			{
				var->lVal=(int)point.asFloat();
			}
			break;
		case VT_R4:
			{
				var->fltVal=point.asFloat();
			}
			break;
		case VT_R8:
			{
				var->dblVal=point.asFloat();
			}
			break;
		case VT_UI4:
			{
				var->ulVal=(unsigned int)point.asFloat();
			}
			break;
		default:
			return false;
		}

		return true;
	}

	return false;
}

static inline bool ItemState2KTNVALUE(const VARIANT *var, WORD quality, FILETIME* time, KTNValue& point){
	switch(var->vt){
		case VT_UI1:
			{
				point=(int)var->bVal;
			}
			break;
		case VT_I1:
			{
				point=(int)var->cVal;
			}
			break;
		case VT_UI2:
			{
				point=(int)var->uiVal;
			}
			break;
		case VT_I2:
			{
				point=(int)var->iVal;
			}
			break;
		case VT_I4:
			{
				point=(int)var->lVal;
			}
			break;
		case VT_R4:
			{
				point=var->fltVal;
			}
			break;
		case VT_R8:
			{
				point=(float)var->dblVal;
			}
			break;
		case VT_BOOL:
			{
				point=(int)var->boolVal;
			}
			break;
		default:
			point.Clear();
			return false;
	}

	//更改IEC质量戳判定方式
	if(quality==OPC_QUALITY_GOOD || quality==OPC_QUALITY_LAST_USABLE)
		point.SetQuality(true);
	else
		point.SetQuality(false);

	//添加时间戳
	FileTimeToSystemTime(time, &point.timeStamp);

	return true;
}

class CMyCallBack : public IOPCDataCallback{
private:
	DWORD mRefCount;

	CHW104ConfigAdapter *m_Adpater;
public:
	CMyCallBack(CHW104ConfigAdapter *adpater):m_Adpater(adpater){
		mRefCount = 0;
	}

	~CMyCallBack(){

	}

	/**
	* Functions associated with IUNKNOWN
	*/
	STDMETHODIMP QueryInterface( REFIID iid, LPVOID* ppInterface){
		if ( ppInterface == NULL){
			return E_INVALIDARG;
		}

		if ( iid == IID_IUnknown ){
			*ppInterface = (IUnknown*) this;
		} else if ( iid == IID_IOPCDataCallback){
			*ppInterface = (IOPCDataCallback*) this;
		} else
		{
			*ppInterface = NULL;
			return E_NOINTERFACE;
		}

		AddRef();
		return S_OK;
	}

	STDMETHODIMP_(ULONG) AddRef(){
		return ++mRefCount;
	}


	STDMETHODIMP_(ULONG) Release(){
		--mRefCount; 

		//if ( mRefCount == 0){
		//	delete this;
		//}
		return mRefCount;
	}

	/**
	* Functions associated with IOPCDataCallback
	*/
	STDMETHODIMP OnDataChange(DWORD Transid, OPCHANDLE grphandle, HRESULT masterquality,
		HRESULT mastererror, DWORD count, OPCHANDLE * clienthandles, 
		VARIANT * values, WORD * quality, FILETIME  * time,
		HRESULT * errors)
	{
		std::vector<int> indexs;
		indexs.reserve(count);
		std::vector<KTNValue> ktnvalues;
		ktnvalues.reserve(count);

		int i;
		for(i=0;i<count;i++){
			KTNValue tmpValue;
			ItemState2KTNVALUE(values + i, quality[i], time + i, tmpValue);

			ktnvalues.push_back(tmpValue);
			indexs.push_back(clienthandles[i]);

			/*
			CString szItemID, szType, szVal, szInf;
			szItemID = m_Adpater->m_R_OPCVector[clienthandles[i]].item_id.c_str();
			switch (tmp.GetDataType())
			{
				case KTNVDT_EMPTY:
					szType.Format(L"EMPTY");
					break;
				case KTNVDT_BOOL:
					szType.Format(L"BOOL");
					szVal.Format(L"%s", tmp.asBool()?L"TRUE":L"FALSE");
					break;
				case KTNVDT_I1:
					szType.Format(L"INT8");
					szVal.Format(L"%d", tmp.asInt8());
					break;
				case KTNVDT_UI1:
					szType.Format(L"UINT8");
					szVal.Format(L"%d", tmp.asUInt8());
					break;
				case KTNVDT_I2:
					szType.Format(L"INT16");
					szVal.Format(L"%d", tmp.asInt16());
					break;
				case KTNVDT_UI2:
					szType.Format(L"UINT16");
					szVal.Format(L"%d", tmp.asUInt16());
					break;
				case KTNVDT_I4:
					szType.Format(L"INT32");
					szVal.Format(L"%d", tmp.asInt32());
					break;
				case KTNVDT_UI4:
					szType.Format(L"UINT32");
					szVal.Format(L"%d", tmp.asUInt32());
					break;
				case KTNVDT_I8:
					szType.Format(L"INT64");
					szVal.Format(L"%l", tmp.asInt64());
					break;
				case KTNVDT_UI8:
					szType.Format(L"UINT64");
					szVal.Format(L"%l", tmp.asUInt64());
					break;
				case KTNVDT_R4:
					szType.Format(L"FLOAT32");
					szVal.Format(L"%f", tmp.asFloat());
					break;
				case KTNVDT_R8:
					szType.Format(L"DOUBLE64");
					szVal.Format(L"%f", tmp.asDouble());
					break;
				case KTNVDT_FILETIME:
					szType.Format(L"FILETIME");
					szVal.Format(L"%s", tmp.asFileTime());
					break;
				case KTNVDT_STR:
					szType.Format(L"STRING");
					szVal.Format(L"%s", tmp.asAnsiString());
					break;
				case KTNVDT_WSTR:
					szType.Format(L"UnicodeSTR");
					szVal.Format(L"%s", tmp.asWideString());
					break;
			}
			szInf.Format(L"OPC Data: ItemID=> %-40sType=> %-20sVal=> %-12sQuality=> %X", szItemID, szType, szVal, *(quality+i));
			HW104PKSLog(L"OPCCollector", (LPCWSTR)szInf);

			if(*(errors+i)!=S_OK)
			{
				szInf.Format(L"OPC Data: ItemID=> %-40ERROR_ID:%X", szItemID, *(errors+i));
				HW104PKSLog(L"OPCCollector", (LPCWSTR)szInf);
			}
			//*/
		}

		m_Adpater->OnChange(indexs, ktnvalues);

		return S_OK;
	}


	STDMETHODIMP OnReadComplete(DWORD Transid, OPCHANDLE grphandle, 
		HRESULT masterquality, HRESULT mastererror, DWORD count, 
		OPCHANDLE * clienthandles, VARIANT* values, WORD * quality,
		FILETIME * time, HRESULT * errors)
	{
		return S_OK;
	}


	STDMETHODIMP OnWriteComplete(DWORD Transid, OPCHANDLE grphandle, HRESULT mastererr, 
		DWORD count, OPCHANDLE * clienthandles, HRESULT * errors)
	{
		return S_OK;
	}



	STDMETHODIMP OnCancelComplete(DWORD transid, OPCHANDLE grphandle){
		return S_OK;
	}
};

#define INVALID_OPC_HANDLE (OPCHANDLE)(-1)

struct COPCCollectorImp{
	COPCCollectorImp(void):is_connected(false),is_advised(false),m_server_handle(INVALID_OPC_HANDLE),callback(NULL)
	{
	}

	~COPCCollectorImp(void){
		delete callback;
	}

	std::wstring prog_id;
	std::wstring host;
	bool is_connected;
	bool is_advised;

	ATL::CComPtr<IOPCServer>			iOpcServer;
	ATL::CComPtr<IOPCGroupStateMgt>		iStateManagement;
	ATL::CComPtr<IOPCSyncIO>			iSychIO;
	ATL::CComPtr<IOPCAsyncIO2>			iAsych2IO;
	ATL::CComPtr<IOPCItemMgt>			iItemManagement;
	ATL::CComPtr<IConnectionPoint>		iAsynchDataCallbackConnectionPoint;
	DWORD								datachang_cookie;
	CMyCallBack *callback;
	OPCHANDLE	m_server_handle;
};

COPCCollector::COPCCollector(const std::wstring& prog_id,const std::wstring& host,
							 unsigned long update_rate,CHW104ConfigAdapter *adapter):m_UpdateRate(update_rate){
	m_Imp=new COPCCollectorImp();

	m_Imp->prog_id=prog_id;
	m_Imp->host=host;
	m_Imp->callback=new CMyCallBack(adapter);
	m_Adpater=adapter;
}

COPCCollector::~COPCCollector(void){
	delete m_Imp;
}

// Satisfy OPCENUM requirements
static const CLSID CLSID_OPCServerList = 
{ 0x13486D51, 0x4821, 0x11D2, { 0xA4, 0x94, 0x3C, 0xB3, 0x06, 0xC1, 0x00, 0x00 } };

bool COPCCollector::ConnectRemote(void){
	COSERVERINFO tServerInfo;
	ZeroMemory (&tServerInfo, sizeof (tServerInfo));

	tServerInfo.pwszName = (LPWSTR)m_Imp->host.c_str();
	// Interfaces of interest
	MULTI_QI arrMultiQI [1] = {&IID_IOPCServerList, NULL, 0};

	// Connect to the OPC server browser
	HRESULT hr = CoCreateInstanceEx (CLSID_OPCServerList, NULL, CLSCTX_SERVER, &tServerInfo, _countof (arrMultiQI), arrMultiQI);

	if (FAILED(hr)){
		CString szError;
		szError.Format(L"Connecting to OPCServer failed. Error_ID:%X [ConnectRemote:CoCreateInstanceEx_1]", hr);
		HW104PKSLog(L"OPCCollector", (LPCWSTR)szError);
		return false;
	}
	ATL::CComPtr<IOPCServerList> pIOPCServerList = (IOPCServerList *)arrMultiQI [0].pItf;
	arrMultiQI [0].pItf = NULL;

	CLSID clsid;
	hr = pIOPCServerList->CLSIDFromProgID (m_Imp->prog_id.c_str(), &clsid);
	if (FAILED(hr)){
		CString szError;
		szError.Format(L"Connecting to OPCServer failed. Error_ID:%X [ConnectRemote:CLSIDFromProgID]", hr);
		HW104PKSLog(L"OPCCollector", (LPCWSTR)szError);
		return false;
	}
	arrMultiQI[0].pIID=&IID_IOPCServer;
	arrMultiQI[0].hr=0;
	arrMultiQI[0].pItf=NULL;
	ZeroMemory (&tServerInfo, sizeof (tServerInfo));
	tServerInfo.pwszName = (LPWSTR)m_Imp->host.c_str();

	hr=CoCreateInstanceEx( clsid,NULL,CLSCTX_REMOTE_SERVER, &tServerInfo, _countof (arrMultiQI), arrMultiQI);
	if (FAILED(hr)){
		CString szError;
		szError.Format(L"Connecting to OPCServer failed. Error_ID:%X [ConnectRemote:CoCreateInstanceEx_2]", hr);
		HW104PKSLog(L"OPCCollector", (LPCWSTR)szError);
		return false;
	}

	m_Imp->iOpcServer=(IOPCServer *)arrMultiQI[0].pItf;
	
	m_Imp->is_connected=true;

	return true;
}

bool COPCCollector::ConnectLocal(void){
	CLSID clsid;
	HRESULT result=CLSIDFromProgID(m_Imp->prog_id.c_str(),&clsid);
	if(FAILED(result))
	{
		CString szError;
		szError.Format(L"Connecting to OPCServer failed. Error_ID:%X [ConnectLocal:CLSIDFromProgID]", result);
		HW104PKSLog(L"OPCCollector", (LPCWSTR)szError);
		return false;
	}

	ATL::CComPtr<IClassFactory> iClassFactory;
	result = CoGetClassObject(clsid, CLSCTX_LOCAL_SERVER, NULL, IID_IClassFactory, (void**)&iClassFactory);
	if (FAILED(result))
	{
		CString szError;
		szError.Format(L"Connecting to OPCServer failed. Error_ID:%X [ConnectLocal:CoGetClassObject]", result);
		HW104PKSLog(L"OPCCollector", (LPCWSTR)szError);
		return false;
	}

	ATL::CComPtr<IUnknown> iUnknown;
	result = iClassFactory->CreateInstance(NULL, IID_IUnknown, (void**)&iUnknown);
	if (FAILED(result))
	{
		CString szError;
		szError.Format(L"Connecting to OPCServer failed. Error_ID:%X [ConnectLocal:CreateInstance]", result);
		HW104PKSLog(L"OPCCollector", (LPCWSTR)szError);
		return false;
	}

	result = iUnknown->QueryInterface(IID_IOPCServer, (void**)&m_Imp->iOpcServer);
	if (FAILED(result))
	{
		CString szError;
		szError.Format(L"Connecting to OPCServer failed. Error_ID:%X [ConnectLocal:QueryInterface]", result);
		HW104PKSLog(L"OPCCollector", (LPCWSTR)szError);
		return false;
	}

	m_Imp->is_connected=true;

	return true;
}

bool COPCCollector::Connect(void){
	if(m_Imp->is_connected)
		return true;

	CString szError;

	if (m_Imp->host.empty() || !m_Imp->host.compare(L"127.0.0.1")){
		if (!ConnectLocal()){
			szError.Format(L"Connecting to OPCServer failed. [ConnectLocal]");
			goto error;
		}
	}
	else{
		if (!ConnectRemote()){
			szError.Format(L"Connecting to OPCServer failed. [ConnectRemote:%s]", m_Imp->host.c_str());
			goto error;
		}
	}

	m_Imp->is_connected=true;

	unsigned long revisedUpdateRate_ms;
	long timeBias=480;
	float deadBand=0.0f;

	HRESULT result = m_Imp->iOpcServer->AddGroup(L"ReadData", 1, m_UpdateRate, 0, &timeBias, &deadBand,
		0, &m_Imp->m_server_handle, &revisedUpdateRate_ms, IID_IOPCGroupStateMgt, (LPUNKNOWN*)&m_Imp->iStateManagement);
	if (FAILED(result))
	{
		szError.Format(L"Connecting to OPCServer failed. Error_ID:%X [%s:AddGroup]", result, m_Imp->host);
		goto error;
	} 

	result = m_Imp->iStateManagement->QueryInterface(IID_IOPCSyncIO, (void**)&m_Imp->iSychIO);
	if (FAILED(result)){
		szError.Format(L"Connecting to OPCServer failed. Error_ID:%X [%s:QuerInterface(iSychIO)]", result, m_Imp->host);
		goto error;
	}

	result = m_Imp->iStateManagement->QueryInterface(IID_IOPCAsyncIO2, (void**)&m_Imp->iAsych2IO);
	if (FAILED(result)){
		szError.Format(L"Connecting to OPCServer failed. Error_ID:%X [%s:QueryInterface(iAsych2IO)]", result, m_Imp->host);
		goto error;
	}

	result = m_Imp->iStateManagement->QueryInterface(IID_IOPCItemMgt, (void**)&m_Imp->iItemManagement);
	if (FAILED(result)){
		szError.Format(L"Connecting to OPCServer failed. Error_ID:%X [%s:QueryInterface(iItemManagement)]", result, m_Imp->host);
		goto error;
	}

	HW104PKSLog(L"OPCCollector",L"Connecting to OPCServer OK");
	return true;

error:
	HW104PKSLog(L"OPCCollector", (LPCWSTR)szError);
	DisConnect();
	return false;
}

bool COPCCollector::ReadAll(std::vector<KTNPKSPoint>& points){
	/*
	1. add
	2. read sync
	3. connect sink
	*/
	DWORD noItems = (DWORD)points.size();
	OPCITEMDEF *itemDef = new OPCITEMDEF[noItems];
	unsigned i;
	for (i=0; i < noItems; i++){
		itemDef[i].szItemID =(LPWSTR) points[i].point.c_str();
		itemDef[i].szAccessPath = NULL;//wideName;
		itemDef[i].bActive = 1;
		itemDef[i].hClient = i;
		itemDef[i].dwBlobSize = 0;
		itemDef[i].pBlob = NULL;
		itemDef[i].vtRequestedDataType = VT_EMPTY;
	}

	HRESULT *itemResult;
	OPCITEMRESULT *itemDetails;
	HRESULT	result = m_Imp->iItemManagement->AddItems(noItems, itemDef, &itemDetails, &itemResult);
	delete[] itemDef;
	if (FAILED(result)){
		CString szError;
		szError.Format(L"Add All Items failed. Error_ID:%X", result);
		HW104PKSLog(L"OPCCollector", (LPCWSTR)szError);
		return false;
	}
	
	int server_index=0;
	OPCHANDLE *serverHandles=new OPCHANDLE[noItems];
	for(i=0;i<noItems;i++){
		if(itemDetails[i].pBlob){ 
			CoTaskMemFree(itemDetails[i].pBlob);
		}

		if(SUCCEEDED(itemResult[i])){
			CString szError;
			szError.Format(L"Add Items %s OK",points[i].point.c_str());
			HW104PKSLog(L"OPCCollector",(LPCWSTR)szError);
			points[i].is_valid=true;
			points[i].handle = itemDetails[i].hServer;
			points[i].type = itemDetails[i].vtCanonicalDataType;
			serverHandles[server_index++]=itemDetails[i].hServer;
		}
		else{
			CString szError;
			szError.Format(L"Add Items %s failed. Error_ID:%X",points[i].point.c_str(), itemResult[i]);
			HW104PKSLog(L"OPCCollector",(LPCWSTR)szError);
			points[i].is_valid = false;
		}
	}

	CoTaskMemFree(itemResult);
	CoTaskMemFree(itemDetails);

	OPCITEMSTATE *itemState;
	noItems=server_index;
	result=m_Imp->iSychIO->Read(OPC_DS_CACHE,noItems,serverHandles,&itemState,&itemResult);
	if(SUCCEEDED(result)){
		
		for(i=0;i<noItems;i++){
			if(SUCCEEDED(itemResult[i])){
				int opc_index=itemState[i].hClient;

				if(opc_index<0||opc_index>points.size())
					continue;
				ItemState2KTNVALUE(&itemState[i].vDataValue, itemState[i].wQuality, &itemState[i].ftTimeStamp, points[opc_index].val);
			}
		}

		CoTaskMemFree(itemResult);
		CoTaskMemFree(itemState);
	}
	else{
		delete[]serverHandles;
		CString szError;
		szError.Format(L"Read All Failed. ID:%X", result);
		HW104PKSLog(L"OPCCollector", (LPCWSTR)szError);
		return false;
	}
	
	delete[]serverHandles;

	//connect sink

	ATL::CComPtr<IConnectionPointContainer> iConnectionPointContainer = 0;
	 result = m_Imp->iStateManagement->QueryInterface(IID_IConnectionPointContainer, (void**)&iConnectionPointContainer);
	if (FAILED(result))
	{
		HW104PKSLog(L"OPCCollector",L"Do not support IID_IConnectionPointContainer");
		return false;
	}

	result = iConnectionPointContainer->FindConnectionPoint(IID_IOPCDataCallback, &m_Imp->iAsynchDataCallbackConnectionPoint);
	if (FAILED(result))
	{
		HW104PKSLog(L"OPCCollector",L"Do not support IID_IOPCDataCallback");
		return false;
	}

	result = m_Imp->iAsynchDataCallbackConnectionPoint->Advise(m_Imp->callback, &m_Imp->datachang_cookie);
	if (FAILED(result))
	{
		CString szError;
		szError.Format(L"Advise failed,DCOM configuration error is a possibliablity. ID:%X", result);
		HW104PKSLog(L"OPCCollector", (LPCWSTR)szError);
		return false;
	}

	m_Imp->is_advised=true;

	HW104PKSLog(L"OPCCollector",L"Read All OK");
	return true;
}

bool COPCCollector::WriteItems( KTNPKSPoint& point){
	if(point.is_valid == false){
		return false;
	}

	VARIANT var;
	var.vt = point.type;

	if(true == ktnValue2Variant(point.val,&var)){
		HRESULT * itemWriteErrors; 
		HRESULT ret = m_Imp->iSychIO->Write(1,(&point.handle),&var,&itemWriteErrors);

		if( FAILED(ret) ){
			CString szError;
			szError.Format(L"write to point %s failed,ret %d write_error %d", point.point.c_str(),ret,itemWriteErrors);
			HW104PKSLog(L"OPCCollector", (LPCWSTR)szError);
			return false;
		}
	}
	else{
		CString szError;
		szError.Format(L"write to point %s failed,type error %d to %d",point.point.c_str(),point.val.GetDataType(),point.type);
		HW104PKSLog(L"OPCCollector", (LPCWSTR)szError);
		return false;
	}
	CString szError;
	szError.Format(L"write to point %s ok",point.point.c_str());
	HW104PKSLog(L"OPCCollector", (LPCWSTR)szError);
	return true;
}

bool COPCCollector::GetStatus(void){
	OPCSERVERSTATUS *server_stat;

	HRESULT hr=m_Imp->iOpcServer->GetStatus(&server_stat);
	if (FAILED(hr))
	{
		CString szError;
		szError.Format(L"Get OPC Server Status:%X", hr);
		HW104PKSLog(L"OPCCollector", (LPCWSTR)szError);
		return false;
	}
	if (OPC_STATUS_RUNNING != server_stat->dwServerState)
	{
		CString szError;
		szError.Format(L"OPC Server Status:%d", server_stat->dwServerState);
		HW104PKSLog(L"OPCCollector", (LPCWSTR)szError);
		return false;
	}
	CoTaskMemFree(server_stat);

	return true;
}

void COPCCollector::DisConnect(void){
	if(!m_Imp->is_connected)	
		return;

	if(m_Imp->is_advised){
		m_Imp->iAsynchDataCallbackConnectionPoint->Unadvise(m_Imp->datachang_cookie);
		m_Imp->is_advised=false;
	}

	m_Imp->iAsynchDataCallbackConnectionPoint=NULL;

	int i;
	for( i=0;i<m_Adpater->m_R_OPCVector.size();i++ ){
		if( m_Adpater->m_R_OPCVector[i].is_valid == true ){
			m_Adpater->m_R_OPCVector[i].is_valid = false;
		}
	}

	m_Imp->iItemManagement=NULL;
	m_Imp->iAsych2IO=NULL;
	m_Imp->iSychIO=NULL;
	if(m_Imp->iStateManagement != NULL){
		if(m_Imp->m_server_handle != INVALID_OPC_HANDLE){
			m_Imp->iOpcServer->RemoveGroup(m_Imp->m_server_handle,FALSE);
			m_Imp->m_server_handle=INVALID_OPC_HANDLE;
		}
		m_Imp->iStateManagement=NULL;
	}
	m_Imp->iOpcServer=NULL;
	m_Imp->is_connected=false;

	HW104PKSLog(L"OPCCollector",L"Disconnect OK");
}
