//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	104Trans.exe
///		文件名: DataType.h
///		作  者: shuqiang.luan
///	</file>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __DataType_H_INCLUDED__
#define __DataType_H_INCLUDED__
//========================================================================

//接收、发送buf的长度
#define BUF_MAX_SIZE	256
#define LENGTH_IP		32

//日志文件名
#define TransLogName    L"HW104Trans.log"

//配置文件相关
#define CONFIGFILENAME   "104Trans.ini"
#define LCONFIGFILENAME   L"104Trans.ini"
#define IPLISTSECTION    "IPLIST"
#define IPNUMKEY		 "IPNum"
#define CLIENTTYPEKEY	 "clientType"
#define IPADDRESSKEY	 "ip"
#define HW104SECTION     "HW104"

//日志输出宏
#define HW104TransLog(Category, msg)	\
	HW104LOG( global_HW104Log, Category, msg )

//传送原因
enum COT
{
	Unuse = 0,          //未用
	Cycle = 1,          //周期、循环
	Back = 2,			//背景扫描
	Spont = 3,			//突发
	Init = 4,			//初始化
	Req = 5,            //请求或被请求
	Act = 6,			//激活
	ActCon = 7,			//激活确认
	Deact = 8,			//停止激活
	DeactCon = 9,		//停止激活确认
	ActTerm = 10,		//激活终止
	RetRem = 11,		//远方命令引起的返送消息
	RetLoc = 12,		//当地命令引起的返送消息
	File = 13,			//文件传输
	//
	Introgen = 20,		//响应站召唤
	Inro1 = 21,			//响应第1组召唤
	Inro2 = 22,			//响应第2组召唤
	Inro3 = 23,			//响应第3组召唤
	Inro4 = 24,			//响应第4组召唤
	Inro5 = 25,			//响应第5组召唤
	Inro6 = 26,			//响应第6组召唤
	Inro7 = 27,			//响应第7组召唤
	Inro8 = 28,			//响应第8组召唤
	Inro9 = 29,			//响应第9组召唤
	Inro10 = 30,		//响应第10组召唤
	Inro11 = 31,		//响应第11组召唤
	Inro12 = 32,		//响应第12组召唤
	Inro13 = 33,		//响应第13组召唤
	Inro14 = 34,		//响应第14组召唤
	Inro15 = 35,		//响应第15组召唤
	Inro16 = 36,		//响应第16组召唤
	Reqcogen = 37,		//响应计数量站召唤
	Reqco1 = 38,		//响应第1组计数量召唤
	Reqco2 = 39,		//响应第2组计数量召唤
	Reqco3 = 40,		//响应第3组计数量召唤
	Reqco4 = 41,		//响应第4组计数量召唤
	//
	UnknownTYP = 44,	//未知的类型标识
	UnknownCOT = 45,	//未知的传送原因
	UnknownCA = 46,     //未知的应用服务数据单元公共地址
	UnknownIOA = 47,	//未知的信息对象地址
};

//帧的格式类型
enum FRAMETYPE
{
	I_TYPE = 1,
	S_TYPE = 2,
	U_TYPE = 3,
};

//U格式帧
enum U_CODE
{
	StartAct = 0x07,
	StartCon = 0x0B,
	StopAct = 0x13,
	StopCon = 0x23,
	TestAct = 0x43,
	TestCon = 0x83,
};

//timeout
enum HW104_APCICODE
{
	T0Out = 0,
	T1Out = 1,
	T2Out = 2,
	T3Out = 3,
	NoInitial = 4,
	SeqError = 5,
	OtherError = 6,
	Normal = 7,
};

//========================================================================
#endif #define __DataType_H_INCLUDED__
