#include "nworld_object.h"
#include "nmap.h"
#include "ntimer.h"
#include "game_map/navigation_map.h"
#include "math/math_const.h"
#include "timer/ntimestamp.h"
#include "nlog_mt.h"
#include "nplayer.h"

#include "nevent_args.h"

#include "nmsg_movement.h"

namespace nexus {

	nworld_object::nworld_object() : m_guid(0), m_map_id(0), m_instance_id(0), m_map_ptr(NULL), m_client_move(false), m_time_stamp(0.0f), 
		m_custom_flag(0), m_aoi_flag(0), m_visible(true), m_tile_id(0)
	{
	}

	nworld_object::~nworld_object()
	{
	}

	bool nworld_object::init(uint32 map_id, uint32 instance_id, const vector3& pos, const vector3& rot)
	{
		m_map_id		= map_id;
		m_instance_id	= instance_id;
		m_tile_pos		= pos;

		m_move_controller.set_controlled(this);
		m_move_controller.set_position(pos);
		m_move_controller.set_rotation(rot);
		m_move_controller.set_current_movement_type(gameframework::EMove_Ground);
		return true;
	}

	void nworld_object::destroy()
	{
	}

	// 先处理移动消息再update位置
	void nworld_object::update(uint32 elapse)
	{
		m_event_processor.update(elapse);

		if (!m_map_ptr)
		{
			return;
		}

		gameframework::ngame_level* game_level_ptr = m_map_ptr->get_game_level();

		if (!game_level_ptr)
		{
			return;
		}

		// 优化move处理不同的npc move调用要简单

		m_move_controller.move(game_level_ptr, float(elapse) / 1000.0f);
			}

	void nworld_object::make_move_msg(void* data_ptr, uint32& size) const
	{
		tagS2C_Move* packet = static_cast<tagS2C_Move*>(data_ptr);
		packet->client_id	= 0;
		packet->msg_id		= S2C_Move;
		packet->unit_id		= get_id();
		packet->position		= get_position();
		packet->yaw				= get_rotation().y;
		packet->acceleration	= get_acceleration();
		packet->time_stamp		= m_client_move ? m_time_stamp : nexus::get_time();
		packet->flag			= get_movement_flag();
		size				= sizeof(tagS2C_Move);
	}

	void nworld_object::make_leave_msg(void* data_ptr, uint32& size) const
	{
		tagS2C_ObjectLeaveAoi* packet = static_cast<tagS2C_ObjectLeaveAoi*>(data_ptr);
		packet->client_id	= 0;
		packet->msg_id		= S2C_ObjectLeaveAoi;
		packet->object_id	= get_id();
		size				= sizeof(tagS2C_ObjectLeaveAoi);
	}

	void nworld_object::send_message_to_aoi(void* msg_ptr, uint32 size, bool to_self)
	{
		// NULL != m_map_ptr, in world
		if (m_map_ptr)
		{
			if (EOT_PLAYER == get_type())
			{
				m_map_ptr->broadcast((nplayer*)this, msg_ptr, size, to_self);
			}
			else
			{
				m_map_ptr->broadcast(this, msg_ptr, size);
			}
		}
	}


	float nworld_object::get_distance(nworld_object const* obj_ptr) const
	{
		float dx = get_position().x - obj_ptr->get_position().x;
		float dy = get_position().y - obj_ptr->get_position().y;
		float dz = get_position().z - obj_ptr->get_position().z;
		float size_factor = get_object_size() + obj_ptr->get_object_size();
		float dist = sqrt((dx * dx) + (dy * dy) + (dz * dz)) - size_factor;
		return (dist > 0.0f ? dist : 0.0f);
	}

	float nworld_object::get_distance(const vector3& pos) const
	{
		float dx = get_position().x - pos.x;
		float dy = get_position().y - pos.y;
		float dz = get_position().z - pos.z;
		float size_factor = get_object_size();
		float dist = sqrt((dx * dx) + (dy * dy) + (dz * dz)) - size_factor;
		return (dist > 0.0f ? dist : 0.0f);
	}

	bool nworld_object::is_in_dist(nworld_object const* obj_ptr, float dist) const
	{
		float dx = get_position().x - obj_ptr->get_position().x;
		float dy = get_position().y - obj_ptr->get_position().y;
		float dz = get_position().z - obj_ptr->get_position().z;
		float dist_sq = (dx * dx) + (dy * dy) + (dz * dz);
		float size_factor = get_object_size() + obj_ptr->get_object_size();
		float max_dist = size_factor + dist;
		return dist_sq < max_dist * max_dist;
	}

	bool nworld_object::is_in_dist(const vector3& pos, float dist) const
	{
		float dx = get_position().x - pos.x;
		float dy = get_position().y - pos.y;
		float dz = get_position().z - pos.z;
		float dist_sq = (dx * dx) + (dy * dy) + (dz * dz);
		float size_factor = get_object_size();
		float max_dist = size_factor + dist;
		return dist_sq < max_dist * max_dist;
	}

	bool nworld_object::is_in_range(nworld_object const* obj_ptr, float min_range, float max_range) const
	{
		float dx = get_position().x - obj_ptr->get_position().x;
		float dy = get_position().y - obj_ptr->get_position().y;
		float dz = get_position().z - obj_ptr->get_position().z;
		float dist_sq = (dx * dx) + (dy * dy) + (dz * dz);
		float size_factor = get_object_size() + obj_ptr->get_object_size();

		if (min_range > 0.0f)
		{
			float min_dist = min_range + size_factor;

			if (dist_sq < min_dist * min_dist)
			{
				return false;
			}
		}

		float max_dist = max_range + size_factor;
		return dist_sq < max_dist * max_dist;
	}

	bool nworld_object::line_of_sight(nworld_object const* obj_ptr) const
	{
		return line_of_sight(obj_ptr->get_position());
	}

	bool nworld_object::line_of_sight(const vector3& pos) const
	{
		ncheck_result result;
		navigation_map::ptr nav_map_ptr = m_map_ptr->get_nav_map();

		if (!nav_map_ptr)
		{
			return true;
		}

		// 身高
		return nav_map_ptr->line_of_sight(get_position(), pos, false, result);
	}

	uint16 nworld_object::move(const vector3& pos, const vector3& rotation, const vector3& acceleration, float time_stamp, uint8 flag)
	{
		float len_sp = vec_length_sq(acceleration);
		bool move = false;

		if (len_sp > 0.0f || flag & 0x02)
		{
			move = true;
		}

		bool turn = false;

		if (abs(get_rotation().y - rotation.y) >= 0.1f)
		{
			turn = true;
		}

		uint16 result = _can_move(pos, rotation, acceleration, time_stamp, flag, move, turn);
		result = EMRC_Success;

		if (EMRC_Success == result)
		{
			m_client_move = true;
			m_time_stamp = time_stamp;
			m_custom_flag = flag & 0xF8;
			// m_move_controller.set_position(pos);
			m_move_controller.set_rotation(rotation);
			m_move_controller.set_acceleration(acceleration);
			gameframework::nsavedmove::flags_to_controller(flag, &m_move_controller);
			m_map_ptr->object_move(this, pos);
			m_client_move = false;
			m_custom_flag = 0;

			// 发送事件
			nevent_move_args move_event;
			if (move)
			{
				move_event.only_turn = false;
				send_event(&move_event, sizeof(move_event));
		}
			else if (turn)
			{
				move_event.only_turn = true;
				send_event(&move_event, sizeof(move_event));
			}
		}
		else
		{
			// 发送失败消息
			if (EOT_PLAYER == get_type())
			{
				uint32 size = 0;
				tagS2C_Move move;
				make_move_msg(&move, size);
				((nplayer*)this)->send_message(&move, size);
		}
		}

		return result;
	}

	void nworld_object::stop_move()
	{
		float len_sp = vec_length_sq(get_acceleration());

		if (len_sp > 0.0f)
		{
			m_move_controller.set_acceleration(vector3::zero);
			gameframework::nsavedmove::flags_to_controller(0, &m_move_controller);
			m_map_ptr->object_move(this, get_position());
		}
	}

	void nworld_object::set_orient_to(const vector3& pos)
	{
		vector3 dir = pos - get_position();

		float len_sp = vec_length_sq(dir);

		if (len_sp > 0.0f)
		{
			float yaw = vec_calc_yaw(dir);
			set_rotation(vector3(0.0f, yaw, 0.0f));
		}
	}

	void nworld_object::_on_pos_change(const vector3& pos)
	{

	}

	uint16 nworld_object::_can_move(const vector3& pos, const vector3& rotation, const vector3& acceleration, float time_stamp, uint8 flag, bool move, bool turn)
	{
		// 检测是不是实数
		if (_isnan(pos.x) || _isnan(pos.y) || _isnan(pos.z) || _isnan(acceleration.x) || _isnan(acceleration.y) || _isnan(acceleration.z))
		{
			return EMRC_Invalid;
		}

		// 检测是不是无穷
		if (0 == _finite(pos.x) || 0 == _finite(pos.y) || 0 == _finite(pos.z) || 0 == _finite(acceleration.x) || 0 == _finite(acceleration.y) || 0 == _finite(acceleration.z))
		{
			return EMRC_Invalid;
		}

		// 判断时间戳的合法性,不能差一个tick时间
		if (time_stamp > g_game_timer.get_time_stamp() + 0.08)
		{
			NLOG_DEBUG(_T("error time stamp"));
			return EMRC_Invalid;
		}

		// 预测延时距离

		using namespace gameframework;

		uint16 result = EMRC_Success;

		if (move)
		{
			result = check_move(false);

		if (EMRC_Success != result)
		{
			return result;
		}
		}
		else if (turn)
		{
			result = check_move(true);

			if (EMRC_Success != result)
			{
				return result;
			}
		}

		// 检查误差是否在准许的范围内
		float dis_sq = vec_distance_sq(pos, get_position());

		//if (dis_sq > 100.0f * 100.0f)
		//{
		//	return EMRC_Invalid;
		//}

		return 0;
	}

} // namespace nexus
