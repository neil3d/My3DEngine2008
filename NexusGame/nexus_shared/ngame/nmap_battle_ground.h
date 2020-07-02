/**
 *	nexus ngame - nmap_battle_ground
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NGAME_NMAP_BATTLE_GROUND_H_
#define _NGAME_NMAP_BATTLE_GROUND_H_

#include "nmap.h"

namespace nexus {

	class nmap_battle_ground : public nmap
	{
	public:
		nmap_battle_ground();
		virtual ~nmap_battle_ground();

		virtual bool init();
		virtual void destroy();
		virtual void update(uint32 elapse);

		virtual bool add_player(nplayer* player_ptr);
		virtual int16 can_enter(nplayer* player_ptr) { return 0; }
		virtual void remove_player(nplayer* player_ptr, bool exit = true);

	private:

	};

} // namespace nexus

#endif // _NGAME_NMAP_BATTLE_GROUND_H_
