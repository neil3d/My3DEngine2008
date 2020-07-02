/**
 *	nexus nperfmon_test - nlocal_player
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	May, 2010
 */

#ifndef _NPERFMON_TEST_NLOCAL_PLAYER_H_
#define _NPERFMON_TEST_NLOCAL_PLAYER_H_

#include "ncommon.h"
#include "nclient_session.h"
#include "nplayer.h"
#include "nmotion_master.h"

namespace nexus {

	class nclient_session;

	class nlocal_player : public nplayer
	{
	public:
		nlocal_player();
		virtual ~nlocal_player();

		bool init(nclient_session* session_ptr);
		virtual void destroy();
		virtual void update(uint32 elapse);

		nclient_session* get_session() const			{ return m_session_ptr; }
		bool send_messge(void* msg_ptr, uint32 size)	{ return m_session_ptr->send_message(msg_ptr, size); }


	private:
		nclient_session*			m_session_ptr;
		nmotion_master				m_motion_master;
	};

} // namespace nexus

#endif // _NPERFMON_TEST_NLOCAL_PLAYER_H_
