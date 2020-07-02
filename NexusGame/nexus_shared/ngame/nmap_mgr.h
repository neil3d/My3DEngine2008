/**
 *	nexus ngame - nmap_mgr
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NGAME_NMAP_MGR_H_
#define _NGAME_NMAP_MGR_H_

#include "ncommon.h"
#include "ntimer.h"
#include "base_define.h"
#include "math/vector.h"
#include "game_map/navigation_map.h"
#include "ngameframework.h"

namespace nexus {

	class nmap;
	class nmap_instance;
	class nplayer;
	struct nmap_info;
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
		nmap_instance* create_instance();
		navigation_map::ptr create_nav_map(const nmap_info* map_info_ptr);
		void destroy_nav_map(navigation_map* nav_map_ptr);

		nmap* get_map(uint32 map_id, uint32 instance_id) const;

		// 串行层调用接口
		bool player_enter(nplayer* player_ptr);											// 玩家进入游戏世界地图
		void player_leave(nplayer* player_ptr);											// 玩家离开游戏世界地图
		// 并行层调用接口, 注意线程安全
		bool is_valid_coord(float x, float y, float z, float yaw, uint32 map_id);

	private:
		bool _load_map(nstd_file_system* fs_ptr, uint32 map_id, uint32 instance_num);

		bool _add_map(nmap* map_ptr);
		void _remove_map(nmap* map_ptr);
		navigation_map::ptr _get_nav_map(uint32 map_id);
		nmap* _create_map(const nmap_info* map_info_ptr, gameframework::nlevel_info_resource::ptr level_res_ptr);

	private:
		typedef boost::unordered_map<uint64, nmap*>		MAP_MAP_TYPE;
		typedef boost::unordered_map<uint32, nmap_info*>			MAP_INFO_MAP_TYPE;
		typedef boost::unordered_map<uint32, navigation_map::ptr>	NAV_MAP_MAP_TYPE;

		MAP_MAP_TYPE		m_maps;
		MAP_INFO_MAP_TYPE	m_map_infos;
		NAV_MAP_MAP_TYPE	m_nav_maps;

		uint32				m_max_instance_id;
	};

	extern nmap_mgr g_map_mgr;

} // namespace nexus

#endif // _NGAME_NMAP_MGR_H_
