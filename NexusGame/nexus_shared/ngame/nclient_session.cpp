#include "nclient_session.h"
#include "nlan_server.h"
#include "nlog_mt.h"
#include "nplayer.h"
#include "nmap_mgr.h"
#include "nmsg_dispatch.h"
#include "nprotocol_base.h"

namespace nexus {

	nclient_session::nclient_session() : m_client_id(0), m_gateway_session_id(0), m_account_id(0), m_status(ESS_None), m_privilege(0), m_port(0), m_ip_address(0), m_account(_T("")), m_logout_time(0),
		m_login_time(0), m_player_ptr(0)
	{
	}

	nclient_session::~nclient_session()
	{
	}

	bool nclient_session::init(uint64 client_id, uint32 gateway_session_id, uint32 account_id, uint8 privilege, uint16 port, uint32 ip_address, const nchar* account, uint64 logout_time)
	{
		m_client_id				= client_id;
		m_gateway_session_id	= gateway_session_id;
		m_account_id			= account_id;
		m_status				= ESS_Logined;
		m_privilege				= privilege;
		m_port					= port;
		m_ip_address			= ip_address;
		m_account				= tstring(account);
		m_logout_time			= logout_time;
		m_login_time			= time(NULL);

		// 读取配置文件初始化消息缓冲配置
		m_msg_buff.init(100);
		return true;
	}

	void nclient_session::destroy()
	{
		if (m_player_ptr)
		{
		g_map_mgr.player_leave(m_player_ptr);
			m_player_ptr->save_data_to_db();
		m_player_ptr->destroy();
		delete m_player_ptr;
		m_player_ptr = NULL;
		}

		// 其他释放操作
	}

	void nclient_session::update(uint32 elapse, bool world_update)
	{
		if (world_update)
		{

	}

		nmessage* msg_ptr = m_msg_buff.pop_msg(!world_update);

		if (msg_ptr)
		{
			const nmsg_base* packet = reinterpret_cast<const nmsg_base*>(&(msg_ptr->session_id));
			nclient_msg_handler* handler_ptr = g_msg_dispatch.get_client_msg_handler(packet->msg_id);

			if (handler_ptr && handler_ptr->handler && 0 != (handler_ptr->status & m_status))
			{
				(this->*(handler_ptr->handler))(packet);
			}
			else
			{
				NLOG_ERROR(_T("msg handler error, status = %u"), m_status);
			}

			msg_ptr->free();
		}
	}

	bool nclient_session::send_message(void* msg_ptr, uint32 size)
	{
		// 封装client_id
		nmsg_base* msg_base_ptr = reinterpret_cast<nmsg_base*>(msg_ptr);
		msg_base_ptr->client_id = m_client_id;

		if (false == g_lan_server.send_msg(m_gateway_session_id, msg_ptr, size))
		{
			NLOG_NERROR(_T("nworld send gateway error, maybe send buffer overflow or connect close!"));
			return false;
		}

		return true;
	}

	bool nclient_session::send_message_to_db(void* msg_ptr, uint32 size)
	{
		// 封装client_id
		nmsg_base* msg_base_ptr = reinterpret_cast<nmsg_base*>(msg_ptr);
		msg_base_ptr->client_id = m_client_id;

		if (false == g_lan_server.send_ndb(msg_ptr, size))
		{
			NLOG_NERROR(_T("nworld send db error, maybe send buffer overflow or connect close!"));
			return false;
	}

		return true;
	}

} // namespace nexus
