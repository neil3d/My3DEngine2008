/**
 *	nexus ndef_cluster - nmsg_world_db.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NDEF_CLUSTER_NMSG_WORLD_DB_H_
#define _NDEF_CLUSTER_NMSG_WORLD_DB_H_

#include "nmsg_base_id.h"
#include "nprotocol_base.h"
#include "nmsg_base_error_code.h"
#include "nobject_fields.h"
#include "nlogin_define.h"
#include "nspell_data.h"

namespace nexus {

#pragma pack(push,1)

	/**
	 * msg id为uint16类型, 所有msg id控制在2000以内, 1001 - 1600
	 */
	enum
	{
		Msg_World_DB_Begin		= Msg_World_DB + 1,

		// character
		Msg_W2D_CharEnum,
		Msg_D2W_CharEnum,
		Msg_W2D_CharCreate,
		Msg_D2W_CharCreate,
		Msg_W2D_CharDelete,
		Msg_D2W_CharDelete,

		// loading, 没有有效数据也要返回相应消息
		Msg_W2D_LoadPlayerData,
		Msg_D2W_LoadPlayerStat,
		Msg_D2W_LoadPlayerItem,
		Msg_D2W_LoadPlayerSpell,
		Msg_D2W_LoadPlayerAura,
		Msg_D2W_LoadPlayerCoolDown,
		Msg_D2W_LoadPlayerRoleInfo,
		Msg_D2W_LoadPlayerSkill,

		// saving, 没有有效数据也要发送相应消息, 最后一个save消息处理退出
		Msg_W2D_SavePlayerStat,
		Msg_W2D_SavePlayerItem,
		Msg_W2D_SavePlayerSpell,
		Msg_W2D_SavePlayerAura,
		Msg_W2D_SavePlayerCoolDown,


		Msg_W2D_UnloadPlayer,
		Msg_D2W_LoadPlayerAtt,

		Msg_W2D_DeleteCharacter,
		Msg_D2W_DeleteCharacter,
		Msg_W2D_Operate_Skill,
		Msg_D2W_Operate_Skill,
		Msg_World_DB_End		= 1600,
	};

	// 错误枚举值
	enum EDBError
	{
		EDBError_None,
		EDBError_CreateCharacterSucess,
		EDBError_DeleteCharacterSucess,
		EDBError_OperateSkillSuccess,
		EDBError_NoExistCharacter,
		EDBError_NoRoleOffLine,
		EDBError_NoExistSkill,
		EDBError_Max,
	};

	// 请求角色列表
	CMD_START(Msg_W2D_CharEnum)
		uint32			account_id;
	CMD_END

	// 返回角色列表
	CMD_START(Msg_D2W_CharEnum)

	CMD_END

	// 请求创建角色
	CMD_START(Msg_W2D_CharCreate)
		uint32	account_id;
		nchar	name[MAX_ACCOUNT_LEN];
		uint8	sex;
		uint8	race;
		uint8	class_type;
	CMD_END

	// 返回角色创建结果
	CMD_START(Msg_D2W_CharCreate)
		uint16			error;
		uint64			player_id;
		// ...
	CMD_END

	// 请求删除角色
	CMD_START(Msg_W2D_CharDelete)
		uint64	player_id;
	CMD_END

	// 返回角色删除结果
	CMD_START(Msg_D2W_CharDelete)
		uint16			error;
		uint64			player_id;
	CMD_END

	// 请求加载角色相关数据
	CMD_START(Msg_W2D_LoadPlayerData)
		uint64			player_id;
	CMD_END

	// 返回角色属性信息
	CMD_START(Msg_D2W_LoadPlayerStat)
		uint32			stat_data[EPF_PlayerEnd - EUF_UnitSeparator];	// 属性, (变化部分)
	CMD_END

	// 返回角色物品信息
	CMD_START(Msg_D2W_LoadPlayerItem)

	CMD_END

	// 返回角色技能信息
	CMD_START(Msg_D2W_LoadPlayerSpell)
		uint16			num;
		uint32			spell_data[MAX_SPELL_NUM];
	CMD_END

	// 返回角色状态信息
	CMD_START(Msg_D2W_LoadPlayerAura)
		uint16			num;
		naura_data_db	aura_data[MAX_AURA_NUM];
	CMD_END

	// 返回角色cd信息
	CMD_START(Msg_D2W_LoadPlayerCoolDown)
		uint16				num;
		nspell_cool_dowm	cd_data[MAX_CD_NUM];
	CMD_END


	// 保存角色属性信息
	CMD_START(Msg_W2D_SavePlayerStat)

	CMD_END


	// 保存角色cd信息
	CMD_START(Msg_W2D_SavePlayerItem)

	CMD_END


	// 保存角色技能信息
	CMD_START(Msg_W2D_SavePlayerSpell)

	CMD_END


	// 保存角色状态信息
	CMD_START(Msg_W2D_SavePlayerAura)

	CMD_END


	// 保存角色cd信息
	CMD_START(Msg_W2D_SavePlayerCoolDown)

	CMD_END

	// 角色下线
	CMD_START(Msg_W2D_UnloadPlayer)
		uint32	role_id;
	CMD_END

	// 请求加载角色相关数据
	CMD_START(Msg_D2W_LoadPlayerAtt)
		uint16			error;
	CMD_END

	// 加载角色信息
	CMD_START(Msg_D2W_LoadPlayerRoleInfo)
		uint32	account_id;
		uint64	role_id;
		nchar	name[MAX_ACCOUNT_LEN];
		uint8	sex;
		uint8	race;
		uint8	class_type;
		uint16	map_id;
		uint32	last_time;
	CMD_END

	const uint32 MAX_SKILL_LEN = 12;
	// 加载角色Skill
	CMD_START(Msg_D2W_LoadPlayerSkill)
		uint64	role_id;
		uint32	num;
		uint32	skilles[MAX_SKILL_LEN];
	CMD_END



	//// 删除角色
	//CMD_START(Msg_W2D_DeleteCharacter)
	//	uint32	role_id;
	//CMD_END

	//// 删除角色
	//CMD_START(Msg_D2W_DeleteCharacter)
	//	uint32	role_id;
	//	uint8	result;
	//CMD_END

	enum EOperateSkill_Type
	{
		EOperateSkill_None,
		EOperateSkill_Add,		//增加技能
		EOperateSkill_Delete,	//移除技能
	};
	// 对技能相关操作(增加、删除)
	CMD_START(Msg_W2D_Operate_Skill)
		uint32	role_id;
		uint8	op_type;
		uint32	skill_id;
	CMD_END
	CMD_START(Msg_D2W_Operate_Skill)
		uint32	role_id;
		uint8	op_type;
		uint8	result;
	CMD_END

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_CLUSTER_NMSG_WORLD_DB_H_
