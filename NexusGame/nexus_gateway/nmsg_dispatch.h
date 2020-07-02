/**
 *	nexus ngateway - nmsg_dispatch
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NGATEWAY_NMSG_DISPATCH_H_
#define _NGATEWAY_NMSG_DISPATCH_H_

#include "ncommon.h"
#include "nmsg_base_id.h"

namespace nexus {

	struct nmessage;

	/**
	 *	nmsg_dispatch
	 */
	class nmsg_dispatch
	{
	private:
		typedef void (nmsg_dispatch::*handler_type)(uint32 session_id, nmessage* msg_ptr, uint32 size);

		struct nmsg_handler
		{
			tstring			msg_name;
			uint32			status;
			volatile uint32	handle_count;
			handler_type	handler;

			nmsg_handler() : msg_name(_T("")), status(0), handle_count(0), handler(NULL) {}
		};

	public:
		nmsg_dispatch();
		~nmsg_dispatch();

		bool init();
		void destroy();

		void do_lan2wan_dispatch(uint32 session_id, nmessage* msg_ptr, uint32 size);
		void do_wan2lan_dispatch(uint32 session_id, nmessage* msg_ptr, uint32 size);

		void set_server_id(uint32 server_id) { m_server_id = server_id; }
		uint32 get_server_id() const { return m_server_id; }

		void handle_disconnect(uint32 session_id, nmessage* msg_ptr, uint32 size);

	private:
		void _register_lan2wan_msg_handler(uint16 msg_id, const tstring& msg_name, uint32 status, handler_type handler);
		void _register_wan2lan_msg_handler(uint16 msg_id, const tstring& msg_name, uint32 status, handler_type handler);
		void _register_lan2wan_default(uint16 msg_id);
		void _register_wan2lan_default(uint16 msg_id);

		// LAN to WAN 默认 nworld to ngateway
		void _handle_ngateway_to_nworld(uint32 session_id, nmessage* msg_ptr, uint32 size);
		void _handle_ngateway_to_nlogin(uint32 session_id, nmessage* msg_ptr, uint32 size);
		void _handle_ngateway_to_nworld_or_nbg(uint32 session_id, nmessage* msg_ptr, uint32 size);
		// WAN to LAN 默认 ncluster to ngateway
		void _handle_ncluster_to_ngateway(uint32 session_id, nmessage* msg_ptr, uint32 size);
		void _handle_ngateway_broadcast(uint32 session_id, nmessage* msg_ptr, uint32 size);

		void _handle_reply_login(uint32 session_id, nmessage* msg_ptr, uint32 size);
		void _handle_world_info(uint32 session_id, nmessage* msg_ptr, uint32 size);

		// client
		void _handle_player_login(uint32 session_id, nmessage* msg_ptr, uint32 size);

		void _handle_c2s_chat_message(uint32 session_id, nmessage* msg_ptr, uint32 size);
		void _handle_s2c_chat_message(uint32 session_id, nmessage* msg_ptr, uint32 size);

	private:
		nmsg_handler		m_l2w_msg_handler[Msg_Max];
		nmsg_handler		m_w2l_msg_handler[Msg_Max];

		uint32				m_server_id;				// world 分配给gateway的server id
	};

	extern nmsg_dispatch g_msg_dispatch;

} // namespace nexus

#endif // _NGATEWAY_NMSG_DISPATCH_H_
