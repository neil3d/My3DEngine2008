#include "ntile.h"
#include "nworld_object.h"
#include "nplayer.h"
#include "nregion.h"

namespace nexus {

	void ntile::broadcast(nbroadcast_helper& helper, const void* msg_ptr, uint32 size)
	{
		for (PLAYER_SET::const_iterator iter = m_player_set.begin(); iter != m_player_set.end(); ++iter)
		{
			helper.send_msg(*iter, msg_ptr, size);
		}
	}

	void ntile::broadcast(nbroadcast_helper& helper, nplayer* player_ptr, const void* msg_ptr, uint32 size, bool to_self)
	{
		for (PLAYER_SET::const_iterator iter = m_player_set.begin(); iter != m_player_set.end(); ++iter)
		{
			if (to_self || player_ptr != *iter)
			{
				helper.send_msg(*iter, msg_ptr, size);
			}
		}
	}

	void ntile::broadcast(nbroadcast_helper& helper, nworld_object* obj_ptr, const void* msg_ptr, uint32 size, float radius)
	{
		float x_dis = 0.0f, z_dis = 0.0f;

		for (PLAYER_SET::const_iterator iter = m_player_set.begin(); iter != m_player_set.end(); ++iter)
		{
			// ÅÐ¶Ï¾àÀë
			x_dis = abs((*iter)->get_tile_pos().x - obj_ptr->get_tile_pos().x);
			z_dis = abs((*iter)->get_tile_pos().z - obj_ptr->get_tile_pos().z);

			if (x_dis < radius && z_dis < radius)
			{
				helper.send_msg(*iter, msg_ptr, size);
			}
		}
	}

	void ntile::broadcast(nbroadcast_helper& helper, nplayer* player_ptr, const void* msg_ptr, uint32 size, float radius, bool to_self)
	{
		float x_dis = 0.0f, z_dis = 0.0f;

		for (PLAYER_SET::const_iterator iter = m_player_set.begin(); iter != m_player_set.end(); ++iter)
		{
			// ÅÐ¶Ï¾àÀë
			x_dis = abs((*iter)->get_tile_pos().x - player_ptr->get_tile_pos().x);
			z_dis = abs((*iter)->get_tile_pos().z - player_ptr->get_tile_pos().z);

			if (x_dis < radius && z_dis < radius && (to_self || player_ptr != *iter))
			{
				helper.send_msg(*iter, msg_ptr, size);
			}
		}
	}

	void ntile::broadcast_event(nworld_object* obj_ptr, const void* data_ptr, uint32 size, float dist, uint32 mask, bool to_self, uint32 delay)
	{
		float x_dis = 0.0f, z_dis = 0.0f;

		if (0 != (EOT_PLAYER & mask))
		{
			for (PLAYER_SET::const_iterator iter = m_player_set.begin(); iter != m_player_set.end(); ++iter)
			{
				// ÅÐ¶Ï¾àÀë
				x_dis = abs((*iter)->get_tile_pos().x - obj_ptr->get_tile_pos().x);
				z_dis = abs((*iter)->get_tile_pos().z - obj_ptr->get_tile_pos().z);

				if ((x_dis * x_dis + z_dis * z_dis <= dist * dist) && (to_self || obj_ptr != *iter))
				{
					if (0 == delay)
					{
						(*iter)->send_event(data_ptr, size);
					}
					else
					{
						(*iter)->post_event(delay, data_ptr, size);
					}
				}
			}
		}

		mask &= (0XFFFFFFFF ^ EOT_PLAYER);

		if (0 != mask)
		{
			for (WORLD_OBJECT_SET::const_iterator iter = m_world_object_set.begin(); iter != m_world_object_set.end(); ++iter)
			{
				// ÅÐ¶Ï¾àÀë
				x_dis = abs((*iter)->get_tile_pos().x - obj_ptr->get_tile_pos().x);
				z_dis = abs((*iter)->get_tile_pos().z - obj_ptr->get_tile_pos().z);

				if ((x_dis * x_dis + z_dis * z_dis <= dist * dist) && (to_self || obj_ptr != *iter))
				{
					if (0 != (mask & (*iter)->get_type()))
					{
						if (0 == delay)
						{
							(*iter)->send_event(data_ptr, size);
						}
						else
						{
							(*iter)->post_event(delay, data_ptr, size);
						}
					}
				}
			}
		}
	}

	void ntile::get_objects(nworld_object* obj_ptr, nregion* region_ptr, std::list<nworld_object*>& objs, uint32 mask, bool include_self)
	{
		if (0 != (EOT_PLAYER & mask))
		{
			for (PLAYER_SET::const_iterator iter = m_player_set.begin(); iter != m_player_set.end(); ++iter)
			{
				if (region_ptr->is_in_region(*iter) && (include_self || obj_ptr != *iter))
				{
					objs.push_back(*iter);
				}
			}
		}

		mask &= (0XFFFFFFFF ^ EOT_PLAYER);

		if (0 != mask)
		{
			for (WORLD_OBJECT_SET::const_iterator iter = m_world_object_set.begin(); iter != m_world_object_set.end(); ++iter)
			{
				if (region_ptr->is_in_region(*iter) && (include_self || obj_ptr != *iter))
				{
					if (0 != (mask & (*iter)->get_type()))
					{
						objs.push_back(*iter);
					}
				}
			}
		}
	}

	void ntile::mark_world_object(const vector3& pos, float radius, std::vector<nworld_object*>& objects, uint8 flag)
	{
		float x_dis = 0.0f, z_dis = 0.0f;

		for (PLAYER_SET::const_iterator iter = m_player_set.begin(); iter != m_player_set.end(); ++iter)
		{
			// ÅÐ¶Ï¾àÀë
			x_dis = abs((*iter)->get_tile_pos().x - pos.x);
			z_dis = abs((*iter)->get_tile_pos().z - pos.z);

			if (x_dis < radius && z_dis < radius)
			{
				(*iter)->set_aoi_flag(flag);
				objects.push_back(*iter);
			}
		}

		for (WORLD_OBJECT_SET::const_iterator iter = m_world_object_set.begin(); iter != m_world_object_set.end(); ++iter)
		{
			// ÅÐ¶Ï¾àÀë
			x_dis = abs((*iter)->get_tile_pos().x - pos.x);
			z_dis = abs((*iter)->get_tile_pos().z - pos.z);

			if (x_dis < radius && z_dis < radius)
			{
				(*iter)->set_aoi_flag(flag);
				objects.push_back(*iter);
			}
		}
	}

} // namespace nexus
