/**
 *	nexus ndef_game - nmsg_char.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NDEF_GAME_NMSG_CHAR_H_
#define _NDEF_GAME_NMSG_CHAR_H_

#include "nprotocol_base.h"
#include "nmsg_base_id.h"
#include "nmsg_base_error_code.h"
#include "base_define.h"
#include "math/vector.h"

namespace nexus {

#pragma pack(push,1)

	/**
	 * char 角色管理相关消息 id区间[Msg_Client_Char, Msg_Client_Char + 19]
	 */
	enum
	{
		S2C_CharError			= Msg_Client_Char,
		C2S_CharEnum,
		S2C_CharEnum,
		C2S_CharCreate,
		S2C_CharCreate,
		C2S_CharDelete,
		S2C_CharDelete,

		C2S_CharEnterGame,
		S2C_CharEnterGame,

		C2S_CharLeaveGame,
	};

	// 错误枚举值
	enum ECharError
	{

	};

	// 角色管理统一错误返回消息，正确的话返回相应数据结果
	CMD_START(S2C_CharError)
		int16			error;
	CMD_END

	// 请求角色列表
	CMD_START(C2S_CharEnum)
	CMD_END

	// 角色列表结果
	CMD_START(S2C_CharEnum)
	CMD_END

	// 请求创建角色
	CMD_START(C2S_CharCreate)
	CMD_END

	// 创建角色结果
	CMD_START(S2C_CharCreate)
	CMD_END

	// 请求删除角色
	CMD_START(C2S_CharDelete)
	CMD_END

	// 删除角色结果
	CMD_START(S2C_CharDelete)
	CMD_END

	// 创建安全码
	// 重置安全码
	// 取消安全码

	CMD_START(C2S_CharEnterGame)
		uint64			player_id;
	CMD_END

	CMD_START(S2C_CharEnterGame)
		uint64			player_id;				// 角色id
		uint32			map_id;					// 地图类型id
		vector3			pos;					// 位置，经过修正的，玩家上线时，加速度为0, flag为静止
		float			yaw;					// 朝向
	CMD_END

	CMD_START(C2S_CharLeaveGame)
	CMD_END

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NMSG_CHAR_H_
