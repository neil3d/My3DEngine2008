#include "nsession_mgr.h"
#include "nsession.h"

#include <iostream>

namespace nexus {

	nsession_mgr::nsession_mgr() : m_session_ptrs(NULL), m_max_session(0)
	{
	}

	nsession_mgr::~nsession_mgr()
	{
		destroy();
	}

	bool nsession_mgr::init(uint32 max_session)
	{
		if (0 == max_session)
		{
			return false;
		}

		m_max_session = max_session;

		m_session_ptrs = new nsession[m_max_session];

		if (!m_session_ptrs)
		{
			return false;
		}

		return true;
	}

	void nsession_mgr::destroy()
	{
		if (m_session_ptrs)
		{
			delete []m_session_ptrs;
			m_session_ptrs = NULL;
		}
	}

	nsession* nsession_mgr::add_session(uint32 session_id)
	{
		uint32 index = session_id & 0X0000FFFF;

		if (index >= m_max_session)
		{
			return NULL;
		}

		// 判断是否正在使用
		if (m_session_ptrs[index].is_in_used())
		{
			// 同一session的on_connect和on_disconnect底层保证串行
			std::cout << "this session id is in use!" << std::endl;
			return NULL;
		}

		m_session_ptrs[index].init(session_id);
		return &(m_session_ptrs[index]);
	}

	void nsession_mgr::remove_session(uint32 session_id)
	{
		uint32 index = session_id & 0X0000FFFF;

		if (index >= m_max_session)
		{
			return;
	}

		m_session_ptrs[index].destroy();
	}

	nsession* nsession_mgr::get_session(uint32 session_id)
	{
		uint32 index = session_id & 0X0000FFFF;

		if (index >= m_max_session)
		{
			return NULL;
		}

		// 校验id
		if (session_id != m_session_ptrs[index].get_sesssion_id())
		{
			return NULL;
		}

		return &(m_session_ptrs[index]);
	}

	bool nsession_mgr::get_broadcast_seesion_id(uint32* sessions_ptr, uint32& num)
		{
		static uint32 index = 0;

		if (index >= m_max_session)
		{
			index = 0;
		}

		num = 0;

		while (true)
		{
			if (m_session_ptrs[index].is_in_used())
			{
				sessions_ptr[num] = m_session_ptrs[index].get_sesssion_id();
				++num;

				if (num >= 12)
				{
					break;
	}
			}

			++index;
		}

		return true;
	}

	nsession_mgr g_nsession_mgr;

} // namespace nexus