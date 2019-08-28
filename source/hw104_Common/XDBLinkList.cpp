//==============================================================================
//
// 项目名 ：工业实时数据库
// 文件名 ：XDBLinkList.cpp
// 作  者 ：段利军( Jack )
// 用  途 ：双向链表实现类。
// 
//==============================================================================
// 版本记录	
//==============================================================================
//
// V0.1	- 段利军 2005-3-4 22:31:51
//	
// 
//==============================================================================
#include "stdafx.h" 
#include "hw104_Common/hw104Common_Ext_inc.h"
//==============================================================================

/// <summary> 
///	将数据项自身加入到链表中。	
/// </summary> 
/// <param name="PrevLink">
///		在该数据项后插入本数据项。
/// </param>
void XDBLink::attach( XDBLink* PrevLink  )
{
	// 当前数据项应该不位于任何链表中
	KXASSERT( m_prev == this );
	KXASSERT( m_next == this ); 
	KXASSERT( m_list == NULL );

	// 修改节点自身
	m_list = PrevLink->m_list;
	m_next = PrevLink->m_next;
	m_prev = PrevLink;

	// 修改前一节点
	PrevLink->m_next = this;

	// 修改后一节点
	m_next->m_prev = this;

	// 增加计数
	m_list->m_size++;
}

/// <summary> 
///	将数据项从链表中删除。	
/// </summary> 
/// <returns>
///		返回被删除的数据项自身。
/// </returns>
XDBLink* XDBLink::detach( )
{
	// 检查数据项是否确实位于某个链表中
	if( m_next != this )
	{
		KXASSERT( m_prev != this );
		KXASSERT( m_list != NULL );
		KXASSERT( m_list->m_size > 0 );
		
		// 修改前一节点
		m_prev->m_next = m_next;

		// 修改后一节点		
		m_next->m_prev = m_prev;

		// 减小链表中成员计数
		m_list->m_size--;

		// 现在链表或者为空，或者不为空
		KXASSERT( ( m_list->m_size >0 ) || 
					( m_list->m_tail.m_next == &m_list->m_tail && 
					  m_list->m_tail.m_prev == &m_list->m_tail ) );

		// 修改被删除节点的数据
		m_next = this;
		m_prev = this;
		m_list = NULL;
	}
	else
	{
		KXASSERT( m_prev == this );
		KXASSERT( m_next == this );
		KXASSERT( m_list == NULL );
	}
	return this;
}
