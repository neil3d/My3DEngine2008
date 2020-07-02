#include "nmsg_queue.h"
#include "nnetwork_def.h"

namespace nexus {

	nmsg_queue::~nmsg_queue()
	{
		clear();
	}

	bool nmsg_queue::enqueue(nmessage* msg_ptr)
	{
		nguard<nfast_recursive_mutex> guard(m_mutex);

		if (m_size >= m_max_size)
		{
			return false;
		}

		msg_ptr->next_ptr = NULL;

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

	nmessage* nmsg_queue::dequeue()
	{
		nguard<nfast_recursive_mutex> guard(m_mutex);

		if (!m_head_ptr)
		{
			return NULL;
		}

		nmessage* msg_ptr = m_head_ptr;
		m_head_ptr = NULL;
		m_tail_ptr = NULL;
		m_size	   = 0;
		return msg_ptr;
	}

	void nmsg_queue::clear()
	{
		nmessage* msg_ptr = NULL;
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

} // namespace nexus
