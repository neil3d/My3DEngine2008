#include "StdAfx.h"
#include "msg_dispatch.h"
#include "client_session.h"

#include "nmsg_login.h"
#include "nmsg_char.h"
#include "nmsg_loading.h"
#include "nmsg_map.h"
#include "nmsg_stat.h"
#include "nmsg_movement.h"
#include "nmsg_chat.h"
#include "nmsg_spell.h"

msg_dispatch::msg_dispatch() : m_recv_total_sec(0), m_recv_sec(0), m_time_stamp(0)
{
}

msg_dispatch::~msg_dispatch()
{
}

bool msg_dispatch::init()
{
	_register_client_msg_handler(S2C_LoginProofResult,	_T("S2C_LoginProofResult"),	ECSS_Connected,	&client_session::handle_login_proof_result);
	_register_client_msg_handler(S2C_CharEnum,			_T("S2C_CharEnum"),			ECSS_Logined,	&client_session::handle_char_enum);
	_register_client_msg_handler(S2C_CharCreate,		_T("S2C_CharCreate"),		ECSS_Logined,	&client_session::handle_char_create);
	_register_client_msg_handler(S2C_CharDelete,		_T("S2C_CharDelete"),		ECSS_Logined,	&client_session::handle_char_delete);
	_register_client_msg_handler(S2C_CharEnterGame,		_T("S2C_CharEnterGame"),	ECSS_Logined,	&client_session::handle_char_enter_game);
	_register_client_msg_handler(S2C_GetInitStat,		_T("S2C_GetInitStat"),		ECSS_Loading,	&client_session::handle_get_init_stat);
	_register_client_msg_handler(S2C_GetInitSpell,		_T("S2C_GetInitSpell"),		ECSS_Loading,	&client_session::handle_get_init_spell);
	_register_client_msg_handler(S2C_GetInitAura,		_T("S2C_GetInitAura"),		ECSS_Loading,	&client_session::handle_get_init_aura);
	_register_client_msg_handler(S2C_MapEnter,			_T("S2C_MapEnter"),			ECSS_Loaded,	&client_session::handle_map_enter);
	_register_client_msg_handler(S2C_TeleportTo,		_T("S2C_TeleportTo"),		ECSS_Gaming,	&client_session::handle_map_teleport_to);

	_register_client_msg_handler(S2C_StatUpdate,		_T("S2C_StatUpdate"),		ECSS_Gaming,	&client_session::handle_stat_update_stat);
	_register_client_msg_handler(S2C_RemoteStatUpdate,	_T("S2C_RemoteStatUpdate"),	ECSS_Gaming,	&client_session::handle_remote_stat_update_stat);

	_register_client_msg_handler(S2C_PlayerEnterAoi,	_T("S2C_PlayerEnterAoi"),	ECSS_Loaded | ECSS_Gaming,	&client_session::handle_player_enter_aoi);
	_register_client_msg_handler(S2C_Move,				_T("S2C_Move"),				ECSS_Gaming,	&client_session::handle_player_move);
	_register_client_msg_handler(S2C_ObjectLeaveAoi,	_T("S2C_ObjectLeaveAoi"),	ECSS_Gaming,	&client_session::handle_object_leave_aoi);

	_register_client_msg_handler(S2C_ChatMessage,		_T("S2C_ChatMessage"),		ECSS_Gaming,	&client_session::handle_player_chat);
	_register_client_msg_handler(S2C_CheckTime,			_T("S2C_CheckTime"),		ECSS_Loading | ECSS_Loaded | ECSS_Gaming,	&client_session::handle_check_time);

	_register_client_msg_handler(S2C_CastSpellResult,	_T("S2C_CastSpellResult"),	ECSS_Gaming,	&client_session::handle_cast_spell_result);
	_register_client_msg_handler(S2C_SpellPrepare,		_T("S2C_SpellPrepare"),		ECSS_Gaming,	&client_session::handle_spell_prepare);
	_register_client_msg_handler(S2C_SpellCasting,		_T("S2C_SpellCasting"),		ECSS_Gaming,	&client_session::handle_spell_casting);
	_register_client_msg_handler(S2C_SpellFailed,		_T("S2C_SpellFailed"),		ECSS_Gaming,	&client_session::handle_spell_failed);
	_register_client_msg_handler(S2C_SpellHit,			_T("S2C_SpellHit"),			ECSS_Gaming,	&client_session::handle_spell_hit);
	_register_client_msg_handler(S2C_AddAura,			_T("S2C_AddAura"),			ECSS_Gaming,	&client_session::handle_add_aura);
	_register_client_msg_handler(S2C_RemoveAura,		_T("S2C_RemoveAura"),		ECSS_Gaming,	&client_session::handle_remove_aura);
	_register_client_msg_handler(S2C_UpdateAura,		_T("S2C_UpdateAura"),		ECSS_Gaming,	&client_session::handle_update_aura);

	return true;
}


void msg_dispatch::update(uint32 elapse)
{
	client_msg* msg_ptr = m_msg_queue.dequeue();
	client_msg* cur_msg_ptr = NULL;

	while (msg_ptr)
	{
		_handle_msg(msg_ptr);

		cur_msg_ptr = msg_ptr;
		msg_ptr = msg_ptr->next_ptr;
		cur_msg_ptr->pool_ptr->free(cur_msg_ptr);
	}

	uint32 now = ::timeGetTime();
	uint32 elapse_ = now - m_time_stamp;

	if (elapse_ > 1000)
	{
		m_recv_sec = uint32(float(m_recv_total_sec * 1000) / float(elapse_));
		m_recv_total_sec = 0;
		m_time_stamp = now;
}
}

void msg_dispatch::destroy()
{

}

void msg_dispatch::_register_client_msg_handler(uint16 msg_id, const tstring& msg_name, uint32 status, client_handler_type handler)
{
	m_client_msg_handler[msg_id].msg_name		= msg_name;
	m_client_msg_handler[msg_id].status			= status;
	m_client_msg_handler[msg_id].handle_count	= 0;
	m_client_msg_handler[msg_id].handler		= handler;
}

void msg_dispatch::_handle_msg(client_msg* msg_ptr)
{
	const nmsg_base* packet = reinterpret_cast<const nmsg_base*>(msg_ptr->data);

	// 校验msg_id
	if (packet->msg_id >= Msg_Max)
	{
		nLog_Info(_T("error msg id"));
		return;
	}

	// 校验状态
	if (0 == (m_client_msg_handler[packet->msg_id].status & client_session::instance()->get_status()))
	{
		nLog_Info(_T("session status error!"));
		return;
	}

	(client_session::instance()->*(m_client_msg_handler[packet->msg_id].handler))(packet);
}

msg_dispatch g_msg_dispatch;
