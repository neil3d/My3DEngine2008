/**
 *	nexus ngame - nmap_instance
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NGAME_NMAP_INSTANCE_H_
#define _NGAME_NMAP_INSTANCE_H_

#include "nmap.h"

namespace nexus {

	class nmap_instance : public nmap
	{
	public:
		nmap_instance();
		virtual ~nmap_instance();

		virtual bool init();
		virtual void destroy();
		virtual void update(uint32 elapse);

		virtual bool add_player(nplayer* player_ptr);
		virtual int16 can_enter(nplayer* player_ptr) { return 0; }
		virtual void remove_player(nplayer* player_ptr, bool exit = true);

	private:

	};

} // namespace nexus

#endif // _NGAME_NMAP_INSTANCE_H_
