/**
 *	nexus ngame - nlan_session
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NGAME_NLAN_SESSION_H_
#define _NGAME_NLAN_SESSION_H_

#include "ncommon.h"
#include "nmsg_queue.h"

namespace nexus {

	struct nmessage;
	struct nmsg_base;

	class nlan_session : private nnoncopyable
	{
	public:
		nlan_session();
		virtual ~nlan_session();

		bool init(uint32 session_id, uint32 server_type, const std::string& address, uint16 port);
		void destroy();
		void update(uint32 elapse);

		bool push_msg(nmessage* msg_ptr) { return m_msg_queue.enqueue(msg_ptr); }
		void set_status(uint32 status)				{ m_status = status; }
		void set_sesssion_id(uint32 session_id)		{ m_session_id = session_id; }

		bool is_in_used() const { return 0XFFFFFFFF != m_session_id; }
		uint32 get_status() const { return m_status; }
		uint32 get_type() const { return m_server_type; }
		uint32 get_sesssion_id() const { return m_session_id; }

		bool send_message(const void* msg_ptr, uint32 size);

	public:
		// handler
		void handle_request_login(const nmsg_base* packet);
		void handle_reply_login(const nmsg_base* packet);
		void handle_player_login(const nmsg_base* packet);
		void handle_player_logout(const nmsg_base* packet);

		// character
		void handle_char_enum_db(const nmsg_base* packet);
		void handle_char_create_db(const nmsg_base* packet);
		void handle_char_delete_db(const nmsg_base* packet);

		// load from db
		void handle_load_player_stat(const nmsg_base* packet);
		void handle_load_player_item(const nmsg_base* packet);
		void handle_load_player_spell(const nmsg_base* packet);
		void handle_load_player_aura(const nmsg_base* packet);
		void handle_load_player_cool_down(const nmsg_base* packet);

	private:
		void _handle_msg();

	private:
		uint32			m_session_id;
		uint32			m_server_type;
		uint32			m_status;
		std::string		m_address;
		uint16			m_port;

		nmsg_queue		m_msg_queue;
	};

} // namespace nexus

#endif // _NGAME_NLAN_SESSION_H_
