/**
 *	nexus nperfmon_test - nclient_session
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	May, 2010
 */


#ifndef _NPERFMON_TEST_NCLIENT_SESSION_H_
#define _NPERFMON_TEST_NCLIENT_SESSION_H_

#include "ncommon.h"

namespace nexus {

	struct nmessage;
	class nlocal_player;

	enum EClientSessionStatus
	{
		ECSS_None				= 0X0000,
		ECSS_Connected			= 0X0001,
		ECSS_ConnectTimeout		= 0X0002,
		ECSS_ConnectError		= 0X0004,
		ECSS_Logined			= 0X0008,
		ECSS_Loading			= 0X0010,
		ECSS_Loaded				= 0X0020,
		ECSS_Gaming				= 0X0040,
		ECSS_Disconnect			= 0X0080,
	};

	class nclient_session : private nnoncopyable
	{
	public:
		nclient_session();
		virtual ~nclient_session();

		bool init(uint32 session_id);
		void destroy();
		void update(uint32 elapse);

		bool send_message(const void* msg_ptr, uint32 size);

		uint32 get_sesssion_id() const				{ return m_session_id; }
		uint8 set_status() const					{ return m_status; }

		void set_sesssion_id(uint32 session_id)		{ m_session_id = session_id; }
		void set_status(uint8 status)				{ m_status = status; }

	public:
		void handle_connect(const nmsg_base* msg_ptr);
		void handle_disconnect(const nmsg_base* msg_ptr);
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
		uint32					m_session_id;
		uint8					m_status;
		nlocal_player*			m_player_ptr;				// session一一对应的角色对象


	private:
		// 生成登陆验证消息
		void make_proof_msg(void* tag_c2s_login_proof, uint32 index);
	public:
		// 登陆的开始账号
		static	uint32	m_index_login_account;
	};

} // namespace nexus

#endif //_NPERFMON_TEST_NCLIENT_SESSION_H_
