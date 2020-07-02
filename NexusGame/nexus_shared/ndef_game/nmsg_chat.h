/**
 *	nexus ndef_game - nmsg_chat.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NDEF_GAME_NMSG_CHAT_H_
#define _NDEF_GAME_NMSG_CHAT_H_

#include "nprotocol_base.h"
#include "nmsg_base_id.h"
#include "nmsg_base_error_code.h"

namespace nexus {

#pragma pack(push,1)

	/**
	 * map 地图相关消息 id区间[Msg_Client_Chat, Msg_Client_Chat + 9]
	 */
	enum
	{
		S2C_ChatError			= Msg_Client_Chat,
		C2S_ChatMessage,
		S2C_ChatMessage,

	};

	// 聊天消息内容的最大长度
	const uint8	MAX_MSG_LEN	= 200;

	enum EChatType
	{
		ECT_System					= 0,		// 系统
		ECT_Say						= 1,		// 当前
		ECT_Party					= 2,		// 队伍
		ECT_Guild					= 3,		// 工会
		ECT_Whisper					= 4,		// 私聊
		ECT_Yell					= 5,		// 大喊
		ECT_Emote					= 6,		// 表情
		ECT_MonsterSay				= 7,		// npc
		ECT_Channel					= 8,		// 自定义频道
		ECT_PartyLeader				= 9,		// 小队队长
		ECT_BattleGround			= 10,		// 战场
		ECT_BattleGroundLeader		= 11,		// 战场领袖
		ECT_BossEmote				= 12,		// boss表情
		ECT_BossWhisper				= 13,		// boss
		ECT_BG_SystemNeutral		= 14,		// 战场系统
		ECT_BG_SystemRed			= 15,		// 战场系统红方
		ECT_BG_SystemBlue			= 16,		// 战场系统蓝方
		ECT_Map						= 17,		// 当前地图

		ECT_Client					= 18,		// 客户端在聊天频道提示用
		ECT_Loot					= ECT_Client + 0,
		ECT_Money					= ECT_Client + 1,
		ECT_Skill					= ECT_Client + 2,
	};

	// 错误枚举值
	enum EChatError
	{

	};

	struct tagTimeStamp
	{
		uint32		time_stamp1;
		uint32		time_stamp2;
		uint32		time_stamp3;
	};

	struct tagC2S_ChatMessage : public nmsg_base
	{
		uint8				type;
		uint8				msg_size;
		union
		{
			struct
			{
				nchar		msg[MAX_MSG_LEN + 1];
			} msg1;

			struct
			{
				uint64		to_player_id;
				nchar		msg[MAX_MSG_LEN + 1];
			} msg2;

			struct
			{
				uint32		channel_id;
				nchar		msg[MAX_MSG_LEN + 1];
			} msg3;
		};

		tagC2S_ChatMessage()
		{
			memset(this, 0, sizeof(*this));
			msg_id = C2S_ChatMessage;
		}

		uint32 get_packet_size() const 
		{
			if (ECT_Channel == type)
			{
				return sizeof(nmsg_base) + sizeof(type) + sizeof(msg_size) + sizeof(msg3.channel_id) + msg_size;
			}
			else if (ECT_Whisper == type)
			{
				return sizeof(nmsg_base) + sizeof(type) + sizeof(msg_size) + sizeof(msg2.to_player_id) + msg_size;
			}
			else
			{
				return sizeof(nmsg_base) + sizeof(type) + sizeof(msg_size) + msg_size;
			}
		}
	};

	struct tagS2C_ChatMessage : public nmsg_base
	{
		uint64				player_id;
		uint8				type;
		uint8				flag;
		uint8				msg_size;
		union
		{
			struct
			{
				nchar		msg[MAX_MSG_LEN + 1];
			} msg1;

			struct
			{
				uint32		channel_id;
				nchar		msg[MAX_MSG_LEN + 1];
			} msg2;
		};

		tagS2C_ChatMessage()
		{
			memset(this, 0, sizeof(*this));
			msg_id = S2C_ChatMessage;
		}

		uint32 get_packet_size() const 
		{
			if (ECT_Channel == type)
			{
				return sizeof(nmsg_base) + sizeof(player_id) + sizeof(type) + sizeof(flag) + sizeof(msg_size) + sizeof(msg2.channel_id) + msg_size;
			}
			else
			{
				return sizeof(nmsg_base) + sizeof(player_id) + sizeof(type) + sizeof(flag) + sizeof(msg_size) + msg_size;
			}
		}
	};

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NMSG_CHAT_H_
