#include "StdAfx.h"
#include "player_input.h"
#include "game_app.h"

#include "local_player.h"
#include "client.h"

#include "att_data/natt_info_data.h"

#include "nmsg_spell.h"

using namespace nexus;

player_input::player_input(void)
{
}

player_input::~player_input(void)
{
}

player_input* player_input::instance()
{
	static player_input s_inst;
	return &s_inst;
}

void player_input::on_mouse_left_down(const npoint& pt)
{
	// ×ø±êÏµ×ª»»
	const game_canvas& canvas = game_app::instance()->get_game_canvas();

	const map_view& view = canvas.get_view();

	vector3 pos = view.pixel2world(pt);

	local_player::instance()->set_move_dst_pos(pos);
}

void player_input::on_mouse_left_up()
{}

void player_input::on_mouse_right_down(const npoint& pt)
{}

void player_input::on_mouse_right_up()
{}

void player_input::on_mouse_wheel(int delta)
{
	game_app::instance()->scale_view(delta/120);
}

void player_input::on_mouse_move(const npoint& pt, bool ctrl_down)
{}

void player_input::on_key_down(int key)
{
	switch (key)
	{
	case 49:	// 1
		{
			nname spell_id(_T("test_cast_2"));
			tagC2S_CastSpell request;
			request.spell_id		= spell_id.name_crc;
			request.target_id		= local_player::instance()->get_id();
			request.target_pos.x	= 0.0f;
			request.target_pos.y	= 0.0f;
			request.target_pos.z	= 0.0f;
			request.time_stamp		= game_app::instance()->get_game_time();
			request.cast_count		= 1;
			client::instance()->send(&request, sizeof(request));
		}
		break;
	case 50:	// 2
		{
			float yaw = local_player::instance()->get_rotation().y;

			nname spell_id(_T("test_attack_2"));
			tagC2S_CastSpell request;
			request.spell_id		= spell_id.name_crc;
			request.target_id		= 0;
			request.target_pos.x	= local_player::instance()->get_pos().x - 1800.0f * sin(yaw);
			request.target_pos.y	= local_player::instance()->get_pos().y;
			request.target_pos.z	= local_player::instance()->get_pos().z - 1800.0f * cos(yaw);
			request.time_stamp		= game_app::instance()->get_game_time();
			request.cast_count		= 1;
			client::instance()->send(&request, sizeof(request));
		}
		break;
	case 51:	// 3
		{
			nname spell_id(_T("test_cast_1"));
			tagC2S_CastSpell request;
			request.spell_id		= spell_id.name_crc;
			request.target_id		= 0;
			request.target_pos.x	= local_player::instance()->get_pos().x + 100.0f;
			request.target_pos.y	= local_player::instance()->get_pos().y;
			request.target_pos.z	= local_player::instance()->get_pos().z + 100.0f;
			request.time_stamp		= game_app::instance()->get_game_time();
			request.cast_count		= 1;
			client::instance()->send(&request, sizeof(request));
		}
		break;
	case 52:	// 4
		{
			float yaw = local_player::instance()->get_rotation().y;

			nname spell_id(_T("test_attack_1"));
			tagC2S_CastSpell request;
			request.spell_id		= spell_id.name_crc;
			request.target_id		= 0;
			request.target_pos.x	= local_player::instance()->get_pos().x - 500.0f * sin(yaw);
			request.target_pos.y	= local_player::instance()->get_pos().y;
			request.target_pos.z	= local_player::instance()->get_pos().z - 500.0f * cos(yaw);
			request.time_stamp		= game_app::instance()->get_game_time();
			request.cast_count		= 1;
			client::instance()->send(&request, sizeof(request));
		}
		break;
	case 53:	// 5
		{
			float yaw = local_player::instance()->get_rotation().y;

			nname spell_id(_T("test_attack_1"));
			tagC2S_CastSpell request;
			request.spell_id		= spell_id.name_crc;
			request.target_id		= 0;
			request.target_pos.x	= local_player::instance()->get_pos().x - 500.0f * sin(yaw);
			request.target_pos.y	= local_player::instance()->get_pos().y;
			request.target_pos.z	= local_player::instance()->get_pos().z - 500.0f * cos(yaw);
			request.time_stamp		= game_app::instance()->get_game_time();
			request.cast_count		= 0;
			client::instance()->send(&request, sizeof(request));
		}
		break;
	case 54:	// 6
		{
			tagC2S_CancelCast request;
			client::instance()->send(&request, sizeof(request));
		}
		break;
	default:
		{
		}
		break;
	}
}
