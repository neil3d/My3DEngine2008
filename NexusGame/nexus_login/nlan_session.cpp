#include "nlan_session.h"
#include "nlan_server.h"
#include "nenums.h"
#include "nlog_mt.h"
#include "nmsg_dispatch.h"
#include "nworld_mgr.h"
#include "nworld.h"

#include "ncfg.h"
#include "nlogin_utility.h"
#include "nlogin_player.h"
#include "nplayer_account_mgr.h"

#include "nmsg_cluster.h"
#include "nmsg_world_login.h"
#include "nmsg_login.h"
#include "nhandle_login.h"

namespace nexus {

	nlan_session::nlan_session() : m_session_id(0XFFFFFFFF), m_server_type(EST_None), m_server_id(0), m_status(ELSS_None), m_address(""), 
		m_port(0), m_msg_queue(10000)
	{
	}

	nlan_session::~nlan_session()
	{
	}

	bool nlan_session::init(uint32 session_id, const std::string& address, uint16 port)
	{
		m_session_id	= session_id;
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
		return g_lan_server.send_msg(m_session_id, msg_ptr, size);
	}

	void nlan_session::_handle_msg()
	{
		nmsg_handler* handler_ptr = NULL;
		nmessage* msg_ptr = m_msg_queue.dequeue();
		nmessage* temp_msg_ptr = NULL;

		while (msg_ptr)
		{
			const nmsg_base* packet = reinterpret_cast<const nmsg_base*>(&(msg_ptr->session_id));
			handler_ptr = g_msg_dispatch.get_msg_handler(packet->msg_id);

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

	void nlan_session::handle_world_request_login(const nmsg_base* packet)
	{
		const tagMsg_W2L_World_Request_Login* data_ptr = static_cast<const tagMsg_W2L_World_Request_Login*>(packet);
		m_server_type	= EST_World;
		m_server_id		= data_ptr->server_id;
		m_status		= ELSS_Logined;

		nworld* world_ptr = new nworld();

		if (!world_ptr)
		{
			NLOG_ERROR(_T("alloc memory error, m_server_id = %u"), m_server_id);
			return;
		}

		world_ptr->init(m_session_id, m_server_id, tstring(data_ptr->world_name));
		// 将world加入到相应管理器
		if (!g_world_mgr.add_world(world_ptr))
		{
			NLOG_ERROR(_T("world mgr add new world error!"));
			delete world_ptr;
			return;
		}

		NLOG_INFO(_T("world login, world_name = %s"), data_ptr->world_name);

		tagMsg_Reply_Login reply;
		reply.error_code  = EError_Success;
		send_message(&reply, sizeof(reply));
	}

	void nlan_session::handle_request_login(const nmsg_base* packet)
	{
		const tagMsg_Request_Login* data_ptr = static_cast<const tagMsg_Request_Login*>(packet);

		if (EST_Gateway != data_ptr->server_type)
		{
			NLOG_ERROR(_T("server type error, m_server_type = %u"), m_server_type);
			return;
		}

		m_server_type	= data_ptr->server_type;
		m_server_id		= data_ptr->server_id;
		m_status		= ELSS_Logined;

		tagMsg_Reply_Login reply;
		reply.error_code  = EError_Success;

		nworld* world_ptr = g_world_mgr.get_world(get_world_id());

		if (!world_ptr)
		{
			NLOG_DEBUG(_T("can't find the world!"));
			reply.error_code  = EError_SystemError;
		}
		else
		{
			world_ptr->set_gateway_session_id(m_session_id);
			NLOG_INFO(_T("gateway login, server_id = %u"), m_server_id);
		}

		send_message(&reply, sizeof(reply));
	}


	void nlan_session::handle_login_proof(const nmsg_base* packet)
	{
		switch( sncfg.get_uint32(ECI_ProofLevel) )
		{
		case EPIType_ProofNo:
			handle_login_proof_1(packet);
			break;

		default:
			handle_login_proof_2(packet);	
		}		
	}


	void nlan_session::handle_login_proof_1(const nmsg_base* packet)
	{
		const tagC2S_LoginProof* data_ptr = static_cast<const tagC2S_LoginProof*>(packet);

		static uint32 index = 0;

		tagMsg_L2W_Player_Login request;
		request.client_id		= data_ptr->client_id;
		request.account_id		= ++index;
		request.online_time		= 100;
		request.privilege		= EAT_GM_LV2;
		memcpy(request.account, _T("test"), sizeof(nchar) * wcslen(_T("test")));

		nworld* world_ptr = g_world_mgr.get_world(get_world_id());

		if (!world_ptr)
		{
			NLOG_DEBUG(_T("can't find the world!"));
			return;
		}

		world_ptr->send_world_msg(&request, sizeof(request));
	}

} // namespace nexus
