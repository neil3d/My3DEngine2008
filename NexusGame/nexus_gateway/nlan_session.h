/**
 *	nexus ngateway - nlan_session
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NGATEWAY_NLAN_SESSION_H_
#define _NGATEWAY_NLAN_SESSION_H_

#include "ncommon.h"

namespace nexus {

	struct nmessage;

	class nlan_session : private nnoncopyable
	{
	public:
		nlan_session();
		virtual ~nlan_session();

		bool init(uint32 session_id, uint32 server_type, uint32 address, uint16 port);
		void destroy();
		void update(uint32 elapse);

		bool is_in_used() const { return 0XFFFFFFFF != m_session_id; }
		void set_status(uint32 status) { m_status = status; }
		uint32 get_status() const { return m_status; }
		uint32 get_type() const { return m_server_type; }
		void set_sesssion_id(uint32 session_id) { m_session_id = session_id; }
		uint32 get_sesssion_id() const { return m_session_id; }

	private:
		uint32			m_session_id;
		uint32			m_server_type;
		uint32			m_status;
		uint32			m_address;
		uint16			m_port;
	};

} // namespace nexus

#endif // _NGATEWAY_NLAN_SESSION_H_
