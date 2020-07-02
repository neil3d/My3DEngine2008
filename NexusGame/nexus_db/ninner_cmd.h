/**
 *	nexus db - ninner_cmd
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	June, 2010
 */

#ifndef _NDB_NINNER_CMD_H_
#define _NDB_NINNER_CMD_H_

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

		// 上线检测加载数据是否正确
		void _role_online(const ncommand_param& param);

		// 下线检测数据存储是否正确
		void _role_offline(const ncommand_param& param);

		// 巨量加载，检测加载速度
		void _role_batch_online(const ncommand_param& param);

		// 所有在线角色下线
		void _role_batch_offline(const ncommand_param& param);

		void _add_item(const ncommand_param& param);

		void _add_equip(const ncommand_param& param);

		void _add_skill(const ncommand_param& param);

		// 生成巨量测试数据
		void _create_data(const ncommand_param& param);

	private:
		bool _create_role(uint32 account_id, uint64 role_id);
		void _create_item(uint64 role_id);
		void _create_equip(uint64 role_id);
		void _create_skill(uint64 role_id);

	private:
		CmdMap			m_commands;

		AccountRoleMap	m_accountes;
		Uint32Map		m_roles;
		uint32			m_role_index;
	};

	extern ninner_cmd g_inner_cmd;

}

#endif _NDB_NINNER_CMD_H_