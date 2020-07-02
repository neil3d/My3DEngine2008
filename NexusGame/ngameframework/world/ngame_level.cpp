#include "stdafx.h"
#include "ngame_level.h"
#include "../character_controller/ncharacter_controller.h"

namespace gameframework
{

	void ngame_level::attach_nav_map( navigation_map::ptr nav_map )
	{
		m_nav_map = nav_map;
	}

	bool ngame_level::try_move_character(ncharacter_controller* cc, const vector3& start, const vector3& disp, float setp_height, ncheck_result& hit )
	{
		if(m_nav_map.get() == NULL)
			return false;

		bool ret = false;
		vector3 last_pos = start;
		hit.dist = 0.0f;
		const vector3 scale = m_nav_map->get_scale();

		// 按照导航图的格子进行迭代检测
		const float step = std::min(scale.x, scale.z);		// 每次迭代的最小范围
		if(step < nEPS )
			return false;

		float disp_length_sq = vec_length(disp);			// 总计需要迭代多长范围
		const vector3 trace_dir = vec_normalize(disp);		// 迭代检测的方向
		npoint start_tile = m_nav_map->world2tile(start.x, start.z); // 起始格子
		float nav_height = m_nav_map->get_tile_height(start_tile.x, start_tile.y);
		npoint last_tile = start_tile;

		while( disp_length_sq > nEPS)
		{
			ret = true;
			float trace_len = min(disp_length_sq, step);
			vector3 trace_pos = last_pos + trace_dir * trace_len;
			last_tile = m_nav_map->world2tile(trace_pos.x, trace_pos.z); // 目标格子

			if(last_tile != start_tile)
			{
				// 新水平位置不可行走
				if( !m_nav_map->get_tile_walkable(last_tile.x, last_tile.y))
				{
					trace_pos.x = last_pos.x;
					trace_pos.z = last_pos.z;

					ret = false;
				}

				const float last_nav_height = m_nav_map->get_tile_height(last_tile.x, last_tile.y);
				// 计算步阶
				if( last_nav_height - nav_height > setp_height )
				{
					ret = false;
					//nLog_Debug(_T("try move min disp trace_len=%.3f nav_height=%.3f last_pos.y=%.3f\r\n"),trace_len, nav_height, last_pos.y);
					break;
				}

				nav_height = last_nav_height;
				start_tile = last_tile;
			}

			hit.dist += trace_len;

			if(!ret)
				break;

			last_pos = trace_pos;
			disp_length_sq -= trace_len;

			// 保持在地面上步进
			const float world_height = m_nav_map->get_world_height(last_pos.x, last_pos.z);
			if(last_pos.y - world_height < nEPS)
			{
				last_pos.y = world_height;
				cc->_on_land();
			}

			cc->set_position(last_pos);
		}

		if(!ret)
		{
			hit.location = last_pos;
			hit.normal = m_nav_map->get_tile_normal(last_tile.x, last_tile.y);
		}

		return ret;
	}

	bool ngame_level::try_transport(vector3& dest, bool place_nav_above )
	{
		if(m_nav_map.get() == NULL)
			return false;

		// 目标点为不可行走区域
		if(!m_nav_map->get_world_walkable(dest.x, dest.z))
		{
			return false;
		}

		bool ret = true;
		// 将目标点放置在导航图上方
		float nav_height = m_nav_map->get_world_height(dest.x, dest.z);
		if(dest.y < nav_height)
		{
			if(place_nav_above)
			{
				dest.y = nav_height;
			}
			else
			{
				ret = false;
			}
		}
		return ret;
	}
} // namespace gameframework