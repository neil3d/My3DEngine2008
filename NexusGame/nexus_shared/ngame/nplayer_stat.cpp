#include "nplayer.h"
#include "nlog_mt.h"

namespace nexus {

	void nplayer::_update_base_stats(uint32 stat_index)
	{
		// 按照公式计算出最终值, 设置该值, 同步
		float value = m_nude_stats[stat_index] + float(m_data.get_int32_value(stat_index + EUF_UnitEnd - 1));
		m_data.set_stat_uint32_value(stat_index, uint32(_get_total_stat_value(stat_index, value)));
		// 更新基本属性影响的属性
		switch (stat_index)
		{
		case EUF_UnitStamina:
			{
				_update_max_health();
				_update_melee_damage();
			}
			break;
		case EUF_UnitVitality:
			{
				_update_max_energy();
				_update_kungfu_damage();
			}
			break;
		case EUF_UnitSpirit:
			{
				_update_max_mana();
				_update_spell_damage();
			}
			break;
		case EUF_UnitGenGu:
			{
				_update_agility();
				_update_health_regeneration();
				_update_power_Regeneration();
			}
			break;
		default:
			{
			}
			break;
		}
	}

	void nplayer::_update_max_health()
	{
		uint32 cur_hp = m_data.get_uint32_value(EUF_UnitHealth);
		float class_rate[ECT_Max] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
		uint8 class_index = get_class();
		float value = (m_data.get_uint32_value(EUF_UnitLevel) * 2000.0f  + m_data.get_uint32_value(EUF_UnitStamina) * 500.0f ) * class_rate[class_index];
		uint32 max_hp = uint32(_get_total_stat_value(EUF_UnitMaxHealth, value));
		m_data.set_stat_uint32_value(EUF_UnitMaxHealth, max_hp);

		if (cur_hp > max_hp)
		{
			set_health(max_hp);
		}
	}

	void nplayer::_update_max_energy()
	{
		uint32 cur_energy = m_data.get_uint32_value(EUF_UnitEnergy);
		float class_rate[ECT_Max] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
		uint8 class_index = get_class();
		float value = (m_data.get_uint32_value(EUF_UnitLevel) * 1000.0f  + m_data.get_uint32_value(EUF_UnitVitality) * 200.0f ) * class_rate[class_index];
		uint32 max_energy = uint32(_get_total_stat_value(EUF_UnitMaxEnergy, value));
		m_data.set_stat_uint32_value(EUF_UnitMaxEnergy, max_energy);

		if (cur_energy > max_energy)
		{
			set_energy(max_energy);
		}
	}

	void nplayer::_update_max_mana()
	{
		uint32 cur_mana = m_data.get_uint32_value(EUF_UnitMana);
		float class_rate[ECT_Max] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
		uint8 class_index = get_class();
		float value = (m_data.get_uint32_value(EUF_UnitLevel) * 1000.0f  + m_data.get_uint32_value(EUF_UnitSpirit) * 200.0f ) * class_rate[class_index];
		uint32 max_mana = uint32(_get_total_stat_value(EUF_UnitMaxMana, value));
		m_data.set_stat_uint32_value(EUF_UnitMaxMana, max_mana);

		if (cur_mana > max_mana)
		{
			set_mana(max_mana);
		}
	}

	void nplayer::_update_melee_damage()
	{
		float class_rate[ECT_Max] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
		uint8 class_index = get_class();
		float value = m_data.get_uint32_value(EUF_UnitLevel) * 10.0f * class_rate[class_index] + m_data.get_uint32_value(EUF_UnitStamina) * 60.0f;
		m_data.set_stat_float_value(EUF_UnitMeleeDamage, _get_total_stat_value(EUF_UnitMeleeDamage, value));

		_update_dodge();
		_update_speed();
		_update_attack_speed();
	}

	void nplayer::_update_kungfu_damage()
	{
		float class_rate[ECT_Max] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
		uint8 class_index = get_class();
		float value = m_data.get_uint32_value(EUF_UnitLevel) * 10.0f * class_rate[class_index] + m_data.get_uint32_value(EUF_UnitVitality) * 60.0f;
		m_data.set_stat_float_value(EUF_UnitKungFuDamage, _get_total_stat_value(EUF_UnitKungFuDamage, value));

		_update_dodge();
	}

	void nplayer::_update_spell_damage()
	{
		float class_rate[ECT_Max] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
		uint8 class_index = get_class();
		float value = m_data.get_uint32_value(EUF_UnitLevel) * 10.0f * class_rate[class_index] + m_data.get_uint32_value(EUF_UnitSpirit) * 60.0f;
		m_data.set_stat_float_value(EUF_UnitSpellDamage, _get_total_stat_value(EUF_UnitSpellDamage, value));
	}

	void nplayer::_update_agility()
	{
		float class_rate[ECT_Max] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
		uint8 class_index = get_class();
		float value = m_data.get_uint32_value(EUF_UnitLevel) * 10.0f * class_rate[class_index] + m_data.get_uint32_value(EUF_UnitGenGu) * 60.0f;
		m_data.set_stat_float_value(EUF_UnitAgility, _get_total_stat_value(EUF_UnitAgility, value));

		_update_crit();
	}

	void nplayer::_update_health_regeneration()
	{
		float value = 50.0f * (1 + m_data.get_uint32_value(EUF_UnitLevel) * 20.0f / float(m_data.get_uint32_value(EUF_UnitLevel) + 2)) + m_data.get_uint32_value(EUF_UnitGenGu) * 10.0f;
		m_data.set_stat_float_value(EUF_UnitHealthRegeneration, _get_total_stat_value(EUF_UnitHealthRegeneration, value));
	}

	void nplayer::_update_power_Regeneration()
	{
		float value = 50.0f * (1 + m_data.get_uint32_value(EUF_UnitLevel) * 5.0f / float(m_data.get_uint32_value(EUF_UnitLevel) + 5)) + m_data.get_uint32_value(EUF_UnitGenGu) * 20.0f;
		m_data.set_stat_float_value(EUF_UnitPowerRegeneration, _get_total_stat_value(EUF_UnitPowerRegeneration, value));
	}

	void nplayer::_regenerate_health(uint32 elapse)
	{
		uint32 cur_value = m_data.get_uint32_value(EUF_UnitHealth);
		uint32 max_value = m_data.get_uint32_value(EUF_UnitMaxHealth);

		if (cur_value >= max_value)
		{
			return;
		}

		// 恢复参数调整(服务器设定等等)

		float add_value = m_data.get_float_value(EUF_UnitHealthRegeneration);

		// 战斗状态对恢复的影响, buff对恢复方式的影响, 不是值的影响


		if (add_value < 0.0f)
		{
			add_value = 0.0f;
		}

		add_value *= (float)elapse / REGEN_TIME;

		health_modifier(int32(add_value));
	}

	void nplayer::_regenerate_energy(uint32 elapse)
	{
		uint32 cur_value = m_data.get_uint32_value(EUF_UnitHealth);
		uint32 max_value = m_data.get_uint32_value(EUF_UnitMaxHealth);

		if (cur_value >= max_value)
		{
			return;
		}

		// 恢复参数调整(服务器设定等等)

		float add_value = m_data.get_float_value(EUF_UnitHealthRegeneration);

		// 战斗状态对恢复的影响, buff对恢复方式的影响, 不是值的影响


		if (add_value < 0.0f)
		{
			add_value = 0.0f;
		}

		add_value *= (float)elapse / REGEN_TIME;

		energy_modifier(int32(add_value));
	}

	void nplayer::_regenerate_mana(uint32 elapse)
	{
		uint32 cur_value = m_data.get_uint32_value(EUF_UnitMana);
		uint32 max_value = m_data.get_uint32_value(EUF_UnitMaxMana);

		if (cur_value >= max_value)
		{
			return;
		}

		// 恢复参数调整(服务器设定等等)

		float add_value = m_data.get_float_value(EUF_UnitPowerRegeneration);

		// 战斗状态对恢复的影响, buff对恢复方式的影响, 不是值的影响


		if (add_value < 0.0f)
		{
			add_value = 0.0f;
		}

		add_value *= (float)elapse / REGEN_TIME;

		mana_modifier(int32(add_value));
	}

} // namespace nexus
