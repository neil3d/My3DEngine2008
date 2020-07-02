/**
 *	nexus login - nlogin_player
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	May, 2010
 */

#ifndef _NLOGIN_NLOGIN_PLAYER_H_
#define _NLOGIN_NLOGIN_PLAYER_H_

#include "nlogin_utility.h"

namespace nexus{


	/**
	 *	nlogin_player
	 */
	class nlogin_player
	{
	public:
		static nlogin_player* alloc_login_player(void);
		static void free_login_player(nlogin_player* player_ptr);

	public:
		nlogin_player(void);
		~nlogin_player(void);

		bool init(uint64 client_id, uint32 account_id, uint32 ip);

		void set_world_crc(uint32 world_crc){ m_world_crc = world_crc; }
		uint32 get_world_crc(void){ return m_world_crc; }

		void set_status(EPlayerLoginStatus status);

		void end_proof_account(void);
		void end_proof_mibao(uint32 mibao_crc);
		bool is_status(EPlayerLoginStatus status);

		uint64 get_client_id(void){ return m_client_id; }
		uint32 get_account_id(void){ return m_info.id; }
		uint32 get_privilege(void){ return m_info.privilege; }
		uint32 get_cur_ip(void){ return m_info.last_ip; }
		
		void set_privilege(uint32 privilege){ m_info.privilege = privilege; }

		// 验证密码
		bool proof(const char* psd);

	private:
		// 检验是否需要密保
		void check_need_mibao(void);

		//生成验证用的密保
		void generate_mibao(nchar mibao[MAX_MIBAO_LEN]);

	private:
		// 游戏世界、账号相关
		uint64	m_client_id;	// 玩家ID		
		uint32	m_world_crc;	// 玩家所在的world
		naccount_info	m_info;			// 玩家账号信息

		// 密保相关
		bool	m_need_mibao;	// 是否需要密保
		uint32	m_mibao_crc;	// 密保CRC
	};


} // namespace nexus

#endif	_NLOGIN_NLOGIN_PLAYER_H_