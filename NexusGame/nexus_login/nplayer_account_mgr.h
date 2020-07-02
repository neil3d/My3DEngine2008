/**
 *	nexus login - nplayer_account_mgr
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	May, 2010
 */

#ifndef _NLOGIN_NPLAYER_ACCOUNT_MGR_H_
#define _NLOGIN_NPLAYER_ACCOUNT_MGR_H_

#include "nlogin_utility.h"

namespace nexus{
	
	class nlogin_player;

	/**
	 *	nplayer_account_mgr
	 */
	class nplayer_account_mgr
	{
	public:
		nplayer_account_mgr(void);
		~nplayer_account_mgr(void);

		bool init(void);
		void destroy(void);

		EProofPolicy get_proof_policy(void){ return m_proof_policy; }
		uint32 get_cur_version(void){ return m_cur_version; }

		// 检查版本是否一致
		bool check_version(uint32 version);

		// 检查名称的合法性
		bool check_name(const nchar* name);

		// 检测账号是否存在
		bool exist_name(const nchar* name);

		// 检测账号是否存在
		bool exist_account(uint32 account_id);

	private:
		uint32			m_cur_version;		// 服务器版本号
		EProofPolicy	m_proof_policy;		// 账号验证策略
		
	};


	extern nplayer_account_mgr g_player_account_mgr;

} // namespace nexus


#endif	_NLOGIN_NPLAYER_ACCOUNT_MGR_H_