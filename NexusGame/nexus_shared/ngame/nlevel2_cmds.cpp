#include "ngm_commands.h"
#include "nclient_session.h"
#include "nplayer.h"
#include "nmap.h"
#include "nspell_aura.h"
#include "nutil.h"
#include "nlog_mt.h"
#include "ninfo_data.h"

namespace nexus {

	void ngm_commands::_handle_add_item_cmd(const ncommand_param& param, nclient_session* session_ptr)
	{
		uint32 info_id = 0, num = 0, color = 0;
		param.get_param(0, info_id);
		param.get_param(1, num);
		param.get_param(2, color);

		NLOG_DEBUG(_T("recv additem command, info_id = %u, num = %u, color = %u"), info_id, num, color);
		return;
	}

	void ngm_commands::_handle_go_cmd(const ncommand_param& param, nclient_session* session_ptr)
	{
		uint32 map_id = 0;
		float pos_x = 0.0f, pos_z = 0.0f;
		param.get_param(0, pos_x);
		param.get_param(1, pos_z);
		param.get_param(2, map_id);

		NLOG_DEBUG(_T("recv go command, pos_x = %f, pos_z = %f, map_id = %u"), pos_x, pos_z, map_id);

		if (!session_ptr)
		{
		return;
	}

		nplayer* player_ptr = session_ptr->get_player();

		if (!player_ptr)
		{
			return;
		}

		if (0 == map_id)
		{
			map_id = player_ptr->get_map_id();
		}

		player_ptr->teleport_to(pos_x, 0.0f, pos_z, player_ptr->get_rotation().y, map_id);
		return;
	}

	void ngm_commands::_handle_level_up_cmd(const ncommand_param& param, nclient_session* session_ptr)
	{

	}

	void ngm_commands::_handle_aura_cmd(const ncommand_param& param, nclient_session* session_ptr)
	{
		if (!session_ptr)
		{
			return;
		}

		nstring aura_name;
		uint64 target = 0;
		param.get_param(0, aura_name);
		param.get_param(0, target);

		NLOG_DEBUG(_T("recv add aura command, target = %u, aura_name = %s"), target, aura_name.c_str());

		uint32 aura_id = s_util.crc32(aura_name.data(), uint32(aura_name.size() * sizeof(nstring::value_type)));

		nplayer* player_ptr = session_ptr->get_player();

		if (!player_ptr)
		{
			NLOG_DEBUG(_T("session get_player error"));
			return;
		}

		nunit* target_ptr = (nunit*)player_ptr;

		if (0 != target)
		{
			nmap* map_ptr = player_ptr->get_map();

			if (!map_ptr)
			{
				NLOG_DEBUG(_T("get map error"));
				return;
			}

			target_ptr = map_ptr->get_unit(target);

			if (!target_ptr)
			{
				NLOG_DEBUG(_T("get unit error"));
				return;
			}
		}


		nspell_aura_info const* aura_info_ptr = s_spell_aura_info.get_info(aura_id);

		if (!aura_info_ptr)
		{
			NLOG_DEBUG(_T("s_spell_aura_info.get_info error, id = %s"), aura_name.c_str());
			return;
		}

		naura* aura_ptr = naura::construct(target_ptr, player_ptr->get_id(), aura_info_ptr, NULL, 0);
		target_ptr->add_aura(aura_ptr);
	}

	void ngm_commands::_handle_unaura_cmd(const ncommand_param& param, nclient_session* session_ptr)
	{
		if (!session_ptr)
		{
			return;
		}

		uint32 slot = 0;
		uint64 target = 0;
		param.get_param(0, slot);
		param.get_param(0, target);

		NLOG_DEBUG(_T("recv remove aura command, target = %u, slot = %u"), target, slot);

		nplayer* player_ptr = session_ptr->get_player();

		if (!player_ptr)
		{
			NLOG_DEBUG(_T("session get_player error"));
			return;
	}

		nunit* target_ptr = (nunit*)player_ptr;

		if (0 != target)
		{
			nmap* map_ptr = player_ptr->get_map();

			if (!map_ptr)
			{
				NLOG_DEBUG(_T("get map error"));
				return;
			}

			target_ptr = map_ptr->get_unit(target);

			if (!target_ptr)
			{
				NLOG_DEBUG(_T("get unit error"));
				return;
			}
		}

		naura* aura_ptr =  NULL;
		nunit::naura_map auras = target_ptr->get_auras();

		for (nunit::naura_map::const_iterator iter = auras.begin(); iter != auras.end(); ++iter)
		{
			if (slot == iter->second->get_slot())
			{
				aura_ptr = iter->second;
				break;
			}
		}

		// 判断是否存在
		if (!aura_ptr)
		{
			NLOG_DEBUG(_T("get slot aura error, slot = %u"), slot);
			return;
		}

		// 判断是否可手动取消
		if (0 == (aura_ptr->get_interrupt_flags() & EAIF_Cancel))
		{
			NLOG_DEBUG(_T("cant cancel this aura, slot = %u"), slot);
			return;
		}

		// remove aura
		target_ptr->remove_aura(aura_ptr);
	}

	void ngm_commands::_handle_cast_spell_cmd(const ncommand_param& param, nclient_session* session_ptr)
	{
		if (!session_ptr)
		{
			return;
		}

		nstring spell_name;
		uint64 target_id = 0;
		vector3 target_pos;
		param.get_param(0, spell_name);
		param.get_param(1, target_id);
		param.get_param(2, target_pos.x);
		param.get_param(3, target_pos.z);
		param.get_param(4, target_pos.y);

		NLOG_DEBUG(_T("recv cast spell command, target_id = %u, spell_name = %s"), target_id, spell_name.c_str());

		uint32 spell_id = s_util.crc32(spell_name.data(), uint32(spell_name.size() * sizeof(nstring::value_type)));

		nplayer* player_ptr = session_ptr->get_player();

		if (!player_ptr)
		{
			NLOG_DEBUG(_T("session get_player error"));
			return;
		}

		player_ptr->cast_spell(spell_id, target_id, target_pos, 0, 0, 0, NULL, NULL);
	}

	void ngm_commands::_handle_cancel_cast_cmd(const ncommand_param& param, nclient_session* session_ptr)
	{
	}

	void ngm_commands::_handle_modify_hp_cmd(const ncommand_param& param, nclient_session* session_ptr)
	{
		uint32 value = 0;
		param.get_param(0, value);

		NLOG_DEBUG(_T("recv modify hp command, value = %u"), value);

		if (!session_ptr)
		{
			return;
		}

		nplayer* player_ptr = session_ptr->get_player();

		if (!player_ptr)
		{
			return;
		}

		uint32 max_value = player_ptr->get_data().get_uint32_value(EUF_UnitMaxHealth);

		if (value > max_value)
		{
			value = max_value;
		}

		player_ptr->set_health(value);
	}

	void ngm_commands::_handle_modify_energy_cmd(const ncommand_param& param, nclient_session* session_ptr)
	{
		uint32 value = 0;
		param.get_param(0, value);

		NLOG_DEBUG(_T("recv modify energy command, value = %u"), value);

		if (!session_ptr)
		{
			return;
		}

		nplayer* player_ptr = session_ptr->get_player();

		if (!player_ptr)
		{
			return;
		}

		uint32 max_value = player_ptr->get_data().get_uint32_value(EUF_UnitMaxEnergy);

		if (value > max_value)
		{
			value = max_value;
		}

		player_ptr->set_energy(value);
	}

	void ngm_commands::_handle_modify_mana_cmd(const ncommand_param& param, nclient_session* session_ptr)
	{
		uint32 value = 0;
		param.get_param(0, value);

		NLOG_DEBUG(_T("recv modify mana command, value = %u"), value);

		if (!session_ptr)
		{
			return;
		}

		nplayer* player_ptr = session_ptr->get_player();

		if (!player_ptr)
		{
			return;
		}

		uint32 max_value = player_ptr->get_data().get_uint32_value(EUF_UnitMaxMana);

		if (value > max_value)
		{
			value = max_value;
		}

		player_ptr->set_mana(value);
	}

	void ngm_commands::_handle_modify_reserve_stat_cmd(const ncommand_param& param, nclient_session* session_ptr)
	{
		int32 add_value = 0;
		param.get_param(0, add_value);

		NLOG_DEBUG(_T("recv modify reserve stat command, add_value = %d"), add_value);

		if (!session_ptr)
		{
			return;
		}

		nplayer* player_ptr = session_ptr->get_player();

		if (!player_ptr)
		{
			return;
		}

		// 
	}

	void ngm_commands::_handle_modify_exp_cmd(const ncommand_param& param, nclient_session* session_ptr)
	{
		int32 add_value = 0;
		param.get_param(0, add_value);

		NLOG_DEBUG(_T("recv modify exp command, add_value = %d"), add_value);

		if (!session_ptr)
		{
			return;
		}

		nplayer* player_ptr = session_ptr->get_player();

		if (!player_ptr)
		{
			return;
		}

		// 修改经验值
	}

	void ngm_commands::_handle_modify_money_cmd(const ncommand_param& param, nclient_session* session_ptr)
	{
		int32 add_value = 0;
		param.get_param(0, add_value);

		NLOG_DEBUG(_T("recv modify money command, add_value = %d"), add_value);

		if (!session_ptr)
		{
			return;
		}

		nplayer* player_ptr = session_ptr->get_player();

		if (!player_ptr)
		{
			return;
		}

		// 修改游戏币 
	}

} // namespace nexus
