//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: ICollectorCallBack.h
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
///			V0.10 : ���� 2009-10-14 21:58 �����ļ�
///		</version>
///	</versions>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __ICollectorCallBack_H_INCLUDED__
#define __ICollectorCallBack_H_INCLUDED__
//========================================================================

namespace NKTNCollector
{
	class IHW104CollectorCallBack
	{
	public:
		/// <summary>
		/// 	��Ϣ��Ԫ�仯�ص�����
		/// </summary>
		/// <param name="changeUnitArray">
		///		�仯����Ϣ��Ԫ����
		/// </param>
		virtual void OnInfoUnitChange( 
			const std::vector< KTNHW104_InfoUnit >& changeUnitArray 
			) = 0;		
	};
}

//========================================================================
#endif #define __ICollectorCallBack_H_INCLUDED__
