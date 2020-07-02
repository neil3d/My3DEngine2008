/**
 *	nexus ngateway - nwan_session
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NGATEWAY_NWAN_SESSION_H_
#define _NGATEWAY_NWAN_SESSION_H_

#include "ncommon.h"

namespace nexus {

	struct nmessage;

	enum EWanSessionStatus
	{
		EWSS_None		= 0X0000,
		EWSS_Connected	= 0X0001,
		EWSS_Logined	= 0X0002,
		EWSS_Closed		= 0X0004,
		EWSS_Max
	};

	class nwan_session : private nnoncopyable
	{
	public:
		nwan_session();
		virtual ~nwan_session();

		bool init(uint32 session_id, uint32 address, uint16 port);
		void destroy();
		void update(uint32 elapse);

		bool is_in_used() const { return 0XFFFFFFFF != m_session_id; }
		void set_sesssion_id(uint32 session_id) { m_session_id = session_id; }
		uint32 get_sesssion_id() const { return m_session_id; }

		void set_status(uint32 status) { m_status = status; }
		uint32 get_status() const { return m_status; }

		void disconnect();

	private:
		uint32			m_session_id;
		uint32			m_status;
		uint32			m_address;
		uint16			m_port;
	};

} // namespace nexus

#endif //_NGATEWAY_NWAN_SESSION_H_
