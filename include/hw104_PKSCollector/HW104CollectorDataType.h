//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: HW104CollectorDataType.h
///		��  ��: ����
///	</file>
///	<copyright>
/// 	���������а�ȨΪ���У��κθ��˻�����֯�������κη�ʽ���ơ�ת����Ų��
/// ���ã����򽫹��ɶԵĲ�Ȩ֪ʶ��Ȩ��
///	
///		
///	</copyright>
///	<versions>
///		<version>
///			V0.10 : ���� 2009-10-14 13:43 �����ļ�
///		</version>
///	</versions>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __HW104CollectorDataType_H_INCLUDED__
#define __HW104CollectorDataType_H_INCLUDED__
//========================================================================

namespace NKTNCollector
{	
	enum KTNRedunStatus
	{
		KTN_PRIMARY = 0,		//����״̬
		KTN_BACKUP	= 1			//�ӻ�״̬
	};

	enum KTNLocalStatus
	{
		KTNOffline = 0,		//������������״̬
		KTNPrimary = 1,		//����ϵͳ�б�����������״̬
		KTNBackUp  = 2		//����ϵͳ�б������ڴӻ�״̬
	};

	#define MAX_INFOSTREAM 20

	/// <summary>
	/// 	��Ϣ��Ԫ������
	/// </summary>
	struct KTNHW104_InfoUnit
	{
		KTNInfoUintType	infoType;								//��Ϣ����/��ϢԪ��
		KTNASDUType		asduType;								//ASDU���ͱ�ʶ
		char			infoUnitStream[ MAX_INFOSTREAM ];		//������
		int				streamLen;								//����������
		int				reversed1;								//����1
		int				reversed2;								//����2

		KTNHW104_InfoUnit():infoType( InfoObject ),
						 asduType( MC_UNKNOWN ),
						 reversed1( 0 ),
						 reversed2( 0 ),
						 streamLen( 0 )
		{
			ZeroMemory( infoUnitStream , MAX_INFOSTREAM );
		}

		KTNHW104_InfoUnit( const KTNHW104_InfoUnit& obj )
		{
			infoType = obj.infoType;
			asduType = obj.asduType;
			memcpy( infoUnitStream , obj.infoUnitStream, obj.streamLen );
			streamLen = obj.streamLen;
			reversed1 = obj.reversed1;
			reversed2 = obj.reversed2;
		}

		KTNHW104_InfoUnit& operator=( const KTNHW104_InfoUnit& obj )
		{
			if ( &obj != this  )
			{
				infoType = obj.infoType;
				asduType = obj.asduType;
				memcpy( infoUnitStream , obj.infoUnitStream, obj.streamLen );
				streamLen = obj.streamLen;
				reversed1 = obj.reversed1;
				reversed2 = obj.reversed2;
			}

			return *this;
		}
	};

	/// <summary>
	/// 	ASDU��
	/// </summary>
	struct HW104_ASDUGroup
	{
		unsigned int					groupID;			//���
		std::vector< KTNHW104_InfoUnit >	infoUnitArray;		//��������
		int								reversed;			//����

		HW104_ASDUGroup():groupID( INVALID_GOURPID )
		{
		}
	};

	/************************************************************************/
	/* Collector ������                                                                     
	/************************************************************************/
#define KTN_COLLECTOR_ERROR_ASDUTYPE			(KTN_HWCOLLECTOR_ERROR_START + 1)   //ASDU���ͱ�ʶ��ƥ��
#define KTN_COLLECTOR_UNSUPPORT_ASDUTYPE		(KTN_HWCOLLECTOR_ERROR_START + 2)   //��֧�ֵ�ASDU����
#define KTN_COLLECTOR_UNSUPPORT_INFOUNITTYPE	(KTN_HWCOLLECTOR_ERROR_START + 3)   //��֧�ֵ���Ϣ��Ԫ����
#define KTN_COLLECTOR_ERROR_CONTROL				(KTN_HWCOLLECTOR_ERROR_START + 4)   //�ɼ�������û������
#define KTN_COLLECTOR_ERROR_INITIAL				(KTN_HWCOLLECTOR_ERROR_START + 5)   //�ɼ���û�б���ʼ��
#define KTN_COLLECTOR_ERROR_LOADCONFIG			(KTN_HWCOLLECTOR_ERROR_START + 6)   //���������ļ�ʧ��
#define KTN_COLLECTOR_UNSUPPORT_COLLECTORTYPE   (KTN_HWCOLLECTOR_ERROR_START + 7)   //��֧�ֵĲɼ�������
#define	KTN_PKSCOLLECTOR_NOT_START				(KTN_HWCOLLECTOR_ERROR_START + 8)   //�ɼ���û������
#define KTN_COLLECTOR_UNSUPPORT_DATATYPE		(KTN_HWCOLLECTOR_ERROR_START + 9)   //��֧�ֵ���������
#define KTN_COLLECTOR_NO_IECADDRESS				(KTN_HWCOLLECTOR_ERROR_START + 9)   //û���ҵ�IEC��ַ
#define KTN_COLLECTROR_SELECTCOMMAND			( KTN_HWCOLLECTOR_ERROR_START+ 10 ) //ѡ��ָ��
#define KTN_POINT_NOT_EXIST						( KTN_HWCOLLECTOR_ERROR_START+ 11 ) //���ݿ��е㲻����
#define KTN_PARMATER_NOT_EXIST				    ( KTN_HWCOLLECTOR_ERROR_START+ 11 ) //���ݿ��в���������
	 
}

//========================================================================
#endif #define __HW104CollectorDataType_H_INCLUDED__