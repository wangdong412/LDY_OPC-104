//========================================================================
/// <summary>
///		用来定义所有的全局变量
/// </summary>
///	<file>
///		项目名:	104Trans
///		文件名: gobalDefine.cpp
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
///			V0.10 : 王勃 [11/1/2009] 创建文件
///		</version>
///	</versions>
//========================================================================

#include "stdafx.h"
#include "hw104_PKSCollector/HW104Collector_ExtInc.h"
//#include "HW104PKSCollector.h"
#include "HW104ConfigAdapter.h"

/// <summary>
/// 	全局采集器
/// </summary>
//CHW104PKSCollector g_PKSCollector;

/// <summary>
/// 	全局104配置信息代理
/// </summary>
CHW104ConfigAdapter g_HW104ConfigAdapter;

/// <summary>
/// 	全局日志
/// </summary>
CHW104_Log        g_HW104Log;

/// <summary>
///   默认阈值
/// </summary>
KTNThresHold			  g_HW104DefaultThresHold;