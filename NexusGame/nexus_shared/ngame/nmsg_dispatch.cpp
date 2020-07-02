#include "nmsg_dispatch.h"
#include "nlan_session.h"
#include "nclient_session.h"
#include "nprotocol_base.h"
#include "nlog_mt.h"
#include "nenums.h"

#include "nmsg_char.h"
#include "nmsg_loading.h"
#include "nmsg_map.h"
#include "nmsg_movement.h"
#include "nmsg_chat.h"
#include "nmsg_spell.h"

#include "nmsg_cluster.h"
#include "nmsg_world_login.h"
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
		// client world update
		// character
		_register_client_msg_handler(C2S_CharEnum,			_T("C2S_CharEnum"),			ESS_Logined,	&nclient_session::handle_char_enum);
		_register_client_msg_handler(C2S_CharCreate,		_T("C2S_CharCreate"),		ESS_Logined,	&nclient_session::handle_char_create);
		_register_client_msg_handler(C2S_CharDelete,		_T("C2S_CharDelete"),		ESS_Logined,	&nclient_session::handle_char_delete);
		_register_client_msg_handler(C2S_CharEnterGame,		_T("C2S_CharEnterGame"),	ESS_Logined,	&nclient_session::handle_char_enter_game);

		// loading
		_register_client_msg_handler(C2S_GetInitStat,		_T("C2S_GetInitStat"),		ESS_Loading,	&nclient_session::handle_get_init_stat);
		_register_client_msg_handler(C2S_GetInitSpell,		_T("C2S_GetInitSpell"),		ESS_Loading,	&nclient_session::handle_get_init_spell);
		_register_client_msg_handler(C2S_GetInitAura,		_T("C2S_GetInitAura"),		ESS_Loading,	&nclient_session::handle_get_init_aura);

		// map
		_register_client_msg_handler(C2S_MapEnter,			_T("C2S_MapEnter"),			ESS_Loaded,		&nclient_session::handle_map_enter);
		_register_client_msg_handler(C2S_TeleportAck,		_T("C2S_TeleportAck"),		ESS_Gaming,		&nclient_session::handle_map_teleport_ack);
		_register_client_msg_handler(C2S_ChatMessage,		_T("C2S_ChatMessage"),		ESS_Gaming,		&nclient_session::handle_player_chat);

		_register_client_msg_handler(C2S_CheckTime,			_T("C2S_CheckTime"),		ESS_Loading | ESS_Loaded | ESS_Gaming,		&nclient_session::handle_check_time);
		// movement (client map update)
		_register_client_msg_handler(C2S_Move,				_T("C2S_Move"),				ESS_Gaming,		&nclient_session::handle_player_move,			true);

		// spell (client map update)
		_register_client_msg_handler(C2S_CastSpell,			_T("C2S_CastSpell"),		ESS_Gaming,		&nclient_session::handle_cast_spell,			true);
		_register_client_msg_handler(C2S_CancelCast,		_T("C2S_CancelCast"),		ESS_Gaming,		&nclient_session::handle_cancel_cast,			true);
		_register_client_msg_handler(C2S_CancelAura,		_T("C2S_CancelAura"),		ESS_Gaming,		&nclient_session::handle_cancel_aura,			true);

		// cluster
		_register_cluster_msg_handler(Msg_Request_Login,		_T("Msg_Request_Login"),	ELSS_Connected, &nlan_session::handle_request_login);
		_register_cluster_msg_handler(Msg_Reply_Login,			_T("Msg_Reply_Login"),		ELSS_Connected, &nlan_session::handle_reply_login);
		_register_cluster_msg_handler(Msg_L2W_Player_Login,		_T("Msg_L2W_Player_Login"), ELSS_Logined,	&nlan_session::handle_player_login);
		_register_cluster_msg_handler(Msg_Player_Logout,		_T("Msg_Player_Logout"),	ELSS_Logined,	&nlan_session::handle_player_logout);
		
		// from db
		_register_cluster_msg_handler(Msg_D2W_CharEnum,				_T("Msg_D2W_CharEnum"),				ELSS_Logined,	&nlan_session::handle_char_enum_db);
		_register_cluster_msg_handler(Msg_D2W_CharCreate,			_T("Msg_D2W_CharCreate"),			ELSS_Logined,	&nlan_session::handle_char_create_db);
		_register_cluster_msg_handler(Msg_D2W_CharDelete,			_T("Msg_D2W_CharDelete"),			ELSS_Logined,	&nlan_session::handle_char_delete_db);

		_register_cluster_msg_handler(Msg_D2W_LoadPlayerStat,		_T("Msg_D2W_LoadPlayerStat"),		ELSS_Logined,	&nlan_session::handle_load_player_stat);
		_register_cluster_msg_handler(Msg_D2W_LoadPlayerItem,		_T("Msg_D2W_LoadPlayerItem"),		ELSS_Logined,	&nlan_session::handle_load_player_item);
		_register_cluster_msg_handler(Msg_D2W_LoadPlayerSpell,		_T("Msg_D2W_LoadPlayerSpell"),		ELSS_Logined,	&nlan_session::handle_load_player_spell);
		_register_cluster_msg_handler(Msg_D2W_LoadPlayerAura,		_T("Msg_D2W_LoadPlayerAura"),		ELSS_Logined,	&nlan_session::handle_load_player_aura);
		_register_cluster_msg_handler(Msg_D2W_LoadPlayerCoolDown,	_T("Msg_D2W_LoadPlayerCoolDown"),	ELSS_Logined,	&nlan_session::handle_load_player_cool_down);

		return true;
	}

	void nmsg_dispatch::destroy()
	{

	}

	ncluster_msg_handler* nmsg_dispatch::get_cluster_msg_handler(uint16 msg_id)
	{
		if (msg_id >= Msg_Max)
		{
			return NULL;
		}

		return &(m_cluster_msg_handler[msg_id]);
	}

	nclient_msg_handler* nmsg_dispatch::get_client_msg_handler(uint16 msg_id)
	{
		if (msg_id >= Msg_Max)
		{
			return NULL;
		}

		return &(m_client_msg_handler[msg_id]);
	}

	bool nmsg_dispatch::is_map_task_msg(uint16 msg_id)
	{
		if (msg_id >= Msg_Max)
		{
			return false;
		}

		return m_client_msg_handler[msg_id].map_task;
	}

	void nmsg_dispatch::_register_cluster_msg_handler(uint16 msg_id, const tstring& msg_name, uint32 status, cluster_handler_type handler)
	{
#ifdef _DEBUG
		if (m_cluster_msg_handler[msg_id].handler)
		{
			NLOG_ERROR(_T("register msg error, msg_id = %u, msg_name = %s"), msg_id, msg_name.c_str());
			return;
		}
#endif
		m_cluster_msg_handler[msg_id].msg_name		= msg_name;
		m_cluster_msg_handler[msg_id].status		= status;
		m_cluster_msg_handler[msg_id].handle_count	= 0;
		m_cluster_msg_handler[msg_id].handler		= handler;
	}

	void nmsg_dispatch::_register_client_msg_handler(uint16 msg_id, const tstring& msg_name, uint32 status, client_handler_type handler, bool map_task)
	{
#ifdef _DEBUG
		if (m_client_msg_handler[msg_id].handler)
		{
			NLOG_ERROR(_T("register msg error, msg_id = %u, msg_name = %s"), msg_id, msg_name.c_str());
			return;
		}
#endif
		m_client_msg_handler[msg_id].msg_name		= msg_name;
		m_client_msg_handler[msg_id].status			= status;
		m_client_msg_handler[msg_id].handle_count	= 0;
		m_client_msg_handler[msg_id].handler		= handler;
		m_client_msg_handler[msg_id].map_task		= map_task;
	}

	nmsg_dispatch g_msg_dispatch;

} // namespace nexus
