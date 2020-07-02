/**
 *	nexus ndef_game - nmsg_loading.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Jun, 2010
 */

#ifndef _NDEF_GAME_NMSG_LOADING_H_
#define _NDEF_GAME_NMSG_LOADING_H_

#include "nprotocol_base.h"
#include "nmsg_base_id.h"
#include "nmsg_base_error_code.h"
#include "nobject_fields.h"
#include "nspell_data.h"

namespace nexus {

#pragma pack(push,1)

	/**
	 * 角色上线数据加载消息, id区间[Msg_Client_Loading, Msg_Client_Loading + 50]
	 */
	enum
	{
		S2C_LoadingError			= Msg_Client_Loading,
		C2S_GetInitStat,
		S2C_GetInitStat,
		C2S_GetInitSpell,
		S2C_GetInitSpell,
		C2S_GetInitAura,
		S2C_GetInitAura,

	};

	// 错误枚举值
	enum ELoadingError
	{

	};

	// 上线加载属性数据
	CMD_START(C2S_GetInitStat)
	CMD_END

	CMD_START(S2C_GetInitStat)
		uint32			stat_data[EPF_PlayerEnd];	// 属性, (变化部分)
	CMD_END

	// 上线加载技能数据
	CMD_START(C2S_GetInitSpell)
	CMD_END

	struct tagS2C_GetInitSpell : public nmsg_base
	{
		uint16			num;
		uint32			spell_data[MAX_SPELL_NUM];

		tagS2C_GetInitSpell()
		{
			memset(this, 0, sizeof(*this));
			msg_id = S2C_GetInitSpell;
		}

		uint32 get_packet_size() const 
		{
			return sizeof(nmsg_base) + sizeof(uint16) + num * sizeof(uint32);
		}
	};

	// 上线加载buff数据
	CMD_START(C2S_GetInitAura)
	CMD_END

	struct tagS2C_GetInitAura : public nmsg_base
	{
		uint16			num;
		naura_data		aura_data[MAX_AURA_NUM];

		tagS2C_GetInitAura()
		{
			memset(this, 0, sizeof(*this));
			msg_id = S2C_GetInitAura;
		}

		uint32 get_packet_size() const 
		{
			return sizeof(nmsg_base) + sizeof(uint16) + num * sizeof(naura_data);
		}
	};

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NMSG_LOADING_H_
