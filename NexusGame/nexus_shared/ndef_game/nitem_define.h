/**
 *	nexus ndef_game - nitem_define
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NDEF_GAME_NITEM_DEFINE_H_
#define _NDEF_GAME_NITEM_DEFINE_H_

#include "ntype_def.h"

namespace nexus {

#pragma pack(push,1)

	// 非装备类物品实例(size = 18)
	struct nitem_data
	{
		uint64					guid;			// 8 物品唯一id
		uint32					info_id;		// 4 模板id
		uint16					count;			// 2 当前堆叠数目
		uint16					slot;			// 2 容器中的位置
		uint8					bag;			// 1 所在容器
		uint8					flag;			// 1 标志位信息(绑定，鉴定，锁定(非界面操作的锁定))
	};

	// 装备类物品实例(size = 18 + 74)
	struct nequip_data : public nitem_data
	{
		uint8					quality;		// 1 品质与强化等级合在一起?
		uint8					level;			// 1 强化等级
		uint32					durability;		// 4 耐久
		uint16					weight;			// 2 重量
		uint16					sharp;			// 2 锐利
		uint16					solid;			// 2 坚实
		uint16					tenacity;		// 2 强韧

		uint8					stat_type[8];	// 3 * 8 = 24 附加属性
		uint16					stat_value[8];
		uint16					gem[6];			// 2 * 6 = 12 镶嵌的宝石, 减少存放在物品实例中id的范围控制,在0xFFFF，存储时PAIR32_LOPART(spell_id), 使用时MAKE_PAIR32(gem[12], 0)

		uint16					spell[4];		// 2 * 4 = 8 附加技能, 存储时PAIR32_LOPART(spell_id), 使用时MAKE_PAIR32(spell[0], 0)
		uint16					spell_rate[3];	// 2 * 3 = 6 附加被动技能触发几率
		uint16					enchantment;	// 2 附魔id, 存储时PAIR32_LOPART(spell_id), 使用时MAKE_PAIR32(enchantment, 0)
		uint64					creator;		// 8 制造者id
	};

	// cd 时间与持久时间单独存储
	struct nitem_duration
	{
		uint32					guid;			// 物品id
		uint64					destroy_time;	// 到期时间time_t
	};

	// 背包容量建议控制在40 + 4 * 25 = 140个最大上限，在使用上尽量将坐骑，钥匙，货币，非战斗宠物做为非物品持久存储
	// 仓库容量建议控制在60 + 8 * 25 = 260个最大上限

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NITEM_DEFINE_H_
