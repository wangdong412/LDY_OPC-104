//========================================================================
/// <summary>
///		
/// </summary>
///	<file>
///		��Ŀ��:	KingModel
///		�ļ���: HW104CollectorCore_Datatype.h
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
///			V0.10 : ���� 2009-10-15 16:05 �����ļ�
///		</version>
///	</versions>
//========================================================================

//========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifndef __HW104CollectorCore_Datatype_H_INCLUDED__
#define __HW104CollectorCore_Datatype_H_INCLUDED__
//========================================================================

namespace NKTNCollector
{
	/// <summary>
	/// 	Ĭ����ֵ
	/// </summary>
#define DEFAULT_THRESHOLD 0.1

	/// <summary>
	/// 	ʵʱ��ռ���ڴ�����,��λ(m)
	/// </summary>
#define DEFAULT_MEMORYBUFFER		100

#define ZERO_STATUS					0
#define HSC_ERROR					-1
#define PROGNAME					"104Trans.exe"
#define DEFAULT_COLLECTINTERVAL		2				//Ĭ��ɨ�赥λ(s)
#define DEFAULT_COLLECTTIMEOUT		30				//�ɼ���ʱʱ��,��λ(s)
#define MAX_REQUEST_PACKETS			200				//�ɼ����ݰ�������

	//==============================================================================
	// 
	// ��������
	// 
	//==============================================================================
	typedef char						   KTN_CHAR;
	typedef wchar_t						   KTN_WCHAR;
	typedef unsigned char				   KTN_BYTE;
	typedef unsigned char				   KTN_OCTET;
	typedef unsigned char				   KTN_BOOLEAN;
	typedef float						   KTN_FLOAT32;
	typedef double						   KTN_FLOAT64;
	typedef signed char					   KTN_INT8;
	typedef unsigned char				   KTN_UINT8;
	typedef short						   KTN_INT16;
	typedef unsigned short				   KTN_UINT16;
	typedef long						   KTN_INT32;
	typedef unsigned long				   KTN_UINT32;
	typedef __int64						   KTN_INT64;
	typedef unsigned __int64			   KTN_UINT64;
	typedef void						   KTN_VOID;
	typedef void*						   KTN_PTR;
	typedef KTN_BYTE*					   KTN_BINARY;
	typedef KTN_CHAR*					   KTN_STR;
	typedef const KTN_CHAR*				   KTN_CSTR;
	typedef KTN_WCHAR*					   KTN_WSTR;
	typedef const KTN_WCHAR*				   KTN_CWSTR;
	typedef KTN_STR*						   KTN_STR_ARRAY;
	typedef KTN_WSTR*					   KTN_WSTR_ARRAY;	
	typedef KTN_PTR						   KTN_HANDLE;					/// ���Ӿ��
	typedef KTN_PTR						   KTN_RESULT;					/// ��ѯ���
	typedef KTN_PTR						   KTN_MULTIPLE_RESULT;			/// ������
	typedef struct KTNValue*			       KTN_VARIANT;					/// �ɱ���������

	/// <summary> 
	///	��׼�Ĳ�������
	/// </summary> 
	enum KTNBoolean
	{
		KTN_FALSE = 0,		/// ������ֵ 
		KTN_TRUE  = 1,		/// ������ֵ
	};

	/// <summary>
	///   ��ֵ����
	/// </summary>
	enum KTNThresType
	{
		KTN_PERCENTAGE = 0,		//�ٷֱ�
		KTN_ABSOLUTE   = 1		//����ֵ
	};

	/// <summary>
	///   ��ֵ
	/// </summary>
	typedef struct KTNThresHold
	{
		float thresHold;				//��ֵ
		KTNThresType thresHoldType;     //��ֵ����

		KTNThresHold():thresHold( (float)DEFAULT_THRESHOLD ),
					   thresHoldType( KTN_PERCENTAGE )
		{}
	}KTNTHRESHOLD;

	/// <summary> 
	/// ��������
	/// </summary> 
	/// <summary>
	/// 	ר����������
	/// </summary>
	typedef enum KTNValueDataType
	{
		KTNVDT_EMPTY		= 0 ,				/// û��ָ����������
		KTNVDT_BOOL			= 0x1 ,				/// ������������
		KTNVDT_I1			= 0x2 ,				/// 8λ����(�з���)
		KTNVDT_UI1			= 0x4 ,				/// 8λ����(�޷���)
		KTNVDT_I2			= 0x8 ,				/// 16λ����(�з���)
		KTNVDT_UI2			= 0x10 ,			/// 16λ����(�޷���)
		KTNVDT_I4			= 0x20	,			/// 32λ����(�з���)
		KTNVDT_UI4			= 0x40	,			///	32λ����(�޷���)
		KTNVDT_I8			= 0x80	,			/// 64λ����(�з���)
		KTNVDT_UI8			= 0x100	,			/// 64λ����(�޷���)
		KTNVDT_R4			= 0x200,			/// 32λ������
		KTNVDT_R8			= 0x400,			/// 64λ������
		KTNVDT_FILETIME		= 0x800,			/// �ļ�ʱ��ṹ����
		KTNVDT_STR			= 0x1000,		    /// ANSI�ַ���
		KTNVDT_WSTR			= 0x2000,			/// Unicode�ַ���
	} KTN_VALUE_DATA_TYPE;

	/// <summary> 
	///	�ɱ�����
	/// </summary> 
	typedef struct KTNValue
	{
	public:
		float m_ChangeCumuLant;				//�仯�ۼ���

	private:
		unsigned short				DataType;	/// ��������(KM_VALUE_DATA_TYPE)
		union
		{
			bool					bitVal;			/// ��������
			KTN_INT8					i1Val;			/// ���ֽ�����
			KTN_INT16				i2Val;			/// ˫�ֽ�����
			KTN_INT32				i4Val;			/// ���ֽ�����
			KTN_INT64				i8Val;		/// ���ֽ�����
			KTN_UINT8				ui1Val;		/// ���ֽ�����(�޷���)	
			KTN_UINT16				ui2Val;		/// ˫�ֽ�����(�޷���)
			KTN_UINT32				ui4Val;		/// ���ֽ�����(�޷���)
			KTN_UINT64				ui8Val;		/// ���ֽ�����(�޷���)
			KTN_FLOAT32				r4Val;		/// �����ȸ�����
			KTN_FLOAT64				r8Val;		/// ˫���ȸ�����
			FILETIME				ftVal;		/// FILETIME����
			KTN_STR					strVal;		/// ANSI�ַ���
			KTN_WSTR					wstrVal;	/// Unicode�ַ���	
		};//DataValue;	/// ����ֵ


		bool m_bStatus;

	public:
		SYSTEMTIME timeStamp;
		/// <summary>
		/// 	Ĭ�Ϲ��캯��
		/// </summary>
		KTNValue():m_bStatus(false),m_ChangeCumuLant(0)
		{
			Initial();
		}

		KTNValue( const KTNValue& obj )
		{
			Initial();

			m_bStatus = obj.m_bStatus;

			DataType = obj.GetDataType() ;

			m_ChangeCumuLant = obj.m_ChangeCumuLant;

			switch(DataType)
			{
			case KTNVDT_EMPTY:
				break;
			case KTNVDT_BOOL:
				bitVal = obj.asBool();
				break;
			case KTNVDT_I1:
				i1Val = obj.asInt8();
				break;
			case KTNVDT_I2:
				i2Val = obj.asInt16();
				break;
			case KTNVDT_I4:
				i4Val = obj.asInt32();
				break;
			case KTNVDT_I8:
				i8Val = obj.asInt64();
				break;
			case KTNVDT_UI1:
				ui1Val = obj.asUInt8();
				break;
			case KTNVDT_UI2:
				ui2Val = obj.asUInt16();
				break;
			case KTNVDT_UI4:
				ui4Val = obj.asUInt32();
				break;
			case KTNVDT_UI8:
				ui8Val = obj.asUInt64();
				break;
			case KTNVDT_R4:
				r4Val = obj.asFloat();
				break;
			case KTNVDT_R8:
				r8Val = obj.asDouble();
				break;
			case KTNVDT_FILETIME:
				ftVal = obj.asFileTime();
				break;
			case KTNVDT_STR:
				{
					if ( obj.asAnsiString() )
					{
						int nLen = strlen( obj.asAnsiString() ) + 1;
						strVal = new char[ nLen ];

						if ( strVal )
						{
							strcpy_s( strVal, nLen , obj.asAnsiString() );
						}
					}
				}
				break;

			case KTNVDT_WSTR:
				{
					if ( obj.asWideString() )
					{	
						int nLen = wcslen( obj.asWideString() ) + 1;
						wstrVal = new wchar_t[ nLen ];

						if ( wstrVal )
						{
							wcscpy_s( wstrVal, nLen , obj.asWideString());
						}
					}
				}
				break;

			default:
				ASSERT(false);
				break;
			}
		}

		KTNValue::~KTNValue()
		{
			Clear();
		}

		/// <summary>
		/// 	����������
		/// </summary>
		void SetQuality( bool bQuality )
		{
			m_bStatus = bQuality;
		}

		/// <summary>
		/// 	��ȡ������
		/// </summary>
		bool GetQuality() const
		{
			return m_bStatus;
		}

		/// <summary>
		/// 	��ʼ��
		/// </summary>
		void Initial()
		{
			DataType = KTNVDT_EMPTY;
			ftVal.dwHighDateTime = 0;
			ftVal.dwLowDateTime  = 0;
		}

		/************************************************************************/
		/* ��ֵ�����                                                                     
		/************************************************************************/

		KTNValue& operator=( const bool& obj )
		{
			if ( KTNVDT_BOOL != DataType )
			{
				Clear();
				DataType = KTNVDT_BOOL;
			}
			bitVal = obj;
			return *this;
		}

		KTNValue& operator=( const char& obj )
		{
			if ( KTNVDT_I1 != DataType )
			{
				Clear();
				DataType = KTNVDT_I1;
			}
			i1Val = obj;
			return *this;
		}

		KTNValue& operator=( const unsigned char& obj )
		{
			if ( KTNVDT_UI1 != DataType )
			{
				Clear();
				DataType = KTNVDT_UI1;
			}
			ui1Val = obj;
			return *this;
		}

		KTNValue& operator=( const short& obj )
		{
			if ( KTNVDT_I2 != DataType )
			{
				Clear();
				DataType = KTNVDT_I2;
			}
			i2Val = obj;
			return *this;
		}

		KTNValue& operator=( const unsigned short& obj )
		{
			if ( KTNVDT_UI2 != DataType )
			{
				Clear();
				DataType = KTNVDT_UI2;
			}
			ui2Val = obj;
			return *this;
		}

		KTNValue& operator=( const signed long& obj)
		{
			if ( KTNVDT_I4 != DataType )
			{
				Clear();
				DataType = KTNVDT_I4;
			}
			i4Val = obj;
			return *this;
		}

		KTNValue& operator=( const signed int& obj )
		{
			if ( KTNVDT_I4 != DataType )
			{
				Clear();
				DataType = KTNVDT_I4;
			}
			i4Val = obj;
			return *this;
		}

		KTNValue& operator=( const unsigned long& obj )
		{
			if ( KTNVDT_UI4 != DataType )
			{
				Clear();
				DataType = KTNVDT_UI4;
			}
			ui4Val = obj;
			return *this;
		}

		KTNValue& operator=( const unsigned int& obj )
		{
			if ( KTNVDT_UI4 != DataType )
			{
				Clear();
				DataType = KTNVDT_UI4;
			}
			ui4Val = obj;
			return *this;
		}

		KTNValue& operator=( const signed __int64& obj )
		{
			if( KTNVDT_I8 != DataType )
			{
				Clear();
				DataType = KTNVDT_I8;
			}
			i8Val = obj;
			return *this;
		}

		KTNValue& operator=( const unsigned __int64& obj )
		{	
			if ( KTNVDT_UI8 != DataType )
			{
				Clear();
				DataType = KTNVDT_UI8;
			}
			ui8Val = obj;
			return *this;
		}

		KTNValue& operator=( const float& obj )
		{
			if ( KTNVDT_R4 != DataType )
			{
				Clear();
				DataType = KTNVDT_R4;
			}
			r4Val = obj;
			return *this;
		}

		KTNValue& operator=( const double& obj )
		{
			if ( KTNVDT_R8 != DataType )
			{
				Clear();
				DataType = KTNVDT_R8;
			}
			r8Val = obj;
			return *this;
		}

		KTNValue& operator=( const char* obj )
		{
			Clear();
			DataType = KTNVDT_STR;

			ASSERT( NULL != obj );
			if ( obj != NULL )
			{
				size_t nLen = strlen( obj ) +1;
				char* p = new char[nLen];
				ASSERT(p != NULL);
				strcpy_s( p, nLen, obj );
				strVal = p;
			}

			return *this;
		}

		KTNValue& operator=( const std::string& obj )
		{
			Clear();
			DataType = KTNVDT_STR;

			size_t nLen = obj.length() +1;
			char* p = new char[nLen];
			ASSERT(p != NULL);
			strcpy_s( p, nLen, obj.c_str() );
			strVal = p;

			return *this;
		}

		KTNValue& operator=( const wchar_t* obj )
		{
			Clear();
			DataType = KTNVDT_WSTR;

			ASSERT( obj != NULL );
			if ( obj != NULL )
			{
				size_t nLen = wcslen( obj ) + 1;
				wchar_t* p = new wchar_t[ nLen ];
				ASSERT(p != NULL);
				wcscpy_s( p, nLen, obj );
				wstrVal = p;
			}
			return *this;
		}

		KTNValue& operator=( const std::wstring& obj)
		{
			Clear();
			DataType = KTNVDT_WSTR;

			size_t nLen = obj.length() + 1;
			wchar_t* p = new wchar_t[ nLen ];
			ASSERT(p != NULL);
			wcscpy_s( p, nLen, obj.c_str() );
			wstrVal = p;

			return *this;
		}

		KTNValue& operator=( const CString& obj )
		{
			Clear();
			DataType = KTNVDT_WSTR;

			size_t nLen = obj.GetLength() + 1;
			wchar_t* p = new wchar_t[ nLen ];
			ASSERT(p != NULL);
			wcscpy_s( p, nLen, obj );
			wstrVal = p;

			return *this;
		}

		KTNValue& operator=( const FILETIME& obj )
		{
			if ( KTNVDT_FILETIME != DataType )
			{
				Clear();
				DataType = KTNVDT_FILETIME;
			}

			ftVal.dwHighDateTime = obj.dwHighDateTime;
			ftVal.dwLowDateTime = obj.dwLowDateTime;
			return *this;
		}

		KTNValue& operator=( const KTNValue& obj )
		{
			if( &obj != this )
			{
				Clear();

				DataType = obj.DataType ;
				m_bStatus = obj.GetQuality();
				m_ChangeCumuLant = obj.m_ChangeCumuLant;

				switch(DataType)
				{
				case KTNVDT_EMPTY:
					break;
				case KTNVDT_BOOL:
					bitVal = obj.asBool();
					break;
				case KTNVDT_I1:
					i1Val = obj.asInt8();
					break;
				case KTNVDT_I2:
					i2Val = obj.asInt16();
					break;
				case KTNVDT_I4:
					i4Val = obj.asInt32();
					break;
				case KTNVDT_I8:
					i8Val = obj.asInt64();
					break;
				case KTNVDT_UI1:
					ui1Val = obj.asUInt8();
					break;
				case KTNVDT_UI2:
					ui2Val = obj.asUInt16();
					break;
				case KTNVDT_UI4:
					ui4Val = obj.asUInt32();
					break;
				case KTNVDT_UI8:
					ui8Val = obj.asUInt64();
					break;
				case KTNVDT_R4:
					r4Val = obj.asFloat();
					break;
				case KTNVDT_R8:
					r8Val = obj.asDouble();
					break;
				case KTNVDT_FILETIME:
					ftVal = obj.asFileTime();
					break;
				case KTNVDT_STR:
					{

						if ( obj.asAnsiString() )
						{

							int nLen = strlen( obj.asAnsiString() ) + 1;
							strVal = new char[ nLen ];

							if ( strVal )
							{
								strcpy_s( strVal, nLen , obj.asAnsiString() );
							}

						}

					}
					break;

				case KTNVDT_WSTR:
					{
						if ( obj.asWideString() )
						{	
							int nLen = wcslen( obj.asWideString() ) + 1;
							wstrVal = new wchar_t[ nLen ];

							if ( wstrVal )
							{
								wcscpy_s( wstrVal, nLen , obj.asWideString());
							}

						}
					}
					break;

				default:
					ASSERT(false);
					break;
				}
			}
			return *this;
		}

		/// <summary>
		/// 	����Լ�
		/// </summary>
		void Clear()
		{
			m_bStatus = false;

			switch(DataType)
			{
			case KTNVDT_EMPTY:
				break;
			case KTNVDT_BOOL:
				bitVal = 0;
				break;
			case KTNVDT_I1:
				i1Val = 0;
				break;
			case KTNVDT_I2:
				i2Val = 0;
				break;
			case KTNVDT_I4:
				i4Val = 0;
				break;
			case KTNVDT_I8:
				i8Val = 0;
				break;
			case KTNVDT_UI1:
				ui1Val= 0;
				break;
			case KTNVDT_UI2:
				ui2Val = 0;
				break;
			case KTNVDT_UI4:
				ui4Val = 0;
				break;
			case KTNVDT_UI8:
				ui8Val = 0;
				break;
			case KTNVDT_R4:
				r4Val = 0;
				break;
			case KTNVDT_R8:
				r8Val = 0;
				break;
			case KTNVDT_FILETIME:
				ftVal.dwHighDateTime = 0;
				ftVal.dwLowDateTime  = 0;
				break;
			case KTNVDT_STR:
				{
					if ( strVal )
					{
						delete []strVal;
					}
				}
				break;

			case KTNVDT_WSTR:
				{
					if ( wstrVal )
					{
						delete []wstrVal;
					}
				}
				break;

			default:
				break;
			}

			DataType = KTNVDT_EMPTY;
		}

		bool operator==( const KTNValue& obj ) const
		{
			if( ( DataType != obj.DataType ) || ( m_bStatus != obj.GetQuality() ) )
			{
				return false;
			}

			switch(DataType)
			{
			case KTNVDT_EMPTY:
				break;
			case KTNVDT_BOOL:
				if ( bitVal == obj.asBool() )
				{
					return true;
				}
				break;
			case KTNVDT_I1:
				if ( i1Val == obj.asInt8() )
				{
					return true;
				}
				break;
			case KTNVDT_I2:
				if ( i2Val == obj.asInt16() )
				{
					return true;
				}
				break;
			case KTNVDT_I4:
				if ( i4Val == obj.asInt32() )
				{
					return true;
				}
				break;
			case KTNVDT_I8:
				if ( i8Val == obj.asInt64() )
				{
					return true;
				}
				break;
			case KTNVDT_UI1:
				if ( obj.asUInt8() == ui1Val )
				{
					return true;
				}
				break;
			case KTNVDT_UI2:
				if ( obj.asUInt16() == ui2Val )
				{
					return true;
				}
				break;
			case KTNVDT_UI4:
				if ( obj.asUInt32() == ui4Val )
				{
					return true;
				}
				break;
			case KTNVDT_UI8:
				if ( obj.asUInt64() == ui8Val )
				{
					return true;
				}

				break;
			case KTNVDT_R4:
				{
					if ( fabs( obj.asFloat() - r4Val ) <= 0.00001 )
					{
						return true;
					}
				}
				
				break;
			case KTNVDT_R8:
				{
					if ( fabs( obj.asDouble() - r8Val ) <= 0.00001 )
					{
						return true;
					}
				}
				
				break;
			case KTNVDT_FILETIME:
				if ( ( obj.asFileTime().dwHighDateTime == ftVal.dwHighDateTime ) &&
					( obj.asFileTime().dwLowDateTime == ftVal.dwLowDateTime ) )
				{
					return true;
				}
				break;
			case KTNVDT_STR:
				{
					if ( ( strVal ) && ( obj.asAnsiString() ) )
					{
						if( 0 == strcmp( strVal, obj.asAnsiString() ) )
						{
							return true;
						}
					}
				}
				break;

			case KTNVDT_WSTR:
				{
					if ( ( wstrVal ) && ( obj.asWideString() ) )
					{
						if ( 0 == wcscmp( wstrVal, obj.asWideString() ) )
						{
							return true;
						}
					}
				}
				break;

			default:
				break;
			}

			return false;
		}

		bool operator!=( const KTNValue& obj ) const
		{
			return !( *this == obj );
		}

		/************************************************************************/
		/* ��ȡ�����                                                                     
		/************************************************************************/

		unsigned short GetDataType() const
		{
			return DataType;
		}

		bool asBool() const
		{
			ASSERT( DataType == KTNVDT_BOOL);
			return bitVal;
		}

		signed char asInt8() const
		{
			ASSERT( KTNVDT_I1 == DataType );
			return i1Val;
		}

		unsigned char asUInt8() const
		{
			ASSERT( KTNVDT_UI1 == DataType );
			return ui1Val;
		}

		short asInt16() const
		{
			ASSERT( KTNVDT_I2 == DataType );
			return i2Val;
		}

		unsigned short asUInt16() const
		{
			ASSERT( KTNVDT_UI2 == DataType );
			return ui2Val;
		}

		long asInt32() const
		{
			ASSERT( KTNVDT_I4 == DataType );
			return i4Val;
		}

		unsigned long asUInt32() const
		{
			ASSERT( KTNVDT_UI4 == DataType );
			return ui4Val;
		}

		signed __int64 asInt64() const
		{
			ASSERT( KTNVDT_I8 == DataType );
			return i8Val;
		}

		unsigned __int64 asUInt64() const
		{
			ASSERT( KTNVDT_UI8 == DataType );
			return ui8Val;
		}

		float asFloat() const
		{
			ASSERT( KTNVDT_R4 == DataType );
			return r4Val;
		}

		double asDouble() const
		{
			ASSERT( KTNVDT_R8 == DataType );
			return r8Val;
		}

		const char* asAnsiString() const
		{
			ASSERT( KTNVDT_STR == DataType );
			return strVal;
		}

		const wchar_t* asWideString() const
		{
			ASSERT( KTNVDT_WSTR == DataType );
			return wstrVal;
		}

		FILETIME asFileTime() const
		{
			ASSERT( KTNVDT_FILETIME == DataType );
			return ftVal;
		}

	} KTN_VALUE , *PKTN_VALUE;

	struct KTNPKSPoint
	{
		std::wstring		point;			//������
		bool				is_valid;
		KTNThresHold		thresHold;			//��ֵ
		DWORD				handle;
		KTNValue			val;
		int					type;
		int					cookie;


		KTNPKSPoint():is_valid(false),handle(-1),cookie(-1)
		{}

		bool operator<( const KTNPKSPoint& obj ) const
		{
			return point < obj.point;
		}

		bool operator==( const KTNPKSPoint& obj ) const
		{
			return (  point == obj.point  );
		}
	};

	struct KTNIECAddress
	{
		byte pos1;		//��һ���ֽ�
		byte pos2;      //�ڶ����ֽ�
		byte pos3;		//�������ֽ�

		KTNASDUType asduType;	//ASDU���ͱ�ʶ

		KTNIECAddress():pos1(0),pos2(0),pos3(0),asduType(MC_UNKNOWN)
		{
		}

		/// <summary>
		/// 	�Ƚ������
		/// </summary>
		bool operator<( const KTNIECAddress& obj ) const
		{
			int nTemp1 = 0,nTemp2 = 0;

			nTemp1 = pos1 + pos2*0xff + pos3*0xffff;
			nTemp2 = obj.pos1 + obj.pos2*0xff + obj.pos3*0xffff;

			return (nTemp1 < nTemp2);
		}

		bool operator==( const KTNIECAddress& obj ) const
		{
			return ( ( asduType == obj.asduType ) && ( pos1 == obj.pos1 )
				&& ( pos2 == obj.pos2 ) && ( pos3 == obj.pos3 ) );
		}

		
	};
}


//========================================================================
#endif #define __HW104CollectorCore_Datatype_H_INCLUDED__