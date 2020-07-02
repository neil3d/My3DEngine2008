#include "nhandler_common.h"
#include "nmap.h"
#include "nmap_mgr.h"
#include "nmsg_map.h"

namespace nexus {

	void nclient_session::handle_map_enter(const nmsg_base* msg_ptr)
	{
		// NLOG_DEBUG(_T("recv C2S_MapEnter message, client_id = %u, size = %u"), m_client_id, sizeof(tagC2S_MapEnter));

		CHECK_MSG_SIZE(msg_ptr, C2S_MapEnter);
		MSG_STATIC_CAST(data_ptr, msg_ptr, C2S_MapEnter);

		// 修改session状态
		m_status = ESS_Gaming;

		if (!g_map_mgr.player_enter(m_player_ptr))
		{
			m_status = ESS_Loaded;
			NLOG_ERROR(_T("player enter map error, player_id = "), m_player_ptr->get_id());
		}
	}

	void nclient_session::handle_map_teleport_ack(const nmsg_base* msg_ptr)
	{
		// NLOG_DEBUG(_T("recv C2S_TeleportAck message, client_id = %u, size = %u"), m_client_id, sizeof(tagC2S_TeleportAck));

		CHECK_MSG_SIZE(msg_ptr, C2S_TeleportAck);
		MSG_STATIC_CAST(data_ptr, msg_ptr, C2S_TeleportAck);

		if (!m_player_ptr->is_teleporting())
		{
			return;
		}

		m_player_ptr->set_teleporting(false);

		// 执行地图切换操作，目前测试只能在本地图传送
		vector3 pos = m_player_ptr->get_teleport_dest_pos();
		float yaw = m_player_ptr->get_teleport_dest_yaw();
		uint32 map_id = m_player_ptr->get_teleport_dest_map_id();

		if (map_id == m_player_ptr->get_map_id())
		{
			// 同一张地图内传送
		nmap* map_ptr = m_player_ptr->get_map();

		if (!map_ptr)
		{
			return;
		}

		navigation_map::ptr nav_map_ptr = map_ptr->get_nav_map();

		if (!nav_map_ptr)
		{
			return;
		}

		pos.y = nav_map_ptr->get_world_height(pos.x, pos.z);

		m_player_ptr->set_position(pos);
		m_player_ptr->set_rotation(vector3(0.0f, yaw, 0.0f));
		m_player_ptr->set_acceleration(vector3::zero);
		m_player_ptr->set_movement_type(0);

		map_ptr->object_move(m_player_ptr, pos);
			return;
	}
		else
		{
			// 地图切换传送，目前没有副本创建
			nmap* map_ptr = m_player_ptr->get_map();

			if (!map_ptr)
			{
				return;
			}

			nmap* dst_map_ptr = g_map_mgr.get_map(map_id, 0);

			if (!dst_map_ptr)
			{
				return;
			}

			navigation_map::ptr nav_map_ptr = dst_map_ptr->get_nav_map();

			if (!nav_map_ptr)
			{
				return;
			}

			// 原地图删除玩家
			map_ptr->remove_player(m_player_ptr, false);
			// 新地图添加玩家
			m_player_ptr->set_position(pos);
			m_player_ptr->set_rotation(vector3(0.0f, yaw, 0.0f));
			m_player_ptr->set_acceleration(vector3::zero);
			m_player_ptr->set_movement_type(0);

			dst_map_ptr->add_player(m_player_ptr);
			return;
		}
	}

} // namespace nexus
