/**
 *	nexus ndb - nmsg_dispatch
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Jun, 2010
 */

#ifndef _NDB_NMSG_DISPATCH_H_
#define _NDB_NMSG_DISPATCH_H_

#include "ncommon.h"
#include "nmsg_base_id.h"

namespace nexus {

	struct nmessage;
	class nlan_session;

	typedef void (nlan_session::*handler_type)(const nmsg_base* msg_ptr);

	struct nmsg_handler
	{
		tstring			msg_name;
		uint32			status;
		volatile uint32	handle_count;
		handler_type	handler;

		nmsg_handler() : msg_name(_T("")), status(0), handle_count(0), handler(NULL) {}
	};

	/**
	 *	nmsg_dispatch
	 */
	class nmsg_dispatch
	{
	public:
		nmsg_dispatch();
		~nmsg_dispatch();

		bool init();
		void destroy();

		nmsg_handler* get_msg_handler(uint16 msg_id);

	private:
		void _register_msg_handler(uint16 msg_id, const tstring& msg_name, uint32 status, handler_type handler);

	private:
		nmsg_handler		m_msg_handler[Msg_Max];
	};

	extern nmsg_dispatch g_msg_dispatch;

} // namespace nexus

#endif // _NDB_NMSG_DISPATCH_H_
