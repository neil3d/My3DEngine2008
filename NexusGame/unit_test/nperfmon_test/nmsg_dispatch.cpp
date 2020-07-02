#include "nmsg_dispatch.h"
#include "nclient_session.h"
#include "nprotocol_base.h"
#include "nlog_mt.h"
#include "ndefine.h"

#include "nmsg_login.h"
#include "nmsg_char.h"
#include "nmsg_loading.h"
#include "nmsg_map.h"
#include "nmsg_stat.h"
#include "nmsg_movement.h"
#include "nmsg_chat.h"
#include "nmsg_spell.h"

namespace nexus {

	nmsg_dispatch::nmsg_dispatch()
	{
	}

	nmsg_dispatch::~nmsg_dispatch()
	{
	}

	bool nmsg_dispatch::init()
	{
		_register_client_msg_handler(CI_Connect,	_T("CI_Connect"),		0xFFFFFFFF,	&nclient_session::handle_connect);
		_register_client_msg_handler(CI_DisConnect,	_T("CI_DisConnect"),	0xFFFFFFFF,	&nclient_session::handle_disconnect);

		_register_client_msg_handler(S2C_LoginProofResult,	_T("S2C_LoginProofResult"),	ECSS_Connected,	&nclient_session::handle_login_proof_result);
		_register_client_msg_handler(S2C_CharEnum,			_T("S2C_CharEnum"),			ECSS_Logined,	&nclient_session::handle_char_enum);
		_register_client_msg_handler(S2C_CharCreate,		_T("S2C_CharCreate"),		ECSS_Logined,	&nclient_session::handle_char_create);
		_register_client_msg_handler(S2C_CharDelete,		_T("S2C_CharDelete"),		ECSS_Logined,	&nclient_session::handle_char_delete);
		_register_client_msg_handler(S2C_CharEnterGame,		_T("S2C_CharEnterGame"),	ECSS_Logined,	&nclient_session::handle_char_enter_game);

		_register_client_msg_handler(S2C_GetInitStat,		_T("S2C_GetInitStat"),		ECSS_Loading,	&nclient_session::handle_get_init_stat);
		_register_client_msg_handler(S2C_GetInitSpell,		_T("S2C_GetInitSpell"),		ECSS_Loading,	&nclient_session::handle_get_init_spell);
		_register_client_msg_handler(S2C_GetInitAura,		_T("S2C_GetInitAura"),		ECSS_Loading,	&nclient_session::handle_get_init_aura);

		_register_client_msg_handler(S2C_MapEnter,			_T("S2C_MapEnter"),			ECSS_Loaded,	&nclient_session::handle_map_enter);
		_register_client_msg_handler(S2C_TeleportTo,		_T("S2C_TeleportTo"),		ECSS_Gaming,	&nclient_session::handle_map_teleport_to);

		_register_client_msg_handler(S2C_StatUpdate,		_T("S2C_StatUpdate"),		ECSS_Gaming,	&nclient_session::handle_stat_update_stat);
		_register_client_msg_handler(S2C_RemoteStatUpdate,	_T("S2C_RemoteStatUpdate"),	ECSS_Gaming,	&nclient_session::handle_remote_stat_update_stat);

		_register_client_msg_handler(S2C_PlayerEnterAoi,	_T("S2C_PlayerEnterAoi"),	ECSS_Loaded | ECSS_Gaming,	&nclient_session::handle_player_enter_aoi);
		_register_client_msg_handler(S2C_Move,				_T("S2C_Move"),				ECSS_Gaming,	&nclient_session::handle_player_move);	
		_register_client_msg_handler(S2C_ObjectLeaveAoi,	_T("S2C_ObjectLeaveAoi"),	ECSS_Gaming,	&nclient_session::handle_object_leave_aoi);

		_register_client_msg_handler(S2C_ChatMessage,		_T("S2C_ChatMessage"),		ECSS_Gaming,	&nclient_session::handle_player_chat);

		_register_client_msg_handler(S2C_CastSpellResult,	_T("S2C_CastSpellResult"),	ECSS_Gaming,	&nclient_session::handle_cast_spell_result);
		_register_client_msg_handler(S2C_SpellPrepare,		_T("S2C_SpellPrepare"),		ECSS_Gaming,	&nclient_session::handle_spell_prepare);
		_register_client_msg_handler(S2C_SpellCasting,		_T("S2C_SpellCasting"),		ECSS_Gaming,	&nclient_session::handle_spell_casting);
		_register_client_msg_handler(S2C_SpellFailed,		_T("S2C_SpellFailed"),		ECSS_Gaming,	&nclient_session::handle_spell_failed);
		_register_client_msg_handler(S2C_SpellHit,			_T("S2C_SpellHit"),			ECSS_Gaming,	&nclient_session::handle_spell_hit);
		_register_client_msg_handler(S2C_AddAura,			_T("S2C_AddAura"),			ECSS_Gaming,	&nclient_session::handle_add_aura);
		_register_client_msg_handler(S2C_RemoveAura,		_T("S2C_RemoveAura"),		ECSS_Gaming,	&nclient_session::handle_remove_aura);
		_register_client_msg_handler(S2C_UpdateAura,		_T("S2C_UpdateAura"),		ECSS_Gaming,	&nclient_session::handle_update_aura);
		return true;
	}

	void nmsg_dispatch::destroy()
	{

	}

	nclient_msg_handler* nmsg_dispatch::get_client_msg_handler(uint16 msg_id)
	{
		if (msg_id >= Msg_Max)
		{
			return NULL;
		}

		return &(m_client_msg_handler[msg_id]);
	}

	void nmsg_dispatch::_register_client_msg_handler(uint16 msg_id, const tstring& msg_name, uint32 status, client_handler_type handler, bool map_task)
	{
		m_client_msg_handler[msg_id].msg_name		= msg_name;
		m_client_msg_handler[msg_id].status			= status;
		m_client_msg_handler[msg_id].handle_count	= 0;
		m_client_msg_handler[msg_id].handler		= handler;
	}

	nmsg_dispatch g_msg_dispatch;

} // namespace nexus
