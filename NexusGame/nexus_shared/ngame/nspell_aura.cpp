#include "nspell_aura.h"
#include "nunit.h"
#include "nmap.h"
#include "nlog_mt.h"
#include "nutil.h"
#include "ninfo_data.h"

#include "nmsg_spell.h"

namespace nexus {

	naura::naura() : m_owner_ptr(NULL), m_caster_id(0), m_max_duration(0), m_duration(0), m_periodic(0), m_periodic_timer(0), m_max_stack_count(0), 
		m_stack_count(0), m_max_amount(0), m_amount(0),	m_slot(0), m_in_use(false), m_deleted(false), m_spell_aura_info_ptr(NULL), 
		m_spell_info_ptr(NULL), m_effect_index(0)
	{
	}

	naura::~naura()
	{
	}

	naura* naura::construct(nunit* owner_ptr, uint64 caster, nspell_aura_info const* aura_info_ptr, nspell_info const* spell_info_ptr, uint8 effect_index)
	{
		naura* aura_ptr = new naura;
		aura_ptr->init(owner_ptr, caster, aura_info_ptr, spell_info_ptr, effect_index);
		return aura_ptr;
	}

	void naura::destroy(naura* aura_ptr)
	{
		delete aura_ptr;
	}

	bool naura::init(nunit* owner_ptr, uint64 caster, nspell_aura_info const* aura_info_ptr, nspell_info const* spell_info_ptr, uint8 effect_index)
	{
		m_owner_ptr				= owner_ptr;
		m_caster_id				= caster;
		m_spell_aura_info_ptr	= aura_info_ptr;
		m_spell_info_ptr		= spell_info_ptr;
		m_effect_index			= effect_index;

		m_slot					= 0;
		m_in_use				= false;

		// TODO: D. Zhao 受天赋影响数值的初始化
		m_max_stack_count		= m_spell_aura_info_ptr->max_stack_count;
		m_max_duration			= int32(m_spell_aura_info_ptr->max_duration * 1000);
		m_periodic				= int32(m_spell_aura_info_ptr->periodic_time * 1000);
		m_max_amount			= int32(m_spell_aura_info_ptr->value1);

		m_stack_count			= m_max_stack_count;
		m_duration				= m_max_duration;
		m_amount				= m_max_amount;
		m_periodic_timer		= m_periodic;
		return true;
	}

	void naura::update(uint32 elapse)
	{
		if (is_deleted())
		{
			return;
		}

		set_in_use(true);
		_update(elapse);
		set_in_use(false);
	}

	nunit* naura::get_caster() const
	{
		if (m_caster_id == m_owner_ptr->get_id())
		{
			return m_owner_ptr;
		}

		nmap* map_ptr = m_owner_ptr->get_map();

		if (!map_ptr)
		{
			return NULL;
		}

		return map_ptr->get_unit(m_caster_id);
	}

	void naura::on_add()
	{
		// 数据初始化时map_ptr==NULL, 不会向客户端周围玩家同步数据, 客户端请求数据时同步

		send_add_aura_message();
	}

	void naura::on_remove()
	{
		// 退出场景后map_ptr==NULL, 不会向客户端周围玩家同步数据
		send_remove_aura_message();
	}

	void naura::on_update()
	{
		send_update_aura_message();
	}

	void naura::apply_effect(bool apply, bool tick)
	{
		if (tick != is_periodic())
		{
			return;
	}

		uint32 effect_index = m_spell_aura_info_ptr->effect_type;

		if (effect_index >= EAET_Max)
		{
			NLOG_DEBUG(_T("aura effect type error, effect_type = %u"), effect_index);
			return;
		}

		if (!tick)
		{
			set_in_use(true);
		}

		(this->*(m_aura_effect_handler[effect_index]))(apply);

		if (!tick)
		{
		set_in_use(false);
	}
	}

	void naura::trigger_spell(uint32 event_type, uint64 unit_id, uint32 cast_spell_id)
	{
		if (EAET_TriggerSpell != get_effect_type() && EAET_TriggerAura != get_effect_type())
		{
			return;
		}

		if (event_type != m_spell_aura_info_ptr->trigger_event_type)
		{
			return;
		}

		if (EATET_OnCasting == m_spell_aura_info_ptr->trigger_event_type && cast_spell_id != m_spell_aura_info_ptr->cast_spell_id.name_crc)
		{
			return;
		}

		if (!s_util.roll_chance_f(m_spell_aura_info_ptr->rate))
		{
			return;
		}

		// 触发
		nunit* target_ptr = NULL;
		nmap* map_ptr = m_owner_ptr->get_map();

		if (!map_ptr)
		{
			return;
		}

		switch (m_spell_aura_info_ptr->target_type)
		{
		case EATTT_Self:
			{
				target_ptr = m_owner_ptr;
			}
			break;
		case EATTT_Caster:
			{
				target_ptr = map_ptr->get_unit(m_caster_id);
			}
			break;
		case EATTT_Party:
			{
			}
			break;
		case EATTT_CasterParty:
			{
			}
			break;
		case EATTT_Attacker:
			{
				target_ptr = map_ptr->get_unit(unit_id);
			}
			break;
		default:
			{
			}
			break;
		}

		if (!target_ptr)
		{
			return;
		}

		if (EAET_TriggerSpell == get_effect_type())
		{
			m_owner_ptr->cast_spell(m_spell_aura_info_ptr->trigger_spell_id.name_crc, target_ptr->get_id(), vector3::zero, 0.0f, 0, 0, m_caster_id, NULL, m_spell_aura_info_ptr);
		}
		else
		{
			nspell_aura_info const* aura_info_ptr = s_spell_aura_info.get_info(m_spell_aura_info_ptr->trigger_aura_id.name_crc);

			if (!aura_info_ptr)
			{
				NLOG_DEBUG(_T("s_spell_aura_info.get_info error, id = %s"), m_spell_aura_info_ptr->trigger_aura_id.name_str.c_str());
				return;
			}

			naura* aura_ptr = naura::construct(target_ptr, m_caster_id, aura_info_ptr, NULL, 0);
			target_ptr->add_aura(aura_ptr);
		}
	}

	void naura::on_damage(uint64 unit_id, int32 value)
	{
		// 触发
		trigger_spell(EATET_OnHited, unit_id);

		// 打断
		if (get_interrupt_flags() & EAIF_Damage)
		{
			m_owner_ptr->remove_aura(this);
			return;
		}

		if (value >= 0)
		{
			return;
		}

		if (get_interrupt_flags() & EAIF_Damage2)
		{
			m_amount += (-value);

			if (m_amount > m_amount)
			{
				m_owner_ptr->remove_aura(this);
				return;
			}
		}
	}

	void naura::on_dead()
	{
		// 触发
		trigger_spell(EATET_OnDead);

		// 打断
		if (get_interrupt_flags() & EAIF_Dead)
		{
			m_owner_ptr->remove_aura(this);
			return;
		}
	}

	void naura::on_casting(uint32 cast_spell_id)
	{
		// 触发
		trigger_spell(EATET_OnCasting);
	}

	void naura::on_dispeled(uint64 unit_id)
	{
		// 触发
		trigger_spell(EATET_Ondispeled);

		// 打断
		if (get_interrupt_flags() & EAIF_Dispel)
		{
			m_owner_ptr->remove_aura(this);
			return;
		}
	}

	void naura::send_add_aura_message()
	{
		// 判断该buff是否需要同步给客户端
		if (!m_spell_aura_info_ptr->to_client)
		{
			return;
		}

		tagS2C_AddAura reply;
		reply.unit_id					= m_owner_ptr->get_id();
		reply.aura_data.info_id			= m_spell_aura_info_ptr->id.name_crc;
		reply.aura_data.slot			= m_slot;
		reply.aura_data.stack_count		= m_stack_count;
		reply.aura_data.remain_time		= m_duration;
		reply.aura_data.max_duration	= m_max_duration;
		m_owner_ptr->send_message_to_aoi(&reply, sizeof(reply));

		// group, 如果是玩家的话发送给不在aoi内的小队成员
	}

	void naura::send_remove_aura_message()
	{
		// 判断该buff是否需要同步给客户端
		if (!m_spell_aura_info_ptr->to_client)
		{
			return;
		}

		tagS2C_RemoveAura reply;
		reply.unit_id					= m_owner_ptr->get_id();
		reply.slot						= m_slot;
		m_owner_ptr->send_message_to_aoi(&reply, sizeof(reply));

		// group, 如果是玩家的话发送给不在aoi内的小队成员
	}

	void naura::send_update_aura_message()
	{
		// 判断该buff是否需要同步给客户端
		if (!m_spell_aura_info_ptr->to_client)
		{
			return;
		}

		tagS2C_UpdateAura reply;
		reply.unit_id					= m_owner_ptr->get_id();
		reply.slot						= m_slot;
		reply.stack_count				= m_stack_count;
		reply.remain_time				= m_duration;
		reply.max_duration				= m_max_duration;
		m_owner_ptr->send_message_to_aoi(&reply, sizeof(reply));

		// group, 如果是玩家的话发送给不在aoi内的小队成员
	}

	void naura::_update(uint32 elapse)
	{
		if (m_duration > 0)
		{
			m_duration -= elapse;

			if (m_duration < 0)
			{
				m_duration = 0;
			}
		}

		if (is_periodic())
		{
			m_periodic_timer -= elapse;

			if (m_periodic_timer <= 0)
			{
				m_periodic_timer += m_periodic;
				apply_effect(true, true);
			}
		}

		// 如果是channel aura的话, 检测caster的有效性
		if (m_spell_info_ptr && ESCast_Channel == m_spell_info_ptr->cast_type && m_caster_id != m_owner_ptr->get_id())
		{
			nunit* caster_ptr = get_caster();

			if (!caster_ptr)
			{
				m_owner_ptr->remove_aura(this);
				return;
			}

			// 天赋影响
			if (!caster_ptr->is_in_range(m_owner_ptr, m_spell_info_ptr->min_range, m_spell_info_ptr->max_range))
			{
				m_owner_ptr->remove_aura(this);
				return;
			}
		}

		if (0 == m_duration && !is_permanent())
		{
			m_owner_ptr->remove_aura(this);
			return;
		}
	}

} // namespace nexus
