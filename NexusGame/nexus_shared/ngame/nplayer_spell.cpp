#include "nplayer.h"
#include "nlog_mt.h"
#include "ninfo_data.h"

#include "nmsg_movement.h"
#include "nmsg_spell.h"

namespace nexus {

	uint16 nplayer::check_move(bool only_turn)
	{
		uint32 result = nunit::check_move(only_turn);

		if (EMRC_Success != result)
		{
			return result;
		}

		if (has_flag(EUS_CycleCasting))
		{
			nspell_info const* spell_info_ptr = s_spell_info.get_info(m_casted_spell_id);

			if (!spell_info_ptr)
			{
				return EMRC_Casting;
			}

			if (only_turn)
			{
				if (!spell_info_ptr->can_turn)
				{
					return EMRC_Casting;
				}
			}
			else
			{
				if (!spell_info_ptr->can_move || !spell_info_ptr->can_turn)
				{
					return EMRC_Casting;
				}
			}
		}

		return EMRC_Success;		
	}

	bool nplayer::has_spell_cd(uint32 spell_id) const
	{
		nspell_info const* spell_info_ptr = s_spell_info.get_info(spell_id);

		if (!spell_info_ptr)
		{
			return true;
		}

		if (!spell_info_ptr->global_cd_not_effect)
		{
		// 公共cd
			uint32 category = ESCCDT_GlobalCD;

		nspell_cd_map::const_iterator iter = m_category_cd.find(category);

		if (iter != m_category_cd.end() && iter->second > time(NULL))
		{
			return true;
		}
		}

		// 判断类型cd
		//category = 1;
		//iter = m_category_cd.find(category);

		//if (iter != m_category_cd.end() && iter->second > time(NULL))
		//{
		//	return true;
		//}

		// 判断单体cd
		nspell_cd_map::const_iterator iter = m_spell_cd.find(spell_id);

		if (iter != m_spell_cd.end() && iter->second > time(NULL))
		{
			return true;
		}

		return false;
	}

	void nplayer::add_spell_cd(uint32 spell_id, time_t expire_time)
	{
		m_spell_cd[spell_id] = expire_time;
	}

	void nplayer::add_category_cd(uint32 category, time_t expire_time)
	{
		m_category_cd[category] = expire_time;
	}

	void nplayer::remove_spell_cd(uint32 spell_id, bool update)
	{
		m_spell_cd.erase(spell_id);

		if (update)
		{
			// 同步消息
		}
	}

	void nplayer::remove_category_cd(uint32 category, bool update)
	{
		m_category_cd.erase(category);

		if (update)
		{
			// 同步消息
		}

	}

	void nplayer::cast_generic_spell(uint32 spell_id, uint64 target_id, const vector3& target_pos, float time_stamp, uint32 cast_count)
	{
		tagS2C_CastSpellResult reply;
		reply.spell_id		= spell_id;

		// 判断技能cd
		if (has_spell_cd(spell_id))
		{
			reply.result = ESCR_Failed_CoolDown;
			send_message(&reply, sizeof(reply));
			return;
		}

		// 判断技能是否存在
		if (!has_spell(spell_id))
		{
			//reply.result = ESCR_Failed_NoSpell;
			//send_message(&reply, sizeof(reply));
			//return;
		}

		// 判断是否处于连续释放
		if (has_flag(EUS_CycleCasting))
		{
			reply.result = ESCR_Failed_SpellInProgress;
			send_message(&reply, sizeof(reply));
			return;
		}

		// 释放技能
		m_casted_spell_id	= spell_id;
		m_target_id			= target_id;
		m_target_pos		= target_pos;
		m_time_stamp		= time_stamp;
		m_cast_count		= cast_count;

		nspell_info const* spell_info_ptr = s_spell_info.get_info(m_casted_spell_id);

		if (!spell_info_ptr)
		{
			reply.result = ESCR_Failed_InvalidSpell;
			send_message(&reply, sizeof(reply));
			return;
		}

		// 检查技能信息
		if (!spell_info_ptr->active)
		{
			reply.result = ESCR_Failed_InvalidSpell;
			send_message(&reply, sizeof(reply));
			return;
		}

		if (1 != m_cast_count)
		{
			if (!spell_info_ptr->can_repeat || ESCast_Channel == spell_info_ptr->cast_type)
			{
				reply.result = ESCR_Failed_CantRepeat;
				send_message(&reply, sizeof(reply));
				return;
			}

			set_flag(EUS_CycleCasting);
			// 初始化连续释放参数
			m_cast_index		= 1;
			m_min_repeat_time	= int32(spell_info_ptr->min_repeat_time * 1000.0f);
			m_max_repeat_time	= int32(spell_info_ptr->max_repeat_time * 1000.0f);

			float base_time = 0.0f;

			if (ESCast_Cast == spell_info_ptr->cast_type)
			{
				base_time = spell_info_ptr->casting_time;
			}

			m_periodic = int32(((base_time + spell_info_ptr->cd_time) > spell_info_ptr->global_cd_time ? (base_time + spell_info_ptr->cd_time) : spell_info_ptr->global_cd_time) * 1000.0f);
		}
		else
		{
			m_cast_index = 0;
		}

		cast_spell(m_casted_spell_id, m_target_id, m_target_pos, m_time_stamp, m_cast_index, 0, 0, NULL, NULL);
	}

	void nplayer::cast_item_spell(uint32 spell_id, uint64 target_id, const vector3& target_pos, float time_stamp, uint64 item_guid)
	{
	}

	void nplayer::cancel_spell()
	{
		_clear_repeat_spell();
		remove_cur_spell(m_casted_spell_id);
	}

	void nplayer::handle_spell_modifier(nspell_aura_info const* mod_ptr, bool apply)
	{
		if (!mod_ptr)
		{
			return;
		}

		uint32 mod_type = mod_ptr->spell_mod_type;

		if (apply)
		{
			// 天赋之间不影响
			m_spell_mods.insert(std::make_pair(mod_type, mod_ptr));
		}
		else
		{
			nspell_mod_range_pair rang_pair = m_spell_mods.equal_range(mod_type);

			for (nspell_mod_map::const_iterator iter = rang_pair.first; iter != rang_pair.second; ++iter)
			{
				if (iter->second == mod_ptr)
				{
					m_spell_mods.erase(iter);
					break;
				}
			}
		}
	}

	void nplayer::_update_repeat_spell(uint32 elapse)
	{
		if (m_cast_index > 0)
		{
			tagS2C_CastSpellResult reply;
			reply.spell_id		= m_casted_spell_id;

			if (0 != m_max_repeat_time)
			{
				m_min_repeat_time -= elapse;
				m_max_repeat_time -= elapse;

				if (m_max_repeat_time < 0)
				{
					_clear_repeat_spell();
					reply.result = ESCR_Failed_Interrupted;
					send_message(&reply, sizeof(reply));
					return;
				}
			}

			m_periodic -= elapse;

			if (m_periodic < 0)
			{
				nspell_info const* spell_info_ptr = s_spell_info.get_info(m_casted_spell_id);

				if (!spell_info_ptr)
				{
					_clear_repeat_spell();
					reply.result = ESCR_Failed_InvalidSpell;
					send_message(&reply, sizeof(reply));
					return;
				}

				float base_time = 0.0f;

				if (ESCast_Cast == spell_info_ptr->cast_type)
				{
					base_time = spell_info_ptr->casting_time;
				}

				m_periodic += int32(((base_time + spell_info_ptr->cd_time) > spell_info_ptr->global_cd_time ? (base_time + spell_info_ptr->cd_time) : spell_info_ptr->global_cd_time) * 1000.0f);

				++m_cast_index;

				// debug 
				// NLOG_DEBUG(_T("repeat cast, index = %u"), m_cast_index);
				cast_spell(m_casted_spell_id, m_target_id, m_target_pos, m_time_stamp, m_cast_index, 0, 0, NULL, NULL);
			}
		}
	}

	void nplayer::_clear_repeat_spell()
	{
		remove_flag(EUS_CycleCasting);
		m_target_id			= 0;
		m_cast_count		= 0;
		m_cast_index		= 0;
		m_min_repeat_time	= 0;
		m_max_repeat_time	= 0;
		m_periodic			= 0;
	}

	void nplayer::_repeat_spell_interrupt(uint32 flag)
	{
		// 判断是否处于连续释放
		if (!has_flag(EUS_CycleCasting))
		{
			return;
		}

		nspell_info const* spell_info_ptr = s_spell_info.get_info(m_casted_spell_id);

		if (!spell_info_ptr)
		{
			return;
		}
	
		if (0 != (spell_info_ptr->interrupt_flags & flag))
		{
			cancel_spell();
		}
	}

	void nplayer::_resurrect(uint32 elapse)
	{
		if (has_flag(EUS_Dead))
		{
			m_resurrect -= elapse;

			if (m_resurrect <= 0)
			{
				remove_flag(EUS_Dead);

				uint32 max_hp = m_data.get_uint32_value(EUF_UnitMaxHealth);
				uint32 max_ep = m_data.get_uint32_value(EUF_UnitMaxEnergy);
				uint32 max_mp = m_data.get_uint32_value(EUF_UnitMaxMana);

				set_health(max_hp);
				set_energy(max_ep);
				set_mana(max_mp);
			}
		}
	}

	bool nplayer::_is_affected_by_spell_mod(nspell_info const* spell_ptr, nspell_aura_info const* mod_ptr)
	{
		if (!spell_ptr || !mod_ptr)
		{
			return false;
		}

		return false;
	}

} // namespace nexus
