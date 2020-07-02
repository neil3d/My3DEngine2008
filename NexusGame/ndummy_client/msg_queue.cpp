#include "StdAfx.h"
#include "msg_queue.h"
#include "client_define.h"

msg_queue::msg_queue() : m_max_size(1000), m_size(0), m_head_ptr(NULL), m_tail_ptr(NULL)
{
}

msg_queue::~msg_queue()
{
	clear();
}

bool msg_queue::enqueue(const void* data_ptr, uint32 size)
{
	client_msg* msg_ptr = reinterpret_cast<client_msg*>(m_msg_pool.alloc(sizeof(client_msg) + size));
	
	if (!msg_ptr)
	{
		return false;
	}

	msg_ptr->pool_ptr	= &m_msg_pool;
	msg_ptr->next_ptr	= NULL;
	msg_ptr->size		= size;
	// 拷贝数据，注意起始地址
	memcpy(&(msg_ptr->data), data_ptr, size);

	nguard<nfast_recursive_mutex> guard(m_mutex);

	if (m_size >= m_max_size)
	{
		return false;
	}

	// 加入队列
	if (!m_tail_ptr)
	{
		m_head_ptr = m_tail_ptr = msg_ptr;
	}
	else
	{
		m_tail_ptr->next_ptr = msg_ptr;
		m_tail_ptr = msg_ptr;
	}

	++m_size;
	return true;
}

client_msg* msg_queue::dequeue()
{
	nguard<nfast_recursive_mutex> guard(m_mutex);

	if (!m_head_ptr)
	{
		return NULL;
	}

	client_msg* msg_ptr = m_head_ptr;
	m_head_ptr = NULL;
	m_tail_ptr = NULL;
	m_size	   = 0;
	return msg_ptr;
}

void msg_queue::clear()
{
	client_msg* msg_ptr = NULL;
	nguard<nfast_recursive_mutex> guard(m_mutex);

	while (m_head_ptr)
	{
		msg_ptr = m_head_ptr;
		m_head_ptr = m_head_ptr->next_ptr;

		// 释放msg
		msg_ptr->pool_ptr->free(msg_ptr);
	}

	m_head_ptr = NULL;
	m_tail_ptr = NULL;
}
