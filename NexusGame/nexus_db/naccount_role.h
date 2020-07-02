/**
 *	nexus db - naccount_role
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	June, 2010
 */

#ifndef _NDB_NCHARACTER_ROLE_H_
#define _NDB_NCHARACTER_ROLE_H_


#include "ncache_utility.h"

namespace nexus{

	/**
	 *	naccount_role
	 *	只保存账号和账号所拥有的所有角色的ID
	 */
	class naccount_role
	{
	public:
		naccount_role(void);
		~naccount_role(void);

		void cache_from_db(void);

		RoleSet* get_role_ids(uint32 account_id);

		void insert_role(ncharacter_cache* data_ptr);
		void destroy_role(uint32 role_id);

		Uint32Map& get_role_account(void){ return m_role_account; }
	private:
		AccountRoleMap	m_account_role;
		Uint32Map		m_role_account;
	};




} // namespace nexus

#endif	_NDB_NCHARACTER_ROLE_H_