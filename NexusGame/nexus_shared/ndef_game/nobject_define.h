/**
 *	nexus ndef_game - nobject_define
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NDEF_GAME_NOBJECT_DEFINE_H_
#define _NDEF_GAME_NOBJECT_DEFINE_H_

#include "ntype_def.h"

namespace nexus {

	/**
	 * 游戏世界中对象类型枚举，64位guid
	 */
	enum EObjectType
	{
		EOT_PLAYER				= 0x01,
		EOT_CREATURE			= 0x02,


		EOT_ITEM				= 0x80,
	};

#define MAKE_PAIR64(l, h)				uint64(uint32(l) | (uint64(h) << 32))
#define PAIR64_HIPART(x)				(uint32)((uint64(x) >> 32) & 0x00000000FFFFFFFFui64)
#define PAIR64_LOPART(x)				(uint32)(uint64(x)         & 0x00000000FFFFFFFFui64)

#define MAKE_PAIR32(l, h)				uint32(uint16(l) | (uint32(h) << 16))
#define PAIR32_HIPART(x)				(uint16)((uint32(x) >> 16) & 0x0000FFFF)
#define PAIR32_LOPART(x)				(uint16)(uint32(x)         & 0x0000FFFF)

// g - 32 guid
// r - 24 realm 区服战场组信息
// t - 8  type  类型信息
#define MAKE_OBJECT_GUID(g, r, t)		uint64(uint64(g) | (uint64(r) << 32) | (uint64(t) << 56))
#define GET_TYPE(guid)					(uint8)(guid >> 56)
#define	IS_PLAYER(guid)					((GET_TYPE(guid) == EOT_PLAYER))
#define	IS_CREATURE(guid)				((GET_TYPE(guid) == EOT_CREATURE))

	const uint32 MAX_NAME_LEN = 8;			// 最大名字长度
	const uint32 REGEN_TIME			= 2000;				// 恢复计算时间间隔1/1000s

	// 种族
	enum ERaceType
	{
		ERT_None				= 0,			// 六界之外
		ERT_Human				= 1,			// 人
		ERT_Fairy				= 2,			// 仙
		ERT_Goblin				= 3,			// 妖
		ERT_Ghost				= 4,			// 鬼
		ERT_Deity				= 5,			// 神
		ERT_Demon				= 6,			// 魔
		ERT_Max					= 7,
	};

	// 职业
	enum EClassType
	{
		ECT_None				= 0,			// 
		ECT_XiaShi				= 1,			// 侠士
		ECT_JianXian			= 2,			// 剑仙
		ECT_LingYao				= 3,			// 灵妖
		ECT_ZheXian				= 4,			// 谪仙
		ECT_Max					= 5,
	};

	// 属性修改方式
	enum EUnitModifierType
	{
		EUMT_BaseValue			= 0,			// 基础值值的改变
		EUMT_BasePct			= 1,			// 基础值百分比的改变
		EUMT_TotalValue			= 2,			// 总值值的改变
		EUMT_TotalPct			= 3,			// 总值百分比的改变
		EUMT_Max				= 4,
	};

	// unit的状态标志位, 非buff能表示的状态
	enum EUnitStatus
	{
		EUS_Normal				= 0x00000000,	// 日常状态
		EUS_InCombat			= 0x00000001,	// 战斗状态
		EUS_Dead				= 0x00000002,	// 死亡状态
		EUS_CycleCasting		= 0x00000004,	// 持续释放状态
		EUS_Trading				= 0x00000008,	// 交易状态

	};

} // namespace nexus

#endif // _NDEF_GAME_NOBJECT_DEFINE_H_
