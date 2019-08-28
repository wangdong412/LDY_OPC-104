//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: HW104Collector.cpp
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
///			V0.10 : ���� 2009-10-15 15:03 �����ļ�
///		</version>
///	</versions>
//========================================================================
#include "StdAfx.h"
#include "HW104ConfigAdapter.h"
#include "HW104Collector.h"

/// <summary>
/// 	���ù�����
/// </summary>
extern NKTNCollector::CHW104ConfigAdapter g_HW104ConfigAdapter;

CHW104Collector::CHW104Collector(void)
{
}

CHW104Collector::~CHW104Collector(void)
{
}

/// <summary>
/// 	���ٲɼ���
/// </summary>
/// <returns>
///		�ɹ�����KTN_OK,ʧ�ܷ��ش�����
/// </returns>
/// <remarks>
///     �ͷŲɼ�����ռ�е���Դ,���ٲɼ���
/// </remarks>
KTN_RET	CHW104Collector::DelCollector()
{
	delete this;

	return KTN_OK;
}

/// <summary>
/// 	���ٻ����е�asdu����
/// </summary>
/// <param name="infoUnitArray">
///		���ٻ����е���Ϣ����
/// </param>
/// <returns>
///		�ɹ�����KTN_OK,ʧ�ܷ��ش�����
/// </returns>
//Ϊ��ݵ����¹���, �����������strClientIP,����IPȷ��ʹ���ĸ����. 2010-6-8,����ǿ�޸�
KTN_RET CHW104Collector::ReadAllRealData( const std::string* pStrClientIP,
						std::vector< KTNHW104_InfoUnit >& infoUnitArray
						)
{
	std::vector< KTNIECAddress > iecAddressArray;
	std::vector< KTNValue > valueArray;
	KTN_RET ret = g_HW104ConfigAdapter.ReadAllPoint( pStrClientIP, iecAddressArray, valueArray );

	if ( KTNERR(ret) )
	{
		return ret;
	}

	infoUnitArray.clear();
	infoUnitArray.reserve( iecAddressArray.size() );
	for ( unsigned int i = 0; i < iecAddressArray.size(); ++i )
	{//ת����ΪIEC��ַ
		KTNHW104_InfoUnit hw104_InfoUnit;
		KTN_RET ret = CHW104ProtcolHelp::GetInfoUnit( 
			iecAddressArray[i],
			valueArray[i],
			hw104_InfoUnit
			);

		if ( KTNERR(ret) )
		{
			continue;
		}

		infoUnitArray.push_back( hw104_InfoUnit );
	}

	return KTN_OK;
}

/// <summary>
/// 	д��ָ������ֵ
/// </summary>
/// <param name="infoUnit">
///		in asdu����
/// </param>
/// <returns>
///		�ɹ�����KTN_OK,ʧ�ܷ��ش�����
/// </returns>
KTN_RET CHW104Collector::WriteInfoUnit(
							  const std::string& ip,
							  const KTNHW104_InfoUnit& infoUnit 
							  ) 
{
	KTNValue ktnValue;
	KTNIECAddress ktnIecAddr;

	KTN_RET ret = CHW104ProtcolHelp::GetIecAddressAndKTNValue( 
					infoUnit,
					ktnValue,
					ktnIecAddr );

	if ( KTNERR(ret) )
	{
		if ( KTN_COLLECTROR_SELECTCOMMAND == ret )
		{
			return KTN_OK;
		}
		return ret;
	}

	ret = g_HW104ConfigAdapter.WriteIecData( ip,ktnIecAddr, ktnValue );

	if ( KTNERR(ret) )
	{
		return ret;
	}

	return KTN_OK;
}


/// <summary>
/// 	ע��仯���ͺ���
/// </summary>
/// <param name="pIHW104CallBack">
///		�ص��ӿ�
/// </param>
KTN_RET CHW104Collector::RegsiterCallBack( std::string strClientIP,
							  IHW104CollectorCallBack* pIHW104CallBack
							  )
{
	return 	g_HW104ConfigAdapter.RegsiterCallBack(strClientIP, pIHW104CallBack );;
}

/// <summary>
/// 	�������
/// </summary>
int CHW104Collector::GetCount()
{
	return g_HW104ConfigAdapter.GetCount();
}

KTN_RET NKTNCollector::CHW104Collector::UpdateLinkErr( KTNCollectorTpye clientType, BOOL bConnect )
{
	return g_HW104ConfigAdapter.WriteLinkErr(clientType, bConnect);
}