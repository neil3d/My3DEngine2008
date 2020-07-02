/**
 *	nexus login - nlogin_utility
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	May, 2010
 */

#ifndef _NLOGIN_NLOGIN_UTILITY_H_
#define _NLOGIN_NLOGIN_UTILITY_H_

#include "nlogin_define.h"
#include "ncommon.h"
#include "nobject_pool.h"
#include "timer/ntimestamp.h"

namespace nexus{

	enum EProofPolicy
	{
		EPP_Nul,
		EPP_Tes,		// 测试专用
		EPP_Own,		// 公司验证策略
		EPP_XunLei,		// xunlei

		EPP_End
	};


	// 描述玩家登陆状态
	enum EPlayerLoginStatus
	{
		EPLS_None,
		EPLS_Proofing,		//验证账号
		EPLS_EndProof,		//账号验证结束
		EPLS_MiBao,			//验证密保
		EPLS_End_MiBao,		//密保验证结束
		EPLS_Queuing,		//正在排队
		EPLS_EnterWorld,	//进入world
	};


	// 账号信息结构体
	struct naccount_info
	{
		uint32	id;						// 账号ID(账号名称的CRC值)
		char	name[MAX_ACCOUNT_LEN];	// 账号名称
		char	psd[MAX_MD5_SIZE];		// 密码
		uint8	privilege;				// 权限
		uint64	last_login;				// 上次登陆时间
		uint32	last_ip;				// 上次登陆IP
		uint8	login_status;			// 登陆状态
		uint8	mac[8];					// mac地址

		naccount_info(void)
		{
			memset(this, 0, sizeof(naccount_info));
		}
	};


	class nlogin_player;
	typedef	std::list<nlogin_player*>			PlayerList;
	typedef	PlayerList::iterator				PlayerListIt;


#define NULL_MUTEX
#ifdef NULL_MUTEX
	//非线程安全的内存池
	extern nobject_pool<nnull_mutex>  g_pool;	//用于分配内存的内存池
#else
	//线程安全的内存池
	extern nobject_pool<nfast_mutex>  g_pool;	//用于分配内存的内存池
#endif


	//以下所有宏用于测试时间相关的
	//#define NWORLD_UPDATE_QUEUE			1		//nworld::update_queue()测试排队队列进入游戏世界的时间
	//#define NHANDLE_LOGIN_PROOF			1		//nlan_session::handle_login_proof2()验证账号、密码的时间
	//#define NLOGIN_DB_CACHE_ACCOUNT_INFO	1		//nlogin_db::cache_account_info()缓存账号的时间
	//#define NLOGIN_DB_EXIST_ACCOUNT		1		//nlogin_db::exist_account()检测账号是否存在的时间
	//#define NLOGIN_DB_FIX_LOGIN_STATUS	1		//nlogin_db::fix_player_login_status()修改账号登陆状态的时间
	#define NHANDLE_LOGIN_PROOF_NOLY		1		//nlan_session::handle_login_proof2()只验证账号、密码不进队列
} // namespace nexus

#endif	_NLOGIN_NLOGIN_UTILITY_H_