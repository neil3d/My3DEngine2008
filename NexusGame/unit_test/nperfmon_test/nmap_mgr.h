/**
 *	nexus nperfmon_test - nmap_mgr
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	May, 2010
 */

#ifndef _NGAME_NMAP_MGR_H_
#define _NGAME_NMAP_MGR_H_

#include "ncommon.h"
#include "base_define.h"
#include "math/vector.h"
#include "game_map/navigation_map.h"

namespace nexus {

	class nmap;
	class nplayer;
	class navigation_map;
	class nstd_file_system;

	class nmap_mgr : private nnoncopyable
	{
	public:
		nmap_mgr();
		virtual ~nmap_mgr();

		bool init();
		void destroy();
		void update(uint32 elapse);

		nmap* create_map();
		navigation_map::ptr create_nav_map(uint32 map_id);
		void destroy_nav_map(navigation_map* nav_map_ptr);

		nmap* get_map(uint32 map_id, uint32 instance_id) const;


	private:
		bool _load_map(nstd_file_system* fs_ptr, uint32 map_id, uint32 instance_num);

		bool _add_map(nmap* map_ptr);
		void _remove_map(nmap* map_ptr);
		navigation_map::ptr _get_nav_map(uint32 map_id);

	private:
		typedef boost::unordered_map<uint64, nmap*>					MAP_MAP_TYPE;
		typedef boost::unordered_map<uint32, navigation_map::ptr>	NAV_MAP_MAP_TYPE;

		MAP_MAP_TYPE		m_maps;
		NAV_MAP_MAP_TYPE	m_nav_maps;
	};

	extern nmap_mgr g_map_mgr;

} // namespace nexus

#endif // _NGAME_NMAP_MGR_H_
