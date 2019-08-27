//==============================================================================
//
// ��Ŀ�� ����ҵʵʱ���ݿ�
// �ļ��� ��XDBLinkList.cpp
// ��  �� ��������( Jack )
// ��  ; ��˫������ʵ���ࡣ
// 
//==============================================================================
// �汾��¼	
//==============================================================================
//
// V0.1	- ������ 2005-3-4 22:31:51
//	
// 
//==============================================================================
#include "stdafx.h" 
#include "hw104_Common/hw104Common_Ext_inc.h"
//==============================================================================

/// <summary> 
///	��������������뵽�����С�	
/// </summary> 
/// <param name="PrevLink">
///		�ڸ����������뱾�����
/// </param>
void XDBLink::attach( XDBLink* PrevLink  )
{
	// ��ǰ������Ӧ�ò�λ���κ�������
	KXASSERT( m_prev == this );
	KXASSERT( m_next == this ); 
	KXASSERT( m_list == NULL );

	// �޸Ľڵ�����
	m_list = PrevLink->m_list;
	m_next = PrevLink->m_next;
	m_prev = PrevLink;

	// �޸�ǰһ�ڵ�
	PrevLink->m_next = this;

	// �޸ĺ�һ�ڵ�
	m_next->m_prev = this;

	// ���Ӽ���
	m_list->m_size++;
}

/// <summary> 
///	���������������ɾ����	
/// </summary> 
/// <returns>
///		���ر�ɾ��������������
/// </returns>
XDBLink* XDBLink::detach( )
{
	// ����������Ƿ�ȷʵλ��ĳ��������
	if( m_next != this )
	{
		KXASSERT( m_prev != this );
		KXASSERT( m_list != NULL );
		KXASSERT( m_list->m_size > 0 );
		
		// �޸�ǰһ�ڵ�
		m_prev->m_next = m_next;

		// �޸ĺ�һ�ڵ�		
		m_next->m_prev = m_prev;

		// ��С�����г�Ա����
		m_list->m_size--;

		// �����������Ϊ�գ����߲�Ϊ��
		KXASSERT( ( m_list->m_size >0 ) || 
					( m_list->m_tail.m_next == &m_list->m_tail && 
					  m_list->m_tail.m_prev == &m_list->m_tail ) );

		// �޸ı�ɾ���ڵ������
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
