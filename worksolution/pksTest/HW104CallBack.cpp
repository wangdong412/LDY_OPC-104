#include "StdAfx.h"
#include "HW104CallBack.h"

/// <summary>
/// 	��Ϣ��Ԫ�仯�ص�����
/// </summary>
/// <param name="changeUnitArray">
///		�仯����Ϣ��Ԫ����
/// </param>
void CHW104CallBack::OnInfoUnitChange( 
							  const std::vector< KTNHW104_InfoUnit >& changeUnitArray 
							  )
{
	SYSTEMTIME t;
	GetSystemTime( &t );
	TRACE( "CallBack Start Change : %d.%d.%d \n" , t.wMinute , t.wSecond, t.wMilliseconds );
}
