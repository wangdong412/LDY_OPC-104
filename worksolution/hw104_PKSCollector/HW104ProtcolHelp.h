//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	KingModel
///		文件名: HW104ProtcolHelp.h
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
///			V0.10 : 王勃 2009-10-15 15:40 创建文件
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
		static KTN_RET GetInfoUnit( 
			const KTNIECAddress& iecAdress, 
			const KTN_VALUE& ktnValue,
			KTNHW104_InfoUnit&  infoUnit,
			bool bWithTime = false
			);

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
		static KTN_RET GetIecAddressAndKTNValue(
			const KTNHW104_InfoUnit& infoUnit,
			KTN_VALUE&			  ktnValue,
			KTNIECAddress&		  iecAddress
			);

	private:
		/// <summary>
		/// 	获得时标数据流
		/// </summary>
		/// <param name="infoUnit">
		///		in		数据流
		/// </param>
		/// <returns>
		///		返回时标的长度
		/// </returns>
		static int GetCP56Time2AStream( char* pBuf );
	};	

}




//========================================================================
#endif #define __HW104ProtcolHelp_H_INCLUDED__
