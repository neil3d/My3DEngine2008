#include "nmsg_dispatch.h"
#include "nlan_session.h"
#include "nprotocol_base.h"
#include "nlog_mt.h"
#include "nenums.h"

#include "nmsg_cluster.h"
#include "nmsg_world_db.h"

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
		_register_msg_handler(Msg_Reply_Login,				_T("Msg_Reply_Login"),				ELSS_Connected,	&nlan_session::handle_reply_login);
		_register_msg_handler(Msg_W2D_LoadPlayerData,		_T("Msg_W2D_LoadPlayerData"),		ELSS_Connected,	&nlan_session::handle_load_player_data);
		_register_msg_handler(Msg_W2D_UnloadPlayer,			_T("Msg_W2D_UnloadPlayer"),			ELSS_Connected,	&nlan_session::handle_unload_player_data);
		_register_msg_handler(Msg_W2D_CharCreate,			_T("Msg_W2D_CharCreate"),			ELSS_Connected,	&nlan_session::handle_create_character);
		_register_msg_handler(Msg_W2D_CharDelete,			_T("Msg_W2D_CharDelete"),			ELSS_Connected,	&nlan_session::handle_delete_character);
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
