#include "nworld_session.h"
#include "nclient_session.h"
#include "nmsg_dispatch.h"
#include "nnetwork_def.h"
#include "nlog_mt.h"
#include "ncommands.h"
#include "ndefine.h"

#include <iostream>

namespace nexus {

	nworld_session::nworld_session() : m_msg_queue(30000), m_cmd_queue(100)
	{
	}

	nworld_session::~nworld_session()
	{
	}

	bool nworld_session::init()
	{
		return true;
	}

	void nworld_session::destroy()
	{

	}

	void nworld_session::update(uint32 elapse)
	{
		_handle_msg();
		_handle_cmd();
		// 更新session的消息缓冲
		for (CLIENT_SESSION_MAP::const_iterator iter = m_sessions.begin(); iter != m_sessions.end(); ++iter)
		{
			iter->second->update(elapse);
		}
	}

	bool nworld_session::add_session(nclient_session* session_ptr)
	{
		return m_sessions.insert(std::make_pair(session_ptr->get_sesssion_id(), session_ptr)).second;
	}

	void nworld_session::remove_session(uint32 sesssion_id)
	{
		m_sessions.erase(sesssion_id);
	}

	nclient_session* nworld_session::get_session(uint32 sesssion_id)
	{
		CLIENT_SESSION_MAP::const_iterator iter = m_sessions.find(sesssion_id);

		if (iter == m_sessions.end())
		{
			return NULL;
		}

		return iter->second;
	}

	void nworld_session::_handle_msg()
	{
		nclient_msg_handler* handler_ptr = NULL;
		nmessage* msg_ptr = m_msg_queue.dequeue();
		nmessage* temp_msg_ptr = NULL;

		while (msg_ptr)
		{
			nclient_session* session_ptr = get_session(msg_ptr->session_id);

			if (!session_ptr)
			{
				NLOG_ERROR(_T("get client session error, session_id = %lu"), msg_ptr->session_id);
			}
			else
			{
				const nmsg_base* packet = reinterpret_cast<const nmsg_base*>(&(msg_ptr->buffer));
				handler_ptr = g_msg_dispatch.get_client_msg_handler(packet->msg_id);

				if (!handler_ptr || !handler_ptr->handler)
				{
					NLOG_ERROR(_T("get msg handler error, msg_id = %u"), packet->msg_id);
				}
				else
				{
					++handler_ptr->handle_count;
					(session_ptr->*(handler_ptr->handler))(packet);
				}
			}
			// 取出下一条消息，释放当前处理的消息
			temp_msg_ptr = msg_ptr;
			msg_ptr = msg_ptr->next_ptr;
			temp_msg_ptr->free();
		}
	}

	void nworld_session::_handle_cmd()
	{
		nmessage* msg_ptr = m_cmd_queue.dequeue();
		nmessage* temp_msg_ptr = NULL;

		while (msg_ptr)
		{
			const tagCI_Command* packet = reinterpret_cast<const tagCI_Command*>(&(msg_ptr->buffer));
			scommands.parse_commands(packet->cmd);
			std::wcout << _T("nexus>");

			// 取出下一条消息，释放当前处理的消息
			temp_msg_ptr = msg_ptr;
			msg_ptr = msg_ptr->next_ptr;
			temp_msg_ptr->free();
		}
	}

	nworld_session g_world_session;

} // namespace nexus
