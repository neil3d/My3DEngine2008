/**
 *	nexus nperfmon_test - nlocal_player_mgr
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	May, 2010
 */

#ifndef _NPERFMON_TEST_NLOCAL_PLAYER_MGR_H_
#define _NPERFMON_TEST_NLOCAL_PLAYER_MGR_H_

#include "ncommon.h"

namespace nexus {

	class nlocal_player;

	class nlocal_player_mgr
	{
	public:
		typedef boost::unordered_map<uint64, nlocal_player*>			LOCAL_PLAYER_MAP;

	public:
		nlocal_player_mgr();
		virtual ~nlocal_player_mgr();

		bool add_local_player(nlocal_player* player_ptr);
		void remove_local_player(nlocal_player* player_ptr);
		nlocal_player* get_local_player(uint64 guid) const;

	private:
		LOCAL_PLAYER_MAP				m_local_player_map;
	};


} // namespace nexus

#endif // _NPERFMON_TEST_NLOCAL_PLAYER_MGR_H_
