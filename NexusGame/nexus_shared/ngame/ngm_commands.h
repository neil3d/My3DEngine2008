/**
 *	nexus ngame - ngm_command
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NGAME_NGM_COMMAND_H_
#define _NGAME_NGM_COMMAND_H_

#include "ncommon.h"
#include "nsingleton.h"

#include <boost/lexical_cast.hpp>

namespace nexus {

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

	class nclient_session;

	class ngm_commands : private nnoncopyable
	{
	public:
		typedef void (ngm_commands::*gm_cmd_handler_type)(const ncommand_param&, nclient_session*);

		struct ncommand
		{
			tstring					name;			// 命令名
			gm_cmd_handler_type		handler;		// 函数指针
			uint8					privilege;		// GM权限
			tstring					desc;			// 命令描述

			ncommand() : name(_T("")), handler(NULL), privilege(0), desc(_T("")) {}
		};

	public:
		ngm_commands();
		virtual ~ngm_commands();

		bool init();
		void destroy();

		bool parse_commands(const nchar* text, nclient_session* session_ptr);

	private:
		void _register_command(const tstring& cmd_name, gm_cmd_handler_type handler, uint8 privilege, const tstring& desc);

		bool _execute_command(const tstring& cmd_name, const ncommand_param& param, nclient_session* session_ptr);

		// handler
		void _handle_test(const ncommand_param& param, nclient_session* session_ptr);
		
		// debug
		void _handle_debug_test(const ncommand_param& param, nclient_session* session_ptr);
		void _handle_map_get_objects_test(const ncommand_param& param, nclient_session* session_ptr);

		// level1
		void _handle_server_info_cmd(const ncommand_param& param, nclient_session* session_ptr);

		// level2
		void _handle_add_item_cmd(const ncommand_param& param, nclient_session* session_ptr);
		void _handle_go_cmd(const ncommand_param& param, nclient_session* session_ptr);
		void _handle_level_up_cmd(const ncommand_param& param, nclient_session* session_ptr);
		void _handle_aura_cmd(const ncommand_param& param, nclient_session* session_ptr);
		void _handle_unaura_cmd(const ncommand_param& param, nclient_session* session_ptr);
		void _handle_cast_spell_cmd(const ncommand_param& param, nclient_session* session_ptr);
		void _handle_cancel_cast_cmd(const ncommand_param& param, nclient_session* session_ptr);

		void _handle_modify_hp_cmd(const ncommand_param& param, nclient_session* session_ptr);
		void _handle_modify_energy_cmd(const ncommand_param& param, nclient_session* session_ptr);
		void _handle_modify_mana_cmd(const ncommand_param& param, nclient_session* session_ptr);
		void _handle_modify_reserve_stat_cmd(const ncommand_param& param, nclient_session* session_ptr);
		void _handle_modify_exp_cmd(const ncommand_param& param, nclient_session* session_ptr);
		void _handle_modify_money_cmd(const ncommand_param& param, nclient_session* session_ptr);

		// level3
		void _handle_reload_config_cmd(const ncommand_param& param, nclient_session* session_ptr);
		void _handle_reload_all_loot_cmd(const ncommand_param& param, nclient_session* session_ptr);

	private:
		typedef boost::unordered_map<tstring, ncommand*>	NCOMMAND_MAP;

	private:
		NCOMMAND_MAP	m_gm_commands;
	};

	#define s_gm_command	nsingleton<ngm_commands>::instance()

} // namespace nexus

#endif // _NGAME_NGM_COMMAND_H_
