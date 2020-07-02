/**
 *	nexus ndef_game - nmsg_login.h
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NDEF_GAME_NMSG_LOGIN_H_
#define _NDEF_GAME_NMSG_LOGIN_H_

#include "nprotocol_base.h"
#include "nmsg_base_id.h"
#include "nmsg_base_error_code.h"
#include "nlogin_define.h"

namespace nexus {

#pragma pack(push,1)

	/**
	 * 账号验证相关消息 id区间[Msg_Client_Login, Msg_Client_Login + 9]
	 */
	enum
	{
		C2S_LoginProof			= Msg_Client_Login,
		S2C_LoginProofResult,
		C2S_MiBao,
		S2C_MiBao,
	};

	// 错误枚举值
	enum ELoginError
	{
		ELoginError_UNKNOWN,


		//角色登录账号验证(LoginProof)
		ELoginProof_No_Match,			//此帐号非法
		ELoginProof_In_Use,				//此帐号当前已经在使用
		ELoginProof_Freezed,			//此帐号被停用
		ELoginProof_Wrong_Version,		//版本号不对
		ELoginProof_Disabled,			//此帐号已经被封停
		ELoginProof_Queue,				//开始排队	
		ELoginProof_Psd_Error,			//密码错误
		ELoginProof_Mibao_Error,		//密保错误
		ELoginProof_Wrong_Proof_Type,	//非法验证策略
		ELoginProof_Proofing,			//正在验证或已经验证完成
		ELoginProof_WorldName_Wrong,	//非法的游戏世界名
		ELoginProof_World_No_Use,		//游戏世界不可用
		ELoginProof_SUCCESS,			//验证成功


	};

	// 请求登陆验证
	CMD_START(C2S_LoginProof)
		uint32	cur_version;			// 当前版本ID
		uint32	account_id;				// 用户名名称crc值
		char	psd[MAX_MD5_SIZE];		// 密码
		uint32	ip;						// 登陆IP
		uint8	mac[8];					// mac地址
	CMD_END

	// 登陆验证结果
	CMD_START(S2C_LoginProofResult)
		uint16			error;
	CMD_END

	// 服务器发送密保
	CMD_START(S2C_MiBao)
		nchar	MiBao[MAX_MIBAO_LEN];
	CMD_END

	// 客户端填写密保验证
	CMD_START(C2S_MiBao)
		uint32	mibao_crc;
	CMD_END

#pragma pack(pop)

} // namespace nexus

#endif // _NDEF_GAME_NMSG_LOGIN_H_
