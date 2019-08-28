//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: IHW104CollectorFactory.h
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
///			V0.10 : ���� 2009-10-14 13:31 �����ļ�
///		</version>
///	</versions>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __IHW104CollectorFactory_H_INCLUDED__
#define __IHW104CollectorFactory_H_INCLUDED__
//========================================================================

namespace NKTNCollector
{
	class IHW104CollectorFactory
	{
	public:
		/// <summary>
		/// 	�����ɼ�������
		/// </summary>
		/// <param name="pIHW104CollectorControl">
		///		out �ɼ�������
		/// </param>
		/// <returns>
		///		�ɹ�����KTN_OK,ʧ�ܷ��ش�����
		/// </returns>
		virtual KTN_RET	CreateIHW104CollectorControl(
			IHW104CollectorControl*& pIHW104CollectorControl
			) = 0;

		/// <summary>
		/// 	����104�ɼ���ʵ��
		/// </summary>
		/// <param name="pIHW104Collector">
		///		out �ɼ���
		/// </param>
		/// <returns>
		///		�ɹ�����KTN_OK,ʧ�ܷ��ش�����
		/// </returns>
		virtual KTN_RET CreateIHW104Collector( 
			IHW104Collector*& pIHW104Collector
			) = 0;
	};

}

//========================================================================
#endif #define __IHW104CollectorFactory_H_INCLUDED__