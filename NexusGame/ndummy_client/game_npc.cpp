#include "StdAfx.h"
#include "game_npc.h"
#include "nobject_fields.h"

game_npc::game_npc(void)
{
	m_draw_color = RGB(138,43,226);
	m_controller_ptr.reset(new nremote_cc);

	m_data.init(EUF_UnitEnd);
}

game_npc::~game_npc(void)
{
}
