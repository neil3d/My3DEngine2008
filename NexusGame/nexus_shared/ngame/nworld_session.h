/**
 *	nexus ngame - nworld_session
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NGAME_NWORLD_SESSION_H_
#define _NGAME_NWORLD_SESSION_H_

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
		void update_world_msg(uint32 elapse);

		bool add_session(nclient_session* session_ptr);
		void remove_session(uint64 client_id);
		nclient_session* get_session(uint64 client_id);
		uint32 get_session_num() const	{ return uint32(m_sessions.size()); }

		bool push_msg(nmessage* msg_ptr) { return m_msg_queue.enqueue(msg_ptr); }

	private:
		void _handle_msg();

	private:
		typedef boost::unordered_map<uint64, nclient_session*>	CLIENT_SESSION_MAP;

	private:
		nmsg_queue			m_msg_queue;
		CLIENT_SESSION_MAP	m_sessions;
	};

	extern nworld_session g_world_session;

} // namespace nexus

#endif // _NGAME_NWORLD_SESSION_H_
