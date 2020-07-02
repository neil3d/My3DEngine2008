#include "stdafx.h"
#include "nsavedmove.h"
#include "../character_controller/ncharacter_controller.h"
#include "timer/ntimestamp.h"

namespace gameframework
{


	nsavedmove::nsavedmove(replicate_move_callback_ptr replicate)
	{
		m_num_replicates = 0;
		m_acceldotthreshold = 0.9f;
		m_max_response_time = 30.0f;

		m_replicate_move_callback = replicate;
	}

	nsavedmove::~nsavedmove()
	{

	}

	void nsavedmove::reset_move_parm( ncharacter_controller* controller )
	{
		// 保存新的移动参数
		m_saved_acceleration = controller->get_acceleration();
		m_saved_location = controller->get_position();
		m_saved_velocity = controller->get_velocity();
		m_saved_rotation = controller->get_rotation();

		m_walk = controller->get_walk();
		m_movement_type = controller->get_current_movement_type();
	}

	void nsavedmove::init( ncharacter_controller* controller )
	{
		m_timestamp = nexus::get_time();

		reset_move_parm(controller);
	}

	void nsavedmove::update(float delta_time, ncharacter_controller* controller)
	{
		float current_timestamp = nexus::get_time();

		if( current_timestamp - m_timestamp > m_max_response_time
			|| is_important_movement(controller)
			)
		{
			// 重置saved参数
			reset_move_parm(controller);

			// 同步移动 
			replicate_move();

			// 重置时间戳
			m_timestamp = current_timestamp;
		}
		else
		{
			// TODO: 处理一些可合并的移动，以减少同步的次数
			int i = 0;
			++ i;
		}
	}

	bool nsavedmove::is_important_movement(ncharacter_controller* controller)
	{
		if(controller->get_walk() != m_walk
			|| controller->get_current_movement_type() != m_movement_type)
		{
			return true;
		}

		vector3 accel_normal = vec_normalize(m_saved_acceleration);
		const vector3& compare_acceleration = controller->get_acceleration();

		return (accel_normal!=compare_acceleration) && (vec_dot(accel_normal,compare_acceleration)<m_acceldotthreshold);
	}

	void nsavedmove::flags_to_controller( nsavedmove::move_flags flags, ncharacter_controller* cc )
	{
		bool walk = (flags & EMFT_Walk)!=0;
		//bool jump = (flags & EMFT_Jump)!=0;
		bool fly = (flags & EMFT_Fly)!=0;
		bool press_jump = (flags & EMFT_PressJump)!=0;

		// 设置回角色控制器
		cc->set_walk(walk);
		if(press_jump)
		{
			cc->jump();
		}

		if(fly)
		{
			cc->fly();
		}
	}

	nsavedmove::move_flags nsavedmove::compressed_controller_flags( ncharacter_controller* cc, move_flags custom_flag )
	{
		move_flags ret = custom_flag;

		if(cc->get_walk())
		{
			ret |= EMFT_Walk;
		}
		else 
		{
			ret &= ~EMFT_Walk;
		}

		EMovementType current_move_type = cc->get_current_movement_type();
		if(current_move_type == EMove_Jump)
		{
			ret |= EMFT_Jump;
		}
		else
		{
			ret &= ~EMFT_Jump;
		}

		if(current_move_type == EMove_Fly)
		{
			ret |= EMFT_Fly;
		}
		else
		{
			ret &= ~EMFT_Fly;
		}

		return ret;
	}

	void nsavedmove::replicate_move()
	{
		++ m_num_replicates;

		if( m_replicate_move_callback.get() != NULL )
		{
			m_replicate_move_callback->on_replicate_move();
		}
	}
} // end of namespace gameframework