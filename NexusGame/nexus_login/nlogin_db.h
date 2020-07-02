/**
 *	nexus login - nlogin_db
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	May, 2010
 */

#ifndef _NLOGIN_NLOGIN_DB_H_
#define _NLOGIN_NLOGIN_DB_H_

#include "base_depend.h"
#include "nlogin_utility.h"

namespace nexus{

	class ndb_mysql;

	/**
	 *	nlogin_db
	 */
	class nlogin_db
	{
	public:
		nlogin_db(void);
		~nlogin_db(void);

		bool init(std::string host, std::string port, std::string name);
		void destroy(void);

		// 缓存指定账号信息
		bool cache_account_info(uint32 account_id, naccount_info* info_ptr);

		// 指定账号是否存在
		bool exist_account(uint32 account_id);

		/**
		 *	索取指定账号的密码
		 *	@return psd : 从DB中获取的密码
		 */
		bool query_psd(uint32 account_id, char* psd);

		// 修正玩家登陆标志
		void fix_player_login_status(uint32 account_id, EPlayerLoginStatus status);

		// 获取玩家密保
		bool get_mibao(uint32 account_id, char* mibao, uint32 len);

		// 获取玩家的privledge
		uint8 query_account_privilege(uint32 account_id);

		// 玩家登出
		void login_out(uint32 account_id, uint32 last_ip);

		// 获取账号表中账号的数量
		uint32 query_account_num(void);

		// 增加账号
		bool insert_account(naccount_info* data_ptr);

		// 修改所有的账号为指定状态
		bool fix_all_login_status(EPlayerLoginStatus status);

	private:
		bool is_exist_db(void){return (NULL !=m_db ); }

	private:
		ndb_mysql*	m_db;

	};

	//声明全局数据库访问接口
	extern nlogin_db g_login_db;

} // namespace nexus

#endif	_NLOGIN_NLOGIN_DB_H_