#include "nmsg_dispatch.h"
#include "nlan_session.h"
#include "nprotocol_base.h"
#include "nlog_mt.h"
#include "nenums.h"

#include "nmsg_login.h"
#include "nmsg_cluster.h"
#include "nmsg_world_login.h"

namespace nexus {

	nmsg_dispatch::nmsg_dispatch()
	{
	}

	nmsg_dispatch::~nmsg_dispatch()
	{

	}

	bool nmsg_dispatch::init()
	{
		// 服务器内部消息
		_register_msg_handler(Msg_W2L_World_Request_Login,	_T("Msg_W2L_World_Request_Login"),	ELSS_Connected,	&nlan_session::handle_world_request_login);
		_register_msg_handler(Msg_Request_Login,			_T("Msg_Request_Login"),			ELSS_Connected,	&nlan_session::handle_request_login);
		_register_msg_handler(C2S_LoginProof,				_T("tagC2S_LoginProof"),			ELSS_Logined,	&nlan_session::handle_login_proof);
		_register_msg_handler(Msg_Player_Logout,			_T("Msg_Player_Logout"),			ELSS_Logined,	&nlan_session::handle_account_logout);

		_register_msg_handler(C2S_MiBao,					_T("tagC2S_MiBao"),					ELSS_Logined,	&nlan_session::handle_mibao);

		return true;
	}

	void nmsg_dispatch::destroy()
	{

	}

	nmsg_handler* nmsg_dispatch::get_msg_handler(uint16 msg_id)
	{
		if (msg_id >= Msg_Max)
		{
			return NULL;
		}

		return &(m_msg_handler[msg_id]);
	}

	void nmsg_dispatch::_register_msg_handler(uint16 msg_id, const tstring& msg_name, uint32 status, handler_type handler)
	{
		m_msg_handler[msg_id].msg_name		= msg_name;
		m_msg_handler[msg_id].status		= status;
		m_msg_handler[msg_id].handle_count	= 0;
		m_msg_handler[msg_id].handler		= handler;
	}

	nmsg_dispatch g_msg_dispatch;

} // namespace nexus
