//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	104Trans.exe
///		�ļ���: DataType.h
///		��  ��: shuqiang.luan
///	</file>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __DataType_H_INCLUDED__
#define __DataType_H_INCLUDED__
//========================================================================

//���ա�����buf�ĳ���
#define BUF_MAX_SIZE	256
#define LENGTH_IP		32

//��־�ļ���
#define TransLogName    L"HW104Trans.log"

//�����ļ����
#define CONFIGFILENAME   "104Trans.ini"
#define LCONFIGFILENAME   L"104Trans.ini"
#define IPLISTSECTION    "IPLIST"
#define IPNUMKEY		 "IPNum"
#define CLIENTTYPEKEY	 "clientType"
#define IPADDRESSKEY	 "ip"
#define HW104SECTION     "HW104"

//��־�����
#define HW104TransLog(Category, msg)	\
	HW104LOG( global_HW104Log, Category, msg )

//����ԭ��
enum COT
{
	Unuse = 0,          //δ��
	Cycle = 1,          //���ڡ�ѭ��
	Back = 2,			//����ɨ��
	Spont = 3,			//ͻ��
	Init = 4,			//��ʼ��
	Req = 5,            //���������
	Act = 6,			//����
	ActCon = 7,			//����ȷ��
	Deact = 8,			//ֹͣ����
	DeactCon = 9,		//ֹͣ����ȷ��
	ActTerm = 10,		//������ֹ
	RetRem = 11,		//Զ����������ķ�����Ϣ
	RetLoc = 12,		//������������ķ�����Ϣ
	File = 13,			//�ļ�����
	//
	Introgen = 20,		//��Ӧվ�ٻ�
	Inro1 = 21,			//��Ӧ��1���ٻ�
	Inro2 = 22,			//��Ӧ��2���ٻ�
	Inro3 = 23,			//��Ӧ��3���ٻ�
	Inro4 = 24,			//��Ӧ��4���ٻ�
	Inro5 = 25,			//��Ӧ��5���ٻ�
	Inro6 = 26,			//��Ӧ��6���ٻ�
	Inro7 = 27,			//��Ӧ��7���ٻ�
	Inro8 = 28,			//��Ӧ��8���ٻ�
	Inro9 = 29,			//��Ӧ��9���ٻ�
	Inro10 = 30,		//��Ӧ��10���ٻ�
	Inro11 = 31,		//��Ӧ��11���ٻ�
	Inro12 = 32,		//��Ӧ��12���ٻ�
	Inro13 = 33,		//��Ӧ��13���ٻ�
	Inro14 = 34,		//��Ӧ��14���ٻ�
	Inro15 = 35,		//��Ӧ��15���ٻ�
	Inro16 = 36,		//��Ӧ��16���ٻ�
	Reqcogen = 37,		//��Ӧ������վ�ٻ�
	Reqco1 = 38,		//��Ӧ��1��������ٻ�
	Reqco2 = 39,		//��Ӧ��2��������ٻ�
	Reqco3 = 40,		//��Ӧ��3��������ٻ�
	Reqco4 = 41,		//��Ӧ��4��������ٻ�
	//
	UnknownTYP = 44,	//δ֪�����ͱ�ʶ
	UnknownCOT = 45,	//δ֪�Ĵ���ԭ��
	UnknownCA = 46,     //δ֪��Ӧ�÷������ݵ�Ԫ������ַ
	UnknownIOA = 47,	//δ֪����Ϣ�����ַ
};

//֡�ĸ�ʽ����
enum FRAMETYPE
{
	I_TYPE = 1,
	S_TYPE = 2,
	U_TYPE = 3,
};

//U��ʽ֡
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
