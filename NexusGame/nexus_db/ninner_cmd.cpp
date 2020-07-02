#include <signal.h>
#include <iostream>
#include <boost/tokenizer.hpp>

#include "nlog_mt.h"
#include "nutil.h"

#include "ndb.h"
#include "ntime_regular.h"
#include "nrole_info.h"
#include "nrole_mgr.h"
#include "ncache_center.h"
#include "nstore_center.h"
#include "ntest_help.h"

#include "ninner_cmd.h"

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
	_register_command(_T("role_online"),		&ninner_cmd::_role_online,			_T("role_online"));
	_register_command(_T("role_offline"),		&ninner_cmd::_role_offline,			_T("role_offline"));

	_register_command(_T("role_batch_online"),	&ninner_cmd::_role_batch_online,	_T("role_batch_online"));
	_register_command(_T("role_batch_offline"),	&ninner_cmd::_role_batch_offline,	_T("role_batch_offline"));

	_register_command(_T("add_item"),			&ninner_cmd::_add_item,				_T("add_item"));
	_register_command(_T("add_equip"),			&ninner_cmd::_add_equip,			_T("add_equip"));
	_register_command(_T("add_skill"),			&ninner_cmd::_add_skill,			_T("role_skill"));

	_register_command(_T("create_data"),		&ninner_cmd::_create_data,			_T("create_data"));
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
	ncmd_queue& cmd_queue = ndb::instance().get_cmd_queue();

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

// 上线检测加载数据是否正确
void ninner_cmd::_role_online(const ncommand_param& param)
{
	uint64 role_id = 0;
	param.get_param(0, role_id);

	nrole_mgr& role_mgr = g_cache_center.get_role_mgr();

	if( !role_mgr.role_online(role_id, NULL) )
	{
		NLOG_ERROR(_T("%s, role_id:%l not exist in mysql database"), __TFUNCTION__, role_id);
		return;
	}

	nrole_info* role_ptr = role_mgr.get_role_info(role_id);
	if(!role_ptr) return;

	// 检测角色信息从数据库加载是否正确
	ntest_help::out_ncharacter_cache(role_ptr->get_character().get_data());
	ntest_help::out_role_item(role_ptr);
	ntest_help::out_role_equip(role_ptr);
	ntest_help::out_role_skill(role_ptr);

}

// 下线检测数据存储是否正确
void ninner_cmd::_role_offline(const ncommand_param& param)
{
	uint64 role_id = 0;
	param.get_param(0, role_id);

	nrole_mgr& role_mgr = g_cache_center.get_role_mgr();
	nrole_info* role_ptr = role_mgr.get_role_info(role_id);
	if(!role_ptr) return;

	// 先对相关的角色信息改变，然后存储，最后下线，整体进行检测
	ntest_help::change_role_item(role_ptr);
	ntest_help::change_role_equip(role_ptr);
	ntest_help::change_role_skill(role_ptr);

	if( !role_mgr.role_offline(role_id) )
	{
		NLOG_ERROR(_T("%s, role_id:%u role offline wrong"), __TFUNCTION__, role_id);
		return;
	}

}

// 巨量加载，检测加载速度
void ninner_cmd::_role_batch_online(const ncommand_param& param)
{
	uint32	num	= 0;
	param.get_param(0, num);

	if( !m_accountes.size() )
	{
		g_nexus_game_db.cache_character_role(m_accountes, m_roles);
		m_role_index = 0;
	}

	nrole_mgr& role_mgr = g_cache_center.get_role_mgr();

	uint32 cur_num	= (uint32)(role_mgr.get_role_map().size());
	uint32 cur_total= num + cur_num;
	if( cur_total >= m_accountes.size() )
	{
		NLOG_ERROR(_T("cur role num : ; arrive max"), __TFUNCTION__, cur_total);
	}


	AccountRoleMapIt it=m_accountes.begin();
	for(uint32 index=0; index<cur_num; ++index)
		++it;

	float elapse_time= get_time();
	for(uint32 index=0; index<num; ++index)
	{
		if( m_accountes.end()==it )break;

		uint64 role_id = (uint64)(*(it->second.begin()));
		if( !role_mgr.role_online(role_id, NULL) )
		{
			NLOG_ERROR(_T("%s, role_id:%l not exist in mysql database"), __TFUNCTION__, role_id);
		}
		++it;
	}
	elapse_time	= get_time()-elapse_time;
	NLOG_INFO(_T("num:%d ; time:%f"), num, elapse_time);

}

// 所有在线角色下线
void ninner_cmd::_role_batch_offline(const ncommand_param& param)
{
	nrole_mgr& role_mgr = g_cache_center.get_role_mgr();

	RoleInfoMap& roles = role_mgr.get_role_map();
	for(RoleInfoMapIt it=roles.begin(); roles.end()!=it; ++it)
	{
		it->second->set_status(ERSType_Offline);
		it->second->set_unload_store(true);
		it->second->get_character().set_cache_policy(EROCPolicyType_OffLine);
		it->second->get_items().set_cache_policy(EROCPolicyType_OffLine);
		it->second->get_equips().set_cache_policy(EROCPolicyType_OffLine);
		it->second->get_skilles().set_cache_policy(EROCPolicyType_OffLine);
	}

	float elapse_time= get_time();
	role_mgr.unload_cache();
	elapse_time	= get_time() - elapse_time;
	NLOG_INFO(_T("all role offline ; time:%f"), elapse_time);
}


void ninner_cmd::_add_item(const ncommand_param& param)
{
	uint64 role_id = 0;
	uint32 item_id = 0;
	param.get_param(0, role_id);
	param.get_param(1, item_id);

	nrole_mgr& role_mgr	= g_cache_center.get_role_mgr();
	nrole_info* role_ptr= role_mgr.get_role_info(role_id);
	if( !role_ptr ) return;

	nitem_cache* new_item = nitem_cache::alloc_item_data();
	new_item->item.guid = item_id;
	new_item->item.info_id = item_id;
	new_item->change = false;
	role_ptr->get_items().insert(new_item);
}

void ninner_cmd::_add_equip(const ncommand_param& param)
{
	uint64 role_id = 0;
	uint32 equip_id = 0;
	param.get_param(0, role_id);
	param.get_param(1, equip_id);

	nrole_mgr& role_mgr	= g_cache_center.get_role_mgr();
	nrole_info* role_ptr= role_mgr.get_role_info(role_id);
	if( !role_ptr ) return;

	nequip_cache* new_equip = nequip_cache::alloc_equip_data();
	new_equip->equip.guid	= equip_id;
	new_equip->equip.info_id= equip_id;
	new_equip->change = false;
	role_ptr->get_equips().insert(new_equip);
}

void ninner_cmd::_add_skill(const ncommand_param& param)
{
	uint64 role_id	= 0;
	uint32 skill_id	= 0;
	param.get_param(0, role_id);
	param.get_param(1, skill_id);

	nrole_mgr& role_mgr	= g_cache_center.get_role_mgr();
	nrole_info* role_ptr= role_mgr.get_role_info(role_id);
	if( !role_ptr ) return;

	nskill_cache* new_skill = nskill_cache::alloc_skill_data();
	new_skill->id = skill_id;
	new_skill->cd = skill_id<<1;
	new_skill->change = false;
	role_ptr->get_skilles().insert(new_skill);
}


// 生成巨量测试数据
void ninner_cmd::_create_data(const ncommand_param& param)
{
	int64 account_total = 0;
	param.get_param(0, account_total);
	if( account_total<1 ) return;

	uint32 account_id	= 0;

	uint32 role_total	= 5;		//每个账号下的角色数量
	uint64 role_id		= 0;

	uint32 account_base = g_nexus_game_db.get_account_max();

	for( uint32 account_index=1;  account_index<= account_total; ++account_index )
	{
		account_id = account_index + account_base;
		for( uint32 role_index=1; role_index<=role_total; 	++role_index)
		{
			role_id =  (account_id<<6) + role_index;
			if( !_create_role(account_id, role_id) )
				NLOG_ERROR(_T("%s, insert role is wrong"), __TFUNCTION__);
		}
		NLOG_INFO(_T("account index = %d"), account_id);
	}
}

bool ninner_cmd::_create_role(uint32 account_id, uint64 role_id)
{
	ncharacter_cache data;

	data.account_id = account_id;
	data.role_id	= role_id;

	tstring name = boost::lexical_cast<tstring>(role_id);
	wmemcpy(data.name, name.c_str(), name.size());

	data.sex		= s_util.urand(0, 1);
	data.race		= s_util.urand(1, 255);
	data.class_type	= s_util.urand(1, 6);
	data.level		= s_util.urand(1, 100);
	data.cur_exp	= s_util.urand(1, 2000000);
	data.hp			= s_util.urand(1, 10000);
	data.mp			= s_util.urand(1, 10000);

	data.map_id		= s_util.irand(1, 100);
	data.x			= (float)( s_util.irand(-1000000, 1000000) );
	data.y			= (float)( s_util.irand(-1000000, 1000000) );
	data.z			= (float)( s_util.irand(-1000000, 1000000) );
	data.orient		= (float)( s_util.irand(-1000000, 1000000) );

	data.create_time= s_util.urand(1000000, 2000000);
	data.login_time	= s_util.urand(1000000, 2000000);
	data.logout_time= s_util.urand(1000000, 2000000);
	data.online_time= s_util.urand(1000000, 2000000);

	if( !g_nexus_game_db.insert_character(&data) )
		return false;

	_create_item(role_id);
	_create_equip(role_id);
	_create_skill(role_id);
	return true;
}

void ninner_cmd::_create_item(uint64 role_id)
{
	uint32	num	= 300;			//物品总数
	for(uint32 cur_num=1; cur_num<=num; ++cur_num)
	{
		nitem_cache	data;
		data.item.guid		= (role_id<<30) + (cur_num<<10) + s_util.urand(1,512);
		data.item.info_id	= s_util.urand(1, 10000000);
		data.item.count		= s_util.urand(1, 32000);
		data.item.slot		= s_util.urand(1, 32000);
		data.item.bag		= s_util.urand(1, 255);
		data.item.flag		= s_util.urand(1, 255);

		if( !g_nexus_game_db.insert_item(role_id, &data) )
			NLOG_ERROR(_T("%s, insert item is wrong"), __TFUNCTION__);
	}
}

void ninner_cmd::_create_equip(uint64 role_id)
{
	//玩家装备GUID生成规则： 装备基数 + 角色号 + 装备顺序号
	uint32	num		= 200;			//装备总数
	uint32	index	= 1;			//装备顺序号
	for(uint32 cur_num=1; cur_num<=num; ++cur_num)
	{
		nequip_cache data;
		data.equip.guid			= (role_id << 30) + (cur_num<<10) + s_util.urand(1,512);
		data.equip.info_id		= s_util.urand(1, 20000000);
		data.equip.count		= s_util.urand(1, 32000);
		data.equip.slot			= s_util.urand(1, 32000);
		data.equip.bag			= s_util.urand(1, 255);
		data.equip.flag			= s_util.urand(1, 255);

		data.equip.quality		= s_util.urand(1, 255);
		data.equip.level		= s_util.urand(1, 20000000);
		data.equip.durability	= s_util.urand(1, 32000);
		data.equip.weight		= s_util.urand(1, 32000);
		data.equip.sharp		= s_util.urand(1, 255);
		data.equip.solid		= s_util.urand(1, 255);
		data.equip.tenacity		= s_util.urand(1, 255);

		for( uint32 index_type=1; index_type<8; ++index_type )
		{
			data.equip.stat_type[index_type] = s_util.urand(1, 255);
		}
		for( uint32 index_state=1; index_state<8; ++index_state )
		{
			data.equip.stat_value[index_state] = s_util.urand(1, 32000);
		}
		for( uint32 index_gem=1; index_gem<6; ++index_gem )
		{
			data.equip.gem[index_gem] = s_util.urand(1, 32000);
		}
		for( uint32 index_spell=1; index_spell<4; ++index_spell )
		{
			data.equip.spell[index_spell] = s_util.urand(1, 32000);
		}
		for( uint32 index_rate=1; index_rate<3; ++index_rate )
		{
			data.equip.spell_rate[index_rate] = s_util.urand(1, 32000);
		}

		data.equip.enchantment	= s_util.urand(1, 32000);
		data.equip.creator		= role_id;

		if( !g_nexus_game_db.insert_equip(role_id, &data) )
			NLOG_ERROR(_T("%s, insert equip is wrong"), __TFUNCTION__);

	}
}

void ninner_cmd::_create_skill(uint64 role_id)
{
	uint32	num	= 500;	//技能数量
	for(uint32 index=0; index<=num; ++index)
	{
		nskill_cache data;
		data.cd	= s_util.urand(1, 32000);
		data.id = (index<<10) + s_util.urand(1, 512);
		if( !g_nexus_game_db.insert_skill(role_id, &data) )
			NLOG_ERROR(_T("%s, insert skill is wrong"), __TFUNCTION__);
	}
}

}
