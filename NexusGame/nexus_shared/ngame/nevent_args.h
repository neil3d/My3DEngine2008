/**
 *	nexus ngame - nevent_args
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Jun, 2010
 */

#ifndef _NGAME_NEVENT_ARGS_H_
#define _NGAME_NEVENT_ARGS_H_

#include "nevent_base_args.h"

namespace nexus {

#pragma pack(push,1)

	enum EEventID
	{
		EEI_MOVE				= 1,
		EEI_ENTER_COMBAT		= 2,
		EEI_DAMAGE				= 3,
		EEI_DEAD				= 4,
		EEI_CHANGE_MAP			= 5,
		EEI_SPELL_PREPARE		= 6,
		EEI_SPELL_CAST			= 7,

		EEI_UNIT_TEST			= 100,

		EEI_PRE_ITEM_USE		= 1 + EEI_UNIT_TEST,
		EEI_ITEM_USED			= 2 + EEI_UNIT_TEST,
		EEI_NPC_KILLED			= 3 + EEI_UNIT_TEST,
	};

	// 移动事件, 表示是否移动或只有转身
	struct nevent_move_args : public nevent_base_args
	{
		bool			only_turn;

		nevent_move_args()
		{
			event_id = EEI_MOVE;
		}
	};

	// 进入战斗事件
	struct nevent_enter_combat_args : public nevent_base_args
	{
		uint64			 unit_id;						// 威胁者id

		nevent_enter_combat_args()
		{
			event_id = EEI_ENTER_COMBAT;
		}
	};

	// 伤害或治疗事件
	struct nevent_damage_args : public nevent_base_args
	{
		uint64			 unit_id;						// 释放者id
		int32			 damage;						// 伤害或治疗值

		nevent_damage_args()
		{
			event_id = EEI_DAMAGE;
		}
	};

	// 伤害或治疗事件
	struct nevent_dead_args : public nevent_base_args
	{
		uint64			 unit_id;						// 造成死亡的对象id可以为0

		nevent_dead_args()
		{
			event_id = EEI_DEAD;
		}
	};

	// 地图切换事件
	struct nevent_change_map_args : public nevent_base_args
	{
		uint32			 old_map_id;						// 老地图id
		uint32			 new_map_id;						// 新地图id

		nevent_change_map_args()
		{
			event_id = EEI_ENTER_COMBAT;
		}
	};

	struct nevent_spell_prepare_args : public nevent_base_args
	{
		uint32			spell_id;

		nevent_spell_prepare_args()
		{
			event_id = EEI_SPELL_PREPARE;
		}
	};

	struct nevent_spell_cast_args : public nevent_base_args
	{
		uint32			spell_id;

		nevent_spell_cast_args()
		{
			event_id = EEI_SPELL_CAST;
		}
	};



	// unit test
	struct nevent_pre_item_use_args : public nevent_base_args
	{
		uint32	item_id;
		uint64	item_guid;

		nevent_pre_item_use_args()
		{
			event_id = EEI_PRE_ITEM_USE;
		}
	};

	struct nevent_item_used_args : public nevent_base_args
	{
		uint32	item_id;
		uint64	item_guid;

		nevent_item_used_args()
		{
			event_id = EEI_ITEM_USED;
		}
	};

	struct nevent_npc_killed_args : public nevent_base_args
	{
		uint32	npc_info_id;

		nevent_npc_killed_args()
		{
			event_id = EEI_NPC_KILLED;
		}
	};

#pragma pack(pop)

} // namespace nexus

#endif // _NGAME_NEVENT_ARGS_H_
