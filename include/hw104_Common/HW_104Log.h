//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		项目名:	KingModel
///		文件名: HW_104Log.h
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
///			V0.10 : 王勃 2009-10-30 14:09 创建文件
///		</version>
///	</versions>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __HW_104Log_H_INCLUDED__
#define __HW_104Log_H_INCLUDED__
//========================================================================

namespace KTNCOMMON
{

#define DEFAULT_TRACE_DIRECTORY					L"d:\\HW104TransLog\\"		
#define UNICODE_BOM								"\xFF\xFE"
#define MAX_TRACE_BUFFER_LEN					2048
#define MAX_TRACE_FILE_SIZE						1024*1024*20
#define MAX_PATH_LEN							256

	class  KTN_COMMON_CLASSEXT CHW104_Log
	{
	public:
		CHW104_Log( );
		~CHW104_Log( );

		void Open( const WCHAR* wszLogFileName );

		void Close( );

		void WriteLog( const wchar_t* wszLog , int nLogWLen  );

	public:
		/// 文件句柄
		HANDLE			m_hLogFile; 

		/// 日志文件目录
		WCHAR			m_wszTraceFileDirectory[  MAX_PATH_LEN  ]; 
		//日志文件名称
		WCHAR			m_wszLogFileName[ MAX_PATH_LEN ];    

		/// 加锁
		NkxLatchLock::XDBSpinLock	m_TraceLock; 

		/// <summary>
		/// 	日志输出标志位
		/// </summary>
		WORD m_bLogOut;
	};
}

	/// <summary>
	///		日志输出信息。
	/// </summary>
	/// <param name="Category">
	///		日志输出类型
	/// </param>
	/// <param name="ClientInfor">
	///		Client端信息
	/// </param>
	/// <param name="File">
	///		文件名
	/// </param>
	/// <param name="Line">
	///		行号
	/// </param>
	/// <param name="Msg">
	///		日志信息
	/// </param>
	/// <remarks>
	///     主要用于打出代码错误信息
	/// </remarks>
	KTN_COMMON_STDEXT void hw104f_LogMessage(   IN KTNCOMMON::CHW104_Log&		 _TraceHelper, 
		IN const std::wstring&	 Category, 
		IN const std::wstring&	 File, 
		IN const DWORD			 Line, 
		IN std::wstring			 Msg
		 ); 

	//用于输出接收及发送的buf的数据
	KTN_COMMON_STDEXT void hw104f_LogStream(   IN KTNCOMMON::CHW104_Log&		 _TraceHelper, 		
											   IN std::wstring					 Msg
									   		   );

	/// <summary> 
	/// 从文件路径中提取文件名
	/// </summary>
	KTN_COMMON_STDEXT const WCHAR* GetOnlyFileName( const WCHAR* FilePath  ); 

	//#define __WFILE__ WIDEN( __FILE__ )
#define __WFILEONLYNAME__	                        GetOnlyFileName( __WFILE__ )

#define HW104LOG( _TraceHelper ,  Category , msg )  \
	hw104f_LogMessage( _TraceHelper, Category,__WFILEONLYNAME__,  __LINE__,  msg ); 

#define HW104FRAMELOG( _TraceHelper, Category, msg )\
	hw104f_LogMessage( _TraceHelper, Category, L"",  L"",  msg ); 

//========================================================================
#endif #define __HW_104Log_H_INCLUDED__