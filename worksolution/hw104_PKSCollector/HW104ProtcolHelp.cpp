//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	KingModel
///		文件名: HW104ProtcolHelp.cpp
///		作  者: 王勃
///	</file>
///	<copyright>
/// 	本代码所有版权为所有，任何个人或者组织不得以任何方式复制、转载以挪作
/// 他用，否则将构成对的产权知识侵权。
///	
///		
///	</copyright>
///	<versions>
///		<version>
///			V0.10 : 王勃 2009-10-15 15:41 创建文件
///		</version>
///	</versions>
//========================================================================
#include "StdAfx.h"
#include "HW104ProtcolHelp.h"

#pragma warning(disable:4244;disable:4309)

/************************************************************************/
/* ASDU与数据类型的映射表                  
/* 索引为 ASDU单元标示
/************************************************************************/
extern int g_asduKTNValueTable[200];

CHW104ProtcolHelp::CHW104ProtcolHelp(void)
{
}

CHW104ProtcolHelp::~CHW104ProtcolHelp(void)
{
}

/// <summary>
/// 	处理信息对象
/// </summary>
/// <param name="iecAdress">
///		信息对象地址
/// </param>
/// <param name="ktnValue">
///		信息对象数值
/// </param>
/// <returns>
///		成功返回KTN_OK,失败返回错误码
/// </returns>
KTN_RET CHW104ProtcolHelp::GetInfoUnit( 
						   const KTNIECAddress& iecAdress, 
						   const KTN_VALUE& ktnValue,
						   KTNHW104_InfoUnit&  infoUnit,
						   bool bWithTime
						   )
{
	infoUnit.asduType = iecAdress.asduType;

	if ( KTNVDT_EMPTY == ktnValue.GetDataType() )
	{
		return KTN_FAILED;
	}

	switch ( iecAdress.asduType )
	{
	case M_SP_NA_1:
		{
			if ( false == bWithTime )
			{//不带时标的单点信息
				
				infoUnit.infoUnitStream[3] = 0;

				switch( ktnValue.GetDataType() )
				{
				case KTNVDT_I1:
				case KTNVDT_UI1:
					{
						infoUnit.infoUnitStream[3] |= ( (bool) ktnValue.asInt8() );
					}
					break;

				case KTNVDT_UI2:
				case KTNVDT_I2:
					{
						infoUnit.infoUnitStream[3] |= ( (bool) ktnValue.asInt16() );
					}
					break;
				case KTNVDT_UI4:
				case KTNVDT_I4:
					{
						infoUnit.infoUnitStream[3] |= ( (bool) ktnValue.asInt32() );						
					}
					break;

				case KTNVDT_BOOL:
					{
						infoUnit.infoUnitStream[3] |= ( (bool) ktnValue.asBool() );						
					}
					break;

				default:
					infoUnit.infoUnitStream[3] |= 0x80;			//NT = 0 IV = 1
					break;
				}

				if ( false == ktnValue.GetQuality() )
				{
					infoUnit.infoUnitStream[3] |= 0x80;				//NT = 0 IV = 1
				}

				infoUnit.streamLen = 4;
			}
			else
			{//带时标的单点信息

				infoUnit.infoUnitStream[3] = 0;

				switch( ktnValue.GetDataType() )
				{
				case KTNVDT_I1:
				case KTNVDT_UI1:
					{
						infoUnit.infoUnitStream[3] |= ( (bool) ktnValue.asInt8() );
					}
					break;

				case KTNVDT_UI2:
				case KTNVDT_I2:
					{
						infoUnit.infoUnitStream[3] |= ( (bool) ktnValue.asInt16() );
					}
					break;
				case KTNVDT_UI4:
				case KTNVDT_I4:
					{
						infoUnit.infoUnitStream[3] |= ( (bool) ktnValue.asInt32() );						
					}
					break;

				case KTNVDT_BOOL:
					{
						infoUnit.infoUnitStream[3] |= ( (bool) ktnValue.asBool() );						
					}
					break;

				default:
					infoUnit.infoUnitStream[3] |= 0x80;			//NT = 0 IV = 1
					break;
				}

				if ( false == ktnValue.GetQuality() )
				{
					infoUnit.infoUnitStream[3] |= 0x80;				//NT = 0 IV = 1
				}

				int nLen = GetCP56Time2AStream( infoUnit.infoUnitStream + 4 );
				
				infoUnit.streamLen = 4 +nLen ;
				infoUnit.asduType = M_SP_TB_1;
			}	
		}
		break;

	case M_ME_NC_1:
		{
			if ( false == bWithTime )
			{//不带时标的浮点信息

				float fTemp = 0;

				if ( KTNVDT_R8 ==  ktnValue.GetDataType() )
				{
					fTemp = ktnValue.asDouble();
				}
				else if ( KTNVDT_R4 == ktnValue.GetDataType() )
				{
					fTemp = ktnValue.asFloat();
				}
				else if ( KTNVDT_I2 == ktnValue.GetDataType() )
				{
					fTemp = ktnValue.asInt16();
				}
				else if ( KTNVDT_UI2 == ktnValue.GetDataType() )
				{
					fTemp = ktnValue.asUInt16();
				}
				else if ( KTNVDT_I4 == ktnValue.GetDataType() )
				{
					fTemp = ktnValue.asInt32();
				}
				else if ( KTNVDT_UI4 == ktnValue.GetDataType() )
				{
					fTemp = ktnValue.asUInt32();
				}
				else if ( KTNVDT_I1 == ktnValue.GetDataType() )
				{
					fTemp = ktnValue.asInt8();
				}
				else if ( KTNVDT_UI1 == ktnValue.GetDataType() )
				{
					fTemp = ktnValue.asUInt8();
				}
				else
				{//读取的数据类型不匹配
					infoUnit.infoUnitStream[ 7 ] = 0x80; // iv = 1
				}

				infoUnit.streamLen = 8;
				memcpy( infoUnit.infoUnitStream+3, &fTemp, 4);

				if ( false == ktnValue.GetQuality() )
				{
					infoUnit.infoUnitStream[ 7 ] = 0x80; // iv = 1
				}
				
			}
			else
			{//带时标的浮点信息
				float fTemp = 0;
				if ( KTNVDT_R8 ==  ktnValue.GetDataType() )
				{
					fTemp = ktnValue.asDouble();
				}
				else if ( KTNVDT_R4 == ktnValue.GetDataType() )
				{
					fTemp = ktnValue.asFloat();
				}
				else if ( KTNVDT_I2 == ktnValue.GetDataType() )
				{
					fTemp = ktnValue.asInt16();
				}
				else if ( KTNVDT_UI2 == ktnValue.GetDataType() )
				{
					fTemp = ktnValue.asUInt16();
				}
				else if ( KTNVDT_I4 == ktnValue.GetDataType() )
				{
					fTemp = ktnValue.asInt32();
				}
				else if ( KTNVDT_UI4 == ktnValue.GetDataType() )
				{
					fTemp = ktnValue.asUInt32();
				}
				else if ( KTNVDT_I1 == ktnValue.GetDataType() )
				{
					fTemp = ktnValue.asInt8();
				}
				else if ( KTNVDT_UI1 == ktnValue.GetDataType() )
				{
					fTemp = ktnValue.asUInt8();
				}
				else
				{//读取的数据类型不匹配
					infoUnit.infoUnitStream[ 7 ] = 0x80; // iv = 1
				}

				infoUnit.streamLen = 8;
				memcpy( infoUnit.infoUnitStream+3, &fTemp, 4);

				if ( false == ktnValue.GetQuality() )
				{
					infoUnit.infoUnitStream[ 7 ] = 0x80; // iv = 1
				}

				int nLen = GetCP56Time2AStream( infoUnit.infoUnitStream + 8 );

				infoUnit.streamLen = 8 +nLen ;
				infoUnit.asduType = M_ME_TF_1;
			}
		}
		break;

	default:
		return KTN_COLLECTOR_ERROR_ASDUTYPE;
	}


	infoUnit.infoUnitStream[0] = iecAdress.pos1;
	infoUnit.infoUnitStream[1] = iecAdress.pos2;
	infoUnit.infoUnitStream[2] = iecAdress.pos3;

	return KTN_OK;
}

/// <summary>
/// 	根据数据流获得
/// </summary>
/// <param name="infoUnit">
///		in		数据流
/// </param>
/// <param name="ktnValue">
///		out		数值
/// </param>
/// <param name="iecAddress">
///		out		iec地址
/// </param>
/// <returns>
///		成功返回KTN_OK,失败返回错误码
/// </returns>
KTN_RET CHW104ProtcolHelp::GetIecAddressAndKTNValue(
										const KTNHW104_InfoUnit& infoUnit,
										KTN_VALUE&			  ktnValue,
										KTNIECAddress&		  iecAddress
										)
{
	switch(infoUnit.asduType)
	{
	case C_SC_NA_1:
		{
			if ( 0x80 & infoUnit.infoUnitStream[3] )
			{
				return KTN_COLLECTROR_SELECTCOMMAND;
			}

			ktnValue = (int)( infoUnit.infoUnitStream[3] & 0x01 );
		}
		break;

	case C_SE_NC_1:
		{
			if ( 0x80 & infoUnit.infoUnitStream[7] )
			{
				return KTN_COLLECTROR_SELECTCOMMAND;
			}

			ktnValue = *(float*)( infoUnit.infoUnitStream+3 );

			iecAddress.asduType = C_SE_NC_1;
		}
		break;

	default:
		return KTN_COLLECTOR_UNSUPPORT_ASDUTYPE;
	}

	iecAddress.asduType = infoUnit.asduType;
	iecAddress.pos1 = infoUnit.infoUnitStream[0];
	iecAddress.pos2 = infoUnit.infoUnitStream[1];
	iecAddress.pos3 = infoUnit.infoUnitStream[2];

	return KTN_OK;
}

/// <summary>
/// 	获得时标数据流
/// </summary>
/// <param name="infoUnit">
///		in		数据流
/// </param>
/// <returns>
///		返回时标的长度
/// </returns>
int CHW104ProtcolHelp::GetCP56Time2AStream( char* pBuf )
{
	int nLen = 0;
	SYSTEMTIME sysCurTime;

	GetLocalTime( &sysCurTime );

	WORD nMilliSeconds = sysCurTime.wMilliseconds + sysCurTime.wSecond*1000;

	nLen = sizeof( nMilliSeconds );
	memcpy( pBuf, &nMilliSeconds, nLen );

	pBuf[ nLen++ ] = sysCurTime.wMinute;

	pBuf[ nLen++ ] = sysCurTime.wHour;

	pBuf[ nLen++]  = sysCurTime.wDay;

	pBuf[ nLen++ ] = sysCurTime.wMonth;

	pBuf[ nLen++ ] = sysCurTime.wYear%100;

	return nLen;
}