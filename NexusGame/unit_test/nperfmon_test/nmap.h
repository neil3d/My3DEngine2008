/**
 *	nexus nperfmon_test - nmap
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	May, 2010
 */

#ifndef _NPERFMON_TEST_NMAP_H_
#define _NPERFMON_TEST_NMAP_H_

#include "ncommon.h"
#include "ngameframework.h"

namespace nexus {

	class nworld_object;
	class nunit;
	class nplayer;
	class nlocal_player;
	class ncreature;
	class navigation_map;

	class nmap : private nnoncopyable
	{
	public:
		typedef boost::unordered_map<uint64, nplayer*>		REMOTE_PLAYER_MAP;			// 64id跨服扩展
		typedef boost::unordered_map<uint32, ncreature*>	CREATURE_MAP;

	public:
		nmap();
		virtual ~nmap();

		virtual bool init(uint32 map_id, gameframework::nlevel_info_resource::ptr level_res_ptr);
		virtual void destroy();
		virtual void update(uint32 elapse);

		uint32 get_map_id() const		{ return m_map_id; }
		uint32 get_instance_id() const	{ return m_instance_id; }
		navigation_map::ptr get_nav_map() const				{ return m_nav_map_ptr; }
		gameframework::ngame_level* get_game_level() const	{ return m_game_level_ptr; }
		uint32 get_player_num() const	{ return uint32(m_player_map.size()); }

		virtual bool add_player(nplayer* player_ptr);
		virtual void remove_player(nplayer* player_ptr);

		bool add_creature(ncreature* creature_ptr);
		void remove_creature(ncreature* creature_ptr);

		nunit* get_unit(uint64 guid) const;
		nplayer* get_player(uint64 guid) const;
		ncreature* get_creature(uint64 guid) const;

	private:
		// 对象容器
		REMOTE_PLAYER_MAP		m_player_map;
		CREATURE_MAP			m_creature_map;

		// 地图信息
		uint32					m_map_id;									// 地图类型id
		uint32					m_instance_id;								// 主世界地图为0
		navigation_map::ptr		m_nav_map_ptr;

		gameframework::ngame_level*					m_game_level_ptr;
		gameframework::nlevel_info_resource::ptr	m_level_res_ptr;
	};

} // namespace nexus

#endif // _NPERFMON_TEST_NMAP_H_
