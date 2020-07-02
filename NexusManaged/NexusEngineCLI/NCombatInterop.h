#pragma once

#include "nspell_define.h"
#include "nspell_data.h"


namespace NexusEngine
{
	// 对战斗相关的Native data的interop封装
	public enum class SpellConstant : System::UInt32
	{
		MaxSpellNum		= MAX_SPELL_NUM,
		MaxAuraNum		= MAX_AURA_NUM,
		MaxCDNum		= MAX_CD_NUM,
	};

	// ref nspell_define.h nspell_data.h
	// 技能主类型
	public enum class SpellType : System::UInt32
	{
		Null		= ESpell_Null,
		Class		= ESpell_Class,			// 职业技能, 子类型ESpellClassSubType
		Profession	= ESpell_Profession,	// 专业技能, 子类型ESpellProfessionSubType
		Talent		= ESpell_Talent,		// 天赋技能
		Item		= ESpell_Item,			// 物品技能
		Pet			= ESpell_Pet,			// 宠物技能
		Creature	= ESpell_Creature,		// npc技能
		Misc		= ESpell_Misc,			// 未分类技能
		Max			= ESpell_Max,
	};

	// 职业技能
	public enum class SpellClassSubType : System::UInt32
	{
		Null	= ESCST_Null,
		Melee	= ESCST_Melee,			// 普通技能
		KungFu	= ESCST_KungFu,			// 武艺技能
		Spell	= ESCST_Spell,			// 术法技能
		Max		= ESCST_Max,
	};

	// 生活技能, 技能子类型(专业技能)
	public enum class SpellProfessionSubType : System::UInt32
	{
		Null	= ESPST_Null,
		Mining	= ESPST_Mining,			// 采矿
		Max		= ESPST_Max,
	};

	// 技能对武器限制类型
	public enum class SpellWeaponType : System::UInt32
	{
		Null		= ESWT_Null,			// 无限制
		MainHand	= ESWT_MainHand,		// 主手必需满足物品类型限制
		OffHand		= ESWT_OffHand,			// 副手必需满足物品类型限制
		MainAndOff	= ESWT_MainAndOff,		// 主副手都满足物品类型限制
		MainOrOff	= ESWT_MainOrOff,		// 主副手其中一个满足物品类型限制
		Max			= ESWT_Max,
	};

	// 释放目标类型
	public enum class CastTargetType : System::UInt32
	{
		Null		= ECTT_Null,			// 无目标
		
		Self		= ECTT_Self,			// 自己
		SingleEnemy	= ECTT_SingleEnemy,		// 单个活的非友善单位
		SingleFriend= ECTT_SingleFriend,	// 单个活的友善单位
		EnemyCorpse	= ECTT_EnemyCorpse,		// 单个非友善尸体
		FriendCorpse= ECTT_FriendCorpse,	// 单个友善尸体
		Point		= ECTT_Point,			// 坐标点
		
		Max			= ECTT_Max,
	};

	// 作用目标类型
	public enum class EffectTargetType : System::UInt32
	{
		Target					= EETT_Target,					// 选定的单个对象
		AllEnemyInArea1			= EETT_AllEnemyInArea1,			// 选定坐标点半径为radius的圆形区域内非友善单位
		AllEnemyInArea2			= EETT_AllEnemyInArea2,			// 选定坐标点和自己位置表示的朝向radius与angle所表示的弧形区域内非友善单位
		AllEnemyInArea3			= EETT_AllEnemyInArea3,			// 选定坐标点和自己位置表示的朝向weight与length所表示的自己出发的矩形区域内非友善单位
		AllEnemyInArea4			= EETT_AllEnemyInArea4,			// 选定坐标点为中心且和自己位置表示的朝向weight与length所表示的自己出发的矩形区域内非友善单位
		
		AllEnemyAroundCaster	= EETT_AllEnemyAroundCaster,	// 释放者周围圆形区域非友善单位
		AllFriendAroundCaster	= EETT_AllFriendAroundCaster,	// 释放者周围圆形区域友善单位

		Max						= EETT_Max,
	};

	// 释放表现方式
	public enum class SpellCastType : System::UInt32
	{
		Null	= ESCast_Null,
		Instant	= ESCast_Instant,			// 瞬发(can_repeat标示是否持续)
		Cast	= ESCast_Cast,				// 吟唱
		Channel = ESCast_Channel,			// 引导
		Max		= ESCast_Max,
	};

	// 技能打断方式
	[System::Flags]
	public enum class SpellInterruptFlags : System::UInt32
	{
		Null		= ESIF_Null,		// 死亡肯定打断
		Damage		= ESIF_Damage,		// 伤害打断
		Move		= ESIF_Move,		// 移动打断
		Turning		= ESIF_Turning,		// 旋转打断
		Interrupt	= ESIF_Interrupt,   // 技能打断
		Damage2		= ESIF_Damage2,		// 伤害损失时间
		SpellPrepare= ESIF_SpellPrepare,// 技能吟唱打断
		SpellCast	= ESIF_SpellCast,   // 技能释放打断
	};

	// 技能消耗
	public enum class SpellCostType : System::UInt32
	{
		Null			= ESCT_Null,			// 无效, 不消耗
		EnergyValue		= ESCT_EnergyValue,		// 按值消耗内力
		ManaValue		= ESCT_ManaValue,		// 按值消耗神识
		EnergyPct		= ESCT_EnergyPct,		// 按最大值的百分比消耗内力
		ManaPct			= ESCT_ManaPct,			// 按最大值的百分比消耗神识
		ManaPerLevel	= ESCT_ManaPerLevel,	// 每一级消耗的神识值
		EnergyPerLevel	= ESCT_EnergyPerLevel,	// 每一级消耗的内力值
		ManaAndItem		= ESCT_ManaAndItem,		// 神识值与物品
		EnergyAndItem	= ESCT_EnergyAndItem,	// 内力值与物品
		Item			= ESCT_Item,			// 物品
		Max				= ESCT_Max,
	};

	// 技能效果生效事件(主动)
	public enum class SpellEffectTriggerType : System::UInt32
	{
		Null			= ESETT_Null,
		SpellPrepare	= ESETT_SpellPrepare,		// 准备
		SpellCasting	= ESETT_SpellCasting,		// 释放
		SpellInterrupt	= ESETT_SpellInterrupt,		// 被动打断
		HitTarget		= ESETT_HitTarget,			// 命中目标
		Crit			= ESETT_Crit,				// 暴击
		EffectDead		= ESETT_EffectDead,			// 效果致死
		Max				= ESETT_Max,
	};

	// 技能效果技能给谁
	public enum class SpellTriggerTargetType : System::UInt32
	{
		Null		= ESTTT_Null,
		Targets		= ESTTT_Targets,		// 作用目标集合所有成员
		Caster		= ESTTT_Caster,			// 技能释放者
		CasterParty	= ESTTT_CasterParty,	// 技能释放者的小队成员
		
		Max			= ESTTT_Max,
	};

	// 技能效果枚举
	public enum class SpellEffectType : System::UInt32
	{
		Null			= ESET_Null,			// 空效果
		
		TriggerSpell	= ESET_TriggerSpell,	// 触发技能
		ApplyAura		= ESET_ApplyAura,		// 应用buff扩展技能效果
		Instantkill		= ESET_Instantkill,		// 秒杀
		MeleeDamage		= ESET_MeleeDamage,		// 普通伤害
		KungFuDamage	= ESET_KungFuDamage,	// 武艺伤害
		SpellDamage		= ESET_SpellDamage,		// 术法伤害
		Heal			= ESET_Heal,			// 治疗
		ModPower		= ESET_ModPower,		// 修改能量值, (内力, 神识)
		HealPct			= ESET_HealPct,			// 按百分比直接治疗
		ModPowerPct		= ESET_ModPowerPct,		// 按百分比修改能量值, (内力, 神识)
		Dispel			= ESET_Dispel,			// 驱散
		Resurrect		= ESET_Resurrect,		// 复活
		KnockDown		= ESET_KnockDown,		// 击倒
		KnockBack		= ESET_KnockBack,		// 击退击飞
		
		Max				= ESET_Max,
	};

	// 伤害类型
	public enum class SpellDMGType : System::UInt32
	{
		Null	= ESDT_Null,
		Physics	= ESDT_Physics,			// 物理(力量, 武艺)减免伤害防御, 韧性
		Metal	= ESDT_Metal,			// 金, 减免伤害金灵力
		Wood	= ESDT_Wood,			// 木, 减免伤害木灵力
		Water	= ESDT_Water,			// 水, 减免伤害水灵力
		Fire	= ESDT_Fire,			// 火, 减免伤害火灵力
		Soil	= ESDT_Soil,			// 土, 减免伤害土灵力
		Max		= ESDT_Max,
	};

	// buff效果枚举
	public enum class AuraEffectType : System::UInt32
	{
		Null				= EAET_Null,				// 空效果
		
		ModStatBaseValue	= EAET_ModStatBaseValue,	// base_value方式修改属性值，stat_index, stat_value
		ModStatBasePct		= EAET_ModStatBasePct,		// base_pct方式修改属性值，stat_index, stat_value
		ModStatTotalValue	= EAET_ModStatTotalValue,	// total_value方式修改属性值，stat_index, stat_value
		ModStatTotalPct		= EAET_ModStatTotalPct,		// total_pct方式修改属性值，stat_index, stat_value
		
		PeriodicDamage		= EAET_PeriodicDamage,		// 周期伤害
		PeriodicHeal		= EAET_PeriodicHeal,		// 周期治疗
		Fear				= EAET_Fear,				// 恐惧
		Silence				= EAET_Silence,				// 沉默
		IncreaseSpeed		= EAET_IncreaseSpeed,		// 加速
		DecreaseSpeed		= EAET_DecreaseSpeed,		// 减速
		Root				= EAET_Root,				// 定身
		
		SpellMod			= EAET_SpellMod,			// 天赋修改技能
		TriggerSpell		= EAET_TriggerSpell,		// 触发技能
		TriggerAura			= EAET_TriggerAura,			// 触发buff
		
		Max					= EAET_Max,
	};


	// buff打断方式(时间结束)
	[System::Flags]
	public enum class AuraInterruptFlags : System::UInt32
	{
		Null		= EAIF_Null,			// 不打断
		Move		= EAIF_Move,			// 移动
		EnterCombat	= EAIF_EnterCombat,		// 进入战斗状态
		Cancel		= EAIF_Cancel,			// 手动取消
		Damage		= EAIF_Damage,			// 受到任何伤害
		Dead		= EAIF_Dead,			// 死亡
		ChangeMap	= EAIF_ChangeMap,		// 离开当前场景或传送
		OffLine		= EAIF_OffLine,			// 角色下线或返回角色列表
		Dispel		= EAIF_Dispel,			// 驱散
		Replace		= EAIF_Replace,			// 被效果更高的buff顶掉
		Mounting	= EAIF_Mounting,		// 上马
		Damage2		= EAIF_Damage2,			// 受到一定量伤害
		Max			= EATF_Max,
	};

	// 持续时间衰减分组
	public enum class AuraDiminishingGroup : System::UInt32
	{
		Null	= EADG_Null,			// 不衰减
		Silence	= EADG_Silence,			// 沉默衰减
		Root	= EADG_Root,			// 定身衰减
		Fear	= EADG_Fear,			// 恐惧衰减
		Max		= EADG_Max,
	};

	// buff添加时先判断是否有id相同的buff, id相同走同源不同源叠加规则，id不相同走冲顶规则
	// 冲顶规则
	public enum class AuraReplaceType : System::UInt32
	{
		Null	= EART_Null,			// 不被其他类型buff冲顶

		Max		= EART_Max,
	};

	// buff叠加方式(同源, 不同源)
	public enum class AuraStackType : System::UInt32
	{
		Null		= EAST_Null,
		Refresh		= EAST_Refresh,			// 重置, (时间, 次数)
		AddCount	= EAST_AddCount,		// 添加剩余次数, 重置时间
		Add			= EAST_Add,				// 添加不影响已有buff
		CantAdd		= EAST_CantAdd,			// 不叠加, 不添加
		Max			= EAST_Max,
	};

	// 死亡buff不消失计时方式
	public enum class AuraDeadTimerType : System::UInt32
	{
		Null		= EADTT_Null,			// 死亡后消失填写该值
		Stop		= EADTT_Stop,			// 死亡后计时停止
		Normal		= EADTT_Normal,			// 死亡后正常停止
		Max			= EADTT_Max,
	};

	// 下线buff不消失计时方式
	public enum class AuraOfflineTimerType : System::UInt32
	{
		Null	= EAOTT_Null,			// 下线后消失填写该值
		Stop	= EAOTT_Stop,			// 下线后计时停止
		Normal	= EAOTT_Normal,			// 下线后正常停止
		Max		= EAOTT_Max,
	};

	// 天赋改变技能枚举
	public enum class SpellModType : System::UInt32
	{
		Null						= ESMT_Null,						// 无效, 不消耗
		SpellCastingTime			= ESMT_SpellCastingTime,			// 影响技能吟唱时间
		SpellRange					= ESMT_SpellRange,					// 影响技能最大射程
		SpellSpeed					= ESMT_SpellSpeed,					// 影响技能飞行速度
		SpellRadius					= ESMT_SpellRadius,					// 影响技能半径
		SpellAngle					= ESMT_SpellAngle,					// 影响技能半径角度
		SpellChainTarget			= ESMT_SpellChainTarget,			// 影响多重射击个数
		SpellDuration				= ESMT_SpellDuration,				// 影响持续时间
		SpellCost					= ESMT_SpellCost,					// 影响技能消耗值
		SpellCostScale				= ESMT_SpellCostScale,				// 影响技能连续消耗缩放
		SpellModEffect				= ESMT_SpellModEffect,				// 替换技能效果
		ModEffectTriggerSpell		= ESMT_ModEffectTriggerSpell,		// 替换触发技能
		ModEffectApplyAura			= ESMT_ModEffectApplyAura,			// 替换应用的buff
		ModTriggerSpellRate			= ESMT_ModTriggerSpellRate,			// 影响技能Trigger触发几率
		SpellEffectValue			= ESMT_SpellEffectValue,			// 影响技能effect数值
		
		AuraMaxDuration				= ESMT_AuraMaxDuration,				// 影响buff持续时间
		AuraTick					= ESMT_AuraTick,					// 影响buff tick方式
		AuraPeriodicTime			= ESMT_AuraPeriodicTime,			// 影响buff tick周期
		AuraResistDispelChance		= ESMT_AuraResistDispelChance,		// 改变Buff驱散等级
		AuraStack					= ESMT_AuraStack,					// 影响buff叠加次数
		AuraEffectValue				= ESMT_AuraEffectValue,				// 影响buff effect数值
		
		Max							= ESMT_Max,
	};

	public enum class SpellModSubType : System::UInt32
	{
		Null	= ESMST_Null,
		Value	= ESMST_Value,			// 按数值修改
		Pct		= ESMST_Pct,			// 按百分比修改
		Max		= ESMST_Max,
	};	

	// buff触发的事件类型(被动相对buff持有者, 不相对角色)
	public enum class AuraTriggerEventType : System::UInt32
	{
		Null			= EATET_Null,
		OnHited			= EATET_OnHited,			// buff持有者被击中
		OnDead			= EATET_OnDead,				// buff持有者死亡
		OnCasting		= EATET_OnCasting,			// buff持有者释放技能
		Ondispeled		= EATET_Ondispeled,			// buff被驱散时
		Max				= EATET_Max,
	};

	// buff触发技能给谁
	public enum class AuraTriggerTargetType : System::UInt32
	{
		Null		= EATTT_Null,
		Self		= EATTT_Self,			// buff持有者
		Caster		= EATTT_Caster,			// buff释放者
		Party		= EATTT_Party,			// buff持有者的小队成员
		CasterParty	= EATTT_CasterParty,	// buff释放者的小队成员
		Attacker	= EATTT_Attacker,		// 攻击者
		
		Max			= EATTT_Max,
	};

	/// 
	public enum class SpellCategoryCDType : System::UInt32
	{
		Null		= ESCCDT_Null,
		GlobalCD	= ESCCDT_GlobalCD,		// 全局公共cd
		Max			= ESCCDT_Max,
	};
}