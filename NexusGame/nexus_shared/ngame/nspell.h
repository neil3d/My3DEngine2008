/**
 *	nexus ngame - nspell
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Jun, 2010
 */

#ifndef _NGAME_NSPELL_H_
#define _NGAME_NSPELL_H_

#include "ncommon.h"
#include "nspell_define.h"
#include "base_define.h"
#include "math/vector.h"

namespace nexus {

	class nunit;

	enum ESpellStatus
	{
		ESStatus_None					= 0,			// 初始化状态
		ESStatus_Preparing				= 1,			// 准备状态, (起手, 吟唱)
		ESStatus_Casting				= 2,			// 释放状态, (出手, 引导)
		ESStatus_Delayed				= 3,			// 延时状态, (飞行)
		ESStatus_Finished				= 4,			// 技能释放完毕, 非延时效果作用完毕
		ESStatus_Deleted				= 5,			// 可删除状态
		ESStatus_Max
	};

	class nspell : private nnoncopyable
	{
	public:
		nspell();
		virtual ~nspell();

		static nspell* construct(nunit* caster_ptr, nspell_info const* spell_info_ptr, uint64 target_id, const vector3& target_pos, float time_stamp);
		static void destroy(nspell* spell_ptr);

		bool init(nunit* caster_ptr, nspell_info const* spell_info_ptr, uint64 target_id, const vector3& target_pos, float time_stamp);

		void set_trigger_spell(uint64 original_caster_id, nspell_info const* spell_info_ptr);
		void set_trigger_aura(uint64 original_caster_id, nspell_aura_info const* aura_info_ptr);
		void set_caster_item_id(uint64 item_id)					{ m_caster_item_id = item_id; }
		void set_status(uint8 status)							{ m_status = status; }
		void set_cast_index(uint32 index)						{ m_cast_index = index; }

		uint8 get_status() const								{ return m_status; }
		nspell_info const* get_spell_info() const				{ return m_spell_info_ptr; }
		uint32 get_spell_id() const								{ return m_spell_info_ptr->id.name_crc; }
		uint32 get_interrupt_flags() const						{ return m_spell_info_ptr->interrupt_flags; }
		bool is_triggered() const								{ return m_trigger_spell_info_ptr || m_trigger_aura_info_ptr; }
		bool is_channeled_spell() const							{ return ESCast_Channel == m_spell_info_ptr->cast_type; }

		void prepare();
		void cancel(uint8 result);
		void update(uint32 elapse);
		void cast();
		void finish();

		void on_damage(uint64 unit_id, int32 value);

	private:
		uint8 _can_cast_spell(bool strict = true);
		uint8 _check_weapon();
		uint8 _check_target();
		uint8 _check_cost();
		uint8 _check_effect();

		void _set_orient();
		void _set_cast_timer();
		void _set_channel_timer();
		void _set_category_cd();
		void _set_spell_cd();

		void _fill_targets(uint32 effect_mask);
		void _fill_no_trigger_targets(uint32 index);
		void _add_unit_target(nunit* target_ptr, uint32 index);
		void _add_delay_unit_target(nunit* target_ptr, uint32 index, int32 delay);

		bool _is_alive_unit_targets();
		bool _is_need_send_to_client();

		void _take_cost();
		void _take_reagents();
		void _take_cast_item();

		void _send_cast_result(uint8 result);
		void _send_spell_prepare(int32 max_time, int32 cur_time);
		void _send_spell_cast();
		void _send_spell_channel(int32 max_time, int32 cur_time);
		void _send_interrupted(uint8 result);
		void _send_spell_hit(nunit* target_ptr, uint8 hit_info, int32 value, int32 absorb);

	private:
		// buff效果处理函数
		void _handle_null(uint32 index) {}
		void _handle_trigger_spell(uint32 index);
		void _handle_apply_aura(uint32 index);
		void _handle_instant_kill(uint32 index);
		void _handle_melee_damage(uint32 index);
		void _handle_kungfu_damage(uint32 index);
		void _handle_spell_damage(uint32 index);
		void _handle_heal(uint32 index);
		void _handle_mod_power(uint32 index);
		void _handle_heal_pct(uint32 index);
		void _handle_mod_power_pct(uint32 index);
		void _handle_dispel(uint32 index);
		void _handle_resurrect(uint32 index);
		void _handle_knock_down(uint32 index);
		void _handle_knock_back(uint32 index);

	private:
		typedef void(nspell::*nspell_effect_handler)(uint32 index);
		static nspell_effect_handler	m_spell_effect_handler[ESET_Max];

	private:
		struct ntarget_info
		{
			nunit*					target_ptr;
			uint32					effect_mask;
			uint8					hit_info;

			ntarget_info() : target_ptr(NULL), effect_mask(0), hit_info(0) {}
		};

		struct ndelay_target_info
		{
			uint64					target_id;
			uint32					effect_mask;
			int32					delay_time;

			ndelay_target_info() : target_id(0), effect_mask(0), delay_time(0) {}
		};

		typedef std::list<ntarget_info> ntarget_list;
		typedef std::list<ndelay_target_info> ndelay_target_list;
		typedef std::list<uint64> nchannel_target_list;

		void _trigger_effect(uint32 trigger_type);
		void _cast_effect();
		void _do_effect_on_unit(uint32 index, ntarget_info* target_ptr);
		void _do_effect_on_delay_unit(uint32 index, ndelay_target_info* target_ptr);
		void _update_delay_target(uint32 elapse);

	private:
		nunit*						m_caster_ptr;				// 释放unit
		uint64						m_original_caster_id;		// trigger技能的原始释放者id
		uint64						m_caster_item_id;			// 物品
		uint32						m_cast_index;				// 当前释放的次数索引
		uint8						m_status;					// 技能状态
		int32						m_periodic;					// 引导tick周期
		int32						m_delay_time;				// 吟唱时间, 引导时间, 飞行时间等
		int32						m_timer;					// 计时

		uint64						m_target_id;				// client请求信息
		vector3						m_target_pos;				// client请求信息
		float						m_time_stamp;				// client请求信息
		bool						m_client_time;				// 是否使用客户端时间戳
		
		nspell_info const*			m_spell_info_ptr;			// 技能模板
		nspell_info const*			m_trigger_spell_info_ptr;	// trigger该技能的技能模板
		nspell_aura_info const*		m_trigger_aura_info_ptr;	// trigger该技能的buff模板

		nspell_effect_info const*	m_effect_info_ptr[MAX_EFFECT_NUM];	// 技能效果
		bool						m_triggered[MAX_EFFECT_NUM];		// 技能效果是否触发

		ntarget_list				m_targets;					// 技能作用对象
		ndelay_target_list			m_delay_targets;			// 技能延时作用对象
		nchannel_target_list		m_channel_targets;			// 引导对象
		// item
		// game_object

		nunit*						m_unit_target;				// 效果处理函数中的应用对象
		uint8						m_hit_info;					// 命中信息
		int32						m_damage;					// 伤害数值
		int32						m_heal;						// 治疗数值
		bool						m_trigger_hit;				// 命中发生
		bool						m_trigger_crit;				// 暴击发生
		bool						m_trigger_dead;				// 致死发生
		// item
		// game_object
	};

} // namespace nexus

#endif // _NGAME_NSPELL_H_
