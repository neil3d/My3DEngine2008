/**
 *	nexus nperfmon_test - nworld_session
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	May, 2010
 */

#ifndef _NPERFMON_TEST_NWORLD_SESSION_H_
#define _NPERFMON_TEST_NWORLD_SESSION_H_

#include "ncommon.h"
#include "nmsg_queue.h"

namespace nexus {

	class nclient_session;

	class nworld_session : private nnoncopyable
	{
	public:
		nworld_session();
		virtual ~nworld_session();

		bool init();
		void destroy();
		void update(uint32 elapse);

		bool add_session(nclient_session* session_ptr);
		void remove_session(uint32 sesssion_id);
		nclient_session* get_session(uint32 sesssion_id);

		bool push_msg(nmessage* msg_ptr) { return m_msg_queue.enqueue(msg_ptr); }
		bool push_cmd(nmessage* msg_ptr) { return m_cmd_queue.enqueue(msg_ptr); }

	private:
		void _handle_msg();
		void _handle_cmd();

	private:
		typedef boost::unordered_map<uint32, nclient_session*>	CLIENT_SESSION_MAP;

	private:
		nmsg_queue			m_msg_queue;
		nmsg_queue			m_cmd_queue;
		CLIENT_SESSION_MAP	m_sessions;
	};

	extern nworld_session g_world_session;

} // namespace nexus

#endif // _NPERFMON_TEST_NWORLD_SESSION_H_
