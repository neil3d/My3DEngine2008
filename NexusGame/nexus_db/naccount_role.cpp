
#include "nlog_mt.h"

#include "nstore_center.h"
#include "naccount_role.h"


namespace nexus{

naccount_role::naccount_role(void)
{
}


naccount_role::~naccount_role(void)
{
	m_account_role.clear();
	m_role_account.clear();
}

void naccount_role::cache_from_db(void)
{
	g_nexus_game_db.cache_character_role(m_account_role, m_role_account);
}

RoleSet* naccount_role::get_role_ids(uint32 account_id)
{
	AccountRoleMapIt it = m_account_role.find(account_id);
	if( m_account_role.end() != it )
	{
		return &(it->second);
	}

	//todo: 如果没有当前账号，需要在往数据库执行查询，如果没有才确认没有


	return NULL;
}

void naccount_role::insert_role(ncharacter_cache* data_ptr)
{
	if( UNVALID_POINT(data_ptr) ) return;

	AccountRoleMapIt it = m_account_role.find(data_ptr->account_id);
	if( m_account_role.end() != it )
	{
		it->second.insert(data_ptr->role_id);
	}
	else
	{
		m_account_role[data_ptr->account_id].insert(data_ptr->role_id);
	}
	m_role_account.insert( std::make_pair(data_ptr->role_id, data_ptr->account_id) );
}

void naccount_role::destroy_role(uint32 role_id)
{
	Uint32MapIt	role_it = m_role_account.find(role_id);
	if(m_role_account.end() == role_it) return;

	uint32 account_id = role_it->second;
	AccountRoleMapIt account_it = m_account_role.find(account_id);
	if( m_account_role.end() == account_it ) return;

	account_it->second.erase(role_id);
	m_role_account.erase(role_it);
	
}

} //namespace nexus