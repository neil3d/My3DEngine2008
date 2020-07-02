/**
 *	nexus nperfmon_test - nclient
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	May, 2010
 */

#ifndef _NPERFMON_TEST_CLIENT_H_
#define _NPERFMON_TEST_CLIENT_H_

#include "nnetwork.h"

namespace nexus {

	/**
	 *	大规模连接客户端
	 */
	class nclient : public ntcp_client_iocp
	{
	public:
		nclient();
		virtual ~nclient();

		virtual void on_connect(uint32 session_id, uint32 param1, uint32 param2);
		virtual void on_datarecv(uint32 session_id, nmessage* msg_ptr, uint32 size);
		virtual void on_disconnect(uint32 session_id);

		bool send_msg(uint32 session_id, const void* msg_ptr, uint32 size);
		nmessage* make_connect_msg(uint32 session_id, uint32 param1, uint32 param2);
		nmessage* make_disconnect_msg(uint32 session_id);
		nmessage* make_command_msg(nchar* command_str);

	private:
		nsafe_mem_pool			m_pool;
	};

	extern nclient g_client;

} // namespace nexus

#endif // _NPERFMON_TEST_CLIENT_H_
