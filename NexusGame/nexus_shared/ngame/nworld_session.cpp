#include "nworld_session.h"
#include "nclient_session.h"
#include "nmsg_dispatch.h"
#include "nnetwork_def.h"
#include "nenums.h"
#include "nlog_mt.h"

#include "nmsg_gm_command.h"

namespace nexus {

	nworld_session::nworld_session() : m_msg_queue(30000)
	{
	}

	nworld_session::~nworld_session()
	{
	}

	bool nworld_session::init()
	{
		return true;
	}

	// destroy 要进行该类所有数据的复原，因为要归还到池里
	void nworld_session::destroy()
	{

	}

	void nworld_session::update(uint32 elapse)
	{
		_handle_msg();
	}

	void nworld_session::update_world_msg(uint32 elapse)
	{
		// 更新session的消息缓冲
		for (CLIENT_SESSION_MAP::const_iterator iter = m_sessions.begin(); iter != m_sessions.end(); ++iter)
		{
			iter->second->update(elapse, true);
		}
	}

	bool nworld_session::add_session(nclient_session* session_ptr)
	{
		return m_sessions.insert(std::make_pair(session_ptr->get_client_id(), session_ptr)).second;
	}

	void nworld_session::remove_session(uint64 client_id)
	{
		m_sessions.erase(client_id);
	}

	nclient_session* nworld_session::get_session(uint64 client_id)
	{
		CLIENT_SESSION_MAP::const_iterator iter = m_sessions.find(client_id);

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
		bool overflow = false;

		// 小心消息被截断，丢失消息
		while (msg_ptr)
		{
			temp_msg_ptr = msg_ptr;
			msg_ptr = msg_ptr->next_ptr;

			const nmsg_base* packet = reinterpret_cast<const nmsg_base*>(&(temp_msg_ptr->session_id));
				handler_ptr = g_msg_dispatch.get_client_msg_handler(packet->msg_id);

			if (handler_ptr && handler_ptr->handler)
				{
					++handler_ptr->handle_count;
				nclient_session* session_ptr = get_session(packet->client_id);

				if (session_ptr)
				{
					temp_msg_ptr = session_ptr->push_msg(temp_msg_ptr, handler_ptr->map_task, overflow);

					if (true == overflow)
					{
						// close, 注意消息的释放，当前消息没有加入到队列, 不要内存泄露
						NLOG_ERROR(_T("msg queue overflow, account = %s"), session_ptr->get_account().c_str());
					}
					else if (temp_msg_ptr)
					{
						packet = reinterpret_cast<const nmsg_base*>(&(temp_msg_ptr->session_id));
						// 状态判断
						if (0 != (handler_ptr->status & session_ptr->get_status()))
						{
						(session_ptr->*(handler_ptr->handler))(packet);
					}
						else
						{
							NLOG_ERROR(_T("get client session status error, client_id = %lu, status = %u"), packet->client_id, session_ptr->get_status());
				}
					}
				}
				else
				{
					NLOG_ERROR(_T("get client session error, client_id = %lu"), packet->client_id);
			}
			}
			else
			{
				NLOG_ERROR(_T("get msg handler error, msg_id = %u"), packet->msg_id);
			}
			// 取出下一条消息，释放当前处理的消息
			if (temp_msg_ptr)
			{
				temp_msg_ptr->free();
			}
		}
	}

	nworld_session g_world_session;

} // namespace nexus
