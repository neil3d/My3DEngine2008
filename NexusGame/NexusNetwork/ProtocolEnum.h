#pragma once
#include "nmsg_base_id.h"
#include "nmsg_char.h"
#include "nmsg_chat.h"
#include "nmsg_gm_command.h"
#include "nmsg_login.h"
#include "nmsg_map.h"
#include "nmsg_movement.h"
#include "nmsg_loading.h"
#include "nmsg_spell.h"
#include "nmsg_stat.h"

using namespace System;

namespace NexusEngine
{
	//public enum class NetMessageSection : System::UInt16
	//{
	//	Login		= nexus::Msg_Client_Login,
	//	Char		= nexus::Msg_Client_Char,	
	//	Map			= nexus::Msg_Client_Map,
	//	Chat		= nexus::Msg_Client_Chat,	
	//	Movement	= nexus::Msg_Client_Movement,
	//	GM			= nexus::Msg_Client_GM,	
	//};

	public enum class NetErrorCode : System::Int32
	{
		Success			= nexus::EError_Success,
		SystemError		= nexus::EError_SystemError,
	};

	public enum class LoginConst : System::Int32
	{
		MaxAccountLen	= nexus::MAX_ACCOUNT_LEN,
		MaxPasswordLen	= nexus::MAX_PASSWORD_LEN,
		MaxMibaoLen		= nexus::MAX_MIBAO_LEN,
		MaxMD5Size		= nexus::MAX_MD5_SIZE
	};

	public enum class LoginErrorCode : System::Int32
	{
		No_Match			= nexus::ELoginProof_No_Match,			//此帐号非法
		In_Use				= nexus::ELoginProof_In_Use,			//此帐号当前已经在使用
		Freezed				= nexus::ELoginProof_Freezed,			//此帐号被停用
		Wrong_Version		= nexus::ELoginProof_Wrong_Version,		//版本号不对
		Disabled			= nexus::ELoginProof_Disabled,			//此帐号已经被封停
		Queue				= nexus::ELoginProof_Queue,				//开始排队	
		Psd_Error			= nexus::ELoginProof_Psd_Error,			//密码错误
		Mibao_Error			= nexus::ELoginProof_Mibao_Error,		//密保错误
		Wrong_Proof_Type	= nexus::ELoginProof_Wrong_Proof_Type,	//非法验证策略
		Proofing			= nexus::ELoginProof_Proofing,			//正在验证或已经验证完成
		WorldName_Wrong		= nexus::ELoginProof_WorldName_Wrong,	//非法的游戏世界名
		World_No_Use		= nexus::ELoginProof_World_No_Use,		//游戏世界不可用
		SUCCESS				= nexus::ELoginProof_SUCCESS,			//验证成功
	};
	
	public enum class NetLogin : System::UInt16
	{
		CS_LoginProof		= nexus::C2S_LoginProof,
		SC_LoginProofResult	= nexus::S2C_LoginProofResult,
		CS_MiBao			= nexus::C2S_MiBao,
		SC_MiBao			= nexus::S2C_MiBao,
	};

	public enum class NetCharacter : System::UInt16
	{
		SC_CharError		= nexus::S2C_CharError,		
		CS_CharEnum			= nexus::C2S_CharEnum,
		SC_CharEnum			= nexus::S2C_CharEnum,
		CS_CharCreate		= nexus::C2S_CharCreate,
		SC_CharCreate		= nexus::S2C_CharCreate,
		CS_CharDelete		= nexus::C2S_CharDelete,
		SC_CharDelete		= nexus::S2C_CharDelete,
		CS_CharEnterGame	= nexus::C2S_CharEnterGame,
		SC_CharEnterGame	= nexus::S2C_CharEnterGame,
		CS_CharLeaveGame	= nexus::C2S_CharLeaveGame,
	};	

	public enum class NetMap : System::UInt16
	{
		SC_MapError			= nexus::S2C_MapError,	
		CS_MapEnter			= nexus::C2S_MapEnter,
		SC_MapEnter			= nexus::S2C_MapEnter,
		SC_TeleportTo		= nexus::S2C_TeleportTo,
		CS_TeleportAck		= nexus::C2S_TeleportAck,
	};

	// nexus::EMapError
	public enum class MapError : System::UInt16
	{
		MapInvalid		= nexus::EMR_MapInvalid,
	};

	public enum class NetChat : System::UInt16
	{
		SC_ChatError		= nexus::S2C_ChatError,	
		CS_ChatMessage		= nexus::C2S_ChatMessage,	
		SC_ChatMessage		= nexus::S2C_ChatMessage,	
	};	

	public enum class ChatType : System::UInt16
	{
		System					= nexus::ECT_System,				// 系统
		Say						= nexus::ECT_Say,					// 当前
		Party					= nexus::ECT_Party,					// 队伍
		Guild					= nexus::ECT_Guild,					// 工会
		Whisper					= nexus::ECT_Whisper,				// 私聊
		Yell					= nexus::ECT_Yell,					// 大喊
		Emote					= nexus::ECT_Emote,					// 表情
		MonsterSay				= nexus::ECT_MonsterSay,			// npc
		Channel					= nexus::ECT_Channel,				// 自定义频道
		PartyLeader				= nexus::ECT_PartyLeader,			// 小队队长
		BattleGround			= nexus::ECT_BattleGround,			// 战场
		BattleGroundLeader		= nexus::ECT_BattleGroundLeader,	// 战场领袖
		BossEmote				= nexus::ECT_BossEmote,				// boss表情
		BossWhisper				= nexus::ECT_BossWhisper,			// boss
		BG_SystemNeutral		= nexus::ECT_BG_SystemNeutral,		// 战场系统
		BG_SystemRed			= nexus::ECT_BG_SystemRed,			// 战场系统红方
		BG_SystemBlue			= nexus::ECT_BG_SystemBlue,			// 战场系统蓝方
		Map						= nexus::ECT_Map,					// 当前地图
		
		Client					= nexus::ECT_Client,				// 客户端在聊天频道提示用
		Loot					= nexus::ECT_Loot,
		Money					= nexus::ECT_Money,
		Skill					= nexus::ECT_Skill,
	};

	public enum class ChatConstant : System::Byte
	{
		P_MAX_MSG_LEN			= nexus::MAX_MSG_LEN,
	};

	// ref nmsg_movement.h
	public enum class NetMovement : System::UInt16
	{
		SC_MoveError			= nexus::S2C_MoveError,		
		SC_PlayerEnterAoi		= nexus::S2C_PlayerEnterAoi,
		SC_CreatureEnterAoi		= nexus::S2C_CreatureEnterAoi,
		SC_GameObjectEnterAoi	= nexus::S2C_GameObjectEnterAoi,
		SC_ObjectLeaveAoi		= nexus::S2C_ObjectLeaveAoi,
		CS_Move					= nexus::C2S_Move,
		SC_Move					= nexus::S2C_Move,
		CS_CheckTime			= nexus::C2S_CheckTime,
		SC_CheckTime			= nexus::S2C_CheckTime,
	};	

	// nexus::EMoveError
	// ref nmsg_movement.h
	public enum class MoveError : System::UInt16
	{
		Success		= nexus::EMRC_Success,		// 移动成功
		CantArrive	= nexus::EMRC_CantArrive,	// 不能到达
		Invalid		= nexus::EMRC_Invalid,		// 非法调用
		Fear		= nexus::EMRC_Fear,		// 恐惧状态
		Root		= nexus::EMRC_Root,		// 定身状态
		Dead		= nexus::EMRC_Dead,		// 死亡状态
		Casting		= nexus::EMRC_Casting,		// 释放技能
	};

	public enum class NetGM : System::UInt16
	{
		CS_GMCommand			= nexus::C2S_GMCommand,			
		SC_GMCommandResult		= nexus::S2C_GMCommandResult,
	};	

	// 角色上线数据加载消息, id区间[Msg_Client_Loading, Msg_Client_Loading + 50]
	// ref native nmsg_loading.h
	public enum class NetLoading : System::UInt16
	{
		SC_LoadingError		= nexus::S2C_LoadingError,	
		CS_GetInitStat		= nexus::C2S_GetInitStat,
		SC_GetInitStat		= nexus::S2C_GetInitStat,
		CS_GetInitSpell		= nexus::C2S_GetInitSpell,
		SC_GetInitSpell		= nexus::S2C_GetInitSpell,
		CS_GetInitAura		= nexus::C2S_GetInitAura,
		SC_GetInitAura		= nexus::S2C_GetInitAura,
	};	

	// ref native nmsg_spell.h
	// 技能buff相关消息 id区间[Msg_Client_Spell, Msg_Client_Spell + 50]
	public enum class NetSpell : System::UInt16
	{
		SC_SpellError		= nexus::S2C_SpellError,		
		CS_CastSpell		= nexus::C2S_CastSpell,
		SC_CastSpellResult	= nexus::S2C_CastSpellResult,
		CS_CancelCast		= nexus::C2S_CancelCast,
		SC_SpellPrepare		= nexus::S2C_SpellPrepare,
		SC_SpellCasting		= nexus::S2C_SpellCasting,
		SC_SpellFailed		= nexus::S2C_SpellFailed,
		
		SC_SpellHit			= nexus::S2C_SpellHit,
		CS_CancelAura		= nexus::C2S_CancelAura,
		SC_AddAura			= nexus::S2C_AddAura,
		SC_RemoveAura		= nexus::S2C_RemoveAura,
		SC_UpdateAura		= nexus::S2C_UpdateAura,
	};

	// ESpellCastResult
	// 技能释放结果枚举值
	// ref native nmsg_spell.h
	public enum class SpellCastResult : System::Byte
	{
		Success					= nexus::ESCR_Success,
		Failed_NoSpell			= nexus::ESCR_Failed_NoSpell,			// 请求释放的技能没有学会
		Failed_InvalidSpell		= nexus::ESCR_Failed_InvalidSpell,		// 请求释放的技能非法(被动技能, 技能id索引无效)
		Failed_SpellInProgress	= nexus::ESCR_Failed_SpellInProgress,	// 释放者正在执行另一动作
		Failed_CoolDown			= nexus::ESCR_Failed_CoolDown,			// 技能冷却未结束
		Failed_TooClose			= nexus::ESCR_Failed_TooClose,			// 指定目标太远了
		Failed_OutofRange		= nexus::ESCR_Failed_OutofRange,		// 指定目标太近了
		Failed_CasterDead		= nexus::ESCR_Failed_CasterDead,		// 释放者已经死亡
		Failed_Interrupted		= nexus::ESCR_Failed_Interrupted,		// 技能被打断(被动）
		Failed_NoMana			= nexus::ESCR_Failed_NoMana,			// 魔法值不够
		Failed_NoEnergy			= nexus::ESCR_Failed_NoEnergy,			// 能量值不够
		Failed_LineofSight		= nexus::ESCR_Failed_LineofSight,		// 视线阻挡
		Failed_InvalidTargets	= nexus::ESCR_Failed_InvalidTargets,	// 指定目标非法
		Failed_InCombat			= nexus::ESCR_Failed_InCombat,			// 处于战斗状态
		Failed_InFear			= nexus::ESCR_Failed_InFear,			// 处于恐惧状态
		Failed_InSilence		= nexus::ESCR_Failed_InSilence,			// 处于沉默状态
		Failed_InRoot			= nexus::ESCR_Failed_InRoot,			// 处于定身状态
		Failed_CantRepeat		= nexus::ESCR_Failed_CantRepeat,		// 该技能不能连续释放
		Failed_Cancel			= nexus::ESCR_Failed_Cancel,			// 技能被打断(主动取消)
		Failed_Move				= nexus::ESCR_Failed_Move,				// 移动中不能释放
	};			

	// ref native nmsg_spell.h
	// 技能命中结果枚举值
	[System::FlagsAttribute]
	public enum class SpellHitResult : System::Byte
	{
		Hit		= nexus::ESHR_Hit,			// 命中
		Crit	= nexus::ESHR_Crit,			// 暴击
		Miss	= nexus::ESHR_Miss,			// 未命中
		Immune	= nexus::ESHR_Immune,		// 免疫
		Absorb	= nexus::ESHR_Absorb,		// 吸收
		Reflect	= nexus::ESHR_Reflect,		// 反弹
		Evade	= nexus::ESHR_Evade,		// 躲避
	};

	// ref native nmsg_spell.h
	public enum class SpellHitType : System::Byte
	{
		Spell	= nexus::ESHT_Spell,		// 技能
		Aura	= nexus::ESHT_Aura,			// buff
	};

	// 属性相关消息 id区间[Msg_Client_Stat, Msg_Client_Stat + 20]
	// ref native nmsg_stat.h
	public enum class NetStat : System::UInt16
	{
		SC_StatError		= nexus::S2C_StatError,
		SC_StatUpdate		= nexus::S2C_StatUpdate,
		SC_RemoteStatUpdate	= nexus::S2C_RemoteStatUpdate,
	};

}									