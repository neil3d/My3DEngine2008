#include "nmotion_master.h"
#include "nlocal_player.h"
#include "nmap.h"
#include "nlog_mt.h"
#include "timer/ntimestamp.h"
#include "nperfmon_test.h"

#include "nmsg_movement.h"
#include "nmsg_spell.h"

#include <stdlib.h>

namespace nexus {

	nmotion_master::nmotion_master() : m_player_ptr(NULL), m_moving(false), m_attacking(false), m_time_stamp(0), m_elapse(0)
	{
	}

	nmotion_master::~nmotion_master()
	{
	}

	bool nmotion_master::init(nlocal_player* player_ptr)
	{
		// srand();
		m_player_ptr = player_ptr;
		return true;
	}

	void nmotion_master::destroy()
	{

	}

	//void nmotion_master::update(uint32 elapse)
	//{
	//	if (!m_moving)
	//	{
	//		const vector3& pos = m_player_ptr->get_position();
	//		int32 value = _rand(0, 360);
	//		float angle = nPI * 2 * float(value) / 360.0f;

	//		vector3  new_pos;
	//		new_pos.x = pos.x - 1000.0f * cos(angle);
	//		new_pos.z = pos.z - 1000.0f * sin(angle);
	//		m_acceleration = vec_normalize(new_pos - pos);

	//		m_time_stamp = ::timeGetTime();
	//		m_moving = true;
	//		m_elapse = 0;
	//		// 重新设置角色加速度
	//		m_player_ptr->set_acceleration(m_acceleration);
	//		m_player_ptr->set_rotation(vector3(0.0f, vec_calc_yaw(m_acceleration), 0.0f));

	//		_send_move_msg();
	//	}
	//	else
	//	{
	//		uint32 time_now = ::timeGetTime();

	//		if (time_now - m_time_stamp >= 300)
	//		{
	//			m_time_stamp = time_now;
	//			m_player_ptr->set_acceleration(m_acceleration);

	//			_send_move_msg();
	//		}
	//	}

	//	const vector3& pos = m_player_ptr->get_position();

	//	if ((abs(pos.x) > 65000 || abs(pos.z) > 65000) && 0 == m_elapse)
	//	{
	//		float yaw = m_player_ptr->get_rotation().y + nPI;

	//		if (yaw > nPI * 2)
	//		{
	//			yaw -= nPI * 2;
	//		}

	//		vector3  new_pos;
	//		new_pos.x = pos.x - 1000.0f * cos(yaw);
	//		new_pos.z = pos.z - 1000.0f * sin(yaw);
	//		m_acceleration = vec_normalize(new_pos - pos);

	//		m_time_stamp = ::timeGetTime();
	//		m_moving = true;
	//		m_elapse = 1;

	//		// 重新设置角色加速度
	//		m_player_ptr->set_acceleration(m_acceleration);
	//		m_player_ptr->set_rotation(vector3(0.0f, vec_calc_yaw(m_acceleration), 0.0f));

	//		_send_move_msg();
	//	}

	//	if (abs(pos.x) <= 65000 && abs(pos.z) <= 65000)
	//	{
	//		m_elapse = 0;
	//	}
	//}

	//void nmotion_master::update(uint32 elapse)
	//{
	//	m_elapse += elapse;

	//	if (m_elapse > 5 * 1000)
	//	{
	//		m_elapse = 0;
	//		m_moving = false;
	//	}

	//	if (m_moving)
	//	{
	//		uint32 time_now = ::timeGetTime();

	//		if (time_now - m_time_stamp >= 250)
	//		{
	//			m_time_stamp = time_now;
	//			m_player_ptr->set_acceleration(m_acceleration);

	//			_send_move_msg();
	//		}
	//	}
	//	else
	//	{
	//		const vector3& pos = m_player_ptr->get_position();
	//		int32 value = _rand(0, 360);
	//		float angle = nPI * 2 * float(value) / 360.0f;

	//		vector3  new_pos;
	//		new_pos.x = pos.x - 1000.0f * cos(angle);
	//		new_pos.z = pos.z - 1000.0f * sin(angle);
	//		m_acceleration = vec_normalize(new_pos - pos);
	//	
	//		m_time_stamp = ::timeGetTime();
	//		m_moving = true;
	//		// 重新设置角色加速度
	//		m_player_ptr->set_acceleration(m_acceleration);
	//		m_player_ptr->set_rotation(vector3(0.0f, vec_calc_yaw(m_acceleration), 0.0f));

	//		_send_move_msg();
	//	}
	//}

	void nmotion_master::update(uint32 elapse)
	{
		m_elapse += elapse;

		if (m_player_ptr->has_flag(EUS_Dead))
		{
			return;
		}

		if (!m_attacking)
		{
		if (m_elapse > 5 * 1000)
		{
			m_elapse = 0;
				m_attacking = true;
			m_moving = false;

				// 停下
				m_player_ptr->set_acceleration(vector3::zero);
				_send_move_msg();
				return;
		}

		if (m_moving)
		{
			uint32 time_now = ::timeGetTime();

			if (time_now - m_time_stamp >= 250)
			{
				m_time_stamp = time_now;
				m_player_ptr->set_acceleration(m_acceleration);

				_send_move_msg();
			}
		}
		else
		{
				m_moving = true;

			const vector3& pos = m_player_ptr->get_position();
			int32 value = _rand(0, 360);
			float angle = nPI * 2 * float(value) / 360.0f;

			vector3  new_pos;
			new_pos.x = pos.x - 1000.0f * cos(angle);
			new_pos.z = pos.z - 1000.0f * sin(angle);
			m_acceleration = vec_normalize(new_pos - pos);
		
			m_time_stamp = ::timeGetTime();
			m_moving = true;
			// 重新设置角色加速度
			m_player_ptr->set_acceleration(m_acceleration);
			m_player_ptr->set_rotation(vector3(0.0f, vec_calc_yaw(m_acceleration), 0.0f));

			_send_move_msg();
		}
	}
		else
		{
			if (m_elapse > 60 * 1000)
			{
				m_elapse = 0;
				m_attacking = false;
				_cancel_attack_msg();
				return;
			}

			if (!m_player_ptr->has_flag(EUS_Dead) && !m_player_ptr->has_flag(EUS_CycleCasting))
			{
				_send_attack_msg();
			}
		}
	}

	int32 nmotion_master::_rand(int32 min, int32 max)
	{
		return rand() % (max - min) + min;
	}

	void nmotion_master::_send_move_msg()
	{
		tagC2S_Move request;
		request.position = m_player_ptr->get_position();
		request.yaw = m_player_ptr->get_rotation().y;
		request.acceleration = m_player_ptr->get_acceleration();
		request.time_stamp = nperfmon_test::instance().get_time_stamp();
		request.flag = m_player_ptr->get_movement_flag();
		m_player_ptr->send_messge(&request, sizeof(request));
	}

	void nmotion_master::_send_attack_msg()
	{
		float yaw = m_player_ptr->get_rotation().y;

		nname spell_id(_T("test_attack_1"));
		tagC2S_CastSpell request;
		request.spell_id		= spell_id.name_crc;
		request.target_id		= 0;
		request.target_pos.x	= m_player_ptr->get_position().x - 500.0f * sin(yaw);
		request.target_pos.y	= m_player_ptr->get_position().y;
		request.target_pos.z	= m_player_ptr->get_position().z - 500.0f * cos(yaw);
		request.time_stamp		= 0;
		request.cast_count		= 0;
		m_player_ptr->send_messge(&request, sizeof(request));
	}

	void nmotion_master::_cancel_attack_msg()
	{
		tagC2S_CancelCast request;
		m_player_ptr->send_messge(&request, sizeof(request));
	}

} // namespace nexus
