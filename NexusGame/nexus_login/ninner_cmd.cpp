#include <signal.h>
#include <iostream>
#include <boost/tokenizer.hpp>

#include "nlog_mt.h"
#include "nutil.h"

#include "nlogin.h"
#include "nlogin_db.h"
#include "nworld.h"
#include "nworld_mgr.h"
#include "nplayer_account_mgr.h"

#include "ninner_cmd.h"

#pragma warning(disable : 4996)

namespace nexus{

ninner_cmd g_inner_cmd;

ninner_cmd::ninner_cmd()
{
}

ninner_cmd::~ninner_cmd()
{
}

void ninner_cmd::init(void)
{
	_register_command(_T("account_logout"),			&ninner_cmd::_account_logout,			_T("account_logout"));
	_register_command(_T("exist_name"),		&ninner_cmd::_exist_name,	_T("exist_name"));
	_register_command(_T("check_name"),		&ninner_cmd::_check_name,	_T("check_name"));

	_register_command(_T("create_account"),			&ninner_cmd::_create_account,			_T("create_account"));
	_register_command(_T("create_batch_account"),	&ninner_cmd::_create_batch_account,		_T("create_batch_account"));

	_register_command(_T("db_exist_account"),		&ninner_cmd::_db_exist_account,			_T("db_exist_account"));
	_register_command(_T("db_query_psd"),			&ninner_cmd::_db_query_psd,				_T("db_query_psd"));
	_register_command(_T("db_fix_login_status"),	&ninner_cmd::_db_fix_login_status,		_T("db_fix_login_status"));
	_register_command(_T("db_query_privilege"),		&ninner_cmd::_db_query_privilege,		_T("db_query_privilege"));
	_register_command(_T("db_query_account_num"),	&ninner_cmd::_db_query_account_num,		_T("db_query_account_num"));

	_register_command(_T("db_modify_all_status"),	&ninner_cmd::_db_modify_all_status,		_T("db_modify_all_status"));

}

void ninner_cmd::destroy(void)
{
	for(CmdMapIt it=m_commands.begin(); m_commands.end() != it; ++it)
	{
		delete it->second;
		it->second = NULL;
	}

	m_commands.clear();
}

void ninner_cmd::update(uint32 elapse)
{
	ncmd_queue& cmd_queue = nlogin::instance().get_cmd_queue();

	if( !cmd_queue.empty() )
	{
		tstring cmd = cmd_queue.dequeue();
		_parse_commands(cmd.c_str());
	}

}

bool ninner_cmd::_parse_commands(const nchar* text)
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

bool ninner_cmd::_execute_command(const tstring& cmd_name, const ncommand_param& param)
{
	CmdMapIt it = m_commands.find(cmd_name);

	if (m_commands.end() == it)
		return false;

	if (!it->second)
		return false;

	(this->*(it->second->handler))(param);

	return true;
}

void ninner_cmd::_register_command(const tstring& cmd_name, cmd_handler_type handler, const tstring& desc)
{
	ncommand* cmd_ptr = new ncommand;

	if (!cmd_ptr)
	{
		NLOG_ERROR(_T("%s, alloc memory error"), __TFUNCTION__);
		return;
	}

	cmd_ptr->name		= cmd_name;
	cmd_ptr->handler	= handler;
	cmd_ptr->desc		= desc;

	if (false == m_commands.insert(std::make_pair(cmd_name, cmd_ptr)).second)
	{
		NLOG_ERROR(_T("%s, insert command error"), __TFUNCTION__);
		return;
	}
}

// 账号下线
void ninner_cmd::_account_logout(const ncommand_param& param)
{
	tstring world_name;
	uint32	account_id = 0;
	param.get_param(0, world_name);
	param.get_param(1, account_id);

	if(!world_name.size())
		NLOG_INFO(_T("%s,  no world"), __TFUNCTION__);

	uint32	world_crc = s_util.crc32( world_name.c_str(), (uint32)(world_name.size()*sizeof(TCHAR)) );
	nworld* world_ptr = g_world_mgr.get_world_by_name_crc(world_crc);

	if(!world_ptr)
		NLOG_ERROR(_T("%s,  %s world is not exist"), __TFUNCTION__, world_name.c_str());

	world_ptr->login_out(account_id);
}


void ninner_cmd::_exist_name(const ncommand_param& param)
{
	tstring account_name;
	param.get_param(0, account_name);

	if( !account_name.size() )
		return;

	if( g_player_account_mgr.exist_name(account_name.c_str()) )
	{
		NLOG_INFO(_T("%s,  %s is exist"), __TFUNCTION__, account_name.c_str());
	}
	else
	{
		NLOG_ERROR(_T("%s,  %s is not exist"), __TFUNCTION__, account_name.c_str());
	}
}


void ninner_cmd::_check_name(const ncommand_param& param)
{
	tstring account_name;
	param.get_param(0, account_name);

	if( !account_name.size() )
		return;

	if( g_player_account_mgr.check_name(account_name.c_str()) )
	{
		NLOG_INFO(_T("%s,  %s is  reasonable"), __TFUNCTION__, account_name.c_str());
	}
	else
	{
		NLOG_ERROR(_T("%s,  %s is not reasonable"), __TFUNCTION__, account_name.c_str());
	}
}


// 创建一个账号
void ninner_cmd::_create_account(const ncommand_param& param)
{
	uint32 index = g_login_db.query_account_num();
	_init_account_data(index+1);
}


// 创建一批账号
void ninner_cmd::_create_batch_account(const ncommand_param& param)
{
	uint32 num = 0;
	param.get_param(0, num);

	if(!num) return;

	uint32 start = g_login_db.query_account_num();
	for(uint32 index=0; index<num; ++index)
	{
		_init_account_data(start+index);
	}
}


// 检测nlogin_db::exist_account
void ninner_cmd::_db_exist_account(const ncommand_param& param)
{
	uint32 account_id = 0;
	param.get_param(0, account_id);

	if( g_login_db.exist_account(account_id) )
		NLOG_INFO(_T("%s  account %d is exist!"), __TFUNCTION__, account_id);
	else
		NLOG_INFO(_T("%s  account %d is not exist!"), __TFUNCTION__, account_id);
}


// 检测nlogin_db::query_psd
void ninner_cmd::_db_query_psd(const ncommand_param& param)
{
	uint32 account_id = 0;
	param.get_param(0, account_id);

	char psd[MAX_MD5_SIZE+1];
	memset(psd, 0, sizeof(psd));
	if( g_login_db.query_psd(account_id, psd) )
	{
		nchar npsd[MAX_MD5_SIZE+1];
		uint32 cur_len = ::MultiByteToWideChar(CP_UTF8, 0, psd, -1, NULL, 0);
		::MultiByteToWideChar(CP_UTF8, 0, psd, -1, npsd, cur_len);

		NLOG_INFO(_T("%s  pds is %s!"), __TFUNCTION__, npsd);
	}
	else
		NLOG_INFO(_T("%s  wrong to query psd!"), __TFUNCTION__);
}

// 检测nlogin_db::fix_player_login_status
void ninner_cmd::_db_fix_login_status(const ncommand_param& param)
{
	uint32 account_id	= 0;
	uint32 status		= 0;
	param.get_param(0, account_id);
	param.get_param(1, status);

	g_login_db.fix_player_login_status(account_id, (EPlayerLoginStatus)status);
}


// 检测nlogin_db::query_account_privilege
void ninner_cmd::_db_query_privilege(const ncommand_param& param)
{
	uint32 account_id = 0;
	param.get_param(0, account_id);

	uint32 privilege = g_login_db.query_account_privilege(account_id);
	NLOG_INFO(_T("%s  privilege is %d"), __TFUNCTION__, privilege);
}


// 检测nlogin_db::query_account_num
void ninner_cmd::_db_query_account_num(const ncommand_param& param)
{
	uint32 account_num = g_login_db.query_account_num();
	NLOG_INFO(_T("%s  account num is %d"), __TFUNCTION__, account_num);
}


// Database中所有的账号状态修正为指定状态
void ninner_cmd::_db_modify_all_status(const ncommand_param& param)
{
	uint32 status = 0;
	param.get_param(0, status);

	g_login_db.fix_all_login_status((EPlayerLoginStatus)status);
}


/**
 *	初始化账号
 *	@param index : 将创建账号排列号
 */
void ninner_cmd::_init_account_data(uint32 index)
{
	naccount_info data;
	uint32	len	= sprintf(data.name, "%s_%d", "nexus", index);

	data.id	= s_util.crc32((void*)data.name, len);
	memcpy(data.psd, data.name, MAX_MD5_SIZE);

	data.privilege		= s_util.urand(1, 255);
	data.login_status	= EPLS_None;
	data.last_ip		= s_util.rand32();
	data.mac[0]			= s_util.urand(1, 255);
	data.mac[1]			= s_util.urand(1, 255);
	data.mac[2]			= s_util.urand(1, 255);
	data.mac[3]			= s_util.urand(1, 255);
	data.mac[4]			= s_util.urand(1, 255);
	data.mac[5]			= s_util.urand(1, 255);
	data.mac[6]			= s_util.urand(1, 255);
	data.mac[7]			= s_util.urand(1, 255);

	g_login_db.insert_account(&data);
}


}
