/**
 *	nexus ndef_game - nobject_fields
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NDEF_GAME_NOBJECT_FIELDS_H_
#define _NDEF_GAME_NOBJECT_FIELDS_H_

namespace nexus {

	// 主要为战斗属性
	enum EUnitFields
	{
		EUF_UnitNull							= 0,	// 1, uint32, 空
		EUF_UnitStamina							= 1,	// 1, uint32, 体魄
		EUF_UnitVitality						= 2,	// 1, uint32, 精气
		EUF_UnitSpirit							= 3,	// 1, uint32, 神识
		EUF_UnitGenGu							= 4,	// 1, uint32, 根骨

		EUF_UnitMaxHealth						= 5,	// 1, uint32, 生命上限
		EUF_UnitMaxEnergy						= 6,	// 1, uint32, 内力上限
		EUF_UnitMaxMana							= 7,	// 1, uint32, 法力上限
		EUF_UnitMeleeDamage 					= 8,	// 1, float, 力量
		EUF_UnitKungFuDamage 					= 9,	// 1, float, 武艺
		EUF_UnitSpellDamage						= 10,	// 1, float, 术法
		EUF_UnitAgility							= 11,	// 1, float, 敏捷
		EUF_UnitHealthRegeneration				= 12,	// 1, float, 活力
		EUF_UnitPowerRegeneration				= 13,	// 1, float, 吐纳

		EUF_UnitDodge							= 14,	// 1, float, 闪避
		EUF_UnitCrit							= 15,	// 1, float, 暴击
		EUF_UnitSpeed							= 16,	// 1, float, 移动速度
		EUF_UnitAttackSpeed						= 17,	// 1, float, 出手速度

		EUF_UnitTotalWeight						= 18,	// 1, uint32, 全身装备重量
		EUF_UnitSharp							= 19,	// 1, uint32, 锐利
		EUF_UnitDefense							= 20,	// 1, uint32, 防御
		EUF_UnitResilience						= 21,	// 1, uint32, 韧性
		EUF_UnitMetalResist						= 22,	// 1, uint32, 金灵力
		EUF_UnitWoodResist						= 23,	// 1, uint32, 目灵力
		EUF_UnitWaterResist						= 24,	// 1, uint32, 水灵力
		EUF_UnitFireResist						= 25,	// 1, uint32, 火灵力
		EUF_UnitSoilResist						= 26,	// 1, uint32, 土灵力

		EUF_UnitSeparator						= 27,	// 华丽的分割线

		EUF_UnitHealth							= 0 + EUF_UnitSeparator,	// 1, uint32, 当前生命
		EUF_UnitEnergy							= 1 + EUF_UnitSeparator,	// 1, uint32, 当前内力
		EUF_UnitMana							= 2 + EUF_UnitSeparator,	// 1, uint32, 当前法力
		EUF_UnitLevel							= 3 + EUF_UnitSeparator,	// 1, uint32, 等级
		EUF_UnitFlags							= 4 + EUF_UnitSeparator,	// 1, uint32, 状态标志位
		EUF_UnitEnd								= 5 + EUF_UnitSeparator,	// end
	};

	enum EPlayerFields
	{
		EPF_PlayerBaseStamina					= 0 + EUF_UnitEnd,		// 1, uint32, 已加体魄点
		EUF_PlayerBaseVitality					= 1 + EUF_UnitEnd,		// 1, uint32, 已加精气点
		EUF_PlayerBaseSpirit					= 2 + EUF_UnitEnd,		// 1, uint32, 已加神识点
		EUF_PlayerBaseGenGu						= 3 + EUF_UnitEnd,		// 1, uint32, 已加根骨点
		EPF_PlayerReserveStat					= 4 + EUF_UnitEnd,		// 1, uint32, 剩余基本属性点
		EPF_PlayerExp							= 5 + EUF_UnitEnd,		// 2, uint64, 经验
		EPF_PlayerMoney							= 7 + EUF_UnitEnd,		// 2, uint64, 游戏币
		EPF_PlayerMoneyBind						= 9 + EUF_UnitEnd,		// 2, uint64, 绑定游戏币
		EPF_PlayerEnd							= 11 + EUF_UnitEnd
	};

	enum EUnitSeparator
	{
		EUS_StatStart							= EUF_UnitStamina,
		EUS_StatEnd								= EUF_UnitGenGu + 1,
		EUS_PowerStart							= EUF_UnitMaxHealth,
		EUS_PowerEnd							= EUF_UnitMaxMana + 1,
		EUS_EquipStart							= EUF_UnitSharp,
		EUS_EquipEnd							= EUF_UnitSoilResist + 1,
	};


} // namespace nexus

#endif // _NDEF_GAME_NOBJECT_FIELDS_H_
