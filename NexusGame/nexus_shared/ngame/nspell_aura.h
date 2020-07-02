/**
 *	nexus ngame - naura
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Jun, 2010
 */

#ifndef _NGAME_NAURA_H_
#define _NGAME_NAURA_H_

#include "ncommon.h"
#include "nspell_define.h"

namespace nexus {

	class nunit;
	struct nspell_aura_info;

	class naura : private nnoncopyable
	{
	public:
		naura();
		virtual ~naura();

		static naura* construct(nunit* owner_ptr, uint64 caster, nspell_aura_info const* aura_info_ptr, nspell_info const* spell_info_ptr, uint8 effect_index);
		static void destroy(naura* aura_ptr);

		bool init(nunit* owner_ptr, uint64 caster, nspell_aura_info const* aura_info_ptr, nspell_info const* spell_info_ptr, uint8 effect_index);
		void update(uint32 elapse);

		nunit* get_owner() const									{ return m_owner_ptr; }
		nunit* get_caster() const;
		nspell_aura_info const* get_spell_aura_info() const			{ return m_spell_aura_info_ptr; }
		uint32 get_info_id() const									{ return m_spell_aura_info_ptr->id.name_crc; }
		uint64 get_caster_id() const								{ return m_caster_id; }
		int32 get_max_duration() const								{ return m_max_duration; }
		int32 get_duration() const									{ return m_duration; }
		uint32 get_max_amount() const								{ return m_max_amount; };
		uint32 get_amount() const									{ return m_amount; };
		uint8 get_slot() const										{ return m_slot; }
		uint8 get_max_stack() const									{ return m_max_stack_count; }
		uint8 get_stack() const										{ return m_stack_count; }
		uint32 get_effect_type() const								{ return m_spell_aura_info_ptr->effect_type; }
		uint32 get_interrupt_flags() const							{ return m_spell_aura_info_ptr->interrupt_flags; }
		bool is_positive() const									{ return m_spell_aura_info_ptr->positive; }
		uint32 get_spell_id() const									{ return (m_spell_info_ptr != NULL) ? m_spell_info_ptr->id.name_crc : 0; }
		bool is_area_aura() const									{ return false; }
		bool is_permanent() const									{ return m_spell_aura_info_ptr->max_duration <= 0.0f; }
		bool is_periodic() const									{ return m_spell_aura_info_ptr->periodic_time > 0.0f; }
		bool is_offline_persistent() const							{ return !(m_spell_aura_info_ptr->interrupt_flags & EAIF_OffLine); }
		bool is_dead_persistent() const								{ return !(m_spell_aura_info_ptr->interrupt_flags & EAIF_Dead); }
		bool is_in_use() const										{ return m_in_use; }
		bool is_deleted() const										{ return m_deleted; }

		void set_max_duration(int32 max_duration)					{ m_max_duration = max_duration; }
		void set_duration(int32 duration)							{ m_duration = duration; }
		void set_periodic(int32 periodic)							{ m_periodic = periodic; }
		void set_slot(uint8 slot)									{ m_slot = slot; }
		void set_max_stack(uint8 max_stack)							{ m_max_stack_count =  max_stack; }
		void set_stack(uint8 stack)									{ m_stack_count = stack; }
		void set_amount(int32 amount)								{ m_amount = amount; }
		void set_in_use(bool in_use)								{ m_in_use = in_use; }
		void set_deleted(bool deleted)								{ m_deleted = deleted; }

		void on_add();
		void on_remove();
		void on_update();
		void apply_effect(bool apply, bool tick = false);
		void trigger_spell(uint32 event_type, uint64 unit_id = 0, uint32 cast_spell_id = 0);

		void on_damage(uint64 unit_id, int32 value);
		void on_dead();
		void on_casting(uint32 cast_spell_id);
		void on_dispeled(uint64 unit_id);

		void send_add_aura_message();
		void send_remove_aura_message();
		void send_update_aura_message();

	private:
		// buff效果处理函数
		void _handle_null(bool apply) {}
		void _handle_mod_stat_base_value(bool apply);
		void _handle_mod_stat_base_pct(bool apply);
		void _handle_mod_stat_total_value(bool apply);
		void _handle_mod_stat_total_pct(bool apply);
		void _handle_periodic_damage(bool apply);
		void _handle_periodic_heal(bool apply);
		void _handle_fear(bool apply);
		void _handle_silence(bool apply);
		void _handle_increase_speed(bool apply);
		void _handle_decrease_speed(bool apply);
		void _handle_root(bool apply);
		void _handle_spell_mod(bool apply);
		void _handle_trigger_spell(bool apply);

	private:
		virtual void _update(uint32 elapse);

	private:
		typedef void(naura::*naura_effect_handler)(bool apply);
		static naura_effect_handler	m_aura_effect_handler[EAET_Max];

	private:
		nunit*						m_owner_ptr;				// 作用的unit
		uint64						m_caster_id;				// 释放者id, unit or item
		int32						m_max_duration;				// 最大持续时间
		int32						m_duration;					// 当前剩余时间
		int32						m_periodic;					// tick周期时间
		int32						m_periodic_timer;			// 下次tick剩余时间
		uint8						m_max_stack_count;			// 最大叠加计数
		uint8						m_stack_count;				// 当前叠加计数
		int32						m_max_amount;				// 最大作用数值
		int32						m_amount;					// 当前作用数值
		uint8						m_slot;						// 客户端显示位置
		bool						m_in_use;					// 正在作用效果, 防止递归
		bool						m_deleted;					// 是否已被删除

		nspell_aura_info const*		m_spell_aura_info_ptr;		// buff模板指针
		nspell_info const*			m_spell_info_ptr;			// 由哪个技能产生
		uint8						m_effect_index;				// 技能中哪个效果产生该buff
	};

} // namespace nexus

#endif // _NGAME_NAURA_H_
