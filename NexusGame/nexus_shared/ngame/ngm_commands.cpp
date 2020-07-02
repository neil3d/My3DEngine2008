#include "ngm_commands.h"
#include "nclient_session.h"
#include "nenums.h"
#include "nlog_mt.h"

#include <boost/tokenizer.hpp>

namespace nexus {

	ngm_commands::ngm_commands()
	{
	}

	ngm_commands::~ngm_commands()
	{
	}

	bool ngm_commands::init()
	{
		// test register
		_register_command(_T("test"),			&ngm_commands::_handle_test,					EAT_GM_LV1,		_T("test gm command parse!"));
		_register_command(_T("world_test"),		&ngm_commands::_handle_test,					EAT_GM_LV1,		_T("world_test gm command parse!"));

		// cmds
		// debug
		_register_command(_T("test_map_get_objects"),		&ngm_commands::_handle_map_get_objects_test,			EAT_GM_LV2,		_T(".test_map_get_objects <type> <include self>, test map get objects function!"));

		// level1
		_register_command(_T("serverinfo"),		&ngm_commands::_handle_server_info_cmd,			EAT_GM_LV1,		_T(".serverinfo, get server info!"));

		// level2
		_register_command(_T("additem"),		&ngm_commands::_handle_add_item_cmd,			EAT_GM_LV2,		_T(".additem <info id> <num> <color>, add item!"));
		_register_command(_T("go"),				&ngm_commands::_handle_go_cmd,					EAT_GM_LV2,		_T(".go <x> <z> <map id>, go to a position!"));
		_register_command(_T("levelup"),		&ngm_commands::_handle_level_up_cmd,			EAT_GM_LV2,		_T(".levelup <add level>, level up!"));
		_register_command(_T("aura"),			&ngm_commands::_handle_aura_cmd,				EAT_GM_LV2,		_T(".aura <aura id> <target id>, add aura!"));
		_register_command(_T("unaura"),			&ngm_commands::_handle_unaura_cmd,				EAT_GM_LV2,		_T(".unaura <aura slot> <target id>, remove aura!"));
		_register_command(_T("cast"),			&ngm_commands::_handle_cast_spell_cmd,			EAT_GM_LV2,		_T(".cast <spell id> <target id> <target pos>, cast spell!"));

		_register_command(_T("hp"),				&ngm_commands::_handle_modify_hp_cmd,			EAT_GM_LV2,		_T(".hp <cur value> <max value>, change hp!"));
		_register_command(_T("energy"),			&ngm_commands::_handle_modify_energy_cmd,		EAT_GM_LV2,		_T(".energy <cur value> <max value>, change energy!"));
		_register_command(_T("mana"),			&ngm_commands::_handle_modify_mana_cmd,			EAT_GM_LV2,		_T(".mana <cur value> <max value>, change mana!"));
		_register_command(_T("res_stat"),		&ngm_commands::_handle_modify_reserve_stat_cmd,	EAT_GM_LV2,		_T(".res_stat <add value>, add reserve stat!"));
		_register_command(_T("exp"),			&ngm_commands::_handle_modify_exp_cmd,			EAT_GM_LV2,		_T(".exp <add value>, add exp!"));
		_register_command(_T("money"),			&ngm_commands::_handle_modify_money_cmd,		EAT_GM_LV2,		_T(".money <add value>, add money!"));

		// level3
		_register_command(_T("reloadconfig"),	&ngm_commands::_handle_reload_config_cmd,		EAT_GM_LV3,		_T(".reloadconfig, reload server config!"));
		_register_command(_T("reloadallloot"),	&ngm_commands::_handle_reload_all_loot_cmd,		EAT_GM_LV3,		_T(".reloadallloot, reload all loot!"));

		return true;
	}

	void ngm_commands::destroy()
	{
		NCOMMAND_MAP::iterator iter = m_gm_commands.begin();

		if (iter != m_gm_commands.end())
		{
			delete iter->second;
		}

		m_gm_commands.clear();
	}

	bool ngm_commands::parse_commands(const nchar* text, nclient_session* session_ptr)
	{
		if (0 == text || 0 == *text)
		{
			return false;
		}

		if (session_ptr)
		{
			if (text[0] != _T('!') && text[0] != _T('.'))
			{
				return false;
			}
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

		return _execute_command(command, param, session_ptr);
	}

	void ngm_commands::_register_command(const tstring& msg_name, gm_cmd_handler_type handler, uint8 privilege, const tstring& desc)
	{
		ncommand* cmd_ptr = new ncommand;

		if (!cmd_ptr)
		{
			NLOG_ERROR(_T("%s, alloc memory error"), __TFUNCTION__);
			return;
		}

		cmd_ptr->name		= msg_name;
		cmd_ptr->handler	= handler;
		cmd_ptr->privilege	= privilege;
		cmd_ptr->desc		= desc;

		if (false == m_gm_commands.insert(std::make_pair(msg_name, cmd_ptr)).second)
		{
			NLOG_ERROR(_T("%s, insert command error"), __TFUNCTION__);
			return;
		}
	}

	bool ngm_commands::_execute_command(const tstring& cmd_name, const ncommand_param& param, nclient_session* session_ptr)
	{
		NCOMMAND_MAP::const_iterator ncmd = m_gm_commands.find(cmd_name);

		if (ncmd == m_gm_commands.end())
		{
			return false;
		}

		if (!ncmd->second)
		{
			return false;
		}

		if (session_ptr)
		{
			if (session_ptr->get_privilege() < ncmd->second->privilege)
			{
				return false;
			}
		}

		(this->*(ncmd->second->handler))(param, session_ptr);
		return true;
	}

	void ngm_commands::_handle_test(const ncommand_param& param, nclient_session* session_ptr)
	{
		uint32 info_id = 0, num = 0;
		float dot = 5.0f;
		tstring str;
		param.get_param(0, info_id);
		param.get_param(1, num);
		param.get_param(2, dot);
		param.get_param(3, str);
		return;
	}

} // namespace nexus
