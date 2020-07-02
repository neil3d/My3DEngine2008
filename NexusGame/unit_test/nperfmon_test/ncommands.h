/**
 *	nexus nperfmon_test - ngm_command
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	May, 2010
 */

#ifndef _NPERFMON_TEST_NCOMMANDS_H_
#define _NPERFMON_TEST_NCOMMANDS_H_

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

	class ncommands : private nnoncopyable
	{
	public:
		typedef void (ncommands::*cmd_handler_type)(const ncommand_param&);

		struct ncommand
		{
			tstring					name;			// √¸¡Ó√˚
			cmd_handler_type		handler;		// ∫Ø ˝÷∏’Î
			tstring					desc;			// √¸¡Ó√Ë ˆ

			ncommand() : name(_T("")), handler(NULL), desc(_T("")) {}
		};

	public:
		ncommands();
		virtual ~ncommands();

		bool init();
		void destroy();

		bool parse_commands(const nchar* text);

	private:
		void _register_command(const tstring& cmd_name, cmd_handler_type handler, const tstring& desc);

		bool _execute_command(const tstring& cmd_name, const ncommand_param& param);

		// handler
		void _handle_login(const ncommand_param& param);
		
		void _handle_start_account(const ncommand_param& param);

	private:
		typedef boost::unordered_map<tstring, ncommand*>	NCOMMAND_MAP;

	private:
		NCOMMAND_MAP	m_commands;
	};

	#define scommands	nsingleton<ncommands>::instance()

} // namespace nexus

#endif // _NPERFMON_TEST_NCOMMANDS_H_
