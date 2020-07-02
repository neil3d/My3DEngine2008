#include "nlan_client.h"
#include "nlan_session.h"
#include "nmsg_dispatch.h"
#include "nlog_mt.h"
#include "ncfg.h"
#include "nenums.h"

#include "nmsg_cluster.h"

namespace nexus {

	const int32 RETRY_TIME = 10 * 1000;		// 10s

	nlan_client::nlan_client() : m_session_ptrs(NULL), m_nworld_session_id(0XFFFFFFFF), m_nlogin_session_id(0XFFFFFFFF), m_nbg_session_id(0XFFFFFFFF),
		m_retry_login_ip(0), m_retry_login_port(0), m_retry_bg_ip(0), m_retry_bg_port(0), m_retry_nworld_elapse(0), m_retry_nlogin_elapse(0), m_retry_nbg_elapse(0)
	{
	}

	nlan_client::~nlan_client()
	{
		destroy();
	}

	bool nlan_client::init(uint32 max_session)
	{
		if (0 == max_session)
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

		if (m_retry_nlogin_elapse > 0)
		{
			m_retry_nlogin_elapse -= elapse;

			if (m_retry_nlogin_elapse <= 0)
			{
				if (false == connect_to_nlogin(m_retry_login_ip, m_retry_login_port))
				{
					NLOG_ERROR(_T("%s, connect nlogin request error!"), __TFUNCTION__);
					m_retry_nlogin_elapse = RETRY_TIME;
				}
			}
		}

		// 目前只处理一个bg的情况
		// TODO: D. Zhao 如果有多个bg断开重新连接的，这里要加入线程安全的连接队列
		if (m_retry_nbg_elapse > 0)
		{
			m_retry_nbg_elapse -= elapse;

			if (m_retry_nbg_elapse <= 0)
			{
				if (false == connect_to_nbg(m_retry_bg_ip, m_retry_bg_port))
				{
					NLOG_ERROR(_T("%s, connect nbg request error!"), __TFUNCTION__);
					m_retry_nbg_elapse = RETRY_TIME;
				}
			}
		}

		// update ...
	}

	void nlan_client::on_connect(uint32 session_id, uint32 param1, uint32 param2)
	{
		// 注册连接
		if (m_nworld_session_id == session_id)
		{
			_handle_connect_nworld(param1);
		}
		else if (m_nlogin_session_id == session_id)
		{
			_handle_connect_nlogin(param1);
		}
		else
		{
			_handle_connect_nbg(param1);
		}
	}

	void nlan_client::on_datarecv(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		// 消息转发, 所有调用函数尽量操作不费时
		g_msg_dispatch.do_lan2wan_dispatch(session_id, msg_ptr, size);
	}

	void nlan_client::on_disconnect(uint32 session_id)
	{
		// 注销连接
		if (m_nworld_session_id == session_id)
		{
			_handle_close_nworld();
		}
		else if (m_nlogin_session_id == session_id)
		{
			_handle_close_nlogin();
		}
		else
		{
			_handle_close_nbg();
		}
	}

	bool nlan_client::send_nworld(nmessage* msg_ptr, uint32 size)
	{
		nlan_session* session_ptr = get_session(m_nworld_session_id);

		if (!session_ptr)
		{
			msg_ptr->free();
			NLOG_ERROR(_T("get world session error, session_id = %u"), m_nworld_session_id);
			return false;
		}

		if (ELSS_Logined != session_ptr->get_status())
		{
			msg_ptr->free();
			NLOG_ERROR(_T("world session status error, may be not logined, status = %u"), session_ptr->get_status());
			return false;
		}

		return send(m_nworld_session_id, msg_ptr, size);
	}

	bool nlan_client::send_nlogin(nmessage* msg_ptr, uint32 size)
	{
		nlan_session* session_ptr = get_session(m_nlogin_session_id);

		if (!session_ptr)
		{
			msg_ptr->free();
			NLOG_ERROR(_T("get login session error, session_id = %u"), m_nlogin_session_id);
			return false;
		}

		if (ELSS_Logined != session_ptr->get_status())
		{
			msg_ptr->free();
			NLOG_ERROR(_T("login session status error, may be not logined, status = %u"), session_ptr->get_status());
			return false;
		}

		return send(m_nlogin_session_id, msg_ptr, size);
	}

	bool nlan_client::send_nbg(nmessage* msg_ptr, uint32 size)
	{
		nlan_session* session_ptr = get_session(m_nbg_session_id);

		if (!session_ptr)
		{
			msg_ptr->free();
			NLOG_ERROR(_T("get bg session error, session_id = %u"), m_nbg_session_id);
			return false;
		}

		if (ELSS_Logined != session_ptr->get_status())
		{
			msg_ptr->free();
			NLOG_ERROR(_T("bg session status error, may be not logined, status = %u"), session_ptr->get_status());
			return false;
		}

		return send(m_nbg_session_id, msg_ptr, size);
	}

	bool nlan_client::send_nworld_msg(const void* msg_ptr, uint32 size)
	{
		nlan_session* session_ptr = get_session(m_nworld_session_id);

		if (!session_ptr)
		{
			NLOG_ERROR(_T("get world session error, session_id = %u"), m_nworld_session_id);
			return false;
		}

		if (ELSS_Logined != session_ptr->get_status())
		{
			NLOG_ERROR(_T("world session status error, may be not logined, status = %u"), session_ptr->get_status());
			return false;
		}

		return send_msg(m_nworld_session_id, msg_ptr, size);
	}

	bool nlan_client::send_nlogin_msg(const void* msg_ptr, uint32 size)
	{
		nlan_session* session_ptr = get_session(m_nlogin_session_id);

		if (!session_ptr)
		{
			NLOG_ERROR(_T("get login session error, session_id = %u"), m_nlogin_session_id);
			return false;
		}

		if (ELSS_Logined != session_ptr->get_status())
		{
			NLOG_ERROR(_T("login session status error, may be not logined, status = %u"), session_ptr->get_status());
			return false;
		}

		return send_msg(m_nlogin_session_id, msg_ptr, size);
	}

	bool nlan_client::send_nbg_msg(const void* msg_ptr, uint32 size)
	{
		nlan_session* session_ptr = get_session(m_nbg_session_id);

		if (!session_ptr)
		{
			NLOG_ERROR(_T("get bg session error, session_id = %u"), m_nbg_session_id);
			return false;
		}

		if (ELSS_Logined != session_ptr->get_status())
		{
			NLOG_ERROR(_T("bg session status error, may be not logined, status = %u"), session_ptr->get_status());
			return false;
		}

		return send_msg(m_nbg_session_id, msg_ptr, size);
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
		std::string ip = sncfg.get_string(ECI_NWorldIp);
		uint32 value = sncfg.get_uint32(ECI_NWorldPort);

		// 返回分配连接的session_id,id等到验证后才能使用
		m_nworld_session_id = g_lan_client.connect_to(ip, uint16(value));

		if (0XFFFFFFFF == m_nworld_session_id)
		{
			NLOG_ERROR(_T("connect to nworld error!, ip = %s, port = %u"), ip.c_str(), sncfg.get_uint32(ECI_NWorldPort));
			m_retry_nworld_elapse = RETRY_TIME;
			return false;
		}

		return true;
	}

	bool nlan_client::connect_to_nlogin(uint32 ip, uint16 port)
	{
		m_retry_login_ip	= ip;
		m_retry_login_port	= port;

		in_addr addr;
		addr.S_un.S_addr = ip;

		std::string ip_str(inet_ntoa(addr));

		// 返回分配连接的session_id,id等到验证后才能使用
		m_nlogin_session_id = g_lan_client.connect_to(ip_str, port);

		if (0XFFFFFFFF == m_nlogin_session_id)
		{
			NLOG_ERROR(_T("connect to nlogin error!, ip = %s, port = %u"), ip_str.c_str(), port);
			m_retry_nlogin_elapse	= RETRY_TIME;
			return false;
		}

		return true;
	}

	bool nlan_client::connect_to_nbg(uint32 ip, uint16 port)
	{
		// TODO: D. Zhao 添加nbg服务器
		return true;

		m_retry_bg_ip	= ip;
		m_retry_bg_port	= port;

		in_addr addr;
		addr.S_un.S_addr = ip;

		std::string ip_str(inet_ntoa(addr));

		// 返回分配连接的session_id,id等到验证后才能使用
		m_nbg_session_id = g_lan_client.connect_to(ip_str, port);

		if (0XFFFFFFFF == m_nbg_session_id)
		{
			NLOG_ERROR(_T("connect to nbg error!, ip = %s, port = %u"), ip_str.c_str(), port);
			m_retry_nbg_elapse	= RETRY_TIME;
			return false;
		}

		return true;
	}

	bool nlan_client::_register_session(uint32 session_id, uint32 server_type, uint32 address, uint16 port)
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
			return;
		}

		// 注册连接
		_register_session(m_nworld_session_id, EST_World, 0, 0);
		// 发送login消息
		tagMsg_Request_Login request;
		request.server_type = EST_Gateway;
		request.server_id	= 0;
		send_msg(m_nworld_session_id, &request, sizeof(request));
	}

	void nlan_client::_handle_connect_nlogin(uint32 param)
	{
		if (ECC_Connected != param)
		{
			// 设置update重新连接,ip和port使用m_retry_login_ip和m_retry_login_port
			m_retry_nlogin_elapse = RETRY_TIME;
			m_nlogin_session_id = 0XFFFFFFFF;
			return;
		}

		// 注册连接
		_register_session(m_nlogin_session_id, EST_Login, m_retry_login_ip, m_retry_login_port);
		// 发送login消息
		tagMsg_Request_Login request;
		request.server_type = EST_Gateway;
		request.server_id	= g_msg_dispatch.get_server_id();
		send_msg(m_nlogin_session_id, &request, sizeof(request));
	}

	void nlan_client::_handle_connect_nbg(uint32 param)
	{
		if (ECC_Connected != param)
		{
			// 设置update重新连接,ip和port使用m_retry_bg_ip和m_retry_bg_port
			m_retry_nlogin_elapse = RETRY_TIME;
			m_nbg_session_id = 0XFFFFFFFF;
			return;
		}

		// 注册连接
		_register_session(m_nbg_session_id, EST_Bg, m_retry_bg_ip, m_retry_bg_port);
		// 发送login消息
		tagMsg_Request_Login request;
		request.server_type = EST_Gateway;
		request.server_id	= g_msg_dispatch.get_server_id();
		send_msg(m_nbg_session_id, &request, sizeof(request));
	}

	void nlan_client::_handle_close_nworld()
	{
		_unregister_session(m_nworld_session_id);
		m_nworld_session_id = 0XFFFFFFFF;
		// log
		NLOG_FATAL(_T("disconnect with nworld server!"));
	}

	void nlan_client::_handle_close_nlogin()
	{
		_unregister_session(m_nlogin_session_id);
		m_nlogin_session_id = 0XFFFFFFFF;
		NLOG_ERROR(_T("disconnect with nlogin server, 10s later reconnect!"));
	}

	void nlan_client::_handle_close_nbg()
	{
		_unregister_session(m_nbg_session_id);
		m_nbg_session_id = 0XFFFFFFFF;
		NLOG_ERROR(_T("disconnect with nlogin server, 10s later reconnect!"));
	}

	nlan_client g_lan_client;

} // namespace nexus
