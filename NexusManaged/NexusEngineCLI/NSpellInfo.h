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
	* class NSpellInfo
	* 对nexus::nspell_info封装
	* 为CLR提供方面Native struct接口
	*/
	DECLARE_ATT_INFO_CLASS_BEGIN(NSpellInfo, nspell_info, nspell_info_dictionary);
		DECLARE_ATTINFO_PROPERTY(UInt32,Type,NativePtr->type,"类型");
		DECLARE_ATTINFO_PROPERTY(UInt32,SubType,NativePtr->sub_type,"子类型");
		DECLARE_ATTINFO_PROPERTY(UInt32,Level,NativePtr->level,"等级");

		// 是否主动技能, (true = 主动技能, false = 被动技能)
		DECLARE_ATTINFO_PROPERTY(bool,Active,NativePtr->active,"是否主动技能");
		DECLARE_ATTINFO_PROPERTY(bool,CanCastInCombat,NativePtr->can_cast_in_combat,"是否可以在战斗状态释放");
		DECLARE_ATTINFO_PROPERTY(bool,AutoPathFind,NativePtr->auto_path_find,"是否可寻路到达释放位置");
		DECLARE_ATTINFO_PROPERTY(bool,NoLineOfSight,NativePtr->no_line_of_sight,"是否跳过视线检测");
		DECLARE_ATTINFO_PROPERTY(bool,CanCastWhileFeared,NativePtr->can_cast_while_feared,"恐惧时是否可以释放");
		DECLARE_ATTINFO_PROPERTY(bool,CanCastWhileSilence,NativePtr->can_cast_while_silence,"沉默时是否可以释放");
		DECLARE_ATTINFO_PROPERTY(bool,CanCastWhileRoot,NativePtr->can_cast_while_root,"定身时是否可以释放");

		DECLARE_ATTINFO_PROPERTY(UInt32,WeaponType,NativePtr->weapon_type,"武器限制类型");
		DECLARE_ATTINFO_PROPERTY(UInt32,MainHand,NativePtr->main_hand,"主手武器类型限制");
		DECLARE_ATTINFO_PROPERTY(UInt32,OffHand,NativePtr->off_hand,"副手武器类型限制");

		DECLARE_ATTINFO_PROPERTY(UInt32,CastTargetType,NativePtr->cast_target_type,"释放目标类型");
		DECLARE_ATTINFO_PROPERTY(float,MinRange,NativePtr->min_range,"选择目标的最小距离");
		DECLARE_ATTINFO_PROPERTY(float,MaxRange,NativePtr->max_range,"选择目标的最大距离");

		DECLARE_ATTINFO_PROPERTY(UInt32,EffectTargetType,NativePtr->effect_target_type,"作用目标类型");
		DECLARE_ATTINFO_PROPERTY(float,Radius,NativePtr->radius,"区域半径");
		DECLARE_ATTINFO_PROPERTY(float,Angle,NativePtr->angle,"区域扇形角度");
		DECLARE_ATTINFO_PROPERTY(float,Weight,NativePtr->weight,"区域宽度");
		DECLARE_ATTINFO_PROPERTY(float,Length,NativePtr->length,"区域长度");

		DECLARE_ATTINFO_PROPERTY(UInt32,CastType,NativePtr->cast_type,"释放方式");
		DECLARE_ATTINFO_PROPERTY(UInt32,InterruptFlags,NativePtr->interrupt_flags,"打断方式");
		DECLARE_ATTINFO_PROPERTY(bool,CanRepeat,NativePtr->can_repeat,"是否可以持续释放");
		DECLARE_ATTINFO_PROPERTY(bool,CanMove,NativePtr->can_move,"是否可以移动");
		DECLARE_ATTINFO_PROPERTY(bool,CanTurn,NativePtr->can_turn,"是否可以转向");
		DECLARE_ATTINFO_PROPERTY(float,MinRepeatTime,NativePtr->min_repeat_time,"最短持续时间");
		DECLARE_ATTINFO_PROPERTY(float,MaxRepeatTime,NativePtr->max_repeat_time,"最长持续时间");
		DECLARE_ATTINFO_PROPERTY(float,SpeedScaleCoef,NativePtr->speed_scale_coef,"连续释放时速度缩放系数");
		DECLARE_ATTINFO_PROPERTY(float,EffectScaleCoef,NativePtr->effect_scale_coef,"连续释放时效果值缩放系数");
		DECLARE_ATTINFO_PROPERTY(float,CastingTime,NativePtr->casting_time,"吟唱时间");
		DECLARE_ATTINFO_PROPERTY(UInt32,MaxTickCount,NativePtr->max_tick_count,"最大引导次数");
		DECLARE_ATTINFO_PROPERTY(float,PeriodicTime,NativePtr->periodic_time,"引导周期时间");
		DECLARE_ATTINFO_PROPERTY(float,Speed,NativePtr->speed,"飞行的速度");

		DECLARE_ATTINFO_PROPERTY(bool,GlobalCDNotEffect,NativePtr->global_cd_not_effect,"是否不受全局cd影响");
		DECLARE_ATTINFO_PROPERTY(float,GlobalCDTime,NativePtr->global_cd_time,"触发的全局cd时间");
		DECLARE_ATTINFO_PROPERTY(float,CDTime,NativePtr->cd_time,"触发的技能cd时间");

		DECLARE_ATTINFO_PROPERTY(UInt32,CostType,NativePtr->cost_type,"技能消耗方式");
		DECLARE_ATTINFO_PROPERTY(float,CostValue,NativePtr->cost_value,"技能消耗属性值");
		DECLARE_ATTINFO_PROPERTY(float,CostScaleCoef,NativePtr->cost_scale_coef,"连续释放时消耗缩放系数");
		DECLARE_ATTINFO_NAME_PROPERTY(ReagentId,NativePtr->reagent_id,"技能所需的药剂");


		DECLARE_ATTINFO_NAME_PROPERTY(SpellEffectId_0,NativePtr->spell_effect_id[0],"技能附加效果0");
		DECLARE_ATTINFO_NAME_PROPERTY(SpellEffectId_1,NativePtr->spell_effect_id[1],"技能附加效果1");
		DECLARE_ATTINFO_NAME_PROPERTY(SpellEffectId_2,NativePtr->spell_effect_id[2],"技能附加效果2");
		
		// 客户端
		DECLARE_ATTINFO_STRING_PROPERTY(SpellIcon, NativePtr->spell_icon, "技能图标");
		DECLARE_ATTINFO_STRING_PROPERTY(ClientEffect, NativePtr->client_effect, "技能动作特效表现");

	DECLARE_ATT_INFO_CLASS_END();

}
