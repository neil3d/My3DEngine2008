#include "nworld_object.h"
#include "nmap.h"
#include "game_map/navigation_map.h"

#include "nmsg_movement.h"

namespace nexus {

	nworld_object::nworld_object() : m_map_id(0), m_instance_id(0), m_map_ptr(NULL)
	{
	}

	nworld_object::~nworld_object()
	{
	}

	bool nworld_object::init(uint32 map_id, uint32 instance_id, const vector3& pos, const vector3& rot)
	{
		m_map_id		= map_id;
		m_instance_id	= instance_id;

		m_controller_ptr->set_position(pos);
		m_controller_ptr->set_rotation(rot);
		m_controller_ptr->set_current_movement_type(gameframework::EMove_Ground);
		return true;
	}

	void nworld_object::destroy()
	{
	}

	// 先处理移动消息再update位置
	void nworld_object::update(uint32 elapse)
	{
		if (!m_map_ptr)
		{
			return;
		}

		gameframework::ngame_level* game_level_ptr = m_map_ptr->get_game_level();

		if (!game_level_ptr)
		{
			return;
		}

		m_controller_ptr->move(game_level_ptr, float(elapse) / 1000.0f);
	}

	uint16 nworld_object::move(const vector3& pos, const vector3& rotation, const vector3& acceleration, uint32 time_stamp, uint8 flag)
	{

		return 0;
	}

} // namespace nexus
