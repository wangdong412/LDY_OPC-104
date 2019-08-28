//==============================================================================
//
// 项目名 ：工业实时数据库
// 文件名 ：XDBLinkList.h
// 作  者 ：段利军( Jack )
// 用  途 ：用于不需动态分配内存的双向链表。
// 
//==============================================================================
// 版本记录	
//==============================================================================
//
// V0.1	- 段利军 2005-3-4 22:11:14
//	
// 
//==============================================================================
#ifndef __XDBLINKLIST__H__INCLUDED__
#define __XDBLINKLIST__H__INCLUDED__
//==============================================================================
namespace NkxLatchLock {


// 前置声明
class XDBLink;
class XDBLinkListBase;
template< class T> class XDBLinkListIterator;
template< class T> class XDBLinkListConstIterator;

// 辅助宏
#define LINK_LIST_ARG( Class ,Member )			offsetof( Class, Member )
#define KEYED_LIST_ARG( Class ,Key,LinkItem )	LINK_LIST_ARG( Class,Key ),LINK_LIST_ARG( Class,LinkItem )


/// <summary> 
///	 用于链表中的项。
/// </summary> 
class XDBLink
{
public:
	friend class XDBLinkListBase;

	/// <summary> 
	/// 构造函数。
	/// </summary>
	XDBLink( )
	{
		m_list = NULL;
		m_prev = this;
		m_next = this;
	}

	/// <summary> 
	/// 析构函数。
	/// </summary> 
	~XDBLink( )
	{
//		ASSERT( m_prev == this );
//		ASSERT( m_next == this );
	}

	/// <summary> 
	/// 返回该项所属的链表。
	/// </summary> 
	XDBLinkListBase* getList( ) const
	{
		return m_list;
	}

	/// <summary> 
	/// 返回在双向链表中的前一项。
	/// </summary> 
	XDBLink* prev( ) const
	{
		return m_prev;
	}

	/// <summary> 
	/// 返回在双向链表中的后一项。
	/// </summary> 
	XDBLink* next( ) const
	{
		return m_next;
	}

	/// <summary> 
	/// 将该项加入到链表中。
	/// </summary> 
	void attach( XDBLink* PrevLink );

	/// <summary> 
	/// 将该项从链表中删除。
	/// </summary> 
	XDBLink* detach( );

	/// <summary> 
	/// 是否位于某个链表中。
	/// </summary> 
	bool inList( ) const
	{
		return ( m_list != NULL );
	}

private:
	XDBLinkListBase*	m_list;	// 链表指针
	XDBLink*			m_prev;	// 前项指针
	XDBLink*			m_next;	// 后项指针

private:
	// disable copy constructor
	XDBLink( const XDBLink& );
	XDBLink& operator=( const XDBLink& );
};

/// <summary> 
/// 双向链表基类。
/// </summary> 
class XDBLinkListBase
{
public:
	friend class XDBLink;

	/// <summary> 
	/// 检测该链表是否为空。
	/// </summary> 
	bool empty( ) const
	{
		return ( m_size == 0 );
	}

	/// <summary> 
	///	 返回链表中项的数目。
	/// </summary> 
	size_t size( ) const
	{
		return m_size;
	}

protected:

	/// <summary> 
	/// 构造函数。
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
		/// 构造函数
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
		/// 析构函数。
		/// </summary> 
		virtual ~XDBLinkListBase( )
		{
			ASSERT( m_size == 0 );
		}

		/// <summary> 
		/// 设置链接项在对象内存排列中的偏移位置。
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
	size_t		m_size;		// 链表中项的个数
	ptrdiff_t	m_offset;	// 链接项成员在对象内的偏移位置
	XDBLink		m_tail;		// 链表的尾指针

private:
	// disable copy constructor
	XDBLinkListBase( const XDBLinkListBase& );
	XDBLinkListBase& operator=( const XDBLinkListBase& );
};


/// <summary> 
/// 双向链表模板类。
/// </summary> 
template < class T>
class XDBLinkList: public XDBLinkListBase
{
public:
	friend class XDBLinkListIterator<T>;

	/// <summary> 
	/// 构造函数。
	/// </summary> 
	XDBLinkList( )
	{ 	
	}

	/// <summary> 
	/// 构造函数。
	/// </summary> 
	XDBLinkList( ptrdiff_t Offset )
		: XDBLinkListBase( Offset )
	{
		ASSERT( sizeof( XDBLink )+Offset <= sizeof( T ) );
	}

	/// <summary> 
	/// 析构函数。
	/// </summary> 
	virtual ~XDBLinkList( )
	{
		while( m_size > 0 )
		{
			pop( );
		}
	}

	/// <summary> 
	/// 设置链接项的偏移位置。
	/// </summary> 
	void setOffset( ptrdiff_t Offset )
	{
		XDBLinkListBase::setOffset( Offset );
	}

	/// <summary> 
	/// 将数据项插入链表头部。
	/// </summary> 
	XDBLinkList<T>& push( T* t )
	{
		getLink( t )->attach( &m_tail );
		return *this;
	}

	/// <summary> 
	/// 将数据添加到链表尾部。
	/// </summary> 
	XDBLinkList<T>& append( T* t )
	{
		getLink( t )->attach( m_tail.prev( ) );
		return *this;
	}

	/// <summary> 
	/// 从链表头部取数据项。
	/// </summary> 
	T* pop( )
	{
		return ( m_size >0 ) ? getBase( m_tail.next( )->detach( ) ) : NULL;
	}

	/// <summary> 
	/// 从链表尾部取数据项。
	/// </summary> 
	T* chop( )
	{
		return ( m_size >0 ) ? getBase( m_tail.prev( )->detach( ) ) : NULL;

	}

	/// <summary> 
	/// 返回链表头部的数据项（ 不从链表中删除）。
	/// </summary> 
	T* top( )
	{
		return ( m_size >0 ) ? getBase( m_tail.next( ) ) : NULL;
	}

	/// <summary> 
	/// 返回链表尾部的数据项（ 不从链表中删除）。
	/// </summary> 
	T* bottom( )
	{
		return ( m_size >0 ) ? getBase( m_tail.prev( ) ) : NULL;
	}

protected:

	/// <summary> 
	/// 返回对象中的链接数据项。
	/// </summary> 
	XDBLink* getLink( T* t )
	{
		ASSERT( t != NULL );
		return ( XDBLink* )( ( unsigned char* )t+m_offset );
	}

	/// <summary> 
	/// 返回对象中的链接数据项。
	/// </summary> 
	const XDBLink* getLink( const T* t ) const
	{
		ASSERT( t != NULL );
		return ( XDBLink* )( ( unsigned char* )t+m_offset );
	}

	/// <summary> 
	/// 返回对象本身。
	/// </summary> 
	T* getBase( XDBLink* p )
	{
		ASSERT( p != NULL );
		return ( T* )( ( unsigned char* )p - m_offset );
	}

	/// <summary> 
	/// 返回对象本身。
	/// </summary> 
	const T* getBase( const XDBLink* p )const
	{
		ASSERT( p != NULL );
		return ( T* )( ( unsigned char* )p - m_offset );
	}
};


/// <summary> 
/// 链表迭代器。
/// </summary> 
template< class T>
class XDBLinkListIterator
{
public:
	/// <summary> 
	/// 默认构造函数。
	/// </summary> 
	XDBLinkListIterator( ):
		m_list( NULL ),
		m_next( NULL ),
		m_current( NULL )
	  {

	  }

	  /// <summary> 
	  /// 构造函数。
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
	  /// 析构函数。
	  /// </summary> 
	  virtual ~XDBLinkListIterator( )
	  {

	  }

	  /// <summary> 
	  /// 重新设置链表参数。
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
	  /// 获得链表的下一数据对象。
	  /// </summary> 
	  T* next( )
	  {
		  if( m_next != NULL )
		  {
			  // 检查迭代器当前位置的数据成员
			  if( m_next == &m_list->m_tail )
				  m_current = NULL;
			  else
				  m_current = m_list->getBase( m_next );

			  // 更新迭代器位置
			  if( m_backward )
				  m_next = m_next->prev( );
			  else
				  m_next = m_next->next( );

		  }
		  return m_current;
	  }

	  /// <summary> 
	  /// 返回当前的数据项。
	  /// </summary> 
	  /// <remark>
	  ///     必须至少调用一次next操作之后，m_current成员才有效。
	  /// </remark>
	  T* current( ) const
	  {
		  return m_current;
	  }

private:
	XDBLinkList<T>*	m_list;		// 链表指针
	XDBLink*		m_next;		// 下一数据项
	T*				m_current;	// 当前数据项
	bool			m_backward;	// 迭代器的前进方向

private:
	// disable copy	constructor
	XDBLinkListIterator( const XDBLinkListIterator& );
	XDBLinkListIterator& operator=( const XDBLinkListIterator& );
};


/// <summary> 
/// 常量链表迭代器。
/// </summary> 
template< class T>
class XDBLinkListConstIterator: public XDBLinkListIterator<T>
{
public:
	/// <summary> 
	/// 构造函数。
	/// </summary> 
	XDBLinkListConstIterator( )
	{
	}

	/// <summary> 
	/// 构造函数。
	/// </summary> 
	XDBLinkListConstIterator( const XDBLinkList<T>& List )
		: XDBLinkList<T>( const_cast< XDBLinkList<T>& >( List ) )
	{
	}

	/// <summary> 
	/// 析构函数。
	/// </summary> 
	virtual ~XDBLinkListConstIterator( )
	{
	}

	/// <summary> 
	/// 重新设置迭代器参数。
	/// </summary> 
	void reset( const XDBLinkList<T>& List )
	{
		XDBLinkList<T>::reset( const_cast< XDBLinkList<T> >( List ) );
	}

	/// <summary> 
	/// 取得下一数据项。
	/// </summary> 
	const T* next( )
	{
		return XDBLinkList<T>::next( );
	}

	/// <summary> 
	/// 返回当前数据项。
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
/// 按键组织的链表。
/// </summary> 
template <class T, class K>
class XDBKeyedList : public XDBLinkList<T>
{
public:

	/// <summary> 
	/// 返回第一个数据项。
	/// </summary> 
	T* first( )
	{ 
		return XDBLinkList<T>::top( );
	}

	/// <summary> 
	/// 返回最后一个数据项。
	/// </summary> 
	T* last( )
	{ 
		return XDBLinkList<T>::bottom( );
	}

	/// <summary> 
	/// 根据键值查找数据项。
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
	/// 构造函数。
	/// </summary> 
	XDBKeyedList( ):
		m_keyOffset( 0 )
	{
	}

	/// <summary> 
	/// 构造函数。
	/// </summary> 
	XDBKeyedList( ptrdiff_t KeyOffset, ptrdiff_t LinkOffset ):
		XDBLinkList<T>( LinkOffset ), 
		m_keyOffset( KeyOffset )
	{
		ASSERT( KeyOffset + sizeof( K ) <= sizeof( T ) );
	}

	/// <summary> 
	/// 析构函数。
	/// </summary> 
	virtual ~XDBKeyedList( )
	{
	}

	/// <summary> 
	/// 设置键和链接项在原数据对象中的偏移位置。
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
	/// 根据数据对象，获得其键值。
	/// </summary> 
	const K& getKey( const T& t )
	{
		return *( K* ) ( ( ( const char* )&t ) + m_keyOffset );
	}


private:
	ptrdiff_t m_keyOffset; // 键值的偏移位置

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
/// 按键值的升序排列的双向链表。
/// </summary> 
template <class T, class K>
class XDBAscendList : public XDBKeyedList<T, K>
{
public:
	/// <summary> 
	/// 构造函数。
	/// </summary> 
	XDBAscendList( ptrdiff_t KeyOffset, ptrdiff_t LinkOffset ):
	  XDBKeyedList<T, K>( KeyOffset, LinkOffset )
	{
	}

	  /// <summary> 
	  /// 析构函数。
	  /// </summary> 
	  virtual ~XDBAscendList( )
	  {
	  }

	  /// <summary> 
	  /// 按键值搜索。
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
	  /// 按顺序加入数据对象。
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
/// 按键值降序排列的链表。
/// </summary> 
template <class T, class K>
class XDBDescendList : public XDBKeyedList<T, K>
{
public:
	/// <summary> 
	/// 构造函数。
	/// </summary> 
	XDBDescendList( 
		ptrdiff_t KeyOffset,
		ptrdiff_t LinkOffset ):
	XDBKeyedList<T, K>( KeyOffset, LinkOffset )
	{
	}

	/// <summary> 
	/// 析构函数。
	/// </summary> 
	virtual ~XDBDescendList( )
	{
	}

	/// <summary> 
	/// 搜索键值对应的项。
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
	/// 按降序排列放入的数据项。
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
