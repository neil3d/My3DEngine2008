#pragma once

#include "ncommon.h"
#include "nprotocol_base.h"

/**
 *	客户端会话类
 */

enum EClientSessionStatus
{
	ECSS_None				= 0X0000,
	ECSS_Connected			= 0X0001,
	ECSS_Logined			= 0X0002,
	ECSS_Loading			= 0X0004,
	ECSS_Loaded				= 0X0008,
	ECSS_Gaming				= 0X0010,
	ECSS_Disconnect			= 0X0020,
};

class client_session
{
public:
	client_session();
	virtual ~client_session();

	static client_session* instance();

	void set_status(uint8 status)	{ m_status = status; }
	uint8 get_status() const		{ return m_status; }
	uint32 get_lag() const			{ return m_lag; }
	uint32 get_gateway_lag() const	{ return m_lag_count; }
	uint32 get_world_lag() const	{ return m_avg_lag; }

public:
	// handler
	void handle_login_proof_result(const nmsg_base* msg_ptr);
	void handle_char_enum(const nmsg_base* msg_ptr);
	void handle_char_create(const nmsg_base* msg_ptr);
	void handle_char_delete(const nmsg_base* msg_ptr);
	void handle_char_enter_game(const nmsg_base* msg_ptr);

	// loading
	void handle_get_init_stat(const nmsg_base* msg_ptr);
	void handle_get_init_spell(const nmsg_base* msg_ptr);
	void handle_get_init_aura(const nmsg_base* msg_ptr);



	// map
	void handle_map_enter(const nmsg_base* msg_ptr);
	void handle_map_teleport_to(const nmsg_base* msg_ptr);

	// stat update
	void handle_stat_update_stat(const nmsg_base* msg_ptr);
	void handle_remote_stat_update_stat(const nmsg_base* msg_ptr);

	// movement
	void handle_player_enter_aoi(const nmsg_base* msg_ptr);
	void handle_player_move(const nmsg_base* msg_ptr);
	void handle_object_leave_aoi(const nmsg_base* msg_ptr);

	void handle_check_time(const nmsg_base* msg_ptr);

	// chat
	void handle_player_chat(const nmsg_base* msg_ptr);

	// spell
	void handle_cast_spell_result(const nmsg_base* msg_ptr);
	void handle_spell_prepare(const nmsg_base* msg_ptr);
	void handle_spell_casting(const nmsg_base* msg_ptr);
	void handle_spell_failed(const nmsg_base* msg_ptr);
	void handle_spell_hit(const nmsg_base* msg_ptr);
	void handle_add_aura(const nmsg_base* msg_ptr);
	void handle_remove_aura(const nmsg_base* msg_ptr);
	void handle_update_aura(const nmsg_base* msg_ptr);

private:

	uint8 				m_status;
	uint32				m_lag;
	uint32				m_avg_lag;
	uint32				m_lag_count;
};
