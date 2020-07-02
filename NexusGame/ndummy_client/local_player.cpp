#include "StdAfx.h"
#include "local_player.h"
#include "client.h"
#include "client_session.h"
#include "game_app.h"
#include "timer/ntimestamp.h"
#include <MMSystem.h>

#pragma comment(lib, "Winmm.lib")

#include "nobject_fields.h"

#include "nmsg_movement.h"
#include "nmsg_chat.h"

local_player::local_player(void)
{
	m_moving		= false;
	m_time_stamp	= 0;
	m_stop_elapse	= 0;
	m_last_chat		= 0.0f;
	m_draw_color	= RGB(255,0,0);
	m_controller_ptr.reset(new nlocal_cc);

	m_data.init(EPF_PlayerEnd);
}

local_player::~local_player(void)
{
}

local_player* local_player::instance()
{
	static local_player s_inst;
	return &s_inst;
}

void local_player::update(float delta_time)
{
	if (ECSS_Gaming != client_session::instance()->get_status())
	{
		return;
	}

	game_unit::update(delta_time);

	if (m_moving && m_controller_ptr->reached_destination(m_controller_ptr->get_position(), m_move_dst))
	{
		m_moving = false;

		m_controller_ptr->set_acceleration(vector3::zero);

		// 发送停下消息
		tagC2S_Move request;
		request.position = m_controller_ptr->get_position();
		request.yaw = m_controller_ptr->get_rotation().y;
		request.time_stamp = game_app::instance()->get_game_time();
		request.flag = 0;

		client::instance()->send(&request, sizeof(request));
	}
	else if (m_moving)
	{
		uint32 time_now = ::timeGetTime();

		if (time_now - m_time_stamp >= 400)
		{
			m_time_stamp = time_now;
			m_controller_ptr->set_acceleration(vec_normalize(m_move_dst - m_controller_ptr->get_position()));

			tagC2S_Move request;
			request.position = m_controller_ptr->get_position();
			request.yaw = m_controller_ptr->get_rotation().y;
			request.acceleration = m_controller_ptr->get_acceleration();
			request.time_stamp = game_app::instance()->get_game_time();
			request.flag = 0;

			client::instance()->send(&request, sizeof(request));
		}
	}
	else
	{
		m_stop_elapse += delta_time;

		if (m_stop_elapse > 1.0f)
		{
			m_stop_elapse = 0.0f;

			tagC2S_Move request;
			request.position = m_controller_ptr->get_position();
			request.yaw = m_controller_ptr->get_rotation().y;
			request.time_stamp = game_app::instance()->get_game_time();
			request.flag = 0;

			client::instance()->send(&request, sizeof(request));
		}
	}

	m_last_chat += delta_time;

	if (m_last_chat > 1.0f)
	{
		m_last_chat = 0.0f;

		tagC2S_ChatMessage request;
		request.type = ECT_Channel;
		request.msg_size = 32;
		request.msg3.channel_id = ::timeGetTime();
		client::instance()->send(&request, request.get_packet_size());
}
}

void local_player::set_move_dst_pos(const vector3& dst)
{
	m_moving = true;
	m_move_dst = dst;
	m_time_stamp = ::timeGetTime();
	// 重新设置角色加速度
	vector3 acceleration = vec_normalize(m_move_dst - m_controller_ptr->get_position());
	m_controller_ptr->set_acceleration(acceleration);
	m_controller_ptr->set_rotation(vector3(0.0f, vec_calc_yaw(acceleration), 0.0f));

	tagC2S_Move request;
	request.position = m_controller_ptr->get_position();
	request.yaw = m_controller_ptr->get_rotation().y;
	request.acceleration = m_controller_ptr->get_acceleration();
	request.time_stamp = game_app::instance()->get_game_time();
	request.flag = 0;

	client::instance()->send(&request, sizeof(request));
}