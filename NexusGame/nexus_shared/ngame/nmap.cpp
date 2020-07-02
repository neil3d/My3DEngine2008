#include "nmap.h"
#include "nmap_mgr.h"
#include "nmap_define.h"
#include "ntile.h"
#include "nunit.h"
#include "nplayer.h"
#include "ncreature.h"
#include "nobject_define.h"
#include "nclient_session.h"
#include "nregion.h"
#include "ai/ngraph_navigation_map.h"

#include "nmsg_movement.h"

namespace nexus {

	const int32 TILE_SIZE				= 5000;				// 50m
	const float CAL_AOI_DIS				= 100.0f;			// 1m

	nmap::nmap() : m_map_id(0), m_instance_id(0), m_map_info_ptr(NULL), /*m_nav_map_ptr(NULL), */m_game_level_ptr(NULL), m_tile_width(0), m_tile_height(0), m_tile_ptrs(NULL),
		m_visible_distance(0.0f), m_last_elapse(0)
	{
	}

	nmap::~nmap()
	{
	}

	bool nmap::init(uint32 instance_id, const nmap_info* map_info_ptr, gameframework::nlevel_info_resource::ptr level_res_ptr)
	{
		m_map_id		= map_info_ptr->info_id;
		m_instance_id	= instance_id;
		m_map_info_ptr = map_info_ptr;
		m_level_res_ptr = level_res_ptr;
		m_nav_map_ptr	= g_map_mgr.create_nav_map(map_info_ptr);

		if (!m_nav_map_ptr)
		{
			NLOG_ERROR(_T("create_nav_map error!"));
			return false;
		}

		m_game_level_ptr = new gameframework::ngame_level;

		if (!m_game_level_ptr)
		{
			NLOG_ERROR(_T("create ngame_level error!"));
			return false;
		}

		m_game_level_ptr->attach_nav_map(m_nav_map_ptr);
		// 根据导航图信息创建tile
		m_tile_width  = (int32(m_nav_map_ptr->get_raw_width() * m_nav_map_ptr->get_scale().x) + TILE_SIZE - 1) / TILE_SIZE;
		m_tile_height = (int32(m_nav_map_ptr->get_raw_height() * m_nav_map_ptr->get_scale().z) + TILE_SIZE - 1) / TILE_SIZE;

		m_tile_ptrs = new ntile[m_tile_width * m_tile_height];

		if (!m_tile_ptrs)
		{
			NLOG_ERROR(_T("alloc tile error, tile number = %u"), m_tile_width * m_tile_height);
			return false;
		}

		m_visible_distance = 3500.0f;			// 35m 半径

		m_update_timer.set_interval(160);
		static uint32 index = 0;

		if (++index % 2 == 0)
		{
			m_update_timer.set_current(80);		// 使地图的对象更新大概错开
		}
		// 地图实例的最大容量, 世界地图的话1000
		// 
		// 创建寻路导航图
		m_path_finder = new ngraph_navigation_map(m_nav_map_ptr);

		if (!m_path_finder)
		{
			NLOG_ERROR(_T("alloc ngraph_navigation_map error"));
			return false;
		}

		return true;
	}

	void nmap::destroy()
	{
		if (m_path_finder)
		{
			delete []m_path_finder;
			m_path_finder = NULL;
		}

		if (m_tile_ptrs)
		{
			delete []m_tile_ptrs;
			m_tile_ptrs = NULL;
		}

		//if (m_nav_map_ptr)
		//{
		//	delete []m_nav_map_ptr;
		//	m_nav_map_ptr = NULL;
		//}

		if (m_game_level_ptr)
		{
			delete []m_game_level_ptr;
			m_game_level_ptr = NULL;
		}
	}

	void nmap::update(uint32 elapse)
	{
		nclient_session* session_ptr = NULL;
		// 更新玩家的消息队列
		for (PLAYER_MAP::const_iterator iter = m_player_map.begin(); iter != m_player_map.end(); ++iter)
		{
			session_ptr = iter->second->get_session();

			if (session_ptr)
			{
				// 更新消息队列
				session_ptr->update(elapse, false);
	}
		}

		m_update_timer.update(elapse);

		// 更新地图中的对象
		if (m_update_timer.passed())
		{
			m_update_timer.reset();
			elapse += m_last_elapse;
			m_last_elapse = 0;

			for (PLAYER_MAP::const_iterator iter = m_player_map.begin(); iter != m_player_map.end(); ++iter)
			{
				iter->second->update(elapse);
			}

			for (CREATURE_MAP::const_iterator iter = m_creature_map.begin(); iter != m_creature_map.end(); ++iter)
			{
				iter->second->update(elapse);
			}
		}
		else
		{
			m_last_elapse += elapse;
	}
	}

	bool nmap::add_player(nplayer* player_ptr)
	{
		if (!player_ptr)
		{
			return false;
		}
	
		// 加入到player map
		if (!m_player_map.insert(std::make_pair(player_ptr->get_id(), player_ptr)).second)
		{
			return false;
		}

		// 设置玩家的地图指针
		player_ptr->set_map(this);
		player_ptr->set_map_id(m_map_id);
		player_ptr->set_instance_id(m_instance_id);

		// 根据高度图修改玩家坐标
		const vector3& pos = player_ptr->get_position();
		vector3 mod_pos = pos;
		mod_pos.y = m_nav_map_ptr->get_world_height(pos.x, pos.z);
		player_ptr->set_position(mod_pos);
		player_ptr->set_tile_pos(mod_pos);

		// TODO: D. Zhao 计算地图区域trigger, 可能更新玩家状态

		// aoi玩家相互之间数据同步
		_update_world_obejct_visibility_to_player(player_ptr);
		// 第一次潜行的肯定看不到
		if (true == player_ptr->is_visible())
		{
		_update_world_obejct_visibility(player_ptr, true);
		}

		// 加入到相应的ntile
		ntile_coord coord = _world2tile(mod_pos.x, mod_pos.z);
		uint32 tile_id = _get_tile_index(coord);
		ntile* tile_ptr = _get_tile(tile_id);

		if (!tile_ptr)
		{
			return false;
		}

		tile_ptr->add_player(player_ptr);
		player_ptr->set_tile_id(tile_id);
		return true;
	}

	void nmap::remove_player(nplayer* player_ptr, bool exit)
	{
		// 同步周围玩家删除自己
		ntile* tile_ptr = _get_tile(player_ptr->get_tile_id());

		if (!tile_ptr)
		{
			NLOG_ERROR(_T("remove player get tile error!"));
			return;
	}

		tile_ptr->remove_player(player_ptr);
		// aoi玩家相互之间数据同步
		if (!exit)
		{
			_update_world_obejct_visibility_to_player(player_ptr, false);
		}

		if (true == player_ptr->is_visible())
		{
		_update_world_obejct_visibility(player_ptr, false);
		}

		// 从player map删除
		m_player_map.erase(player_ptr->get_id());
		player_ptr->set_map(NULL);
		player_ptr->set_map_id(0);
		player_ptr->set_instance_id(0);
	}

	bool nmap::add_creature(ncreature* creature_ptr, bool map_loaded)
	{
		return true;
	}

	void nmap::remove_creature(ncreature* creature_ptr)
	{

	}

	nunit* nmap::get_unit(uint64 guid) const
	{
		switch (GET_TYPE(guid))
		{
		case EOT_PLAYER:
			{
				return (nunit*)get_player(guid);
			}
			break;
		case EOT_CREATURE:
			{
				return (nunit*)get_creature(guid);
			}
			break;
		default:
			{
				return NULL;
			}
			break;
		}

		return NULL;
	}

	nplayer* nmap::get_player(uint64 guid) const
	{
		PLAYER_MAP::const_iterator iter = m_player_map.find(guid);

		if (iter == m_player_map.end())
		{
			return NULL;
		}

		return iter->second;
	}

	ncreature* nmap::get_creature(uint64 guid) const
	{
		CREATURE_MAP::const_iterator iter = m_creature_map.find(PAIR64_LOPART(guid));

		if (iter == m_creature_map.end())
		{
			return NULL;
		}

		return iter->second;
	}

	void nmap::broadcast(nworld_object* obj_ptr, const void* msg_ptr, uint32 size)
	{
		broadcast(obj_ptr, msg_ptr, size, m_visible_distance);
	}

	void nmap::broadcast(nworld_object* obj_ptr, const void* msg_ptr, uint32 size, float dist)
	{
		const vector3& pos = obj_ptr->get_tile_pos();
		int32 start_x = 0, start_z = 0, end_x = 0, end_z = 0;
		_world2tile_mod(pos.x - dist, pos.z - dist, start_x, start_z);
		_world2tile_mod(pos.x + dist, pos.z + dist, end_x, end_z);

		ntile* tile_ptr = NULL;
		int32 i = 0, j = 0;

		for (i = start_x; i <= end_x; ++i)
	{
			for (j = start_z; j <= end_z; ++j)
			{
				tile_ptr = _get_tile(j * m_tile_width + i);

				if (tile_ptr)
				{
					tile_ptr->broadcast(m_broadcaster, obj_ptr, msg_ptr, size, dist);
	}
			}
		}

		m_broadcaster.broadcast();
	}

	void nmap::broadcast(nplayer* player_ptr, const void* msg_ptr, uint32 size, bool to_self)
	{
		broadcast(player_ptr, msg_ptr, size, m_visible_distance, to_self);
	}

	void nmap::broadcast(nplayer* player_ptr, const void* msg_ptr, uint32 size, float dist, bool to_self)
	{
		const vector3& pos = player_ptr->get_tile_pos();
		int32 start_x = 0, start_z = 0, end_x = 0, end_z = 0;
		_world2tile_mod(pos.x - dist, pos.z - dist, start_x, start_z);
		_world2tile_mod(pos.x + dist, pos.z + dist, end_x, end_z);

		ntile* tile_ptr = NULL;
		int32 i = 0, j = 0;

		for (i = start_x; i <= end_x; ++i)
		{
			for (j = start_z; j <= end_z; ++j)
			{
				tile_ptr = _get_tile(j * m_tile_width + i);

				if (tile_ptr)
				{
					tile_ptr->broadcast(m_broadcaster, player_ptr, msg_ptr, size, dist, to_self);
	}
			}
		}

		m_broadcaster.broadcast();
	}

	void nmap::map_broadcast(nplayer* player_ptr, const void* msg_ptr, uint32 size, bool to_self)
	{
		for (PLAYER_MAP::const_iterator iter = m_player_map.begin(); iter != m_player_map.end(); ++iter)
		{
			if (to_self || player_ptr != iter->second)
			{
				m_broadcaster.send_msg(iter->second, msg_ptr, size);
			}
		}

		m_broadcaster.broadcast();
	}

	void nmap::broadcast_event(nworld_object* obj_ptr, const void* data_ptr, uint32 size, float dist, uint32 mask, bool to_self, uint32 delay)
	{
		const vector3& pos = obj_ptr->get_tile_pos();
		int32 start_x = 0, start_z = 0, end_x = 0, end_z = 0;
		_world2tile_mod(pos.x - dist, pos.z - dist, start_x, start_z);
		_world2tile_mod(pos.x + dist, pos.z + dist, end_x, end_z);

		ntile* tile_ptr = NULL;
		int32 i = 0, j = 0;

		for (i = start_x; i <= end_x; ++i)
		{
			for (j = start_z; j <= end_z; ++j)
			{
				tile_ptr = _get_tile(j * m_tile_width + i);

				if (tile_ptr)
				{
					tile_ptr->broadcast_event(obj_ptr, data_ptr, size, dist, mask, to_self, delay);
				}
			}
		}
	}

	void nmap::map_broadcast_event(nworld_object* obj_ptr, const void* data_ptr, uint32 size, uint32 mask, bool to_self, uint32 delay)
	{
		if (0 != (EOT_PLAYER & mask))
		{
			for (PLAYER_MAP::const_iterator iter = m_player_map.begin(); iter != m_player_map.end(); ++iter)
			{
				if (to_self || obj_ptr != iter->second)
				{
					if (0 == delay)
					{
						iter->second->send_event(data_ptr, size);
					}
					else
					{
						iter->second->post_event(delay, data_ptr, size);
					}
				}
			}
		}

		if (0 != (EOT_CREATURE & mask))
		{
			for (CREATURE_MAP::const_iterator iter = m_creature_map.begin(); iter != m_creature_map.end(); ++iter)
			{
				if (to_self || obj_ptr != iter->second)
				{
					if (0 == delay)
					{
						iter->second->send_event(data_ptr, size);
					}
					else
					{
						iter->second->post_event(delay, data_ptr, size);
					}
				}
			}
		}
	}

	void nmap::get_objects(nworld_object* obj_ptr, nregion* region_ptr, std::list<nworld_object*>& objs, uint32 mask, bool include_self)
	{
		objs.clear();
		const vector3& pos = region_ptr->get_center();
		int32 start_x = 0, start_z = 0, end_x = 0, end_z = 0;
		float dist = region_ptr->get_max_radius();
		_world2tile_mod(pos.x - dist, pos.z - dist, start_x, start_z);
		_world2tile_mod(pos.x + dist, pos.z + dist, end_x, end_z);

		ntile* tile_ptr = NULL;
		int32 i = 0, j = 0;

		for (i = start_x; i <= end_x; ++i)
		{
			for (j = start_z; j <= end_z; ++j)
			{
				tile_ptr = _get_tile(j * m_tile_width + i);

				if (tile_ptr)
				{
					tile_ptr->get_objects(obj_ptr, region_ptr, objs, mask, include_self);
				}
			}
		}
	}

	void nmap::object_move(nworld_object* obj_ptr, const vector3& new_pos)
	{
		const vector3& update_pos = obj_ptr->get_tile_pos();
		// 判断update距离
		float x_dis = abs(update_pos.x - new_pos.x);
		float z_dis = abs(update_pos.z - new_pos.z);
		ntile* tile_ptr = NULL;

		uint32 size = 0;
		tagS2C_Move move;
		obj_ptr->make_move_msg(&move, size);

		if (x_dis > CAL_AOI_DIS || z_dis > CAL_AOI_DIS)
		{
			std::vector<nworld_object*> old_set;
			std::vector<nworld_object*> new_set;
			int32 start_x = 0, start_z = 0, end_x = 0, end_z = 0;
			int32 i = 0, j = 0;

			// old object set
			_world2tile_mod(update_pos.x - m_visible_distance, update_pos.z - m_visible_distance, start_x, start_z);
			_world2tile_mod(update_pos.x + m_visible_distance, update_pos.z + m_visible_distance, end_x, end_z);

			for (i = start_x; i <= end_x; ++i)
			{
				for (j = start_z; j <= end_z; ++j)
				{
					tile_ptr = _get_tile(j * m_tile_width + i);

					if (tile_ptr)
					{
						tile_ptr->mark_world_object(update_pos, m_visible_distance, old_set, EAF_Old);
	}
				}
			}

			// new object set
			_world2tile_mod(new_pos.x - m_visible_distance, new_pos.z - m_visible_distance, start_x, start_z);
			_world2tile_mod(new_pos.x + m_visible_distance, new_pos.z + m_visible_distance, end_x, end_z);

			for (i = start_x; i <= end_x; ++i)
	{
				for (j = start_z; j <= end_z; ++j)
				{
					tile_ptr = _get_tile(j * m_tile_width + i);

					if (tile_ptr)
					{
						tile_ptr->mark_world_object(new_pos, m_visible_distance, new_set, EAF_New);
					}
				}
			}

			// set obj的tile pos，计算tile是否发生变化
			obj_ptr->set_tile_pos(new_pos);
			_set_world_object_tile(obj_ptr, new_pos);

			// 计算进入进出集合
			/************************************************************************/
			/* old aoi region, O													*/
			/* new aoi region, N													*/
			/* set1(O & !N) set2(O & N) set3(!O & N)								*/
			/* obj广播set2的move													*/
			/* obj广播set1的leave													*/
			/* obj广播set3的enter													*/
			/* set1 to obj leave													*/
			/* set3 to obj enter													*/
			/************************************************************************/

			std::vector<nworld_object*> set1;
			std::vector<nworld_object*> set3;

			for (std::vector<nworld_object*>::const_iterator iter = old_set.begin(); iter != old_set.end(); ++iter)
			{
				switch ((*iter)->get_aoi_flag())
				{
				case EAF_All:
					{
						if (EOT_PLAYER == (*iter)->get_type())
						{
							m_broadcaster.send_msg((nplayer*)(*iter), &move, size);
						}
					}
					break;
				case EAF_Old:
					{
						set1.push_back(*iter);
					}
					break;
				default:
					{
					}
					break;
				}

				(*iter)->clear_aoi_flag();
			}

			m_broadcaster.broadcast();

			for (std::vector<nworld_object*>::const_iterator iter = new_set.begin(); iter != new_set.end(); ++iter)
			{
				if (EAF_New == (*iter)->get_aoi_flag())
				{
					(*iter)->clear_aoi_flag();
					set3.push_back(*iter);
				}
			}

			uint32 size2 = 0;
			tagS2C_ObjectLeaveAoi leave, leave2;
			obj_ptr->make_leave_msg(&leave, size);

			for (std::vector<nworld_object*>::const_iterator iter = set1.begin(); iter != set1.end(); ++iter)
			{
				if (EOT_PLAYER == (*iter)->get_type() && (*iter) != obj_ptr)
				{
					m_broadcaster.send_msg((nplayer*)(*iter), &leave, size);
				}

				(*iter)->make_leave_msg(&leave2, size2);

				if (EOT_PLAYER == obj_ptr->get_type() && (*iter) != obj_ptr)
				{
					((nplayer*)obj_ptr)->send_message(&leave2, size2);
				}
			}

			m_broadcaster.broadcast();

			EnterAoi enter, enter2;
			obj_ptr->make_enter_msg(&enter, size);

			for (std::vector<nworld_object*>::const_iterator iter = set3.begin(); iter != set3.end(); ++iter)
			{
				if (EOT_PLAYER == (*iter)->get_type())
				{
					m_broadcaster.send_msg((nplayer*)(*iter), &enter, size);
				}

				(*iter)->make_enter_msg(&enter2, size2);

				if (EOT_PLAYER == obj_ptr->get_type())
				{
					((nplayer*)obj_ptr)->send_message(&enter2, size2);
				}
			}

			m_broadcaster.broadcast();
		}
		else
		{
			broadcast(obj_ptr, &move, size);
		}
	}

	ntile_coord nmap::_world2tile(float wx, float wz) const
	{
		const vector3& pos = m_nav_map_ptr->get_location();
		return ntile_coord(int32(wx - pos.x) / TILE_SIZE, int32(wz - pos.z) / TILE_SIZE);
	}

	uint32 nmap::_get_tile_index(const ntile_coord& coord) const
	{
		assert(uint32(coord.x) < m_tile_width);
		assert(uint32(coord.y) < m_tile_height);
		return coord.y * m_tile_width + coord.x;
	}

	ntile* nmap::_get_tile(uint32 tile_index) const
	{
		if (tile_index >= m_tile_width * m_tile_height)
		{
			return NULL;
		}

		return &(m_tile_ptrs[tile_index]);
	}

	ntile* nmap::_get_tile(const ntile_coord& coord) const
	{
		return _get_tile(_get_tile_index(coord));
	}

	void nmap::_update_world_obejct_visibility_to_player(nplayer* player_ptr, bool enter_map)
	{
		const vector3& pos = player_ptr->get_tile_pos();
		float x_dis = 0.0f, z_dis = 0.0f;
		int32 i = 0, j = 0, start_x = 0, start_z = 0, end_x = 0, end_z = 0;
		uint32 size = 0;
		EnterAoi enter;
		tagS2C_ObjectLeaveAoi leave;
		ntile* tile_ptr = NULL;

		_world2tile_mod(pos.x - m_visible_distance, pos.z - m_visible_distance, start_x, start_z);
		_world2tile_mod(pos.x + m_visible_distance, pos.z + m_visible_distance, end_x, end_z);

		for (i = start_x; i <= end_x; ++i)
		{
			for (j = start_z; j <= end_z; ++j)
			{
				tile_ptr = _get_tile(j * m_tile_width + i);

				if (!tile_ptr)
				{
					continue;
				}

				// 玩家对象
				for (ntile::PLAYER_SET::const_iterator iter = tile_ptr->get_player_set().begin(); iter != tile_ptr->get_player_set().end(); ++iter)
				{
					// 判断距离
					x_dis = abs((*iter)->get_tile_pos().x - player_ptr->get_tile_pos().x);
					z_dis = abs((*iter)->get_tile_pos().z - player_ptr->get_tile_pos().z);

					if (x_dis < m_visible_distance && z_dis < m_visible_distance)
					{
						if ((*iter)->can_seen(player_ptr))
						{
							if (enter_map)
							{
							(*iter)->make_enter_msg(&enter, size);
								player_ptr->send_message(&enter, size);
							}
							else
							{
								(*iter)->make_leave_msg(&leave, size);
								player_ptr->send_message(&leave, size);
						}
					}
					}
				}

				// 非玩家对象
				for (ntile::WORLD_OBJECT_SET::const_iterator iter = tile_ptr->get_world_object_set().begin(); iter != tile_ptr->get_world_object_set().end(); ++iter)
				{
					// 判断距离
					x_dis = abs((*iter)->get_tile_pos().x - player_ptr->get_tile_pos().x);
					z_dis = abs((*iter)->get_tile_pos().z - player_ptr->get_tile_pos().z);

					if (x_dis < m_visible_distance && z_dis < m_visible_distance)
					{
						if ((*iter)->can_seen(player_ptr))
						{
							if (enter_map)
							{
							(*iter)->make_enter_msg(&enter, size);
								player_ptr->send_message(&enter, size);
							}
							else
							{
								(*iter)->make_leave_msg(&leave, size);
								player_ptr->send_message(&leave, size);
						}
					}
				}
			}
		}
		}
	}

	void nmap::_update_world_obejct_visibility(nworld_object* obj_ptr, bool visible)
	{
		uint32 size = 0;

		if (visible)
		{
			EnterAoi enter;
			obj_ptr->make_enter_msg(&enter, size);
			broadcast(obj_ptr, &enter, size);
		}
		else
		{
			tagS2C_ObjectLeaveAoi leave;
			obj_ptr->make_leave_msg(&leave, size);
			broadcast(obj_ptr, &leave, size);
		}
	}

	void nmap::_world2tile_mod(float wx, float wz, int32& tx, int32& tz)
	{
		const vector3& pos = m_nav_map_ptr->get_location();
		tx = int32(wx - pos.x) / TILE_SIZE;
		tz = int32(wz - pos.z) / TILE_SIZE;

		// mod
		tx = tx >= (int32)m_tile_width ? (int32)m_tile_width - 1 : tx;
		tx = tx > 0 ? tx : 0;

		tz = tz >= (int32)m_tile_height ? (int32)m_tile_height - 1 : tz;
		tz = tz > 0 ? tz : 0;
	}

	void nmap::_set_world_object_tile(nworld_object* obj_ptr, const vector3& new_pos)
	{
		uint32 new_tile_id = _get_tile_index(_world2tile(new_pos.x, new_pos.z));

		if (new_tile_id != obj_ptr->get_tile_id())
		{
			ntile* old_tile_ptr = _get_tile(obj_ptr->get_tile_id());
			ntile* tile_ptr = _get_tile(new_tile_id);

			if (EOT_PLAYER == obj_ptr->get_type())
			{
				old_tile_ptr->remove_player((nplayer*)obj_ptr);
				tile_ptr->add_player((nplayer*)obj_ptr);
			}
			else
			{
				old_tile_ptr->remove_world_object(obj_ptr);
				tile_ptr->add_world_object(obj_ptr);
			}

			obj_ptr->set_tile_id(new_tile_id);
	}
	}

} // namespace nexus
