#include "nunit.h"
#include "nspell.h"
#include "nspell_aura.h"
#include "ninfo_data.h"
#include "nlog_mt.h"
#include "nutil.h"

#include "nevent_args.h"

#include "nmsg_movement.h"
#include "nmsg_spell.h"

namespace nexus {

	nunit::nunit() : nworld_object(), m_can_modify_stats(false), m_regen_timer(0)
	{
		for (uint32 i = 0; i < EUF_UnitSeparator; ++i)
		{
			m_modifiers[EUMT_BaseValue][i]	= 0.0f;
			m_modifiers[EUMT_BasePct][i]	= 1.0f;
			m_modifiers[EUMT_TotalValue][i]	= 0.0f;
			m_modifiers[EUMT_TotalPct][i]	= 1.0f;
		}

		memset(m_aura_flag, 0, sizeof(m_aura_flag));
	}

	nunit::~nunit()
	{
	}

	bool nunit::init()
	{
		return true;
	}

	void nunit::destroy()
	{
		// 注意资源释放顺序
		_clear_spells();
		_clear_auras();
		m_data.destroy();

		nworld_object::destroy();
	}

	void nunit::update(uint32 elapse)
	{
		nworld_object::update(elapse);
		// 注意更新顺序, 属性最后
		_regenerate(elapse);
		_update_auras(elapse);
		_update_spells(elapse);
		m_data.update(elapse);
	}

	uint16 nunit::check_move(bool only_turn)
	{
		if (has_flag(EUS_Dead))
		{
			return EMRC_Dead;
	}

		// 当前释放的技能是否不充许移动或转身
		nspell_info const* spell_info_ptr = NULL;
		uint8 status = ESStatus_None;

		for (nspell_list::const_iterator iter = m_cur_spells.begin(); iter != m_cur_spells.end(); ++iter)
		{
			status = (*iter)->get_status();

			if (ESStatus_Preparing == status || ESStatus_Casting == status)
			{
				spell_info_ptr = (*iter)->get_spell_info();

				if (spell_info_ptr)
				{
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
			}
		}

		// 判断不让移动的buff
		if (has_aura_effect(EAET_Fear))
		{
			return EMRC_Fear;
		}

		if (has_aura_effect(EAET_Root))
		{
			return EMRC_Root;
		}

		return EMRC_Success;
	}

	bool nunit::register_event()
	{
		get_event_processor().register_event_handler(EEI_MOVE,					boost::bind(&nunit::on_move, this, _1));
		get_event_processor().register_event_handler(EEI_ENTER_COMBAT,			boost::bind(&nunit::on_enter_combat, this, _1));
		get_event_processor().register_event_handler(EEI_DAMAGE,				boost::bind(&nunit::on_damage, this, _1));
		get_event_processor().register_event_handler(EEI_DEAD,					boost::bind(&nunit::on_dead, this, _1));
		get_event_processor().register_event_handler(EEI_CHANGE_MAP,			boost::bind(&nunit::on_change_map, this, _1));
		get_event_processor().register_event_handler(EEI_SPELL_PREPARE,			boost::bind(&nunit::on_spell_prepare, this, _1));
		get_event_processor().register_event_handler(EEI_SPELL_CAST,			boost::bind(&nunit::on_spell_cast, this, _1));

		return true;
	}

	void nunit::on_move(const nevent_base_args* base_args)
	{
		const nevent_move_args* data_ptr = static_cast<const nevent_move_args*>(base_args);

		// 对于正在释放的主动技能
		if (data_ptr->only_turn)
		{
			remove_cur_spells_by_interrupt_flags(ESIF_Turning);
	}
		else
		{
			remove_cur_spells_by_interrupt_flags(ESIF_Move);
		}

		remove_auras_by_interrupt_flags(EAIF_Move);
	}

	void nunit::on_enter_combat(const nevent_base_args* base_args)
	{
		const nevent_enter_combat_args* data_ptr = static_cast<const nevent_enter_combat_args*>(base_args);

		remove_auras_by_interrupt_flags(EAIF_EnterCombat);
	}

	void nunit::on_damage(const nevent_base_args* base_args)
	{
		const nevent_damage_args* data_ptr = static_cast<const nevent_damage_args*>(base_args);

		// spell
		for (nspell_list::const_iterator iter = m_cur_spells.begin(); iter != m_cur_spells.end(); ++iter)
		{
			(*iter)->on_damage(data_ptr->unit_id, data_ptr->damage);
	}

		// aura
		for (naura_map::const_iterator iter = m_auras.begin(); iter != m_auras.end(); ++iter)
		{
			iter->second->on_damage(data_ptr->unit_id, data_ptr->damage);
		}
	}

	void nunit::on_dead(const nevent_base_args* base_args)
	{
		const nevent_dead_args* data_ptr = static_cast<const nevent_dead_args*>(base_args);

		stop_move();
		// 
		for (nspell_list::const_iterator iter = m_cur_spells.begin(); iter != m_cur_spells.end(); ++iter)
		{
			(*iter)->cancel(ESCR_Failed_Interrupted);
	}

		// aura
		for (naura_map::const_iterator iter = m_auras.begin(); iter != m_auras.end(); ++iter)
		{
			iter->second->on_dead();
		}
	}

	void nunit::on_change_map(const nevent_base_args* base_args)
	{
		const nevent_change_map_args* data_ptr = static_cast<const nevent_change_map_args*>(base_args);

		remove_auras_by_interrupt_flags(EAIF_ChangeMap);
	}

	void nunit::on_spell_prepare(const nevent_base_args* base_args)
	{
		const nevent_spell_prepare_args* data_ptr = static_cast<const nevent_spell_prepare_args*>(base_args);

		remove_cur_spells_by_interrupt_flags(ESIF_SpellPrepare);
	}

	void nunit::on_spell_cast(const nevent_base_args* base_args)
	{
		const nevent_spell_cast_args* data_ptr = static_cast<const nevent_spell_cast_args*>(base_args);
		remove_cur_spells_by_interrupt_flags(ESIF_SpellCast);
	}

	void nunit::handle_stat_modifier(uint32 mod_type, uint32 stat_index, float value, bool apply)
	{
		if (mod_type >= EUMT_Max || stat_index >= EUF_UnitSeparator)
		{
			NLOG_ERROR(_T("(mod_type = %u) >= EUMT_Max || (stat_index = %u)>= EUF_UnitSeparator"), mod_type, stat_index);
			return;
		}

		switch (mod_type)
		{
		case EUMT_BaseValue:
		case EUMT_TotalValue:
			{
				m_modifiers[mod_type][stat_index] += apply ? value : -value;
			}
			break;
		case EUMT_BasePct:
		case EUMT_TotalPct:
			{
				float temp_value = 1.0f;

				if (value <= -100.0f)                           //small hack-fix for -100% modifiers
				{
					value = -200.0f;
				}

				temp_value = (100.0f + value) / 100.0f;
				m_modifiers[mod_type][stat_index] *= apply ? temp_value : (1.0f / temp_value);
			}
			break;
		default:
			{
			}
			break;
		}

		if (!_can_modify_stats())
		{
			return;
		}

		switch (stat_index)
		{
		case EUF_UnitStamina:
		case EUF_UnitVitality:
		case EUF_UnitSpirit:
		case EUF_UnitGenGu:				{ _update_base_stats(stat_index);			} break;
		case EUF_UnitMaxHealth:			{ _update_max_health();						} break;
		case EUF_UnitMaxEnergy:			{ _update_max_energy();						} break;
		case EUF_UnitMaxMana:			{ _update_max_mana();						} break;
		case EUF_UnitMeleeDamage:		{ _update_melee_damage();					} break;
		case EUF_UnitKungFuDamage:		{ _update_kungfu_damage();					} break;
		case EUF_UnitSpellDamage:		{ _update_spell_damage();					} break;
		case EUF_UnitAgility:			{ _update_agility();						} break;
		case EUF_UnitHealthRegeneration:{ _update_health_regeneration();			} break;
		case EUF_UnitPowerRegeneration:	{ _update_power_Regeneration();				} break;
		case EUF_UnitDodge:				{ _update_dodge();							} break;
		case EUF_UnitCrit:				{ _update_crit();							} break;
		case EUF_UnitSpeed:				{ _update_speed();							} break;
		case EUF_UnitAttackSpeed:		{ _update_attack_speed();					} break;
		case EUF_UnitTotalWeight:		{ _update_equip_weight();					} break;
		case EUF_UnitSharp:
		case EUF_UnitDefense:
		case EUF_UnitResilience:
		case EUF_UnitMetalResist:
		case EUF_UnitWoodResist:
		case EUF_UnitWaterResist:
		case EUF_UnitFireResist:
		case EUF_UnitSoilResist:		{ _update_equip_stat(stat_index); 			} break;
		default:						{											} break;
		}
	}

	void nunit::set_health(uint32 value)
			{
		uint32 max_health = m_data.get_uint32_value(EUF_UnitMaxHealth);

		if (max_health < value)
		{
			value = max_health;
			}

		m_data.set_stat_uint32_value(EUF_UnitHealth, value);
	}

	int32 nunit::health_modifier(int32 value)
			{
		if (0 == value)
		{
			return 0;
		}

		int32 gain = 0;
		int32 cur_health = int32(m_data.get_uint32_value(EUF_UnitHealth));
		int32 val = value + cur_health;

		if (val <= 0)
		{
			set_health(0);
			return -cur_health;
			}

		int32 max_health = int32(m_data.get_uint32_value(EUF_UnitMaxHealth));

		if (val < max_health)
			{
			set_health(val);
			gain = val - cur_health;
		}
		else if (cur_health != max_health)
		{
			set_health(max_health);
			gain = max_health - cur_health;
		}

		return gain;
			}

	void nunit::set_energy(uint32 value)
			{
		uint32 max_energy = m_data.get_uint32_value(EUF_UnitMaxEnergy);

		if (max_energy < value)
		{
			value = max_energy;
			}

		m_data.set_stat_uint32_value(EUF_UnitEnergy, value);
	}

	int32 nunit::energy_modifier(int32 value)
			{
		if (0 == value)
		{
			return 0;
			}

		int32 gain = 0;
		int32 cur_energy = int32(m_data.get_uint32_value(EUF_UnitEnergy));
		int32 val = value + cur_energy;

		if (val <= 0)
		{
			set_energy(0);
			return -cur_energy;
		}

		int32 max_energy = int32(m_data.get_uint32_value(EUF_UnitMaxEnergy));

		if (val < max_energy)
		{
			set_energy(val);
			gain = val - cur_energy;
	}
		else if (cur_energy != max_energy)
		{
			set_energy(max_energy);
			gain = max_energy - cur_energy;
		}

		return gain;
	}

	void nunit::set_mana(uint32 value)
	{
		uint32 max_mana = m_data.get_uint32_value(EUF_UnitMaxMana);

		if (max_mana < value)
		{
			value = max_mana;
		}

		m_data.set_stat_uint32_value(EUF_UnitMana, value);
	}

	int32 nunit::mana_modifier(int32 value)
	{
		if (0 == value)
		{
			return 0;
		}

		int32 gain = 0;
		int32 cur_mana = int32(m_data.get_uint32_value(EUF_UnitMana));
		int32 val = value + cur_mana;

		if (val <= 0)
		{
			set_mana(0);
			return -cur_mana;
		}

		int32 max_mana = int32(m_data.get_uint32_value(EUF_UnitMaxMana));

		if (val < max_mana)
		{
			set_mana(val);
			gain = val - cur_mana;
		}
		else if (cur_mana != max_mana)
		{
			set_mana(max_mana);
			gain = max_mana - cur_mana;
		}

		return gain;
	}

	void nunit::regenerate_all(uint32 elapse)
	{
		// 恢复限制如战斗状态不能恢复HP等等
		_regenerate_health(elapse);
		_regenerate_energy(elapse);
		_regenerate_mana(elapse);

		m_regen_timer = REGEN_TIME;
	}

	bool nunit::is_spell_in_progress() const
	{
		nspell_info const* spell_info_ptr = NULL;

		for (nspell_list::const_iterator iter = m_cur_spells.begin(); iter != m_cur_spells.end(); ++iter)
		{
			if (!(*iter)->is_triggered())
			{
				spell_info_ptr = (*iter)->get_spell_info();

				if (spell_info_ptr)
				{
					switch (spell_info_ptr->cast_type)
					{
					case ESCast_Cast:
						{
							if (ESStatus_Preparing == (*iter)->get_status())
							{
								return true;
			}
		}
						break;
					case ESCast_Channel:
						{
							if (ESStatus_Casting == (*iter)->get_status())
							{
								// 判断通道是否自动打断
								return true;
							}
						}
						break;
					default:
						{
						}
						break;
					}
				}
			}

			return false;
		}

		return NULL;
	}

	void nunit::add_cur_spell(nspell* spell_ptr)
	{
		m_cur_spells.push_back(spell_ptr);
	}

	void nunit::remove_cur_spell(uint32 spell_id)
	{
		for (nspell_list::const_iterator iter = m_cur_spells.begin(); iter != m_cur_spells.end(); ++iter)
		{
			if ((*iter)->get_spell_id() == spell_id)
			{
				(*iter)->cancel(ESCR_Failed_Cancel);
			}
		}
	}

	void nunit::remove_cur_spells_by_interrupt_flags(uint32 flag)
	{
		for (nspell_list::const_iterator iter = m_cur_spells.begin(); iter != m_cur_spells.end(); ++iter)
		{
			if (0 != ((*iter)->get_interrupt_flags() & flag))
			{
				(*iter)->cancel(ESCR_Failed_Interrupted);
			}
		}
	}

	void nunit::remove_all_cur_spells()
	{
		for (nspell_list::const_iterator iter = m_cur_spells.begin(); iter != m_cur_spells.end(); ++iter)
		{
			(*iter)->cancel(ESCR_Failed_Cancel);
		}
	}

	void nunit::cast_spell(uint32 spell_id, uint64 target_id, const vector3& target_pos, float time_stamp, uint32 cast_index, uint64 item_guid, uint64 original_caster_id, nspell_info const* triggered_by_spell_ptr, nspell_aura_info const* triggered_by_aura_ptr)
	{
		nspell_info const* spell_info_ptr = s_spell_info.get_info(spell_id);

		if (!spell_info_ptr)
		{
			NLOG_ERROR(_T("s_spell_info.get_info error!"));
			return;
		}

		nspell* spell_ptr = nspell::construct(this, spell_info_ptr, target_id, target_pos, time_stamp);

		if (!spell_ptr)
		{
			NLOG_ERROR(_T("nspell::construct error!"));
			return;
		}

		if (cast_index > 0)
		{
			spell_ptr->set_cast_index(cast_index);
		}

		if (0 != item_guid)
		{
			spell_ptr->set_caster_item_id(item_guid);
		}

		uint64 guid = original_caster_id;

		if (0 == guid)
		{
			guid = get_id();
		}

		if (triggered_by_spell_ptr)
		{
			spell_ptr->set_trigger_spell(guid, triggered_by_spell_ptr);
		}

		if (triggered_by_aura_ptr)
		{
			spell_ptr->set_trigger_aura(guid, triggered_by_aura_ptr);
		}

		spell_ptr->prepare();
	}

	nplayer* nunit::get_spell_mod_owner()
	{
		if (EOT_PLAYER == get_type())
		{
			return (nplayer*)this;
		}

		return NULL;
	}

	bool nunit::has_aura_effect(uint32 effect_type) const
	{
		return m_auras_by_effect.find(effect_type) != m_auras_by_effect.end();
	}

	bool nunit::has_aura(uint32 aura_id) const
	{
		return m_auras.find(aura_id) != m_auras.end();
	}

	void nunit::add_aura(naura* aura_ptr)
	{
		assert(aura_ptr);
		nspell_aura_info const* aura_info_ptr = aura_ptr->get_spell_aura_info();

		if (!aura_info_ptr)
		{
			naura::destroy(aura_ptr);
			return;
	}
		// 判断死亡状态, 死亡状态下能否添加
		// 各种简单校验

		// 管理aura对象
		naura_range_pair range_pair = m_auras.equal_range(aura_ptr->get_info_id());

		naura* iter_aura_ptr = NULL;
		// 判断同源是否叠加
		for (naura_map::const_iterator iter = range_pair.first; iter != range_pair.second && iter != m_auras.end(); ++iter)
		{
			iter_aura_ptr = iter->second;

			if (iter_aura_ptr && iter_aura_ptr->get_caster_id() == aura_ptr->get_caster_id() && 
				_aura_stack(aura_ptr, iter_aura_ptr, aura_info_ptr->ssrc_stack_type))
			{
				iter_aura_ptr->on_update();
				naura::destroy(aura_ptr);
				return; 
		}
		}

		// 判断不同源是否叠加
		for (naura_map::const_iterator iter = range_pair.first; iter != range_pair.second && iter != m_auras.end(); ++iter)
		{
			iter_aura_ptr = iter->second;

			if (iter_aura_ptr && iter_aura_ptr->get_caster_id() != aura_ptr->get_caster_id() && 
				_aura_stack(aura_ptr, iter_aura_ptr, aura_info_ptr->dsrc_stack_type))
			{
				iter_aura_ptr->on_update();
				naura::destroy(aura_ptr);
				return; 
			}
		}

		// 判断是否冲顶
		if (0 != aura_info_ptr->replace_type)
		{
			nspell_aura_info const* iter_aura_info_ptr = NULL;
		
			for (naura_map::const_iterator iter = m_auras.begin(); iter != m_auras.end(); ++iter)
			{
				iter_aura_ptr = iter->second;
		
				if (!iter_aura_ptr)
				{
					NLOG_DEBUG(_T("replace iter_aura_ptr == NULL"));
					continue;
				}

				iter_aura_info_ptr = iter_aura_ptr->get_spell_aura_info();

				if (!iter_aura_info_ptr)
				{
					NLOG_DEBUG(_T("replace iter_aura_info_ptr == NULL"));
					continue;
				}

				if (aura_info_ptr->replace_type == iter_aura_info_ptr->replace_type)
				{
					if (aura_info_ptr->level <= iter_aura_info_ptr->level)
					{
						naura::destroy(aura_ptr);
						return;
					}
					else
					{
						remove_aura(iter_aura_ptr);
					}

					break;
				}
			}
		}

		// 查找可用的aura slot
		uint8 slot = 0;

		for ( ; slot < MAX_AURA_NUM; ++slot)
		{
			if (!_get_aura_bits(slot))
			{
				aura_ptr->set_slot(slot);
				break;
			}
		}

		if (MAX_AURA_NUM == slot)
		{
			NLOG_DEBUG(_T("aura num overflow!"));
			naura::destroy(aura_ptr);
			return;
		}

		m_auras.insert(std::make_pair(aura_ptr->get_info_id(), aura_ptr));
		m_auras_by_effect.insert(std::make_pair(aura_ptr->get_effect_type(), aura_ptr));

		aura_ptr->on_add();
		aura_ptr->apply_effect(true);
	}

	void nunit::remove_aura(naura* aura_ptr)
	{
		assert(aura_ptr);

		if (aura_ptr->is_deleted())
		{
			return;
		}

		// 不涉及容器的删除, 删除在_clear_deleted_auras处理, 并行或高于aura update
		aura_ptr->apply_effect(false);
		aura_ptr->set_deleted(true);
		aura_ptr->on_remove();

		m_auras_deleted.push_back(aura_ptr);
	}

	void nunit::remove_auras_by_interrupt_flags(uint32 flag)
	{
		naura* aura_ptr = NULL;

		for (naura_map::const_iterator iter = m_auras.begin(); iter != m_auras.end(); ++iter)
		{
			aura_ptr = iter->second;

			if (aura_ptr && 0 != (aura_ptr->get_interrupt_flags() & flag))
			{
				remove_aura(aura_ptr);
			}
		}
	}

	void nunit::remove_auras_by_caster_spell(uint32 spell_id, uint64 caster_id)
	{
		naura* aura_ptr = NULL;

		for (naura_map::const_iterator iter = m_auras.begin(); iter != m_auras.end(); ++iter)
		{
			aura_ptr = iter->second;

			if (aura_ptr && spell_id == aura_ptr->get_spell_id() && caster_id == aura_ptr->get_caster_id())
			{
				remove_aura(aura_ptr);
			}
		}
	}

	void nunit::remove_all_auras()
	{
		naura* aura_ptr = NULL;

		for (naura_map::const_iterator iter = m_auras.begin(); iter != m_auras.end(); ++iter)
		{
			aura_ptr = iter->second;

			if (aura_ptr)
			{
				remove_aura(aura_ptr);
			}
		}
	}

	void nunit::set_incombat_with(nunit* enemy_ptr)
	{

	}

	void nunit::clear_incombat()
	{

	}

	uint8 nunit::spell_hit_result(nunit* target_ptr, nspell_info const* spell_info_ptr)
	{
		// 判断躲避, creature脱战返回时
		if (false)
		{
			return ESHR_Evade;
		}

		// 判断免疫
		if (target_ptr->_is_immuned_to_spell(spell_info_ptr))
		{
			return ESHR_Immune;
		}

		// 判断 命中 暴击 未命中
		int32 value = 0;

		switch (spell_info_ptr->sub_type)
		{
		case ESCST_Melee:
			{
				value = 100;
			}
			break;
		case ESCST_KungFu:
			{
				value = 50;
			}
			break;
		case ESCST_Spell:
			{
				value = 30;
			}
			break;
		default:
			{
			}
			break;
		}

		// 
		uint8 result = 0;
		return result;
	}

	int32 nunit::calculate_melee_damage(nunit* target_ptr, uint8 hit_info)
	{
		if (has_flag(EUS_Dead) || !target_ptr || target_ptr->has_flag(EUS_Dead))
		{
			return 0;
		}

		float weapon_damage = 0;

		float damage = float(m_data.get_float_value(EUF_UnitMeleeDamage) + weapon_damage - target_ptr->get_data().get_uint32_value(EUF_UnitDefense)) *
			(1.0f + float(s_util.irand(-50, 50)) / 1000.0f);

		if (ESHR_Crit & hit_info)
		{
			damage += damage * (1 + float(s_util.irand(20, 50)) / 100.0f);
		}

		// 计算吸收

		return int32(damage);
	}

	int32 nunit::calculate_kungfu_damage(nunit* target_ptr, uint8 hit_info, float damage, float mod_value)
	{
		if (has_flag(EUS_Dead) || !target_ptr || target_ptr->has_flag(EUS_Dead))
		{
			return 0;
		}
		damage = damage * m_data.get_float_value(EUF_UnitKungFuDamage) / 100.0f * 
			(1 + float(s_util.irand(int32(-10 * mod_value), int32(10 * mod_value))) / 1000.0f);

		// 计算吸收

		return int32(damage);
	}

	int32 nunit::calculate_spell_damage(nunit* target_ptr, uint8 hit_info, float damage, float mod_value)
	{
		if (has_flag(EUS_Dead) || !target_ptr || target_ptr->has_flag(EUS_Dead))
		{
			return 0;
		}

		damage = damage * m_data.get_float_value(EUF_UnitSpellDamage) / 75 * 
			(1 + float(s_util.irand(int32(-10 * mod_value), int32(10 * mod_value))) / 1000.0f);

		// 计算吸收

		return int32(damage);
	}

	int32 nunit::do_damage(nunit* caster_ptr, int32 damage)
	{
		// 向目标发送伤害事件
		nevent_damage_args damage_args;
		damage_args.unit_id	= caster_ptr ? caster_ptr->get_id() : 0;
		damage_args.damage	= damage;
		get_event_processor().send_event(&damage_args, sizeof(damage_args));

		uint32 health = m_data.get_uint32_value(EUF_UnitHealth);

		// 决斗的话血量最多为1

		// 设置战斗状态, 在添加效果前也要有哪里处理未命中无敌免疫, npc回到原位是不进入战斗状态的
		if (caster_ptr && this != caster_ptr)
		{
			set_incombat_with(caster_ptr);
			caster_ptr->set_incombat_with(this);
		}

		// TODO: D. Zhao 统计伤害量
		if (health <= uint32(damage))
		{
			set_health(0);
			set_flag(EUS_Dead);
			damage = health;

			// 造成死亡
			nevent_dead_args dead_args;
			dead_args.unit_id	= caster_ptr ? caster_ptr->get_id() : 0;
			get_event_processor().send_event(&dead_args, sizeof(dead_args));
		}
		else		// if (health <= damage)
		{
			health_modifier(-damage);

			// 耐久消耗等等其他
		}

		return damage;
	}

	int32 nunit::do_heal(nunit* caster_ptr, int32 heal)
	{
		// 处理决斗等
		int32 real_heal = health_modifier(heal);

		// 处理仇恨

		return real_heal;
	}

	bool nunit::_is_immuned_to_spell(nspell_info const* spell_info_ptr)
	{
		return false;
	}

	float nunit::_get_total_stat_value(uint32 stat_index, float value) const
	{
		if (m_modifiers[EUMT_TotalPct][stat_index] <= -1.0f)
		{
			return 0.0f;
		}

		// 按照计算公式计算 如: value = (base_value * base_pct + total_value) * total_pct
		// player, creature, pet的计算公式分开
		value += m_modifiers[EUMT_BaseValue][stat_index];
		value *= m_modifiers[EUMT_BasePct][stat_index];
		value += m_modifiers[EUMT_TotalValue][stat_index];
		value *= m_modifiers[EUMT_TotalPct][stat_index];
		return value;
	}

	void nunit::_update_all_stats()
		{
		for (uint32 i = EUS_StatStart; i < EUS_StatEnd; ++i)
		{
			_update_base_stats(i);
		}

		_update_max_health();
		_update_max_energy();
		_update_max_mana();
		_update_melee_damage();
		_update_kungfu_damage();
		_update_spell_damage();
		_update_agility();
		_update_health_regeneration();
		_update_power_Regeneration();

		_update_dodge();
		_update_crit();
		_update_speed();
		_update_attack_speed();

		_update_equip_weight();

		for (uint32 i = EUS_EquipStart; i < EUS_EquipEnd; ++i)
		{
			_update_equip_stat(i);
	}
	}

	void nunit::_update_dodge()
	{
		float value = (m_data.get_int32_value(EUF_UnitTotalWeight) * 10.0f - m_data.get_int32_value(EUF_UnitMeleeDamage) / 5.0f) / (m_data.get_int32_value(EUF_UnitTotalWeight) * 10.0f + m_data.get_int32_value(EUF_UnitMeleeDamage) / 10.0f);
		value = value < 0.0f ? 0.0f : value;
		float value2 = float((0.01 + 0.15 * m_data.get_int32_value(EUF_UnitKungFuDamage) / (30000.0f + m_data.get_int32_value(EUF_UnitKungFuDamage))) * (1 - value));
		m_data.set_stat_float_value(EUF_UnitDodge, _get_total_stat_value(EUF_UnitDodge, value2));
	}

	void nunit::_update_crit()
	{
		float value = float((0.2 * m_data.get_int32_value(EUF_UnitAgility)) / float(m_data.get_int32_value(EUF_UnitAgility) * 2.0f + m_data.get_int32_value(EUF_UnitLevel) * 140.0f));
		m_data.set_stat_float_value(EUF_UnitCrit, _get_total_stat_value(EUF_UnitCrit, value));
	}

	void nunit::_update_speed()
	{
		float value = (m_data.get_int32_value(EUF_UnitTotalWeight) * 10.0f - m_data.get_int32_value(EUF_UnitMeleeDamage) / 5.0f) / (m_data.get_int32_value(EUF_UnitTotalWeight) * 10.0f + m_data.get_int32_value(EUF_UnitMeleeDamage) / 10.0f);
		value = value < 0.0f ? 0.0f : value;
		float value2 = float(550.0f * (1 + 0.2 * m_data.get_int32_value(EUF_UnitAgility) / (600.0f + m_data.get_int32_value(EUF_UnitAgility))) * (1 - 1.50f * value));
		m_data.set_stat_float_value(EUF_UnitSpeed, _get_total_stat_value(EUF_UnitSpeed, value2));
	}

	void nunit::_update_attack_speed()
	{
		// TODO: D. Zhao, get item (left weapon and right weapon)
		float value = 0.0f + 0.0f;
		value = (value * 3.0f - m_data.get_int32_value(EUF_UnitMeleeDamage) / 70.0f) / (value * 3.0f + m_data.get_int32_value(EUF_UnitMeleeDamage) / 100.0f);
		value = value < 0.0f ? 0.0f : value;
		float value2 = float(100.0f * (1 + 0.1 * m_data.get_int32_value(EUF_UnitAgility) / (600.0f + m_data.get_int32_value(EUF_UnitAgility))) * (1 - value));
		m_data.set_stat_float_value(EUF_UnitAttackSpeed, _get_total_stat_value(EUF_UnitAttackSpeed, value2));
	}

	void nunit::_update_equip_weight()
	{
		m_data.set_stat_float_value(EUF_UnitTotalWeight, _get_total_stat_value(EUF_UnitTotalWeight, 0.0f));
		_update_speed();
		_update_attack_speed();
	}

	void nunit::_update_equip_stat(uint32 stat_index)
	{
		m_data.set_stat_float_value(stat_index, _get_total_stat_value(stat_index, 0.0f));
	}

	void nunit::_regenerate(uint32 elapse)
	{
		m_regen_timer -= elapse;

		if (m_regen_timer <= 0)
		{
			m_regen_timer = 0;

			if (has_flag(EUS_Dead))
			{
				return;
			}

			regenerate_all(REGEN_TIME);
		}
	}

	void nunit::_update_spells(uint32 elapse)
	{
		nspell* spell_ptr = NULL;

		for (nspell_list::const_iterator iter = m_cur_spells.begin(); iter != m_cur_spells.end(); )
		{
			spell_ptr = *iter;

			if (spell_ptr)
			{
				if (ESStatus_None == spell_ptr->get_status() || ESStatus_Deleted == spell_ptr->get_status())
				{
					iter = m_cur_spells.erase(iter);
					nspell::destroy(spell_ptr);
	}
				else
				{
					// update中可以对容器执行添加操作, 但不能做删除操作, 删除操作在上面进行, 注意修改iter
					spell_ptr->update(elapse);
					++iter;
				}
			}
			else
			{
				NLOG_DEBUG(_T("spell update error, spell_ptr == NULL!"));
				++iter;
			}
		}
	}

	void nunit::_clear_spells()
	{
		for (nspell_list::const_iterator iter = m_cur_spells.begin(); iter != m_cur_spells.end(); ++iter)
		{
			if (*iter)
			{
				nspell::destroy(*iter);
			}
		}

		m_cur_spells.clear();
	}

	void nunit::_update_auras(uint32 elapse)
	{
		for (naura_map::const_iterator iter = m_auras.begin(); iter != m_auras.end(); ++iter)
		{
			// update中可以对容器执行添加操作, 但不能做删除操作, 删除操作在_clear_deleted_auras进行
			(iter->second)->update(elapse);
		}

		_clear_deleted_auras();
	}

	void nunit::_earse_auras(naura* aura_ptr)
	{
		naura_range_pair rang_pair = get_auras(aura_ptr->get_info_id());

		for (naura_map::const_iterator iter = rang_pair.first; iter != rang_pair.second; ++iter)
		{
			 if (iter->second == aura_ptr)
			 {
				m_auras.erase(iter);
				break;
			 }
		}

		naura_effect_range_pair rang_pair2 = get_auras_by_effect(aura_ptr->get_effect_type());

		for (naura_effect_map::const_iterator iter = rang_pair2.first; iter != rang_pair2.second; ++iter)
		{
			if (iter->second == aura_ptr)
			{
				m_auras_by_effect.erase(iter);
				break;
			}
		}

		_set_aura_bits(aura_ptr->get_slot());
	}

	void nunit::_clear_deleted_auras()
	{
		for (naura_list::const_iterator iter = m_auras_deleted.begin(); iter != m_auras_deleted.end(); ++iter)
		{
			_earse_auras(*iter);
			naura::destroy(*iter);
		}

		m_auras_deleted.clear();
	}

	void nunit::_clear_auras()
	{
		naura* aura_ptr = NULL;

		for (naura_map::const_iterator iter = m_auras.begin(); iter != m_auras.end(); ++iter)
		{
			aura_ptr = iter->second;

			if (aura_ptr)
			{
				naura::destroy(aura_ptr);
			}
		}

		m_auras.clear();
		m_auras_by_effect.clear();
		m_auras_deleted.clear();
	}

	bool nunit::_aura_stack(naura* src_aura_ptr, naura* dst_aura_ptr, uint32 stack_type)
	{
		switch (stack_type)
		{
		case EAST_Refresh:
			{
				dst_aura_ptr->set_max_duration(src_aura_ptr->get_max_duration());
				dst_aura_ptr->set_duration(src_aura_ptr->get_duration());
				dst_aura_ptr->set_max_stack(src_aura_ptr->get_max_stack());
				dst_aura_ptr->set_stack(src_aura_ptr->get_stack());
				return true;
			}
			break;
		case EAST_AddCount:
			{
				dst_aura_ptr->set_max_duration(src_aura_ptr->get_max_duration());
				dst_aura_ptr->set_duration(src_aura_ptr->get_duration());

				uint8 max_stack = dst_aura_ptr->get_max_stack();
				uint8 stack = dst_aura_ptr->get_stack() + src_aura_ptr->get_stack();

				dst_aura_ptr->set_stack(max_stack > stack ? stack : max_stack);
				return true;
			}
			break;
		case EAST_Add:
			{
				if (src_aura_ptr->get_caster_id() == dst_aura_ptr->get_caster_id())
				{
					return false;
				}

				return true;
			}
			break;
		case EAST_CantAdd:
			{
				return false;
			}
			break;
		default:
			{
				return false;
			}
			break;
		}
	}

} // namespace nexus
