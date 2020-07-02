#include "nmsg_buff.h"
#include "nnetwork_def.h"
#include "nmsg_dispatch.h"

namespace nexus {

	nmsg_buff::nmsg_buff() : m_head_ptr(NULL), m_tail_ptr(NULL), m_max_size(0), m_size(0)
	{
	}

	nmsg_buff::~nmsg_buff()
	{
		clear();
	}

	bool nmsg_buff::init(uint8 max_size)
	{
		m_max_size			= max_size;
		return true;
	}

	nmessage* nmsg_buff::push_msg(nmessage* msg_ptr, bool map_task, bool& overflow)
	{
		overflow = false;

		if (0 == m_size  && !map_task)
		{
			return msg_ptr;
		}

		if (m_size >= m_max_size)
		{
			overflow = true;
			return NULL;
		}

		++m_size;
		// 加入队列,截断了
		msg_ptr->next_ptr = NULL;

		if (!m_tail_ptr)
		{
			m_head_ptr = msg_ptr;
		}
		else
		{
			m_tail_ptr->next_ptr = msg_ptr;
		}

		m_tail_ptr = msg_ptr;
		return NULL;
	}

	nmessage* nmsg_buff::pop_msg(bool map_task)
		{
		nmessage* cur_ptr = m_head_ptr;

		if (!cur_ptr)
			{
		return NULL;
	}

		const nmsg_base* packet = reinterpret_cast<const nmsg_base*>(&(cur_ptr->session_id));

		if (map_task)
		{
			if (!g_msg_dispatch.is_map_task_msg(packet->msg_id))
			{
				return NULL;
		}
		}
		else
		{
			if (g_msg_dispatch.is_map_task_msg(packet->msg_id))
			{
				return NULL;
			}
		}

		m_head_ptr = m_head_ptr->next_ptr;
		cur_ptr->next_ptr = NULL;

		if (!m_head_ptr)
		{
			m_tail_ptr = NULL;
		}

		--m_size;
		return cur_ptr;
	}

	void nmsg_buff::clear()
	{
		nmessage* msg_ptr = NULL;

		while (m_head_ptr)
		{
			msg_ptr = m_head_ptr;
			m_head_ptr = m_head_ptr->next_ptr;
			msg_ptr->free();
		}

		m_head_ptr = NULL;
		m_tail_ptr = NULL;
	}

} // namespace nexus
