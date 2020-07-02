/**
 *	nexus ngame - nunit
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NGAME_NUNIT_H_
#define _NGAME_NUNIT_H_

#include "ncommon.h"
#include "nworld_object.h"
#include "nobject_data.h"
#include "nobject_fields.h"
#include "nspell_data.h"

namespace nexus {

	class nmap;
	class nspell;
	class naura;
	class nplayer;

	struct nspell_info;
	struct nspell_aura_info;

	class nunit : public nworld_object
	{
	public:
		typedef std::multimap<uint32, naura*> naura_map;
		typedef std::pair<naura_map::const_iterator, naura_map::const_iterator> naura_range_pair;
		typedef std::multimap<uint32, naura*> naura_effect_map;
		typedef std::pair<naura_effect_map::const_iterator, naura_effect_map::const_iterator> naura_effect_range_pair;
		typedef std::list<naura*> naura_list;
		typedef std::list<nspell*> nspell_list;

	public:
		nunit();
		virtual ~nunit();

		bool init();
		virtual void destroy();
		virtual void update(uint32 elapse);

		virtual uint16 check_move(bool only_turn);

		virtual bool register_event();

		virtual void on_move(const nevent_base_args* base_args);
		virtual void on_enter_combat(const nevent_base_args* base_args);
		virtual void on_damage(const nevent_base_args* base_args);
		virtual void on_dead(const nevent_base_args* base_args);
		virtual void on_change_map(const nevent_base_args* base_args);
		virtual void on_spell_prepare(const nevent_base_args* base_args);
		virtual void on_spell_cast(const nevent_base_args* base_args);

		// stat
		const nobject_data& get_data() const	{ return m_data; }

		void handle_stat_modifier(uint32 mod_type, uint32 stat_index, float value, bool apply);
		void set_health(uint32 value);
		int32 health_modifier(int32 value);
		void set_energy(uint32 value);
		int32 energy_modifier(int32 value);
		void set_mana(uint32 value);
		int32 mana_modifier(int32 value);
		void regenerate_all(uint32 elapse);
		int32 get_regen_timer() const										{ return m_regen_timer; }

		bool has_flag(uint32 flag) const									{ return m_data.has_flag(EUF_UnitFlags, flag); }
		void set_flag(uint32 flag)											{ return m_data.set_flag(EUF_UnitFlags, flag); }
		void remove_flag(uint32 flag)										{ return m_data.remove_flag(EUF_UnitFlags, flag); }

		// spell
		bool is_spell_in_progress() const;
		void add_cur_spell(nspell* spell_ptr);
		void remove_cur_spell(uint32 spell_id);
		void remove_cur_spells_by_interrupt_flags(uint32 flag);
		void remove_all_cur_spells();

		void cast_spell(uint32 spell_id, uint64 target_id, const vector3& target_pos, float time_stamp = 0.0f, uint32 cast_index = 0, uint64 item_guid = 0, uint64 original_caster_id = 0, nspell_info const* triggered_by_spell_ptr = NULL, nspell_aura_info const* triggered_by_aura_ptr = NULL);

		nplayer* get_spell_mod_owner();

		// aura
		naura_map const& get_auras() const					{ return m_auras; }
		naura_range_pair get_auras(uint32 id) const			{ return m_auras.equal_range(id); }
		naura_effect_map const& get_auras_by_effect() const					{ return m_auras_by_effect; }
		naura_effect_range_pair get_auras_by_effect(uint32 effect) const	{ return m_auras_by_effect.equal_range(effect); }
		bool has_aura_effect(uint32 effect_type) const;
		bool has_aura(uint32 aura_id) const;
		void add_aura(naura* aura_ptr);
		void remove_aura(naura* aura_ptr);
		void remove_auras_by_interrupt_flags(uint32 flag);
		void remove_auras_by_caster_spell(uint32 spell_id, uint64 caster_id);
		void remove_all_auras();

		// damage calculate
		void set_incombat_with(nunit* enemy_ptr);
		void clear_incombat();
		uint8 spell_hit_result(nunit* target_ptr, nspell_info const* spell_info_ptr);
		int32 calculate_melee_damage(nunit* target_ptr, uint8 hit_info);
		int32 calculate_kungfu_damage(nunit* target_ptr, uint8 hit_info, float damage, float mod_value);
		int32 calculate_spell_damage(nunit* target_ptr, uint8 hit_info, float damage, float mod_value);
		int32 do_damage(nunit* caster_ptr, int32 damage);
		int32 do_heal(nunit* caster_ptr, int32 heal);

	// stat system
	protected:
		float _get_total_stat_value(uint32 stat_index, float value = 0.0f) const;

		void _update_all_stats();
		virtual void _update_base_stats(uint32 stat_index) {}

		virtual void _update_max_health() {}
		virtual void _update_max_energy() {}
		virtual void _update_max_mana() {}
		virtual void _update_melee_damage() {}
		virtual void _update_kungfu_damage() {}
		virtual void _update_spell_damage() {}
		virtual void _update_agility() {}
		virtual void _update_health_regeneration() {}
		virtual void _update_power_Regeneration() {}

		void _update_dodge();
		void _update_crit();
		void _update_speed();
		void _update_attack_speed();

		void _update_equip_weight();
		void _update_equip_stat(uint32 stat_index);

		void _set_modifier_value(uint32 mod_type, uint32 stat_index, float value)
		{
			m_modifiers[mod_type][stat_index] = value;
		}

		float _get_modifier_value(uint32 mod_type, uint32 stat_index) const
		{
			return m_modifiers[mod_type][stat_index];
		}

		bool _can_modify_stats() const
		{
			return m_can_modify_stats;
		}

		void enable_modify_stats(bool enabled)
		{
			m_can_modify_stats = enabled;
		}

		void _regenerate(uint32 elapse);
		virtual void _regenerate_health(uint32 elapse) {}
		virtual void _regenerate_energy(uint32 elapse) {}
		virtual void _regenerate_mana(uint32 elapse) {}

	// spell system
	protected:
		void _update_spells(uint32 elapse);
		void _clear_spells();

	// spell aura system
	protected:
		void _update_auras(uint32 elapse);
		void _earse_auras(naura* aura_ptr);
		void _clear_deleted_auras();
		void _clear_auras();

		void _set_aura_bits(uint8 index)
		{
			((uint8*)m_aura_flag)[index >> 3] |= (1 << (index & 0X7));
		}
		
		bool _get_aura_bits(uint8 index) const
		{
			return (((uint8*)m_aura_flag)[index >> 3] & (1 << (index & 0X7))) != 0;
		}

		bool _aura_stack(naura* src_aura_ptr, naura* dst_aura_ptr, uint32 stack_type);

	// damage calculate
	protected:
		bool _is_immuned_to_spell(nspell_info const* spell_info_ptr);

	protected:
		// stat
		nobject_data			m_data;											// 属性管理同步类
		float					m_modifiers[EUMT_Max][EUF_UnitSeparator];		// 属性修改累计值
		bool					m_can_modify_stats;								// 控制上线属性计算
		int32					m_regen_timer;									// 恢复时钟

		// spell
		nspell_list				m_cur_spells;						// 当前作用的技能集合

		// aura
		naura_map				m_auras;							// id 索引, 相同id不同源
		naura_effect_map		m_auras_by_effect;					// 效果索引, 同源不同源
		naura_list				m_auras_deleted;					// 已经删除的aura
		uint32					m_aura_flag[MAX_AURA_NUM / sizeof(uint32)];	// aura slot标志位
	};

} // namespace nexus

#endif // _NGAME_NUNIT_H_
