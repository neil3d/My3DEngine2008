/**
 *	nexus ndef_game - nmsg_spell.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Jun, 2010
 */

#ifndef _NDEF_GAME_NMSG_SPELL_H_
#define _NDEF_GAME_NMSG_SPELL_H_

#include "nprotocol_base.h"
#include "nmsg_base_id.h"
#include "nmsg_base_error_code.h"
#include "nspell_data.h"
#include "base_define.h"
#include "math/vector.h"

namespace nexus {

#pragma pack(push,1)

	/**
	 * 技能buff相关消息 id区间[Msg_Client_Spell, Msg_Client_Spell + 50]
	 */
	enum
	{
		S2C_SpellError			= Msg_Client_Spell,
		C2S_CastSpell,
		S2C_CastSpellResult,
		C2S_CancelCast,
		S2C_SpellPrepare,
		S2C_SpellCasting,
		S2C_SpellFailed,
		S2C_SpellHit,

		C2S_CancelAura,
		S2C_AddAura,
		S2C_RemoveAura,
		S2C_UpdateAura,


	};

	// 技能释放结果枚举值
	enum ESpellCastResult
	{
		ESCR_Success						= 0,
		ESCR_Failed_NoSpell					= 1,		// 请求释放的技能没有学会
		ESCR_Failed_InvalidSpell			= 2,		// 请求释放的技能非法(被动技能, 技能id索引无效)
		ESCR_Failed_SpellInProgress			= 3,		// 释放者正在执行另一动作
		ESCR_Failed_CoolDown				= 4,		// 技能冷却未结束
		ESCR_Failed_TooClose				= 5,		// 指定目标太远了
		ESCR_Failed_OutofRange				= 6,		// 指定目标太近了
		ESCR_Failed_CasterDead				= 7,		// 释放者已经死亡
		ESCR_Failed_Interrupted				= 8,		// 技能被打断(被动)
		ESCR_Failed_NoMana					= 9,		// 魔法值不够
		ESCR_Failed_NoEnergy				= 10,		// 能量值不够
		ESCR_Failed_LineofSight				= 11,		// 视线阻挡
		ESCR_Failed_InvalidTargets			= 12,		// 指定目标非法
		ESCR_Failed_InCombat				= 13,		// 处于战斗状态
		ESCR_Failed_InFear					= 14,		// 处于恐惧状态
		ESCR_Failed_InSilence				= 15,		// 处于沉默状态
		ESCR_Failed_InRoot					= 16,		// 处于定身状态
		ESCR_Failed_CantRepeat				= 17,		// 该技能不能连续释放
		ESCR_Failed_Cancel					= 18,		// 技能被打断(主动取消)
		ESCR_Failed_Move					= 19,		// 移动中不能释放

	};

	// 技能命中结果枚举值
	enum ESpellHitResult
	{
		ESHR_Hit							= 0x00,		// 命中
		ESHR_Crit							= 0x01,		// 暴击
		ESHR_Miss							= 0x02,		// 未命中
		ESHR_Immune							= 0x04,		// 免疫
		ESHR_Absorb							= 0x08,		// 吸收
		ESHR_Reflect						= 0x10,		// 反弹
		ESHR_Evade							= 0x20,		// 躲避
	};

	enum ESpellHitType
	{
		ESHT_Spell							= 0,		// 技能
		ESHT_Aura							= 1,		// buff
	};

	// 请求释放技能(size = 31)
	CMD_START(C2S_CastSpell)
		uint32			spell_id;			// 请求释放的技能id
		uint64			target_id;			// 释放目标id, 没有为0
		vector3			target_pos;			// 释放目标坐标, 没有不设置
		float			time_stamp;			// 时间戳
		uint8			cast_count;			// 0 连续释放, > 0 实际次数
	CMD_END

	// 释放技能结果(只发送给请求者)(size = 7)
	CMD_START(S2C_CastSpellResult)
		uint32			spell_id;			// 释放哪个技能
		uint8			result;				// 结果
	CMD_END

	// 请求取消释放技能(size = 2)
	CMD_START(C2S_CancelCast)
	CMD_END

	// 技能准备(吟唱, 无吟唱不会受到该消息)开始或更新(size = 26)
	CMD_START(S2C_SpellPrepare)
		uint64			caster_id;			// 释放者id
		uint32			spell_id;			// 释放的技能
		float			time_stamp;			// 时间戳
		uint32			max_time;			// 准备最大时间, 单位1/1000s
		uint32			cur_time;			// 准备剩余时间, 单位1/1000s

		// 目标没有想到有技能吟唱需要目标, 用到的话在加
		//uint64			target_id;		// 释放目标id
		//vector3			target_pos;		// 释放目标坐标
	CMD_END

	// 技能释放(瞬发, 引导或引导更新)(size = 47)
	CMD_START(S2C_SpellCasting)
		uint64			caster_id;			// 释放者id
		uint32			spell_id;			// 释放的技能
		float			time_stamp;			// 时间戳
		uint32			max_time;			// 瞬发时为0, != 0 为通道最大时间, 单位1/1000s
		uint32			cur_time;			// 瞬发时为0, != 0 为通道剩余时间, 单位1/1000s
		uint64			target_id;			// 释放目标id, 如果有对多个目标的技能, 需添加新的消息, 如闪电链, 对多个目标引导
		vector3			target_pos;			// 释放目标坐标, 根据技能模板信息, 判断是使用坐标还是target_id
		uint8			cast_index;			// 连续释放的索引
	CMD_END

	//int8			target_flag;	// 0表示不需要目标信息, <0表示vector3 pos个数, >0表示target_id的个数
	//union
	//{
	//	struct
	//	{
	//		uint64	target_id[10];
	//	} target1;

	//	struct
	//	{
	//		float	x;
	//		float	y;
	//		float	z;
	//	} target2[10];
	//};

	// 技能准备或释放过程中打断等(需要广播)(size = 15)
	CMD_START(S2C_SpellFailed)
		uint64			caster_id;			// 释放者id
		uint32			spell_id;			// 释放的技能
		uint8			error;				// 错误原因
	CMD_END

	// 技能命中信息(需要广播)(size = 15)
	CMD_START(S2C_SpellHit)
		uint64			caster_id;			// 造成伤害或治疗的玩家
		uint64			target_id;			// 承受伤害或治疗的玩家
		uint32			info_id;			// 哪个技能或buff
		float			time_stamp;			// 时间戳
		int32			value;				// HP伤害或治疗数值(<0为伤害, >0为治疗, =0为未命中, 完全吸收, 免疫, 反弹, 躲避)
		int32			absorb;				// 伤害吸收数值
		uint8			flag;				// 命中, 暴击, 未命中, 吸收, 免疫, 反弹, 躲避, 见ESpellHitResult
		uint8			type;				// 0=技能, 1=buff, 见ESpellHitType
	CMD_END

	// 请求取消buff(size = 3)
	CMD_START(C2S_CancelAura)
		uint8			slot;				// buff的唯一标识
	CMD_END

	// 添加一个buff实例
	CMD_START(S2C_AddAura)
		uint64			unit_id;			// 谁添加
		naura_data		aura_data;			// buff实例
	CMD_END

	// 删除一个buff实例
	CMD_START(S2C_RemoveAura)
		uint64			unit_id;			// 谁删除
		uint8			slot;				// buff的唯一标识
	CMD_END

	// buff实例数据更新
	CMD_START(S2C_UpdateAura)
		uint64			unit_id;			// buff拥有者id
		uint8			slot;				// buff的唯一标识
		uint8			stack_count;		// 当前叠加次数
		uint32			remain_time;		// 剩余时间(单位s)
		uint32			max_duration;		// 总时间(单位s)
	CMD_END

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NMSG_SPELL_H_
