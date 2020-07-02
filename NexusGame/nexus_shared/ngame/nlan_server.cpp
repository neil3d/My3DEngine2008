#include "nlan_server.h"
#include "nlan_session.h"
#include "nlog_mt.h"
#include "nenums.h"
#include "nprotocol_base.h"
#include "nmsg_base_id.h"
#include "nworld_session.h"

#include "nmsg_world_gateway.h"

namespace nexus {

	nlan_server::nlan_server() : m_session_ptrs(NULL), m_max_session(0), m_db_sesssion_id(0)
	{
	}

	nlan_server::~nlan_server()
	{
		destroy();
	}

	bool nlan_server::init(uint32 max_session)
	{
		if (0 == max_session || max_session >= 0xFF)
		{
			NLOG_ERROR(_T("invaild init param, max_session = %u"), max_session);
			return false;
		}

		m_max_session = max_session;

		m_session_ptrs = new nlan_session[m_max_session];

		if (!m_session_ptrs)
		{
			NLOG_ERROR(_T("allocate sesssion memory error!"));
			return false;
		}

		return true;
	}

	void nlan_server::destroy()
	{
		if (m_session_ptrs)
		{
			delete []m_session_ptrs;
			m_session_ptrs = NULL;
		}
	}

	void nlan_server::update(uint32 elapse)
	{
		for (uint32 i = 0; i < m_max_session; ++i)
		{
			if (m_session_ptrs[i].is_in_used())
			{
				m_session_ptrs[i].update(elapse);
			}
		}
	}

	void nlan_server::on_connect(uint32 session_id, uint32 address, uint32 port)
	{
		// 注册连接
		uint32 index = session_id & 0X0000FFFF;

		if (index >= m_max_session)
		{
			NLOG_ERROR(_T("invaild session id, index = %u, session_id = %u"), index, session_id);
			return;
		}

		// 判断是否正在使用, 同一session的on_connect和on_disconnect底层保证串行
		if (m_session_ptrs[index].is_in_used())
		{
			NLOG_ERROR(_T("this session id is in use!, index = %u, session_id = %u"), index, session_id);
			return;
		}

		in_addr addr;
		addr.S_un.S_addr = address;

		std::string ip_str(inet_ntoa(addr));

		m_session_ptrs[index].init(session_id, EST_None, ip_str, port);
	}

	// 注意线程安全, 消息转发, 所有调用函数尽量操作不费时
	void nlan_server::on_datarecv(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		const nmsg_base* packet = reinterpret_cast<const nmsg_base*>(&(msg_ptr->session_id));

		// 根据msg_id区分是客户端发来的消息还是服务器内部的消息
		if (packet->msg_id < Msg_World_DB)
		{
			if (false == g_world_session.push_msg(msg_ptr))
			{
				msg_ptr->free();
				NLOG_ERROR(_T("nworld_session msg queue overflow!"));
				return;
			}
		}
		else
		{
			nlan_session* session_ptr = get_session(session_id);

			if (!session_ptr)
			{
				msg_ptr->free();
				NLOG_ERROR(_T("get nlan_session error, session_id = %u"), session_id);
				return;
			}

			if (false == session_ptr->push_msg(msg_ptr))
			{
				msg_ptr->free();
				NLOG_ERROR(_T("nlan_session msg queue overflow, session_id = %u"), session_id);
				return;
			}
		}
	}

	void nlan_server::on_disconnect(uint32 session_id)
	{
		// 注销连接
		uint32 index = session_id & 0X0000FFFF;

		if (index >= m_max_session)
		{
			NLOG_ERROR(_T("invaild session id, index = %u, session_id = %u"), index, session_id);
			return;
		}

		NLOG_ERROR(_T("connect lose, server_type = %u"), m_session_ptrs[index].get_type());
		m_session_ptrs[index].destroy();
	}

	nlan_session* nlan_server::get_session(uint32 session_id)
	{
		uint32 index = session_id & 0X0000FFFF;

		if (index >= m_max_session)
		{
			NLOG_ERROR(_T("invaild session id, index = %u, session_id = %u"), index, session_id);
			return NULL;
		}

		// 校验id, 不输出log，正常运行时会出现
		if (session_id != m_session_ptrs[index].get_sesssion_id())
		{
			return NULL;
		}

		return &(m_session_ptrs[index]);
	}

	bool nlan_server::send_ndb(const void* msg_ptr, uint32 size)
	{
#ifndef _NBG
		if (false == send_msg(m_db_sesssion_id, msg_ptr, size))
		{
			NLOG_NERROR(_T("send db messgae error, maybe send buffer overflow or connect close! session_id = %u"), m_db_sesssion_id);
			return false;
		}
#endif
		return true;
	}

	void nlan_server::broadcast(const void* msg_ptr, uint32 size)
	{
		uint32 server_type = EST_None;
		tagMsg_G2W_Broadcast	msg;
		msg.push_msg(msg_ptr, size);
		msg.set_original_msg_id(static_cast<const nmsg_base*>(msg_ptr)->msg_id);
		msg.msg_id = Msg_G2W_Broadcast;
		// client id == 0

		for (uint32 i = 0; i <m_max_session; ++i)
		{
			if (m_session_ptrs[i].is_in_used() && EST_Gateway == m_session_ptrs[i].get_type())
			{
				send_msg(m_session_ptrs[i].get_sesssion_id(), &msg, sizeof(msg));

				if (false == send_msg(m_session_ptrs[i].get_sesssion_id(), &msg, sizeof(msg)))
				{
					NLOG_NERROR(_T("send gateway messgae error, maybe send buffer overflow or connect close! session_id = %u"), m_session_ptrs[i].get_sesssion_id());
				}
			}
		}
	}

	nlan_server g_lan_server;

} // namespace nexus
