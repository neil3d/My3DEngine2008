/**
 *	nexus login - ninner_cmd
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	June, 2010
 */

#ifndef _NDB_NLOGIN_CMD_H_
#define _NDB_NLOGIN_CMD_H_

#include <boost/lexical_cast.hpp>

namespace nexus{

	class ncommand_param : private nnoncopyable
	{
	public:
		ncommand_param() {}
		virtual ~ncommand_param() {}

		void push_param(const tstring& tokens)
		{
			m_tokens.push_back(tokens);
		}

		uint32 get_param_num() const
		{
			return uint32(m_tokens.size());
		}

		template<typename T>
		bool get_param(uint32 index, T& param) const
		{
			param = T();

			try
			{
				if (index >= m_tokens.size())
				{
					return false;
				}

				param = boost::lexical_cast<T>(m_tokens[index]);
				return true;
			}
			catch (boost::bad_lexical_cast&)
			{
				return false;
			}
		}

	private:
		std::vector<tstring> m_tokens;
	};

	class ninner_cmd
	{
	public:
		typedef void (ninner_cmd::*cmd_handler_type)(const ncommand_param&);

		struct ncommand
		{
			tstring					name;			// 命令名
			cmd_handler_type		handler;		// 函数指针
			tstring					desc;			// 命令描述

			ncommand() : name(_T("")), handler(NULL), desc(_T("")) {}
		};

		typedef boost::unordered_map<tstring, ncommand*>	CmdMap;
		typedef	CmdMap::iterator							CmdMapIt;

	public:
		ninner_cmd();
		~ninner_cmd();

		void init(void);
		void destroy(void);

		void update(uint32 elapse);

	private:
		bool _parse_commands(const nchar* cmd);

		bool _execute_command(const tstring& cmd_name, const ncommand_param& param);

		void _register_command(const tstring& cmd_name, cmd_handler_type handler, const tstring& desc);

	private:
		// 账号下线
		void _account_logout(const ncommand_param& param);

		void _exist_name(const ncommand_param& param);
		void _check_name(const ncommand_param& param);

		// 创建一个账号
		void _create_account(const ncommand_param& param);

		// 创建一批账号
		void _create_batch_account(const ncommand_param& param);

		// 检测nlogin_db::exist_account
		void _db_exist_account(const ncommand_param& param);

		// 检测nlogin_db::query_psd
		void _db_query_psd(const ncommand_param& param);

		// 检测nlogin_db::fix_player_login_status
		void _db_fix_login_status(const ncommand_param& param);

		// 检测nlogin_db::query_account_privilege
		void _db_query_privilege(const ncommand_param& param);

		// 检测nlogin_db::query_account_num
		void _db_query_account_num(const ncommand_param& param);

		// Database中所有的账号状态修正为指定状态
		void _db_modify_all_status(const ncommand_param& param);

	private:
		/**
		 *	初始化账号
		 *	@param index : 将创建账号排列号
		 */
		void _init_account_data(uint32 index);

	private:
		CmdMap	m_commands;
	};

	extern ninner_cmd g_inner_cmd;

}

#endif _NDB_NLOGIN_CMD_H_