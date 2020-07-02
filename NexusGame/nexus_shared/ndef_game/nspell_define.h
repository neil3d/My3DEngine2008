/**
 *	nexus ngame_define - nspell_define
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Jun, 2010
 */

#ifndef _NDEF_GAME_NSPELL_DEFINE_H_
#define _NDEF_GAME_NSPELL_DEFINE_H_

#include "ntype_def.h"
#include "object/nname.h"
#include "ndef_base.h"

namespace nexus {

#pragma pack(push,1)

	const uint32 MAX_EFFECT_NUM						= 3;			// 技能附加效果的最大数目
	const uint32 MAX_AURA_STAT_NUM					= 4;			// buff影响的人物属性最大数目
	const uint32 MAX_SPELL_TALENT_RANK				= 10;			// 技能天赋最高等级

	// 技能主类型
	enum ESpellType
	{
		ESpell_Null									= 0,
		ESpell_Class								= 1,			// 职业技能, 子类型ESpellClassSubType
		ESpell_Profession							= 2,			// 专业技能, 子类型ESpellProfessionSubType
		ESpell_Talent								= 3,			// 天赋技能
		ESpell_Item									= 4,			// 物品技能
		ESpell_Pet									= 5,			// 宠物技能
		ESpell_Creature								= 6,			// npc技能
		ESpell_Misc									= 7,			// 未分类技能
		ESpell_Max,
	};

	// 职业技能
	enum ESpellClassSubType
	{
		ESCST_Null									= 0,
		ESCST_Melee									= 1,			// 普通技能
		ESCST_KungFu								= 2,			// 武艺技能
		ESCST_Spell									= 3,			// 术法技能
		ESCST_Max,
	};

	// 生活技能, 技能子类型(专业技能)
	enum ESpellProfessionSubType
	{
		ESPST_Null									= 0,
		ESPST_Mining								= 1,			// 采矿
		ESPST_Max,
	};

	// 技能对武器限制类型
	enum ESpellWeaponType
	{
		ESWT_Null									= 0,			// 无限制
		ESWT_MainHand								= 1,			// 主手必需满足物品类型限制
		ESWT_OffHand								= 2,			// 副手必需满足物品类型限制
		ESWT_MainAndOff								= 3,			// 主副手都满足物品类型限制
		ESWT_MainOrOff								= 4,			// 主副手其中一个满足物品类型限制
		ESWT_Max,
	};

	// 释放目标类型
	enum ECastTargetType
	{
		ECTT_Null									= 0,			// 无目标

		ECTT_Self									= 1,			// 自己
		ECTT_SingleEnemy							= 2,			// 单个活的非友善单位
		ECTT_SingleFriend							= 3,			// 单个活的友善单位
		ECTT_EnemyCorpse							= 4,			// 单个非友善尸体
		ECTT_FriendCorpse							= 5,			// 单个友善尸体
		ECTT_Point									= 6,			// 坐标点

		ECTT_Max,
	};

	// 作用目标类型
	enum EEffectTargetType
	{
		EETT_Target									= 0,			// 选定的单个对象
		EETT_AllEnemyInArea1						= 1,			// 选定坐标点半径为radius的圆形区域内非友善单位
		EETT_AllEnemyInArea2						= 2,			// 选定坐标点和自己位置表示的朝向radius与angle所表示的弧形区域内非友善单位
		EETT_AllEnemyInArea3						= 3,			// 选定坐标点和自己位置表示的朝向weight与length所表示的自己出发的矩形区域内非友善单位
		EETT_AllEnemyInArea4						= 4,			// 选定坐标点为中心且和自己位置表示的朝向weight与length所表示的自己出发的矩形区域内非友善单位

		EETT_AllEnemyAroundCaster					= 5,			// 释放者周围圆形区域非友善单位
		EETT_AllFriendAroundCaster					= 6,			// 释放者周围圆形区域友善单位

		EETT_Max,
	};

	// 释放表现方式
	enum ESpellCastType
	{
		ESCast_Null									= 0,
		ESCast_Instant								= 1,			// 瞬发(can_repeat标示是否持续)
		ESCast_Cast									= 2,			// 吟唱
		ESCast_Channel								= 3,			// 引导
		ESCast_Max,
	};

	// 技能打断方式
	enum ESpellInterruptFlags
	{
		ESIF_Null									= 0x00000000,	// 死亡肯定打断
		ESIF_Damage									= 0x00000001,   // 伤害打断
		ESIF_Move									= 0x00000002,   // 移动打断
		ESIF_Turning								= 0x00000004,   // 旋转打断
		ESIF_Interrupt								= 0x00000008,   // 技能打断
		ESIF_Damage2								= 0x00000010,   // 伤害损失时间
		ESIF_SpellPrepare							= 0x00000020,   // 技能吟唱打断
		ESIF_SpellCast								= 0x00000040,   // 技能释放打断
	};

	// 技能消耗
	enum ESpellCostType
	{
		ESCT_Null									= 0,			// 无效, 不消耗
		ESCT_EnergyValue							= 1,			// 按值消耗内力
		ESCT_ManaValue								= 2,			// 按值消耗神识
		ESCT_EnergyPct								= 3,			// 按最大值的百分比消耗内力
		ESCT_ManaPct								= 4,			// 按最大值的百分比消耗神识
		ESCT_ManaPerLevel							= 5,			// 每一级消耗的神识值
		ESCT_EnergyPerLevel							= 6,			// 每一级消耗的内力值
		ESCT_ManaAndItem							= 7,			// 神识值与物品
		ESCT_EnergyAndItem							= 8,			// 内力值与物品
		ESCT_Item									= 9,			// 物品
		ESCT_Max,
	};

	// 技能效果生效事件(主动)
	enum ESpellEffectTriggerType
	{
		ESETT_Null									= 0,
		ESETT_SpellPrepare							= 1,			// 准备
		ESETT_SpellCasting							= 2,			// 释放
		ESETT_SpellInterrupt						= 3,			// 被动打断
		ESETT_HitTarget								= 4,			// 命中目标
		ESETT_Crit									= 5,			// 暴击
		ESETT_EffectDead							= 6,			// 效果致死
		ESETT_Max,
	};

	// 技能效果技能给谁
	enum ESpellTriggerTargetType
	{
		ESTTT_Null									= 0,
		ESTTT_Targets								= 1,			// 作用目标集合所有成员
		ESTTT_Caster								= 2,			// 技能释放者
		ESTTT_CasterParty							= 3,			// 技能释放者的小队成员

		ESTTT_Max,
	};

	// 技能效果枚举
	enum ESpellEffectType
	{
		ESET_Null									= 0,			// 空效果

		ESET_TriggerSpell							= 1,			// 触发技能
		ESET_ApplyAura								= 2,			// 应用buff扩展技能效果
		ESET_Instantkill							= 3,			// 秒杀
		ESET_MeleeDamage							= 4,			// 普通伤害
		ESET_KungFuDamage							= 5,			// 武艺伤害
		ESET_SpellDamage							= 6,			// 术法伤害
		ESET_Heal									= 7,			// 治疗
		ESET_ModPower								= 8,			// 修改能量值, (内力, 神识)
		ESET_HealPct								= 9,			// 按百分比直接治疗
		ESET_ModPowerPct							= 10,			// 按百分比修改能量值, (内力, 神识)
		ESET_Dispel									= 11,			// 驱散
		ESET_Resurrect								= 12,			// 复活
		ESET_KnockDown								= 13,			// 击倒
		ESET_KnockBack								= 14,			// 击退击飞

		ESET_Max,
	};

	// 伤害类型
	enum ESpellDMGType
	{
		ESDT_Null									= 0,
		ESDT_Physics								= 1,			// 物理(力量, 武艺)减免伤害防御, 韧性
		ESDT_Metal									= 2,			// 金, 减免伤害金灵力
		ESDT_Wood									= 3,			// 木, 减免伤害木灵力
		ESDT_Water									= 4,			// 水, 减免伤害水灵力
		ESDT_Fire									= 5,			// 火, 减免伤害火灵力
		ESDT_Soil									= 6,			// 土, 减免伤害土灵力
		ESDT_Max,
	};

	// buff效果枚举
	enum EAuraEffectType
	{
		EAET_Null									= 0,			// 空效果

		EAET_ModStatBaseValue						= 1,			// base_value方式修改属性值，stat_index, stat_value
		EAET_ModStatBasePct							= 2,			// base_pct方式修改属性值，stat_index, stat_value
		EAET_ModStatTotalValue						= 3,			// total_value方式修改属性值，stat_index, stat_value
		EAET_ModStatTotalPct						= 4,			// total_pct方式修改属性值，stat_index, stat_value

		EAET_PeriodicDamage							= 5,			// 周期伤害
		EAET_PeriodicHeal							= 6,			// 周期治疗
		EAET_Fear									= 7,			// 恐惧
		EAET_Silence								= 8,			// 沉默
		EAET_IncreaseSpeed							= 9,			// 加速
		EAET_DecreaseSpeed							= 10,			// 减速
		EAET_Root									= 11,			// 定身

		EAET_SpellMod								= 12,			// 天赋修改技能
		EAET_TriggerSpell							= 13,			// 触发技能
		EAET_TriggerAura							= 14,			// 触发buff

		EAET_Max,
	};

	// buff打断方式(时间结束)
	enum EAuraInterruptFlags
	{
		EAIF_Null									= 0x00000000,	// 不打断
		EAIF_Move									= 0x00000001,	// 移动
		EAIF_EnterCombat							= 0x00000002,	// 进入战斗状态
		EAIF_Cancel									= 0x00000004,	// 手动取消
		EAIF_Damage									= 0x00000008,	// 受到任何伤害
		EAIF_Dead									= 0x00000010,	// 死亡
		EAIF_ChangeMap								= 0x00000020,	// 离开当前场景或传送
		EAIF_OffLine								= 0x00000040,	// 角色下线或返回角色列表
		EAIF_Dispel									= 0x00000080,	// 驱散
		EAIF_Replace								= 0x00000100,	// 被效果更高的buff顶掉
		EAIF_Mounting								= 0x00000200,	// 上马
		EAIF_Damage2								= 0x00000400,	// 受到一定量伤害
		EATF_Max,
	};

	// 持续时间衰减分组
	enum EAuraDiminishingGroup
	{
		EADG_Null									= 0,			// 不衰减
		EADG_Silence								= 1,			// 沉默衰减
		EADG_Root									= 2,			// 定身衰减
		EADG_Fear									= 3,			// 恐惧衰减
		EADG_Max,
	};

	// buff添加时先判断是否有id相同的buff, id相同走同源不同源叠加规则，id不相同走冲顶规则
	// 冲顶规则
	enum EAuraReplaceType
	{
		EART_Null									= 0,			// 不被其他类型buff冲顶

		EART_Max,
	};

	// buff叠加方式(同源, 不同源)
	enum EAuraStackType
	{
		EAST_Null									= 0,
		EAST_Refresh								= 1,			// 重置, (时间, 次数)
		EAST_AddCount								= 2,			// 添加剩余次数, 重置时间
		EAST_Add									= 3,			// 添加不影响已有buff
		EAST_CantAdd								= 4,			// 不叠加, 不添加
		EAST_Max,
	};

	// 死亡buff不消失计时方式
	enum EAuraDeadTimerType
	{
		EADTT_Null									= 0,			// 死亡后消失填写该值
		EADTT_Stop									= 1,			// 死亡后计时停止
		EADTT_Normal								= 2,			// 死亡后正常停止
		EADTT_Max,
	};

	// 下线buff不消失计时方式
	enum EAuraOfflineTimerType
	{
		EAOTT_Null									= 0,			// 下线后消失填写该值
		EAOTT_Stop									= 1,			// 下线后计时停止
		EAOTT_Normal								= 2,			// 下线后正常停止
		EAOTT_Max,
	};

	// 天赋改变技能枚举
	enum ESpellModType
	{
		ESMT_Null									= 0,			// 无效, 不消耗
		ESMT_SpellCastingTime						= 1,			// 影响技能吟唱时间
		ESMT_SpellRange								= 2,			// 影响技能最大射程
		ESMT_SpellSpeed								= 3,			// 影响技能飞行速度
		ESMT_SpellRadius							= 4,			// 影响技能半径
		ESMT_SpellAngle								= 5,			// 影响技能半径角度
		ESMT_SpellChainTarget						= 6,			// 影响多重射击个数
		ESMT_SpellDuration							= 7,			// 影响持续时间
		ESMT_SpellCost								= 8,			// 影响技能消耗值
		ESMT_SpellCostScale							= 9,			// 影响技能连续消耗缩放
		ESMT_SpellModEffect							= 10,			// 替换技能效果
		ESMT_ModEffectTriggerSpell					= 11,			// 替换触发技能
		ESMT_ModEffectApplyAura						= 12,			// 替换应用的buff
		ESMT_ModTriggerSpellRate					= 13,			// 影响技能Trigger触发几率
		ESMT_SpellEffectValue						= 14,			// 影响技能effect数值

		ESMT_AuraMaxDuration						= 15,			// 影响buff持续时间
		ESMT_AuraTick								= 16,			// 影响buff tick方式
		ESMT_AuraPeriodicTime						= 17,			// 影响buff tick周期
		ESMT_AuraResistDispelChance					= 18,			// 改变Buff驱散等级
		ESMT_AuraStack								= 19,			// 影响buff叠加次数
		ESMT_AuraEffectValue						= 20,			// 影响buff effect数值

		ESMT_Max,
	};

	enum ESpellModSubType
	{
		ESMST_Null									= 0,
		ESMST_Value									= 1,			// 按数值修改
		ESMST_Pct									= 2,			// 按百分比修改
		ESMST_Replace								= 3,			// 替换
		ESMST_Max,
	};

	// buff触发的事件类型(被动相对buff持有者, 不相对角色)
	enum EAuraTriggerEventType
	{
		EATET_Null									= 0,
		EATET_OnHited								= 1,			// buff持有者被击中
		EATET_OnDead								= 2,			// buff持有者死亡
		EATET_OnCasting								= 3,			// buff持有者释放技能
		EATET_Ondispeled							= 4,			// buff被驱散时
		EATET_Max,
	};

	// buff触发技能给谁
	enum EAuraTriggerTargetType
	{
		EATTT_Null									= 0,
		EATTT_Self									= 1,			// buff持有者
		EATTT_Caster								= 2,			// buff释放者
		EATTT_Party									= 3,			// buff持有者的小队成员
		EATTT_CasterParty							= 4,			// buff释放者的小队成员
		EATTT_Attacker								= 5,			// 攻击者

		EATTT_Max,
	};

	// 技能树模板表, 技能学习
	struct nspell_talent_table : public att_info_base
	{
		uint32					class_;								// 职业
		uint32					level;								// 人物等级
		uint32					point;								// 已加技能点
		uint32					money;								// 消耗游戏币
		uint32					reputation_faction;					// 所需声望
		uint32					reputation_rank;					// 所需声望等级 
		nname					depends_on;							// 所需技能树节点id nspell_talent_table
		uint32					depends_on_rank;					// 所需加点等级
		nname					spell_id[MAX_SPELL_TALENT_RANK];	// 各等级对应的技能id
		//nname					icon_effect[MAX_SPELL_TALENT_RANK + 1];	// 0级和各等级对应的图标
	
		nspell_talent_table()
		{
		}
		explicit nspell_talent_table(const nstring& id_str) 
			: att_info_base(id_str)
		{
		}

	protected:
		virtual void custom_serialize(narchive& ar)
		{
			nSERIALIZE(ar, class_);
			nSERIALIZE(ar, level);
			nSERIALIZE(ar, point);
			nSERIALIZE(ar, money);
			nSERIALIZE(ar, reputation_faction);
			nSERIALIZE(ar, reputation_rank);
			nSERIALIZE(ar, depends_on);
			nSERIALIZE(ar, depends_on_rank);
			nSERIALIZE(ar, spell_id[0]);
			nSERIALIZE(ar, spell_id[1]);
			nSERIALIZE(ar, spell_id[2]);
			nSERIALIZE(ar, spell_id[3]);
			nSERIALIZE(ar, spell_id[4]);
			nSERIALIZE(ar, spell_id[5]);
			nSERIALIZE(ar, spell_id[6]);
			nSERIALIZE(ar, spell_id[7]);
			nSERIALIZE(ar, spell_id[8]);
			nSERIALIZE(ar, spell_id[9]);
		}
	};

	// 技能模板表
	struct nspell_info : public att_info_base
	{
		uint32					type;								// 类型, 见ESpellType
		uint32					sub_type;							// 子类型, 根据type子类型取不同枚举值
		uint32					level;								// 等级

		bool					active;								// 是否主动技能, (true = 主动技能, false = 被动技能)
		bool					can_cast_in_combat;					// 是否可以在战斗状态释放, (true = 可以, false = 不可以)
		bool					auto_path_find;						// 是否可寻路到达释放位置, (true = 可寻路, false = 不可寻路)
		bool					auto_orient_target;					// 是否需要自动面向目标, (true = 是, false = 否)
		bool					no_line_of_sight;					// 是否跳过视线检测, (true = 跳过视线检测, false = 不跳过视线检测)
		bool					can_cast_while_feared;				// 恐惧时是否可以释放, (true = 可以, false = 不可以)
		bool					can_cast_while_silence;				// 沉默时是否可以释放, (true = 可以, false = 不可以)
		bool					can_cast_while_root;				// 定身时是否可以释放, (true = 可以, false = 不可以)
		bool					can_cast_while_move;				// 移动或转身时是否可以释放, (true = 可以, false = 不可以)

		uint32					weapon_type;						// 武器限制类型, 见ESpellWeaponType
		uint32					main_hand;							// 主手武器类型限制, 对应武器类物品子类型
		uint32					off_hand;							// 副手武器类型限制, 对应武器类物品子类型

		uint32					cast_target_type;					// 释放目标类型, 见ECastTargetType
		float					min_range;							// 选择目标的最小距离
		float					max_range;							// 选择目标的最大距离

		uint32					effect_target_type;					// 作用目标类型，见EEffectTargetType
		float					radius;								// 区域半径
		float					angle;								// 区域扇形角度
		float					weight;								// 区域宽度
		float					length;								// 区域长度

		uint32					cast_type;							// 释放方式, 见ESpellCastType
		uint32					interrupt_flags;					// 打断方式 (持续, 吟唱, 引导)见ESpellInterruptFlags
		bool					can_repeat;							// 是否可以持续释放,(true == 可持续, false == 不可以持续)
		bool					can_move;							// 是否可以移动, (true == 可以, false == 不可以移动)
		bool					can_turn;							// 是否可以转向, 如果是瞬发表示持续过程中是否可以转向,  (true == 可以, false == 不可以转向)
		float					min_repeat_time;					// 最短持续时间, 单位s
		float					max_repeat_time;					// 最长持续时间, 单位s
		float					speed_scale_coef;					// 连续释放时速度缩放系数speed * (1 + speed_scale_coef * count)
		float					effect_scale_coef;					// 连续释放时效果值缩放系数effect * (1 + effect_scale_coef * count)
		float					casting_time;						// 吟唱时间, 单位s
		uint32					max_tick_count;						// 最大引导次数
		float					periodic_time;						// 引导周期时间, 单位s
		float					speed;								// 飞行的速度

		bool					global_cd_not_effect;				// 是否不受全局cd影响, (true == 不受公共cd影响, false == 受公共cd影响)
		float					global_cd_time;						// 触发的全局cd时间, 单位s
		float					cd_time;							// 触发的技能cd时间, 单位s

		uint32					cost_type;							// 技能消耗方式, 见ESpellCostType
		float					cost_value;							// 技能消耗属性值
		float					cost_scale_coef;					// 连续释放时消耗缩放系数, 或引导(-1.0f) cost_value * (1 + cost_scale_coef * count)
		nname					reagent_id;							// 技能所需的药剂, 引用技能消耗产出物品表

		nname					spell_effect_id[MAX_EFFECT_NUM];	// 技能附加效果集合

		// 客户端专用信息
		nstring					spell_icon;							// 技能图标
		nstring					client_effect;						// 技能动作特效表现
		//nname					inactive_icon;						// 无效状态图标
		//nname					prepare_action;						// 准备动作
		//nname					casting_action;						// 释放动作
		//nname					casting_effect;						// 释放特效
		//nname					fly_effect;							// 飞行特效
		//nname					hit_effect;							// 击中特效
	
		nspell_info()
		{
		}
		explicit nspell_info(const nstring& id_str) 
			: att_info_base(id_str)
		{
		}

	protected:
		virtual void custom_serialize(narchive& ar)
		{
			nserialize(ar,type,						_T("type"));
			nserialize(ar,sub_type,					_T("sub_type"));
			nserialize(ar,level,					_T("level"));

			nserialize(ar,active,					_T("active"));
			nserialize(ar,can_cast_in_combat,		_T("can_cast_in_combat"));
			nserialize(ar,auto_path_find,			_T("auto_path_find"));
			nserialize(ar,auto_orient_target,		_T("auto_orient_target"));
			nserialize(ar,no_line_of_sight,			_T("no_line_of_sight"));
			nserialize(ar,can_cast_while_feared,	_T("can_cast_while_feared"));
			nserialize(ar,can_cast_while_silence,	_T("can_cast_while_silence"));
			nserialize(ar,can_cast_while_root,		_T("can_cast_while_root"));
			nserialize(ar,can_cast_while_move,		_T("can_cast_while_move"));

			nserialize(ar,weapon_type,				_T("weapon_type"));
			nserialize(ar,main_hand,				_T("main_hand"));
			nserialize(ar,off_hand,					_T("off_hand"));

			nserialize(ar,cast_target_type,			_T("cast_target_type"));
			nserialize(ar,min_range,				_T("min_range"));
			nserialize(ar,max_range,				_T("max_range"));

			nserialize(ar,effect_target_type,		_T("effect_target_type"));
			nserialize(ar,radius,					_T("radius"));
			nserialize(ar,angle,					_T("angle"));
			nserialize(ar,weight,					_T("weight"));
			nserialize(ar,length,					_T("length"));

			nserialize(ar,cast_type,				_T("cast_type"));
			nserialize(ar,interrupt_flags,			_T("interrupt_flags"));
			nserialize(ar,can_repeat,				_T("can_repeat"));
			nserialize(ar,can_move,					_T("can_move"));
			nserialize(ar,can_turn,					_T("can_turn"));
			nserialize(ar,min_repeat_time,			_T("min_repeat_time"));
			nserialize(ar,max_repeat_time,			_T("max_repeat_time"));
			nserialize(ar,speed_scale_coef,			_T("speed_scale_coef"));
			nserialize(ar,effect_scale_coef,		_T("effect_scale_coef"));
			nserialize(ar,casting_time,				_T("casting_time"));
			nserialize(ar,max_tick_count,			_T("max_tick_count"));
			nserialize(ar,periodic_time,			_T("periodic_time"));
			nserialize(ar,speed,					_T("speed"));

			nserialize(ar,global_cd_not_effect,		_T("global_cd_not_effect"));
			nserialize(ar,global_cd_time,			_T("global_cd_time"));
			nserialize(ar,cd_time,					_T("cd_time"));

			nserialize(ar,cost_type,				_T("cost_type"));
			nserialize(ar,cost_value,				_T("cost_value"));
			nserialize(ar,cost_scale_coef,			_T("cost_scale_coef"));
			nserialize(ar,reagent_id,				_T("reagent_id"));

			nserialize(ar,spell_effect_id[0],		_T("spell_effect_id[0]"));
			nserialize(ar,spell_effect_id[1],		_T("spell_effect_id[1]"));
			nserialize(ar,spell_effect_id[2],		_T("spell_effect_id[2]"));

			nserialize(ar,spell_icon,				_T("spell_icon"));
			nserialize(ar,client_effect,			_T("client_effect"));
		}
	};

	// 技能效果模板表
	struct nspell_effect_info : public att_info_base
	{
		uint32					effect_trigger_type;				// 生效方式, 见ESpellEffectTriggerType
		uint32					trigger_target_type;				// 给谁, 见ESpellTriggerTargetType
		float					rate;								// 生效几率

		uint32					effect_type;						// 技能效果类型, 见ESpellEffectType

		uint32					damage_type;						// 伤害类型, 见ESpellDMGType
		float					value1;								// 数值1
		float					value2;								// 数值2

		nname					dst_teleport_id;					// 传送目的地id，索引其他表
		nname					summon_id;							// 召唤物id，索引其他表

		int32					threat;								// 威胁值, 仇恨值

		nname					trigger_spell_id;					// 触发技能id
		nname					apply_aura_id;						// 应用aura扩展技能效果

		nspell_effect_info()
		{
		}
		explicit nspell_effect_info(const nstring& id_str) 
			: att_info_base(id_str)
		{
		}

	protected:
		virtual void custom_serialize(narchive& ar)
		{
			nserialize(ar, effect_trigger_type,		_T("effect_trigger_type"));
			nserialize(ar, trigger_target_type,		_T("trigger_target_type"));
			nserialize(ar, rate,					_T("rate"));

			nserialize(ar, effect_type,				_T("effect_type"));

			nserialize(ar, damage_type,				_T("damage_type"));
			nserialize(ar, value1,					_T("value1"));
			nserialize(ar, value2,					_T("value2"));

			nserialize(ar, dst_teleport_id,			_T("dst_teleport_id"));
			nserialize(ar, summon_id,				_T("summon_id"));

			nserialize(ar, threat,					_T("threat"));

			nserialize(ar, trigger_spell_id,		_T("trigger_spell_id"));
			nserialize(ar, apply_aura_id,			_T("apply_aura_id"));
		}
	};

	// buff效果模板表
	struct nspell_aura_info : public att_info_base
	{
		uint32					effect_type;						// 效果类型, 见EAuraEffectType
		bool					positive;							// 是否增益, (true == 增益, false == 减益)
		bool					to_client;							// 是否同步给客户端, (true == 同步, false == 不同步)
		uint32					interrupt_flags;					// 打断(aura remove)标志, 见EAuraInterruptFlags

		uint32					diminishing_group;					// 持续时间衰减分组, 见EAuraDiminishingGroup
		float					max_duration;						// 最大持续时间, 单位s
		float					periodic_time;						// 周期时间, 单位s

		uint32					dispel_level;						// 驱散等级

		uint32					replace_type;						// 替换类型, 相同类型的不同等级buff相互替换, 见EAuraReplaceType
		uint32					level;								// buff等级, 控制同替换类型buff冲顶
		uint32					ssrc_stack_type;					// 同源叠加方式, 见EAuraStackType
		uint32					dsrc_stack_type;					// 不源叠加方式, 见EAuraStackType
		uint32					max_stack_count;					// 叠加数量

		uint32					death;								// 死亡不消失计时方式, 见EAuraDeadTimerType
		uint32					offline;							// 下线不消失计时方式, 见EAuraOfflineTimerType

		uint32					stat_index[MAX_AURA_STAT_NUM];		// 影响属性, 见EUnitFields(EUF_UnitNull -- EUF_UnitSeparator)
		float					stat_value[MAX_AURA_STAT_NUM];		// 影响属性值

		uint32					damage_type;						// 伤害类型, 见ESpellDMGType
		float					value1;								// 数值1
		float					value2;								// 数值2

		int32					threat;								// 威胁值, 仇恨值

		uint32					spell_mod_type;						// 天赋效果类型, 见ESpellModType
		uint32					spell_mod_sub_type;					// 天赋修改方式, 见ESpellModSubType
		nname					spell_mod_spell_id;					// 天赋对哪个技能有影响
		float					spell_mod_value1;					// 天赋作用值
		nname					spell_mod_value2;					// 天赋替换id值

		uint32					trigger_event_type;					// 触发方式, 见EAuraTriggerEventType
		nname					cast_spell_id;						// 使用技能id
		float					rate;								// 触发几率
		uint32					target_type;						// 触发给谁, 见EAuraTriggerTargetType
		uint32					max_chain_target;					// 目标的最大数目
		nname					trigger_spell_id;					// 触发技能id
		nname					trigger_aura_id;					// 触发buff id

		// 客户端专用信息
		nstring					aura_icon;							// buff图标, 无效值不显示图标
		nstring					add_effect;							// buff添加特效
		nstring					duration_effect;					// buff持续特效
		nstring					remove_effect;						// buff消失特效
	
		nspell_aura_info()
		{
		}
		explicit nspell_aura_info(const nstring& id_str) 
			: att_info_base(id_str)
		{
		}

	protected:
		virtual void custom_serialize(narchive& ar)
		{
			nserialize(ar, effect_type,				_T("effect_type"));
			nserialize(ar, positive,				_T("positive"));
			nserialize(ar, to_client,				_T("to_client"));
			nserialize(ar, interrupt_flags,			_T("interrupt_flags"));

			nserialize(ar, diminishing_group,		_T("diminishing_group"));
			nserialize(ar, max_duration,			_T("max_duration"));
			nserialize(ar, periodic_time,			_T("periodic_time"));

			nserialize(ar, dispel_level,			_T("dispel_level"));

			nserialize(ar, replace_type,			_T("replace_type"));
			nserialize(ar, level,					_T("level"));
			nserialize(ar, ssrc_stack_type,			_T("ssrc_stack_type"));
			nserialize(ar, dsrc_stack_type,			_T("dsrc_stack_type"));
			nserialize(ar, max_stack_count,			_T("max_stack_count"));

			nserialize(ar, death,					_T("death"));
			nserialize(ar, offline,					_T("offline"));

			nserialize(ar, stat_index[0],			_T("stat_index[0]"));
			nserialize(ar, stat_index[1],			_T("stat_index[1]"));
			nserialize(ar, stat_index[2],			_T("stat_index[2]"));
			nserialize(ar, stat_index[3],			_T("stat_index[3]"));
			nserialize(ar, stat_value[0],			_T("stat_value[0]"));
			nserialize(ar, stat_value[1],			_T("stat_value[1]"));
			nserialize(ar, stat_value[2],			_T("stat_value[2]"));
			nserialize(ar, stat_value[3],			_T("stat_value[3]"));

			nserialize(ar, damage_type,				_T("damage_type"));
			nserialize(ar, value1,					_T("value1"));
			nserialize(ar, value2,					_T("value2"));

			nserialize(ar, threat,					_T("threat"));

			nserialize(ar, spell_mod_type,			_T("spell_mod_type"));
			nserialize(ar, spell_mod_sub_type,		_T("spell_mod_sub_type"));
			nserialize(ar, spell_mod_spell_id,		_T("spell_mod_spell_id"));
			nserialize(ar, spell_mod_value1,		_T("spell_mod_value1"));
			nserialize(ar, spell_mod_value2,		_T("spell_mod_value2"));

			nserialize(ar, trigger_event_type,		_T("trigger_event_type"));
			nserialize(ar, cast_spell_id,			_T("cast_spell_id"));
			nserialize(ar, rate,					_T("rate"));
			nserialize(ar, target_type,				_T("target_type"));
			nserialize(ar, max_chain_target,		_T("max_chain_target"));
			nserialize(ar, trigger_spell_id,		_T("trigger_spell_id"));
			nserialize(ar, trigger_aura_id,			_T("trigger_aura_id"));

			nserialize(ar, aura_icon,				_T("aura_icon"));
			nserialize(ar, add_effect,				_T("add_effect"));
			nserialize(ar, duration_effect,			_T("duration_effect"));
			nserialize(ar, remove_effect,			_T("remove_effect"));
		}
	};


	//-- 定义序列化模板函数
	SERIALIZE_ATT_INFO(nspell_talent_table);
	SERIALIZE_ATT_INFO(nspell_info);
	SERIALIZE_ATT_INFO(nspell_effect_info);
	SERIALIZE_ATT_INFO(nspell_aura_info);


#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NSPELL_DEFINE_H_
