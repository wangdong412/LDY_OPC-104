//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: HW104ProtcolHelp.h
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
///			V0.10 : ���� 2009-10-15 15:40 �����ļ�
///		</version>
///	</versions>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "hw104_PKSCollector/HW104Collector_ExtInc.h"
#include "HW104CollectorCore_Datatype.h"

#ifndef __HW104ProtcolHelp_H_INCLUDED__
#define __HW104ProtcolHelp_H_INCLUDED__
//========================================================================

namespace NKTNCollector
{

	class CHW104ProtcolHelp
	{
	public:
		CHW104ProtcolHelp(void);
		~CHW104ProtcolHelp(void);

	public:
		/// <summary>
		/// 	������Ϣ����
		/// </summary>
		/// <param name="iecAdress">
		///		��Ϣ�����ַ
		/// </param>
		/// <param name="ktnValue">
		///		��Ϣ������ֵ
		/// </param>
		/// <returns>
		///		�ɹ�����KTN_OK,ʧ�ܷ��ش�����
		/// </returns>
		static KTN_RET GetInfoUnit( 
			const KTNIECAddress& iecAdress, 
			const KTN_VALUE& ktnValue,
			KTNHW104_InfoUnit&  infoUnit,
			bool bWithTime = false
			);

		/// <summary>
		/// 	�������������
		/// </summary>
		/// <param name="infoUnit">
		///		in		������
		/// </param>
		/// <param name="ktnValue">
		///		out		��ֵ
		/// </param>
		/// <param name="iecAddress">
		///		out		iec��ַ
		/// </param>
		/// <returns>
		///		�ɹ�����KTN_OK,ʧ�ܷ��ش�����
		/// </returns>
		static KTN_RET GetIecAddressAndKTNValue(
			const KTNHW104_InfoUnit& infoUnit,
			KTN_VALUE&			  ktnValue,
			KTNIECAddress&		  iecAddress
			);

	private:
		/// <summary>
		/// 	���ʱ��������
		/// </summary>
		/// <param name="infoUnit">
		///		in		������
		/// </param>
		/// <returns>
		///		����ʱ��ĳ���
		/// </returns>
		static int GetCP56Time2AStream( char* pBuf );
	};	

}




//========================================================================
#endif #define __HW104ProtcolHelp_H_INCLUDED__
