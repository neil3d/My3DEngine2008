#include "nlan_session.h"
#include "nlan_server.h"
#include "nlan_client.h"
#include "nenums.h"
#include "ninit_flag.h"
#include "nlog_mt.h"
#include "nmsg_dispatch.h"
#include "nworld_session.h"
#include "nclient_session.h"
#include "nplayer.h"

#include "nmsg_cluster.h"
#include "nmsg_world_login.h"
#include "nmsg_world_db.h"
#include "nmsg_login.h"

namespace nexus {

	nlan_session::nlan_session() : m_session_id(0XFFFFFFFF), m_server_type(EST_None), m_status(ELSS_None), m_address(""), m_port(0),
		m_msg_queue(10000)
	{
	}

	nlan_session::~nlan_session()
	{
	}

	bool nlan_session::init(uint32 session_id, uint32 server_type, const std::string& address, uint16 port)
	{
		m_session_id	= session_id;
		m_server_type	= server_type;
		m_status		= ELSS_Connected;
		m_address		= address;
		m_port			= port;
		return true;
	}

	// destroy 要进行该类所有数据的复原，因为要归还到池里
	void nlan_session::destroy()
	{
		m_session_id	= 0XFFFFFFFF;
		m_server_type	= EST_None;
		m_status		= ELSS_Closed;
		m_address		= "";
		m_port			= 0;
	}

	void nlan_session::update(uint32 elapse)
	{
		_handle_msg();
	}

	bool nlan_session::send_message(const void* msg_ptr, uint32 size)
	{
		if (EST_World == m_server_type || EST_Gateway == m_server_type || EST_DB == m_server_type)
		{
			return g_lan_server.send_msg(m_session_id, msg_ptr, size);
		}
		else
		{
			return g_lan_client.send_msg(m_session_id, msg_ptr, size);
		}
	}

	void nlan_session::_handle_msg()
	{
		ncluster_msg_handler* handler_ptr = NULL;
		nmessage* msg_ptr = m_msg_queue.dequeue();
		nmessage* temp_msg_ptr = NULL;

		while (msg_ptr)
		{
			const nmsg_base* packet = reinterpret_cast<const nmsg_base*>(&(msg_ptr->session_id));
			handler_ptr = g_msg_dispatch.get_cluster_msg_handler(packet->msg_id);

			if (!handler_ptr || !handler_ptr->handler)
			{
				NLOG_ERROR(_T("get msg handler error, msg_id = %u, client_id = %lu"), packet->msg_id, packet->client_id);
			}
			else
			{
				++handler_ptr->handle_count;

				// TODO: D. Zhao状态判断
				(this->*(handler_ptr->handler))(packet);
			}
			// 注意msg的释放
			temp_msg_ptr = msg_ptr;
			msg_ptr = msg_ptr->next_ptr;
			temp_msg_ptr->free();
		}
	}

	void nlan_session::handle_request_login(const nmsg_base* packet)
	{
		const tagMsg_Request_Login* data_ptr = static_cast<const tagMsg_Request_Login*>(packet);
		m_server_type = data_ptr->server_type;

		// reply, 肯定是gateway或db
		tagMsg_Reply_Login reply;
		reply.error_code  = EError_Success;
		g_lan_server.send_msg(m_session_id, &reply, sizeof(reply));

		switch (m_server_type)
		{
		case EST_Gateway:
			{
#ifndef _NBG
				// TODO: D. Zhao加入db服务器
				g_init_flag.set_init_flag(EIF_Db);
				g_init_flag.gateway_login(m_session_id);
#endif
			}
			break;
		case EST_World:
			{
#ifdef _NBG
				// 加入到world管理器
#endif
			}
			break;
		case EST_DB:
			{
				g_lan_server.set_db_session_id(m_session_id);
				g_init_flag.set_init_flag(EIF_Db);
			}
			break;
		default:
			{
				NLOG_ERROR(_T("error server type!"));
			}
			break;
		}
	}

	void nlan_session::handle_reply_login(const nmsg_base* packet)
	{
		const tagMsg_Reply_Login* data_ptr = static_cast<const tagMsg_Reply_Login*>(packet);

		if (EError_Success == data_ptr->error_code)
		{
			m_status = ELSS_Logined;

			switch (m_server_type)
			{
			case EST_Login:
				{
					g_init_flag.set_init_flag(EIF_Loin);
				}
				break;
			case EST_Bg:
				{
					g_init_flag.set_init_flag(EIF_Bg);
				}
				break;
			default:
				{
					NLOG_ERROR(_T("error server type!"));
				}
				break;
			}
		}
		else
		{
			NLOG_ERROR(_T("reply login error, error_code = %d"), data_ptr->error_code);
		}
	}

	void nlan_session::handle_player_login(const nmsg_base* packet)
	{
		const tagMsg_L2W_Player_Login* data_ptr = static_cast<const tagMsg_L2W_Player_Login*>(packet);
		nclient_session* session_ptr = new nclient_session();

		if (!session_ptr)
		{
			NLOG_ERROR(_T("alloc nclient_session error!"));
			return;
		}

		// 校对gateway session id
		nlan_session* lan_session_ptr = g_lan_server.get_session(uint32(data_ptr->client_id >> 56) - 1);

		if (!lan_session_ptr)
		{
			NLOG_ERROR(_T("get nlan_session error!"));
			return;
		}

		session_ptr->init(data_ptr->client_id, lan_session_ptr->get_sesssion_id(), data_ptr->account_id, data_ptr->privilege, 
			data_ptr->login_port, data_ptr->login_ip, data_ptr->account, data_ptr->prelogout_time);

		// 将session 加入到相应管理器
		if (false == g_world_session.add_session(session_ptr))
		{
			NLOG_ERROR(_T("world session add client session error!"));
			// TODO: D. Zhao 释放内存通知客户端
			delete session_ptr;
			return;
		}

		// auth proof sucess
		tagS2C_LoginProofResult reply;
		reply.error = EError_Success;
		session_ptr->send_message(&reply, sizeof(reply));
	}

	void nlan_session::handle_player_logout(const nmsg_base* packet)
	{
		const tagMsg_Player_Logout* data_ptr = static_cast<const tagMsg_Player_Logout*>(packet);
		nclient_session* session_ptr = g_world_session.get_session(data_ptr->client_id);

		if (!session_ptr)
		{
			NLOG_ERROR(_T("get nclient_session error!"));
			return;
		}

		session_ptr->destroy();
		g_world_session.remove_session(data_ptr->client_id);
		delete session_ptr;
	}

	void nlan_session::handle_char_enum_db(const nmsg_base* packet)
	{

	}

	void nlan_session::handle_char_create_db(const nmsg_base* packet)
	{

	}

	void nlan_session::handle_char_delete_db(const nmsg_base* packet)
	{

	}

	void nlan_session::handle_load_player_stat(const nmsg_base* packet)
	{
		const tagMsg_D2W_LoadPlayerStat* data_ptr = static_cast<const tagMsg_D2W_LoadPlayerStat*>(packet);
		nclient_session* session_ptr = g_world_session.get_session(data_ptr->client_id);

		if (!session_ptr)
		{
			NLOG_ERROR(_T("get nclient_session error!"));
			return;
		}

		nplayer* player_ptr = session_ptr->get_player();

		if (!player_ptr)
		{
			NLOG_ERROR(_T("get nplayer error!"));
			return;
		}

		player_ptr->load_stat_from_db(data_ptr);
	}

	void nlan_session::handle_load_player_item(const nmsg_base* packet)
	{

	}

	void nlan_session::handle_load_player_spell(const nmsg_base* packet)
	{
		const tagMsg_D2W_LoadPlayerSpell* data_ptr = static_cast<const tagMsg_D2W_LoadPlayerSpell*>(packet);
		nclient_session* session_ptr = g_world_session.get_session(data_ptr->client_id);

		if (!session_ptr)
		{
			NLOG_ERROR(_T("get nclient_session error!"));
			return;
		}

		nplayer* player_ptr = session_ptr->get_player();

		if (!player_ptr)
		{
			NLOG_ERROR(_T("get nplayer error!"));
			return;
		}

		player_ptr->load_spell_from_db(data_ptr);
	}

	void nlan_session::handle_load_player_aura(const nmsg_base* packet)
	{
		const tagMsg_D2W_LoadPlayerAura* data_ptr = static_cast<const tagMsg_D2W_LoadPlayerAura*>(packet);
		nclient_session* session_ptr = g_world_session.get_session(data_ptr->client_id);

		if (!session_ptr)
		{
			NLOG_ERROR(_T("get nclient_session error!"));
			return;
		}

		nplayer* player_ptr = session_ptr->get_player();

		if (!player_ptr)
		{
			NLOG_ERROR(_T("get nplayer error!"));
			return;
		}

		player_ptr->load_aura_from_db(data_ptr);
	}

	void nlan_session::handle_load_player_cool_down(const nmsg_base* packet)
	{

	}

} // namespace nexus
