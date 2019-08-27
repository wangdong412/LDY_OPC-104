//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: IHW104CollectControl.h
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
///			V0.10 : ���� 2009-10-14 15:08 �����ļ�
///		</version>
///	</versions>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __IHW104CollectControl_H_INCLUDED__
#define __IHW104CollectControl_H_INCLUDED__
//========================================================================

namespace NKTNCollector
{
	class IHW104CollectorControl
	{
	public:
		/// <summary>
		/// 	�����ɼ�
		/// </summary>
		/// <returns>
		///		�ɹ�����KTN_OK,ʧ�ܷ��ش�����
		/// </returns>
		/// <remarks>
		///     ÿ������֮ǰ����ͨ���������֪ͨ�ɼ�����ʼ�ɼ�
		///     ֻ�������߳��н��е���
		/// </remarks>
		virtual KTN_RET	 StartCollect() = 0;

		/// <summary>
		/// 	ֹͣ�ɼ�
		/// </summary>
		/// <returns>
		///		�ɹ�����KTN_OK,ʧ�ܷ��ش�����
		/// </returns>
		/// <remarks>
		///     ÿ���˳�ʱ����ͨ���������ֹͣ�ɼ����ͷ������Դ
		/// </remarks>
		virtual KTN_RET	EndCollect() = 0;

		/// <summary>
		/// 	��ñ���������״̬
		/// </summary>
		/// <param name="redunStatus">
		///		����������״̬
		/// </param>
		virtual KTN_RET GetRedunStatus(
			KTNRedunStatus& redunStatus ) = 0;
	};
}

//========================================================================
#endif #define __IHW104CollectControl_H_INCLUDED__