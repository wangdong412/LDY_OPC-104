#include "StdAfx.h"
#include "HW104CallBack.h"

/// <summary>
/// 	信息单元变化回调函数
/// </summary>
/// <param name="changeUnitArray">
///		变化的信息单元数据
/// </param>
void CHW104CallBack::OnInfoUnitChange( 
							  const std::vector< KTNHW104_InfoUnit >& changeUnitArray 
							  )
{
	SYSTEMTIME t;
	GetSystemTime( &t );
	TRACE( "CallBack Start Change : %d.%d.%d \n" , t.wMinute , t.wSecond, t.wMilliseconds );
}
