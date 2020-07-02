/**
 *	nexus ngame - ntile
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NGAME_NTILE_H_
#define _NGAME_NTILE_H_

#include "ncommon.h"
#include "nbroadcast_helper.h"
#include "nlog_mt.h"
#include "base_define.h"
#include "math/vector.h"

namespace nexus {

	class nworld_object;
	class nplayer;
	class nregion;

	class ntile : private nnoncopyable
	{
	public:
		typedef boost::unordered_set<nplayer*>			PLAYER_SET;				// 玩家对象指针, 广播对象
		typedef boost::unordered_set<nworld_object*>	WORLD_OBJECT_SET;		// 非玩家对象指针，world_object类型指针(ncreature, ngame_object etc.)

	public:
		ntile() {}
		virtual ~ntile() {}

		void add_player(nplayer* player_ptr)
		{
			if (false == m_player_set.insert(player_ptr).second)
			{
				NLOG_ERROR(_T("tile insert player error"));
			}
		}
		// 不能删除失败, 否则广播时会挂掉
		void remove_player(nplayer* player_ptr)
		{
			m_player_set.erase(player_ptr);
		}

		void add_world_object(nworld_object* object_ptr)	
		{
			if (false == m_world_object_set.insert(object_ptr).second)
			{
				NLOG_ERROR(_T("tile insert creature error"));
			}
		}
		// 不能删除失败, 否则广播时会挂掉
		void remove_world_object(nworld_object* object_ptr)
		{
			m_world_object_set.erase(object_ptr);
		}

		// void add_game_object();
		// void add_dynamic_object();
		//
		void broadcast(nbroadcast_helper& helper, const void* msg_ptr, uint32 size);
		void broadcast(nbroadcast_helper& helper, nplayer* player_ptr, const void* msg_ptr, uint32 size, bool to_self);
		void broadcast(nbroadcast_helper& helper, nworld_object* unit_ptr, const void* msg_ptr, uint32 size, float radius);
		void broadcast(nbroadcast_helper& helper, nplayer* player_ptr, const void* msg_ptr, uint32 size, float radius, bool to_self);

		void broadcast_event(nworld_object* obj_ptr, const void* data_ptr, uint32 size, float dist, uint32 mask, bool to_self, uint32 delay);
		void get_objects(nworld_object* obj_ptr, nregion* region_ptr, std::list<nworld_object*>& objs, uint32 mask, bool include_self);

		// aoi move调用
		void mark_world_object(const vector3& pos, float radius, std::vector<nworld_object*>& objects, uint8 flag);

		const PLAYER_SET& get_player_set() const { return m_player_set; }
		const WORLD_OBJECT_SET& get_world_object_set() const { return m_world_object_set; }

	private:
		PLAYER_SET			m_player_set;
		WORLD_OBJECT_SET	m_world_object_set;
	};

} // namespace nexus

#endif // _NGAME_NTILE_H_
