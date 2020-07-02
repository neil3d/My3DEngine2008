#include "StdAfx.h"
#include "commands.h"
#include "client.h"
#include "ConsoleDlg.h"
#include "local_player.h"

#include "att_data/natt_info_data.h"

#include "nmsg_login.h"
#include "nmsg_char.h"
#include "nmsg_map.h"
#include "nmsg_movement.h"
#include "nmsg_chat.h"
#include "nmsg_spell.h"

using namespace gameframework;

commands::commands()
{

}

commands::~commands()
{

}

commands* commands::instance()
{
	static commands s_inst;
	return &s_inst;
}

void commands::handle_chat(const console_cmd& cmd)
{
	tagC2S_ChatMessage request;
	uint32 type = 0;
	cmd.get_arg(0, type);
	request.type = type;
	nstring msg;
	cmd.get_arg(1, msg);
	request.msg_size = sizeof(nchar) * msg.length();
	memcpy(request.msg1.msg, msg.c_str(), request.msg_size);
	client::instance()->send(&request, request.get_packet_size());
}

void commands::handle_cast_spell(const console_cmd& cmd)
{
	nstring spell_name;
	cmd.get_arg(0, spell_name);

	nname spell_id(spell_name);

	nspell_info_dictionary::info_type_ptr spell_info_ptr = nspell_info_dictionary::instance()->find_info(spell_id);

	if (!spell_info_ptr)
	{
		nLog_Error(_T("cant find the spell, spell_id = %s!"), spell_id.name_str.c_str());
		return;
	}

	tagC2S_CastSpell request;
	request.spell_id		= spell_id.name_crc;
	request.target_id		= 0;
	request.target_pos.x	= local_player::instance()->get_pos().x + 100.0f;
	request.target_pos.y	= local_player::instance()->get_pos().y;
	request.target_pos.z	= local_player::instance()->get_pos().z + 100.0f;
	request.time_stamp		= 0;
	request.cast_count		= 1;
	client::instance()->send(&request, sizeof(request));
}
