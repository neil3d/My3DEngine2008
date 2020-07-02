#include "ncommands.h"
#include "nclient.h"
#include "nclient_session.h"
#include "nworld_session.h"
#include "nlog_mt.h"
#include "ncfg.h"

#include <boost/tokenizer.hpp>

namespace nexus {

	ncommands::ncommands()
	{
	}

	ncommands::~ncommands()
	{
	}

	bool ncommands::init()
	{
		// test register
		_register_command(_T("login"),			&ncommands::_handle_login,					_T("login <num client>"));
		_register_command(_T("start_account"),	&ncommands::_handle_start_account,			_T("login <start client>"));


		return true;
	}

	void ncommands::destroy()
	{
		NCOMMAND_MAP::iterator iter = m_commands.begin();

		if (iter != m_commands.end())
		{
			delete iter->second;
		}

		m_commands.clear();
	}

	bool ncommands::parse_commands(const nchar* text)
	{
		if (0 == text || 0 == *text)
		{
			return false;
		}

		if (text[0] != _T('!') && text[0] != _T('.'))
		{
			return false;
		}

		// ignore single . and ! in line
		if (_tcslen(text) < 2)
		{
			return false;
		}

		/// ignore messages staring from many dots.
		if ((text[0] == _T('.') && text[1] == _T('.')) || (text[0] == _T('!') && text[1] == _T('!')))
		{
			return 0;
		}

		/// skip first . or ! (in console allowed use command with . and ! and without its)
		if (text[0] == _T('!') || text[0] == _T('.'))
		{
			++text;
		}

		// original `text` can't be used. It content destroyed in command code processing.
		tstring cmd_line(text);
		typedef boost::tokenizer<boost::char_separator<nchar>, tstring::const_iterator, tstring> tokenizer;
		boost::char_separator<nchar> sep(_T(" ,\t"));
		tokenizer tokens(cmd_line, sep);

		ncommand_param param;
		tstring command;

		tokenizer::iterator iter = tokens.begin();
		command = *iter;

		for (++iter; iter != tokens.end(); ++iter)
		{
			param.push_param(*iter);
		}

		return _execute_command(command, param);
	}

	void ncommands::_register_command(const tstring& msg_name, cmd_handler_type handler, const tstring& desc)
	{
		ncommand* cmd_ptr = new ncommand;

		if (!cmd_ptr)
		{
			NLOG_ERROR(_T("%s, alloc memory error"), __TFUNCTION__);
			return;
		}

		cmd_ptr->name		= msg_name;
		cmd_ptr->handler	= handler;
		cmd_ptr->desc		= desc;

		if (false == m_commands.insert(std::make_pair(msg_name, cmd_ptr)).second)
		{
			NLOG_ERROR(_T("%s, insert command error"), __TFUNCTION__);
			return;
		}
	}

	bool ncommands::_execute_command(const tstring& cmd_name, const ncommand_param& param)
	{
		NCOMMAND_MAP::const_iterator ncmd = m_commands.find(cmd_name);

		if (ncmd == m_commands.end())
		{
			return false;
		}

		if (!ncmd->second)
		{
			return false;
		}

		(this->*(ncmd->second->handler))(param);
		return true;
	}

	void ncommands::_handle_login(const ncommand_param& param)
	{
		uint32 num = 0;
		param.get_param(0, num);

		std::string ip = sncfg.get_string(ECI_IP);
		uint32 port = sncfg.get_uint32(ECI_Port);
		nclient_session* session_ptr = NULL;
		uint32 session_id = 0;
		
		for (uint32 i = 0; i < num; ++i)
		{
			session_id = g_client.connect_to(ip, port);

			if (0XFFFFFFFF == session_id)
			{
				continue;
			}

			session_ptr = new nclient_session;

			if (!session_ptr)
			{
				NLOG_ERROR(_T("%s, alloc session error"), __TFUNCTION__);
				return;
			}

			session_ptr->init(session_id);

			if (!g_world_session.add_session(session_ptr))
			{
				NLOG_ERROR(_T("%s, add session error"), __TFUNCTION__);
				return;
			}
		}

		nclient_session::m_index_login_account	+= sncfg.get_uint32(ECI_MaxSession);

		return;
	}

	void ncommands::_handle_start_account(const ncommand_param& param)
	{
		uint32 index_start = 0;
		param.get_param(0, index_start);
		nclient_session::m_index_login_account	= index_start;

		return;
	}
} // namespace nexus
