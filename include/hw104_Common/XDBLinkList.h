//==============================================================================
//
// ��Ŀ�� ����ҵʵʱ���ݿ�
// �ļ��� ��XDBLinkList.h
// ��  �� ��������( Jack )
// ��  ; �����ڲ��趯̬�����ڴ��˫������
// 
//==============================================================================
// �汾��¼	
//==============================================================================
//
// V0.1	- ������ 2005-3-4 22:11:14
//	
// 
//==============================================================================
#ifndef __XDBLINKLIST__H__INCLUDED__
#define __XDBLINKLIST__H__INCLUDED__
//==============================================================================
namespace NkxLatchLock {


// ǰ������
class XDBLink;
class XDBLinkListBase;
template< class T> class XDBLinkListIterator;
template< class T> class XDBLinkListConstIterator;

// ������
#define LINK_LIST_ARG( Class ,Member )			offsetof( Class, Member )
#define KEYED_LIST_ARG( Class ,Key,LinkItem )	LINK_LIST_ARG( Class,Key ),LINK_LIST_ARG( Class,LinkItem )


/// <summary> 
///	 ���������е��
/// </summary> 
class XDBLink
{
public:
	friend class XDBLinkListBase;

	/// <summary> 
	/// ���캯����
	/// </summary>
	XDBLink( )
	{
		m_list = NULL;
		m_prev = this;
		m_next = this;
	}

	/// <summary> 
	/// ����������
	/// </summary> 
	~XDBLink( )
	{
//		ASSERT( m_prev == this );
//		ASSERT( m_next == this );
	}

	/// <summary> 
	/// ���ظ�������������
	/// </summary> 
	XDBLinkListBase* getList( ) const
	{
		return m_list;
	}

	/// <summary> 
	/// ������˫�������е�ǰһ�
	/// </summary> 
	XDBLink* prev( ) const
	{
		return m_prev;
	}

	/// <summary> 
	/// ������˫�������еĺ�һ�
	/// </summary> 
	XDBLink* next( ) const
	{
		return m_next;
	}

	/// <summary> 
	/// ��������뵽�����С�
	/// </summary> 
	void attach( XDBLink* PrevLink );

	/// <summary> 
	/// �������������ɾ����
	/// </summary> 
	XDBLink* detach( );

	/// <summary> 
	/// �Ƿ�λ��ĳ�������С�
	/// </summary> 
	bool inList( ) const
	{
		return ( m_list != NULL );
	}

private:
	XDBLinkListBase*	m_list;	// ����ָ��
	XDBLink*			m_prev;	// ǰ��ָ��
	XDBLink*			m_next;	// ����ָ��

private:
	// disable copy constructor
	XDBLink( const XDBLink& );
	XDBLink& operator=( const XDBLink& );
};

/// <summary> 
/// ˫��������ࡣ
/// </summary> 
class XDBLinkListBase
{
public:
	friend class XDBLink;

	/// <summary> 
	/// ���������Ƿ�Ϊ�ա�
	/// </summary> 
	bool empty( ) const
	{
		return ( m_size == 0 );
	}

	/// <summary> 
	///	 ���������������Ŀ��
	/// </summary> 
	size_t size( ) const
	{
		return m_size;
	}

protected:

	/// <summary> 
	/// ���캯����
	/// </summary> 
	XDBLinkListBase( ):
			m_size( 0 ),
			m_offset( -1 )
		{
			m_tail.m_list = NULL;
			ASSERT( m_tail.m_next == &m_tail );
			ASSERT( m_tail.m_prev == &m_tail );
		}

		/// <summary> 
		/// ���캯��
		/// </summary> 
		XDBLinkListBase( ptrdiff_t Offset ):
			m_size( 0 ),
			m_offset( Offset )
		{
			m_tail.m_list = this;
			ASSERT( m_tail.m_next == &m_tail );
			ASSERT( m_tail.m_prev == &m_tail );
		}

		/// <summary> 
		/// ����������
		/// </summary> 
		virtual ~XDBLinkListBase( )
		{
			ASSERT( m_size == 0 );
		}

		/// <summary> 
		/// �����������ڶ����ڴ������е�ƫ��λ�á�
		/// </summary> 
		void setOffset( ptrdiff_t Offset )
		{
			ASSERT( m_size == 0 );
			ASSERT( m_offset == -1 );
			ASSERT( m_tail.m_list == NULL );
			m_offset = Offset;
			m_tail.m_list = this;
		}

protected:
	size_t		m_size;		// ��������ĸ���
	ptrdiff_t	m_offset;	// �������Ա�ڶ����ڵ�ƫ��λ��
	XDBLink		m_tail;		// �����βָ��

private:
	// disable copy constructor
	XDBLinkListBase( const XDBLinkListBase& );
	XDBLinkListBase& operator=( const XDBLinkListBase& );
};


/// <summary> 
/// ˫������ģ���ࡣ
/// </summary> 
template < class T>
class XDBLinkList: public XDBLinkListBase
{
public:
	friend class XDBLinkListIterator<T>;

	/// <summary> 
	/// ���캯����
	/// </summary> 
	XDBLinkList( )
	{ 	
	}

	/// <summary> 
	/// ���캯����
	/// </summary> 
	XDBLinkList( ptrdiff_t Offset )
		: XDBLinkListBase( Offset )
	{
		ASSERT( sizeof( XDBLink )+Offset <= sizeof( T ) );
	}

	/// <summary> 
	/// ����������
	/// </summary> 
	virtual ~XDBLinkList( )
	{
		while( m_size > 0 )
		{
			pop( );
		}
	}

	/// <summary> 
	/// �����������ƫ��λ�á�
	/// </summary> 
	void setOffset( ptrdiff_t Offset )
	{
		XDBLinkListBase::setOffset( Offset );
	}

	/// <summary> 
	/// ���������������ͷ����
	/// </summary> 
	XDBLinkList<T>& push( T* t )
	{
		getLink( t )->attach( &m_tail );
		return *this;
	}

	/// <summary> 
	/// ��������ӵ�����β����
	/// </summary> 
	XDBLinkList<T>& append( T* t )
	{
		getLink( t )->attach( m_tail.prev( ) );
		return *this;
	}

	/// <summary> 
	/// ������ͷ��ȡ�����
	/// </summary> 
	T* pop( )
	{
		return ( m_size >0 ) ? getBase( m_tail.next( )->detach( ) ) : NULL;
	}

	/// <summary> 
	/// ������β��ȡ�����
	/// </summary> 
	T* chop( )
	{
		return ( m_size >0 ) ? getBase( m_tail.prev( )->detach( ) ) : NULL;

	}

	/// <summary> 
	/// ��������ͷ��������� ����������ɾ������
	/// </summary> 
	T* top( )
	{
		return ( m_size >0 ) ? getBase( m_tail.next( ) ) : NULL;
	}

	/// <summary> 
	/// ��������β��������� ����������ɾ������
	/// </summary> 
	T* bottom( )
	{
		return ( m_size >0 ) ? getBase( m_tail.prev( ) ) : NULL;
	}

protected:

	/// <summary> 
	/// ���ض����е����������
	/// </summary> 
	XDBLink* getLink( T* t )
	{
		ASSERT( t != NULL );
		return ( XDBLink* )( ( unsigned char* )t+m_offset );
	}

	/// <summary> 
	/// ���ض����е����������
	/// </summary> 
	const XDBLink* getLink( const T* t ) const
	{
		ASSERT( t != NULL );
		return ( XDBLink* )( ( unsigned char* )t+m_offset );
	}

	/// <summary> 
	/// ���ض�����
	/// </summary> 
	T* getBase( XDBLink* p )
	{
		ASSERT( p != NULL );
		return ( T* )( ( unsigned char* )p - m_offset );
	}

	/// <summary> 
	/// ���ض�����
	/// </summary> 
	const T* getBase( const XDBLink* p )const
	{
		ASSERT( p != NULL );
		return ( T* )( ( unsigned char* )p - m_offset );
	}
};


/// <summary> 
/// �����������
/// </summary> 
template< class T>
class XDBLinkListIterator
{
public:
	/// <summary> 
	/// Ĭ�Ϲ��캯����
	/// </summary> 
	XDBLinkListIterator( ):
		m_list( NULL ),
		m_next( NULL ),
		m_current( NULL )
	  {

	  }

	  /// <summary> 
	  /// ���캯����
	  /// </summary> 
	  XDBLinkListIterator( XDBLinkList<T>& List, bool Backward = false ):
	  m_list( &List ),
		  m_current( NULL ),
		  m_backward( Backward )
	  {
		  if( Backward )
			  m_next = List.m_tail.prev( );
		  else
			  m_next = List.m_tail.next( );
	  }

	  /// <summary> 
	  /// ����������
	  /// </summary> 
	  virtual ~XDBLinkListIterator( )
	  {

	  }

	  /// <summary> 
	  /// �����������������
	  /// </summary> 
	  void reset( XDBLinkList<T>& List, bool Backward = false )
	  {
		  m_list		= &List;
		  m_current	= NULL;
		  m_backward	= Backward;
		  if( Backward ) 
			  m_next = List.m_tail.prev( );
		  else
			  m_next = List.m_tail.next( );
	  }

	  /// <summary> 
	  /// ����������һ���ݶ���
	  /// </summary> 
	  T* next( )
	  {
		  if( m_next != NULL )
		  {
			  // ����������ǰλ�õ����ݳ�Ա
			  if( m_next == &m_list->m_tail )
				  m_current = NULL;
			  else
				  m_current = m_list->getBase( m_next );

			  // ���µ�����λ��
			  if( m_backward )
				  m_next = m_next->prev( );
			  else
				  m_next = m_next->next( );

		  }
		  return m_current;
	  }

	  /// <summary> 
	  /// ���ص�ǰ�������
	  /// </summary> 
	  /// <remark>
	  ///     �������ٵ���һ��next����֮��m_current��Ա����Ч��
	  /// </remark>
	  T* current( ) const
	  {
		  return m_current;
	  }

private:
	XDBLinkList<T>*	m_list;		// ����ָ��
	XDBLink*		m_next;		// ��һ������
	T*				m_current;	// ��ǰ������
	bool			m_backward;	// ��������ǰ������

private:
	// disable copy	constructor
	XDBLinkListIterator( const XDBLinkListIterator& );
	XDBLinkListIterator& operator=( const XDBLinkListIterator& );
};


/// <summary> 
/// ���������������
/// </summary> 
template< class T>
class XDBLinkListConstIterator: public XDBLinkListIterator<T>
{
public:
	/// <summary> 
	/// ���캯����
	/// </summary> 
	XDBLinkListConstIterator( )
	{
	}

	/// <summary> 
	/// ���캯����
	/// </summary> 
	XDBLinkListConstIterator( const XDBLinkList<T>& List )
		: XDBLinkList<T>( const_cast< XDBLinkList<T>& >( List ) )
	{
	}

	/// <summary> 
	/// ����������
	/// </summary> 
	virtual ~XDBLinkListConstIterator( )
	{
	}

	/// <summary> 
	/// �������õ�����������
	/// </summary> 
	void reset( const XDBLinkList<T>& List )
	{
		XDBLinkList<T>::reset( const_cast< XDBLinkList<T> >( List ) );
	}

	/// <summary> 
	/// ȡ����һ�����
	/// </summary> 
	const T* next( )
	{
		return XDBLinkList<T>::next( );
	}

	/// <summary> 
	/// ���ص�ǰ�����
	/// </summary> 
	const T* current( ) const
	{
		return XDBLinkList<T>::current( );
	}

private:
	// disable copy constructor
	XDBLinkListConstIterator( const XDBLinkListConstIterator& );
	XDBLinkListConstIterator& operator=( const XDBLinkListConstIterator& );
};


/// <summary> 
/// ������֯������
/// </summary> 
template <class T, class K>
class XDBKeyedList : public XDBLinkList<T>
{
public:

	/// <summary> 
	/// ���ص�һ�������
	/// </summary> 
	T* first( )
	{ 
		return XDBLinkList<T>::top( );
	}

	/// <summary> 
	/// �������һ�������
	/// </summary> 
	T* last( )
	{ 
		return XDBLinkList<T>::bottom( );
	}

	/// <summary> 
	/// ���ݼ�ֵ���������
	/// </summary> 
	virtual T* search( const K& k )
	{
		register XDBLink* p = NULL;
		for (   p = m_tail.next( );
			( p != &m_tail ) && ( ( getKey( *getBase( p ) ) != k ) );
			p = p->next( ) );
		return ( ( NULL != p )  && ( p != &m_tail ) )?getBase( p ):NULL;
	}

	/// <summary> 
	/// ���캯����
	/// </summary> 
	XDBKeyedList( ):
		m_keyOffset( 0 )
	{
	}

	/// <summary> 
	/// ���캯����
	/// </summary> 
	XDBKeyedList( ptrdiff_t KeyOffset, ptrdiff_t LinkOffset ):
		XDBLinkList<T>( LinkOffset ), 
		m_keyOffset( KeyOffset )
	{
		ASSERT( KeyOffset + sizeof( K ) <= sizeof( T ) );
	}

	/// <summary> 
	/// ����������
	/// </summary> 
	virtual ~XDBKeyedList( )
	{
	}

	/// <summary> 
	/// ���ü�����������ԭ���ݶ����е�ƫ��λ�á�
	/// </summary> 
	void setOffset( 
		ptrdiff_t KeyOffset,
		ptrdiff_t LinkOffset )
	{
		ASSERT( m_keyOffset == 0 );
		XDBLinkList<T>::setOffset( LinkOffset );
		m_keyOffset = KeyOffset;
	}

protected:
	/// <summary> 
	/// �������ݶ��󣬻�����ֵ��
	/// </summary> 
	const K& getKey( const T& t )
	{
		return *( K* ) ( ( ( const char* )&t ) + m_keyOffset );
	}


private:
	ptrdiff_t m_keyOffset; // ��ֵ��ƫ��λ��

private:
	// disabled copy constructor
	XDBKeyedList( const XDBKeyedList<T,K>& );
	XDBKeyedList& operator=( const XDBKeyedList<T,K>& );

	// disabled useless base class method
	XDBLinkList<T>&  push( T* t );
	XDBLinkList<T>&  append( T* t ) ;
	T*            chop( );
	T*            top( );
	T*            bottom( );
};

/// <summary> 
/// ����ֵ���������е�˫������
/// </summary> 
template <class T, class K>
class XDBAscendList : public XDBKeyedList<T, K>
{
public:
	/// <summary> 
	/// ���캯����
	/// </summary> 
	XDBAscendList( ptrdiff_t KeyOffset, ptrdiff_t LinkOffset ):
	  XDBKeyedList<T, K>( KeyOffset, LinkOffset )
	{
	}

	  /// <summary> 
	  /// ����������
	  /// </summary> 
	  virtual ~XDBAscendList( )
	  {
	  }

	  /// <summary> 
	  /// ����ֵ������
	  /// </summary> 
	  virtual T* search( const K& k )
	  {
		  register XDBLink* p = NULL;
		  for (  p = m_tail.next( );
			  ( p != &m_tail ) && ( getKey( *getBase( p ) ) < k );
			  p = p->next( ) );
		  return ( p != NULL ) ? getBase( p ) : NULL;
	  }

	  /// <summary> 
	  /// ��˳��������ݶ���
	  /// </summary> 
	  virtual void putInOrder( T* t )
	  {
		  register XDBLink* p = NULL;
		  for (  p = m_tail.next( );
			  (  p != &m_tail ) && ( getKey( *getBase( p ) ) <= getKey( *t ) );
			  p = p->next( ) );
		  if( p != NULL )
			  getLink( t )->attach( p->prev( ) );
		  else
			  getLink( t )->attach( m_tail.prev( ) );
	  }

private:
	// disable copy constructor
	XDBAscendList( const XDBAscendList<T,K>& ); 
	XDBAscendList& operator=( const XDBAscendList<T,K>& );
};

/// <summary> 
/// ����ֵ�������е�����
/// </summary> 
template <class T, class K>
class XDBDescendList : public XDBKeyedList<T, K>
{
public:
	/// <summary> 
	/// ���캯����
	/// </summary> 
	XDBDescendList( 
		ptrdiff_t KeyOffset,
		ptrdiff_t LinkOffset ):
	XDBKeyedList<T, K>( KeyOffset, LinkOffset )
	{
	}

	/// <summary> 
	/// ����������
	/// </summary> 
	virtual ~XDBDescendList( )
	{
	}

	/// <summary> 
	/// ������ֵ��Ӧ���
	/// </summary> 
	virtual T* search( const K& k )
	{
		register XDBLink* p = NULL;
		for( p = m_tail.next( );
			( p != &m_tail ) && ( getKey( *getBase( p ) ) > k );
			p = p->next( ) );
		return ( p != NULL )?getBase( p ) : NULL;
	}

	/// <summary> 
	/// ���������з���������
	/// </summary> 
	virtual void putInOrder( T* t )
	{
		register XDBLink* p = NULL;
		for( p = m_tail.next( );
			( p != &m_tail ) && ( getKey( *getBase( p ) ) >= getKey( *t ) );
			p = p->next( ) );
		if( p != NULL )
			getLink( t )->attach( p->prev( ) );
		else
			getLink( t )->attach( m_tail.prev( ) );
	}

private:
	// disable copy constructor
	XDBDescendList( const XDBDescendList<T,K>& );
	XDBDescendList& operator=( const XDBDescendList<T,K>& );
};


}
//==============================================================================
#endif // __XDBLINKLIST__H__INCLUDED__
//==============================================================================
