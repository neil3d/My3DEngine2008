/**
 *	nexus ngame - nbroadcast_helper
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NGAME_NBROADCAST_HELPER_H_
#define _NGAME_NBROADCAST_HELPER_H_

#include "ncommon.h"
#include "nsafe_mem_pool.h"

namespace nexus {

	const uint32 MAX_GATEWAY_NUM = 32;

	struct nmessage;
	class nplayer;

	// 注意线程安全
	class nbroadcast_helper : private nnoncopyable
	{
	public:
		nbroadcast_helper();
		virtual ~nbroadcast_helper();

		bool send_msg(nplayer* player_ptr, const void* msg_ptr, uint32 size);
		void broadcast();

	private:
		nsafe_mem_pool			m_pool;
		uint32					m_session_ids[MAX_GATEWAY_NUM];
		nmessage*				m_msg_ptrs[MAX_GATEWAY_NUM];
	};

} // namespace nexus

#endif // _NGAME_NBROADCAST_HELPER_H_
