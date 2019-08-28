//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: HW104CollectorFactory.cpp
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
///			V0.10 : ���� 2009-10-15 10:11 �����ļ�
///		</version>
///	</versions>
//========================================================================
#include "StdAfx.h"
#include "HW104CollectorFactory.h"
#include "HW104Collector.h"
#include "HW104CollectControl.h"

CHW104CollectorFactory g_HW104CollectorFactory;

/// <summary>
/// 	ȫ�ֵ�������,�����೧
/// </summary>
IHW104CollectorFactory* ktn_GetHW104CollectorFactory()
{
	return (IHW104CollectorFactory*)(&g_HW104CollectorFactory);
}

CHW104CollectorFactory::CHW104CollectorFactory(void)
{
}

CHW104CollectorFactory::~CHW104CollectorFactory(void)
{
}

/// <summary>
/// 	�����ɼ�������
/// </summary>
/// <param name="pIHW104CollectorControl">
///		out �ɼ�������
/// </param>
/// <returns>
///		�ɹ�����KTN_OK,ʧ�ܷ��ش�����
/// </returns>
KTN_RET	CHW104CollectorFactory::CreateIHW104CollectorControl(
	IHW104CollectorControl*& pIHW104CollectorControl
	)
{
	pIHW104CollectorControl = dynamic_cast< IHW104CollectorControl* >( new CHW104CollectControl );

	ASSERT( NULL != pIHW104CollectorControl );

	if ( NULL == pIHW104CollectorControl )
	{
		return KTN_OUT_OF_MEMORY;
	}

	return KTN_OK;
}

/// <summary>
/// 	����104�ɼ���ʵ��
/// </summary>
/// <param name="collectorType">
///		in �ɼ�������
/// </param>
/// <param name="pIHW104Collector">
///		out �ɼ���
/// </param>
/// <returns>
///		�ɹ�����KTN_OK,ʧ�ܷ��ش�����
/// </returns>
KTN_RET CHW104CollectorFactory::CreateIHW104Collector( 
									  IHW104Collector*& pIHW104Collector
									  )
{
	pIHW104Collector = dynamic_cast< IHW104Collector* >( new CHW104Collector );

	ASSERT( pIHW104Collector != NULL );
	if ( NULL == pIHW104Collector )
	{
		return KTN_OUT_OF_MEMORY;
	}

	return KTN_OK;
}

