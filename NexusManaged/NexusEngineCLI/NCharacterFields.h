#pragma once
#include "nobject_fields.h"
#include "nobject_define.h"

namespace NexusEngine
{
	// 对角色属性枚举定义的Interop封装

	public ref class NCharacterFieldsHelper
	{
	public:
		// IS_PLAYER
		static bool IsPlayer(System::Int64 guid);

		// IS_CREATURE
		static bool IsCreature(System::Int64 guid);
	};

	/**
	* 游戏世界中对象类型枚举，64位guid
	*/
	public enum class ObjectType : System::UInt32
	{
		Player		= EOT_PLAYER,
		Creature	= EOT_CREATURE,

		Item		= EOT_ITEM,
	};

	// 种族
	public enum class RaceType : System::UInt32
	{
		None	= ERT_None,				// 六界之外
		Human	= ERT_Human,			// 人
		Fairy	= ERT_Fairy,			// 仙
		Goblin	= ERT_Goblin,			// 妖
		Ghost	= ERT_Ghost,			// 鬼
		Deity	= ERT_Deity,			// 神
		Demon	= ERT_Demon,			// 魔
		Max		= ERT_Max,
	};

	// 属性修改方式
	public enum class UnitModifierType : System::UInt32
	{
		BaseValue	= EUMT_BaseValue,			// 基础值值的改变
		BasePct		= EUMT_BasePct,				// 基础值百分比的改变
		TotalValue	= EUMT_TotalValue,			// 总值值的改变
		TotalPct	= EUMT_TotalPct,			// 总值百分比的改变
		Max			= EUMT_Max,
	};


	// unit的状态标志位, 非buff能表示的状态
	[System::Flags]
	public enum class UnitStatus : System::UInt32
	{
		Normal		= EUS_Normal,		// 日常状态
		InCombat	= EUS_InCombat,		// 战斗状态
		Dead		= EUS_Dead,			// 死亡状态
		CycleCasting= EUS_CycleCasting,	// 持续释放状态
		Trading		= EUS_Trading,		// 交易状态
	};

	// native define ref nobject_fields.h 
	public enum class UnitFields : System::UInt32
	{
		Null		= EUF_UnitNull,				// 1, uint32, 空
		Stamina		= EUF_UnitStamina,			// 1, uint32, 体魄
		Vitality	= EUF_UnitVitality,			// 1, uint32, 精气
		Spirit		= EUF_UnitSpirit,			// 1, uint32, 神识
		GenGu		= EUF_UnitGenGu,			// 1, uint32, 根骨

		MaxHealth	= EUF_UnitMaxHealth,		// 1, uint32, 生命上限
		MaxEnergy	= EUF_UnitMaxEnergy,		// 1, uint32, 内力上限
		MaxMana		= EUF_UnitMaxMana,			// 1, uint32, 法力上限
		MeleeDamage	= EUF_UnitMeleeDamage,		// 1, float, 力量
		KungFuDamage= EUF_UnitKungFuDamage,		// 1, float, 武艺
		SpellDamage	= EUF_UnitSpellDamage,		// 1, float, 术法
		Agility		= EUF_UnitAgility,			// 1, float, 敏捷
		HealthRegeneration	= EUF_UnitHealthRegeneration,	// 1, float, 活力
		PowerRegeneration	= EUF_UnitPowerRegeneration,	// 1, float, 吐纳

		Dodge		= EUF_UnitDodge,			// 1, float, 闪避
		Crit		= EUF_UnitCrit,				// 1, float, 暴击
		Speed		= EUF_UnitSpeed,			// 1, float, 移动速度
		AttackSpeed	= EUF_UnitAttackSpeed,		// 1, float, 出手速度

		TotalWeight = EUF_UnitTotalWeight,		// 1, uint32, 全身装备重量
		Sharp		= EUF_UnitSharp,			// 1, uint32, 锐利
		Defense		= EUF_UnitDefense,			// 1, uint32, 防御
		Resilience	= EUF_UnitResilience,		// 1, uint32, 韧性
		MetalResist	= EUF_UnitMetalResist,		// 1, uint32, 金灵力
		WoodResist	= EUF_UnitWoodResist,		// 1, uint32, 目灵力
		WaterResist	= EUF_UnitWaterResist,		// 1, uint32, 水灵力
		FireResist	= EUF_UnitFireResist,		// 1, uint32, 火灵力
		SoilResist	= EUF_UnitSoilResist,		// 1, uint32, 土灵力

		Health		= EUF_UnitHealth,			// 1, uint32, 当前生命
		Energy		= EUF_UnitEnergy,			// 1, uint32, 当前内力
		Mana		= EUF_UnitMana,				// 1, uint32, 当前法力
		Level		= EUF_UnitLevel,			// 1, uint32, 等级
		Flags		= EUF_UnitFlags,			// 1, uint32, 状态标志位
		
		End			= EUF_UnitEnd,				// end
	};

	public enum class PlayerFields : System::UInt16
	{
		BaseStamina		= EPF_PlayerBaseStamina,		// 1, uint32, 已加体魄点
		BaseVitality	= EUF_PlayerBaseVitality,		// 1, uint32, 已加精气点
		BaseSpirit		= EUF_PlayerBaseSpirit,			// 1, uint32, 已加神识点
		BaseGenGu		= EUF_PlayerBaseGenGu,			// 1, uint32, 已加根骨点
		ReserveStat		= EPF_PlayerReserveStat,		// 1, uint32, 剩余基本属性点
		Exp				= EPF_PlayerExp,				// 2, uint64, 经验
		Money			= EPF_PlayerMoney,				// 2, uint64, 游戏币
		MoneyBind		= EPF_PlayerMoneyBind,			// 2, uint64, 绑定游戏币
		End				= EPF_PlayerEnd					// end
	};

	public enum class FieldsSeparator : System::UInt32
	{
		StatStart	= EUS_StatStart,
		StatEnd		= EUS_StatEnd,
		PowerStart	= EUS_PowerStart,
		PowerEnd	= EUS_PowerEnd,
		EquipStart	= EUS_EquipStart,
		EquipEnd	= EUS_EquipEnd,
	};
	

}
