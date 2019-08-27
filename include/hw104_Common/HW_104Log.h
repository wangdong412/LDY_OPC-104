//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: HW_104Log.h
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
///			V0.10 : ���� 2009-10-30 14:09 �����ļ�
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
		/// �ļ����
		HANDLE			m_hLogFile; 

		/// ��־�ļ�Ŀ¼
		WCHAR			m_wszTraceFileDirectory[  MAX_PATH_LEN  ]; 
		//��־�ļ�����
		WCHAR			m_wszLogFileName[ MAX_PATH_LEN ];    

		/// ����
		NkxLatchLock::XDBSpinLock	m_TraceLock; 

		/// <summary>
		/// 	��־�����־λ
		/// </summary>
		WORD m_bLogOut;
	};
}

	/// <summary>
	///		��־�����Ϣ��
	/// </summary>
	/// <param name="Category">
	///		��־�������
	/// </param>
	/// <param name="ClientInfor">
	///		Client����Ϣ
	/// </param>
	/// <param name="File">
	///		�ļ���
	/// </param>
	/// <param name="Line">
	///		�к�
	/// </param>
	/// <param name="Msg">
	///		��־��Ϣ
	/// </param>
	/// <remarks>
	///     ��Ҫ���ڴ�����������Ϣ
	/// </remarks>
	KTN_COMMON_STDEXT void hw104f_LogMessage(   IN KTNCOMMON::CHW104_Log&		 _TraceHelper, 
		IN const std::wstring&	 Category, 
		IN const std::wstring&	 File, 
		IN const DWORD			 Line, 
		IN std::wstring			 Msg
		 ); 

	//����������ռ����͵�buf������
	KTN_COMMON_STDEXT void hw104f_LogStream(   IN KTNCOMMON::CHW104_Log&		 _TraceHelper, 		
											   IN std::wstring					 Msg
									   		   );

	/// <summary> 
	/// ���ļ�·������ȡ�ļ���
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