/**
 *	nexus ndef_game - nmsg_movement.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NDEF_GAME_NMSG_MOVEMENT_H_
#define _NDEF_GAME_NMSG_MOVEMENT_H_

#include "nprotocol_base.h"
#include "nmsg_base_id.h"
#include "nmsg_base_error_code.h"
#include "base_define.h"
#include "math/vector.h"

namespace nexus {

#pragma pack(push,1)

	/**
	 * 移动消息[Msg_Client_Movement, Msg_Client_Movement + 19]
	 */
	enum
	{
		S2C_MoveError			= Msg_Client_Movement,
		S2C_PlayerEnterAoi,
		S2C_CreatureEnterAoi,
		S2C_GameObjectEnterAoi,
		S2C_ObjectLeaveAoi,
		C2S_Move,
		S2C_Move,
		C2S_CheckTime,
		S2C_CheckTime,

	};

	enum EMoveError
	{
		EMRC_Success					= 0,		// 移动成功
		EMRC_CantArrive					= 1,		// 不能到达
		EMRC_Invalid					= 2,		// 非法调用
		EMRC_Fear						= 3,		// 恐惧状态
		EMRC_Root						= 4,		// 定身状态
		EMRC_Dead						= 5,		// 死亡状态
		EMRC_Casting					= 6,		// 释放技能
	};

	// 返回移动失败消息，只返回给请求移动的客户端
	CMD_START(S2C_MoveError)
		uint64			object_id;
		uint16			error_code;
		vector3			position;
		float			orient;
	CMD_END

	// 玩家进入视野
	CMD_START(S2C_PlayerEnterAoi)
		uint64			player_id;
		vector3			position;
		float			yaw;
		vector3			acceleration;
		float			time_stamp;
		uint8			flag;
	CMD_END

	// npc进入视野
	CMD_START(S2C_CreatureEnterAoi)
		uint64			creature_id;
		vector3			position;
		float			yaw;
		vector3			acceleration;
		float			time_stamp;
		uint8			flag;
	CMD_END

	// 游戏中静止的对象进入视野
	CMD_START(S2C_GameObjectEnterAoi)
		uint64			go_id;
		vector3			position;
		float			yaw;
	CMD_END

	// 对象离开视野
	CMD_START(S2C_ObjectLeaveAoi)
		uint64			object_id;
	CMD_END

	// 请求移动
	CMD_START(C2S_Move)
		vector3			position;
		float			yaw;
		vector3			acceleration;
		float			time_stamp;
		uint8			flag;
	CMD_END

	// 同步移动
	CMD_START(S2C_Move)
		uint64			unit_id;
		vector3			position;
		float			yaw;
		vector3			acceleration;
		float			time_stamp;
		uint8			flag;
	CMD_END

	// 请求校对时间
	CMD_START(C2S_CheckTime)
		float			client_time_stamp;
	CMD_END

	// 校对时间结果
	CMD_START(S2C_CheckTime)
		float			client_time_stamp;
		float			server_time_stamp;
	CMD_END

	// 击飞击退特殊处理，朝向问题，退的方向和朝向不一样，速度问题，击飞的速度会大于速度限制

	struct EnterAoi
	{
		union
		{
			char data1[sizeof(tagS2C_PlayerEnterAoi)];
			char data2[sizeof(tagS2C_CreatureEnterAoi)];
			char data3[sizeof(tagS2C_GameObjectEnterAoi)];
		};
	};

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NMSG_MOVEMENT_H_
