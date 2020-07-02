#include "StdAfx.h"
#include "game_player.h"
#include "nobject_fields.h"

game_player::game_player(void)
{
	m_draw_color = RGB(0,255,0);
	m_controller_ptr.reset(new nremote_cc);

	m_data.init(EUF_UnitEnd);
}

game_player::~game_player(void)
{
}
