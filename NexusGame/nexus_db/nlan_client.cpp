#include "nlan_client.h"
#include "nlan_session.h"
#include "nlog_mt.h"
#include "nenums.h"
#include "ncfg.h"

#include "nmsg_cluster.h"

namespace nexus {

	const int32 RETRY_TIME = 10 * 1000;		// 10s

	nlan_client::nlan_client() : m_session_ptrs(NULL), m_max_session(0), m_nworld_session_id(0XFFFFFFFF), m_retry_nworld_elapse(0)
	{
	}

	nlan_client::~nlan_client()
	{
		destroy();
	}

	bool nlan_client::init(uint32 max_session)
	{
		if (0 == max_session || max_session >= 0xFF)
		{
			NLOG_ERROR(_T("invaild max session num, max_session = %u"), max_session);
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

	void nlan_client::destroy()
	{
		if (m_session_ptrs)
		{
			delete []m_session_ptrs;
			m_session_ptrs = NULL;
		}
	}

	void nlan_client::update(uint32 elapse)
	{
		// update重新连接
		if (m_retry_nworld_elapse > 0)
		{
			m_retry_nworld_elapse -= elapse;

			if (m_retry_nworld_elapse <= 0)
			{
				if (false == connect_to_nworld())
				{
					NLOG_ERROR(_T("%s, connect nworld request error!"), __TFUNCTION__);
					m_retry_nworld_elapse = RETRY_TIME;
				}
			}
		}

		// update session
		for (uint32 i = 0; i < m_max_session; ++i)
		{
			if (m_session_ptrs[i].is_in_used())
			{
				m_session_ptrs[i].update(elapse);
			}
		}
	}

	void nlan_client::on_connect(uint32 session_id, uint32 param1, uint32 param2)
	{
		// 注册连接
		if (m_nworld_session_id == session_id)
		{
			_handle_connect_nworld(param1);
		}
		else
		{
			NLOG_ERROR(_T("%s, on_connect session_id error!"), __TFUNCTION__);
		}
	}

	// 注意线程安全, 所有调用函数尽量操作不费时
	void nlan_client::on_datarecv(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		nlan_session* session_ptr = get_session(session_id);

		if (!session_ptr)
		{
			msg_ptr->free();
			NLOG_ERROR(_T("get session error, session_id = %u"), session_id);
			return;
		}

		if (false == session_ptr->push_msg(msg_ptr))
		{
			msg_ptr->free();
			NLOG_ERROR(_T("nlan_session msg queue overflow!, session_id = %u"), session_id);
			return;
		}
	}

	void nlan_client::on_disconnect(uint32 session_id)
	{
		// 注销连接
		if (m_nworld_session_id == session_id)
		{
			_handle_close_nworld();
		}
		else
		{
			NLOG_ERROR(_T("%s, on_disconnect session_id error!"), __TFUNCTION__);
		}
	}

	bool nlan_client::send_nworld(const void* msg_ptr, uint32 size)
	{
		if (false == g_lan_client.send_msg(m_nworld_session_id, msg_ptr, size))
		{
			NLOG_NERROR(_T("send world messgae error, maybe send buffer overflow or connect close! session_id = %u"), m_nworld_session_id);
			return false;
		}

		return true;
	}

	nlan_session* nlan_client::get_session(uint32 session_id)
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

	bool nlan_client::connect_to_nworld()
	{
		std::string ip = sncfg.get_string(ECI_WorldIP);
		uint32 value = sncfg.get_uint32(ECI_WorldPort);

		// 返回分配连接的session_id,id等到验证后才能使用
		m_nworld_session_id = g_lan_client.connect_to(ip, uint16(value));

		if (0XFFFFFFFF == m_nworld_session_id)
		{
			NLOG_ERROR(_T("connect to nworld error!, ip = %s, port = %u"), ip.c_str(), value);
			m_retry_nworld_elapse = RETRY_TIME;
			return false;
		}

		return true;
	}

	bool nlan_client::_register_session(uint32 session_id, uint32 server_type, const std::string& address, uint16 port)
	{
		// 注册连接
		uint32 index = session_id & 0X0000FFFF;

		if (index >= m_max_session)
		{
			NLOG_ERROR(_T("invaild session id, index = %u, session_id = %u"), index, session_id);
			return false;
		}

		// 判断是否正在使用, 同一session的on_connect和on_disconnect底层保证串行
		if (m_session_ptrs[index].is_in_used())
		{
			NLOG_ERROR(_T("this session id is in use!, index = %u, session_id = %u"), index, session_id);
			return false;
		}

		return m_session_ptrs[index].init(session_id, server_type, address, port);
	}

	void nlan_client::_unregister_session(uint32 session_id)
	{
		uint32 index = session_id & 0X0000FFFF;

		if (index >= m_max_session)
		{
			NLOG_ERROR(_T("invaild session id, index = %u, session_id = %u"), index, session_id);
			return;
		}

		m_session_ptrs[index].destroy();
	}

	void nlan_client::_handle_connect_nworld(uint32 param)
	{
		if (ECC_Connected != param)
		{
			// 设置update重新连接
			m_retry_nworld_elapse = RETRY_TIME;
			m_nworld_session_id = 0XFFFFFFFF;

			#ifndef _DEBUG
			//NLOG_ERROR(_T("connect world error, error_code = %u, retry connect world 10s later"), param);
			#endif
			return;
		}

		// 注册连接
		_register_session(m_nworld_session_id, EST_World, sncfg.get_string(ECI_WorldIP), sncfg.get_uint32(ECI_WorldPort));

		// 发送login消息
		tagMsg_Request_Login request;
		request.server_type = EST_DB;
		request.server_id	= 0;
		send_msg(m_nworld_session_id, &request, sizeof(request));
	}

	void nlan_client::_handle_close_nworld()
	{
		_unregister_session(m_nworld_session_id);
		m_nworld_session_id = 0XFFFFFFFF;
		NLOG_ERROR(_T("disconnect with nworld server, 10s later reconnect!"));
	}

	nlan_client g_lan_client;

} // namespace nexus
