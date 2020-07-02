/**
 *	nexus nperfmon_test - nmsg_dispatch
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	May, 2010
 */

#ifndef _NPERFMON_TEST_NMSG_DISPATCH_H_
#define _NPERFMON_TEST_NMSG_DISPATCH_H_

#include "ncommon.h"
#include "nmsg_base_id.h"

namespace nexus {

	struct nmsg_base;
	class nclient_session;

	typedef void (nclient_session::*client_handler_type)(const nmsg_base* msg_ptr);

	struct nclient_msg_handler
	{
		tstring					msg_name;
		uint32					status;
		uint32					handle_count;
		client_handler_type		handler;

		nclient_msg_handler() : msg_name(_T("")), status(0), handle_count(0), handler(NULL) {}
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

		nclient_msg_handler* get_client_msg_handler(uint16 msg_id);

	private:
		void _register_client_msg_handler(uint16 msg_id, const tstring& msg_name, uint32 status, client_handler_type handler, bool map_task = false);

	private:
		nclient_msg_handler			m_client_msg_handler[Msg_Max];
	};

	extern nmsg_dispatch g_msg_dispatch;

} // namespace nexus

#endif // _NPERFMON_TEST_NMSG_DISPATCH_H_
