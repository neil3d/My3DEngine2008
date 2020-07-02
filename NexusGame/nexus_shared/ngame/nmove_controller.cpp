#include "nmove_controller.h"
#include "math/ncheck_result.h"
#include "nworld_object.h"

namespace nexus {

	nmove_controller::nmove_controller() : m_obj_ptr(NULL)
	{
	}

	nmove_controller::~nmove_controller()
	{
	}

	bool nmove_controller::can_fly() const
	{
		// 判断是否存在飞行buff
		return false;
	}

	void nmove_controller::post_move()
	{
		m_obj_ptr->_on_pos_change(get_position());

		// npc的话savedmove调用网络发包，玩家服务器不通过savedmove发送消息
	}

	float nmove_controller::maxspeed_modifier()
	{
		float ret = 1.0f;

		switch (get_current_movement_type())
		{
		case gameframework::EMove_Ground:
			{
				if (get_walk())
				{
					ret *= 0.4f;
				}
			}
			break;
		case gameframework::EMove_Jump:
			{
			}
			break;
		case gameframework::EMove_Fly:
			{
			}
			break;
		}

		return ret;
	}

	void nmove_controller::notify_ground()
	{

	}

	void nmove_controller::notify_jump()
	{

	}

	void nmove_controller::notify_fly()
	{

	}

	void nmove_controller::notify_custom()
	{

	}

	void nmove_controller::notify_hit(ncheck_result* hit)
	{

	}

} // namespace nexus
