//========================================================================
/// <summary>
///		�����������е�ȫ�ֱ���
/// </summary>
///	<file>
///		��Ŀ��:	104Trans
///		�ļ���: gobalDefine.cpp
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
///			V0.10 : ���� [11/1/2009] �����ļ�
///		</version>
///	</versions>
//========================================================================

#include "stdafx.h"
#include "hw104_PKSCollector/HW104Collector_ExtInc.h"
//#include "HW104PKSCollector.h"
#include "HW104ConfigAdapter.h"

/// <summary>
/// 	ȫ�ֲɼ���
/// </summary>
//CHW104PKSCollector g_PKSCollector;

/// <summary>
/// 	ȫ��104������Ϣ����
/// </summary>
CHW104ConfigAdapter g_HW104ConfigAdapter;

/// <summary>
/// 	ȫ����־
/// </summary>
CHW104_Log        g_HW104Log;

/// <summary>
///   Ĭ����ֵ
/// </summary>
KTNThresHold			  g_HW104DefaultThresHold;