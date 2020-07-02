#include "nclient_session_mgr.h"
#include "nclient_session.h"
#include "ncfg.h"

#include <iostream>

namespace nexus {

	nclient_session_mgr::nclient_session_mgr() : m_session_ptrs(NULL), m_max_session(0)
	{
	}

	nclient_session_mgr::~nclient_session_mgr()
	{
		destroy();
	}

	bool nclient_session_mgr::init(uint32 max_session)
	{
		if (0 == max_session)
		{
			return false;
		}

		m_max_session = max_session;

		m_session_ptrs = new nclient_session[m_max_session];

		if (!m_session_ptrs)
		{
			return false;
		}

		return true;
	}

	void nclient_session_mgr::destroy()
	{
		if (m_session_ptrs)
		{
			delete []m_session_ptrs;
			m_session_ptrs = NULL;
		}
	}

	nclient_session* nclient_session_mgr::add_session(uint32 session_id)
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

	void nclient_session_mgr::remove_session(uint32 session_id)
	{
		uint32 index = session_id & 0X0000FFFF;

		if (index >= m_max_session)
		{
			return;
		}

		m_session_ptrs[index].destroy();
	}

	nclient_session* nclient_session_mgr::get_session(uint32 session_id)
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

	// test
	void nclient_session_mgr::send_test()
	{
		for (uint32 i = 0; i < m_max_session; ++i)
		{
			if (false == m_session_ptrs[i].is_connected())
			{
				continue;
			}

			switch (sncfg.get_uint32(ECI_MsgType))
			{
			case 0:
				{
					m_session_ptrs[i].send_move_test();
				}
				break;
			case 1:
				{
					m_session_ptrs[i].send_load_test();
				}
				break;
			case 2:
				{
					m_session_ptrs[i].send_cmd_test();
				}
				break;
			default:
				{

				}
				break;
			}
		}
	}

	nclient_session_mgr g_nclient_session_mgr;

} // namespace nexus