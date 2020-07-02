#include "ncreature.h"
#include "ntimer.h"
#include "nmsg_movement.h"

namespace nexus {

	ncreature::ncreature() : nunit()
	{
	}

	ncreature::~ncreature()
	{
	}

	bool ncreature::init()
	{
		return true;
	}

	void ncreature::destroy()
	{

	}

	void ncreature::update(uint32 elapse)
	{

	}

	void ncreature::make_enter_msg(void* data_ptr, uint32& size) const
	{
		tagS2C_CreatureEnterAoi* packet = static_cast<tagS2C_CreatureEnterAoi*>(data_ptr);
		packet->client_id	= 0;
		packet->msg_id		= S2C_CreatureEnterAoi;
		packet->creature_id	= get_id();
		packet->position		= get_position();
		packet->yaw				= get_rotation().y;
		packet->acceleration	= get_acceleration();
		packet->time_stamp		= g_game_timer.get_time_stamp();
		packet->flag			= get_movement_flag();
		size				= sizeof(tagS2C_CreatureEnterAoi);
	}

	void ncreature::_update_max_health()
	{
		uint32 cur = m_data.get_uint32_value(EUF_UnitHealth);
		uint32 max = uint32(_get_total_stat_value(EUF_UnitMaxHealth, 0.0f));
		m_data.set_stat_uint32_value(EUF_UnitMaxHealth, max);

		if (cur > max)
		{
			set_health(max);
		}
	}

	void ncreature::_update_max_energy()
	{
		uint32 cur = m_data.get_uint32_value(EUF_UnitEnergy);
		uint32 max = uint32(_get_total_stat_value(EUF_UnitMaxEnergy, 0.0f));
		m_data.set_stat_uint32_value(EUF_UnitMaxEnergy, max);

		if (cur > max)
		{
			set_energy(max);
		}
	}

	void ncreature::_update_max_mana()
	{
		uint32 cur = m_data.get_uint32_value(EUF_UnitMana);
		uint32 max = uint32(_get_total_stat_value(EUF_UnitMaxMana, 0.0f));
		m_data.set_stat_uint32_value(EUF_UnitMaxMana, max);

		if (cur > max)
		{
			set_mana(max);
		}
	}

	void ncreature::_update_melee_damage()
	{
		m_data.set_stat_float_value(EUF_UnitMeleeDamage, _get_total_stat_value(EUF_UnitMeleeDamage, 0.0f));
		_update_dodge();
		_update_speed();
		_update_attack_speed();
	}

	void ncreature::_update_kungfu_damage()
	{
		m_data.set_stat_float_value(EUF_UnitKungFuDamage, _get_total_stat_value(EUF_UnitKungFuDamage, 0.0f));
		_update_dodge();
	}

	void ncreature::_update_spell_damage()
	{
		m_data.set_stat_float_value(EUF_UnitSpellDamage, _get_total_stat_value(EUF_UnitSpellDamage, 0.0f));
	}

	void ncreature::_update_agility()
	{
		m_data.set_stat_float_value(EUF_UnitAgility, _get_total_stat_value(EUF_UnitAgility, 0.0f));
		_update_crit();
	}

	void ncreature::_update_health_regeneration()
	{
		m_data.set_stat_float_value(EUF_UnitHealthRegeneration, _get_total_stat_value(EUF_UnitHealthRegeneration, 0.0f));
	}

	void ncreature::_update_power_Regeneration()
	{
		m_data.set_stat_float_value(EUF_UnitPowerRegeneration, _get_total_stat_value(EUF_UnitPowerRegeneration, 0.0f));
	}

	void ncreature::_regenerate_health(uint32 elapse)
	{

	}

	void ncreature::_regenerate_energy(uint32 elapse)
	{

	}

	void ncreature::_regenerate_mana(uint32 elapse)
	{

	}

} // namespace nexus
