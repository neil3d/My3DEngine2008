#pragma once
#include "NEngine.h"
#include "NFileSystem.h"
#include "NAttDataHelper.h"
#include "nspell_define.h"
#include "ngameframework.h"
#include <boost/unordered_map.hpp>
#include <boost/foreach.hpp>

namespace NexusEngine
{
	using namespace NexusEngineExtension;
	using namespace nexus;
	using namespace gameframework;

	/**
	* class NSpellAuraInfo
	* 对nexus::nspell_aura_info封装
	* 为CLR提供方面Native struct接口
	*/
	DECLARE_ATT_INFO_CLASS_BEGIN(NSpellAuraInfo, nspell_aura_info, nspell_aura_info_dictionary);

		DECLARE_ATTINFO_PROPERTY(UInt32,effect_type,NativePtr->effect_type,"效果类型");
		DECLARE_ATTINFO_PROPERTY(bool,positive,NativePtr->positive,"是否增益");
		DECLARE_ATTINFO_PROPERTY(bool,to_client,NativePtr->to_client,"是否同步给客户端");
		DECLARE_ATTINFO_PROPERTY(UInt32,interrupt_flags,NativePtr->interrupt_flags,"打断标志");

		DECLARE_ATTINFO_PROPERTY(UInt32,diminishing_group,NativePtr->diminishing_group,"持续时间衰减分组");
		DECLARE_ATTINFO_PROPERTY(float,max_duration,NativePtr->max_duration,"最大持续时间");
		DECLARE_ATTINFO_PROPERTY(float,periodic_time,NativePtr->periodic_time,"周期时间");

		DECLARE_ATTINFO_PROPERTY(UInt32,dispel_level,NativePtr->dispel_level,"驱散等级");

		DECLARE_ATTINFO_PROPERTY(UInt32,replace_type,NativePtr->replace_type,"替换类型");
		DECLARE_ATTINFO_PROPERTY(UInt32,level,NativePtr->level,"等级");
		DECLARE_ATTINFO_PROPERTY(UInt32,ssrc_stack_type,NativePtr->ssrc_stack_type,"同源叠加方式");
		DECLARE_ATTINFO_PROPERTY(UInt32,dsrc_stack_type,NativePtr->dsrc_stack_type,"不源叠加方式");
		DECLARE_ATTINFO_PROPERTY(UInt32,max_stack_count,NativePtr->max_stack_count,"叠加数量");

		DECLARE_ATTINFO_PROPERTY(UInt32,death,NativePtr->death,"死亡不消失计时方式");
		DECLARE_ATTINFO_PROPERTY(UInt32,offline,NativePtr->offline,"下线不消失计时方式");

		DECLARE_ATTINFO_PROPERTY(UInt32,stat_index_0,NativePtr->stat_index[0],"影响属性0");
		DECLARE_ATTINFO_PROPERTY(UInt32,stat_index_1,NativePtr->stat_index[1],"影响属性1");
		DECLARE_ATTINFO_PROPERTY(UInt32,stat_index_2,NativePtr->stat_index[2],"影响属性2");
		DECLARE_ATTINFO_PROPERTY(UInt32,stat_index_3,NativePtr->stat_index[3],"影响属性3");

		DECLARE_ATTINFO_PROPERTY(float,stat_value_0,NativePtr->stat_value[0],"影响属性值0");
		DECLARE_ATTINFO_PROPERTY(float,stat_value_1,NativePtr->stat_value[1],"影响属性值1");
		DECLARE_ATTINFO_PROPERTY(float,stat_value_2,NativePtr->stat_value[2],"影响属性值2");
		DECLARE_ATTINFO_PROPERTY(float,stat_value_3,NativePtr->stat_value[3],"影响属性值3");

		DECLARE_ATTINFO_PROPERTY(UInt32,damage_type,NativePtr->damage_type,"伤害类型");
		DECLARE_ATTINFO_PROPERTY(float,value1,NativePtr->value1,"数值1");
		DECLARE_ATTINFO_PROPERTY(float,value2,NativePtr->value2,"数值2");

		DECLARE_ATTINFO_PROPERTY(Int32,threat,NativePtr->threat,"仇恨值");

		DECLARE_ATTINFO_PROPERTY(UInt32,spell_mod_type,NativePtr->spell_mod_type,"天赋效果类型");
		DECLARE_ATTINFO_PROPERTY(UInt32,spell_mod_sub_type,NativePtr->spell_mod_sub_type,"天赋修改方式");
		DECLARE_ATTINFO_NAME_PROPERTY(spell_mod_spell_id,NativePtr->spell_mod_spell_id,"天赋对哪个技能有影响");
		DECLARE_ATTINFO_PROPERTY(float,spell_mod_value1,NativePtr->spell_mod_value1,"天赋作用值");
		DECLARE_ATTINFO_NAME_PROPERTY(spell_mod_value2,NativePtr->spell_mod_value2,"天赋替换id值");

		DECLARE_ATTINFO_PROPERTY(UInt32,trigger_event_type,NativePtr->trigger_event_type,"触发方式");
		DECLARE_ATTINFO_NAME_PROPERTY(cast_spell_id,NativePtr->cast_spell_id,"使用技能id");
		DECLARE_ATTINFO_PROPERTY(float,rate,NativePtr->rate,"触发几率");
		DECLARE_ATTINFO_PROPERTY(UInt32,target_type,NativePtr->target_type,"触发给谁");
		DECLARE_ATTINFO_PROPERTY(UInt32,max_chain_target,NativePtr->max_chain_target,"目标的最大数目");
		DECLARE_ATTINFO_NAME_PROPERTY(trigger_spell_id,NativePtr->trigger_spell_id,"触发技能id");
		DECLARE_ATTINFO_NAME_PROPERTY(trigger_aura_id,NativePtr->trigger_aura_id,"触发buff id");

		// 客户端
		DECLARE_ATTINFO_STRING_PROPERTY(aura_icon, NativePtr->aura_icon, "图标");
		DECLARE_ATTINFO_STRING_PROPERTY(add_effect, NativePtr->add_effect, "添加特效");
		DECLARE_ATTINFO_STRING_PROPERTY(duration_effect, NativePtr->duration_effect, "持续特效");
		DECLARE_ATTINFO_STRING_PROPERTY(remove_effect, NativePtr->remove_effect, "消失特效");

	DECLARE_ATT_INFO_CLASS_END();

}
