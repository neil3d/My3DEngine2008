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
	* class NSpellEffectInfo
	* 对nexus::nspell_effect_info封装
	* 为CLR提供方面Native struct接口
	*/
	DECLARE_ATT_INFO_CLASS_BEGIN(NSpellEffectInfo, nspell_effect_info, nspell_effect_info_dictionary);
	
		DECLARE_ATTINFO_PROPERTY(UInt32,effect_trigger_type,NativePtr->effect_trigger_type,"生效方式");
		DECLARE_ATTINFO_PROPERTY(UInt32,trigger_target_type,NativePtr->trigger_target_type,"给谁");
		DECLARE_ATTINFO_PROPERTY(float,rate,NativePtr->rate,"生效几率");

		DECLARE_ATTINFO_PROPERTY(UInt32,effect_type,NativePtr->effect_type,"技能效果类型");

		DECLARE_ATTINFO_PROPERTY(UInt32,damage_type,NativePtr->damage_type,"伤害类型");
		DECLARE_ATTINFO_PROPERTY(float,value1,NativePtr->value1,"数值1");
		DECLARE_ATTINFO_PROPERTY(float,value2,NativePtr->value2,"数值2");

		DECLARE_ATTINFO_NAME_PROPERTY(dst_teleport_id,NativePtr->dst_teleport_id,"传送目的地");
		DECLARE_ATTINFO_NAME_PROPERTY(summon_id,NativePtr->summon_id,"召唤物");

		DECLARE_ATTINFO_PROPERTY(Int32,threat,NativePtr->threat,"仇恨值");

		DECLARE_ATTINFO_NAME_PROPERTY(trigger_spell_id,NativePtr->trigger_spell_id,"触发技能");
		DECLARE_ATTINFO_NAME_PROPERTY(apply_aura_id,NativePtr->apply_aura_id,"扩展技能效果");

	DECLARE_ATT_INFO_CLASS_END();

}
