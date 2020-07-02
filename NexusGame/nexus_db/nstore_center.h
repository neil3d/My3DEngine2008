/**
 *	nexus db - nstore_center
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	June, 2010
 */

#ifndef _NDB_NSTORE_CENTER_H_
#define _NDB_NSTORE_CENTER_H_

#include "ncache_utility.h"

namespace nexus{

	class ndb_mysql;

	/**
	 *	nstore_center
	 */
	class nstore_center
	{
	public:
		nstore_center(void);
		~nstore_center(void);

		bool init(std::string host, std::string port, std::string name);
		void destroy(void);

		void cache_character_role(AccountRoleMap& account_role, Uint32Map& role_account);

		bool cache_character(ncharacter_cache*& info, uint64 role_id);
		void update_character(ncharacter_cache* data_ptr);
		bool insert_character(ncharacter_cache* data_ptr);
		void delete_character(uint64 role_id);

		void cache_item(uint64 owner_id, ItemMap& itemes);
		void update_item(uint64 owner_id, nitem_cache* data_ptr);
		bool insert_item(uint64 owner_id, nitem_cache* data_ptr);
		void delete_item(uint64 guid);

		void cache_equip(uint64 owner_id, EquipMap& equips);
		void update_equip(uint64 owner_id, nequip_cache* data_ptr);
		bool insert_equip(uint64 owner_id, nequip_cache* data_ptr);
		void delete_equip(uint64 guid);

		void cache_skill(uint64 role_id, SkillMap& skills);
		void update_skill(uint64 role_id,nskill_cache* data_ptr);
		bool insert_skill(uint64 role_id,nskill_cache* data_ptr);
		void delete_skill(uint64 role_id, uint32 skill_id);

		/**
		 *	@return : 返回角色表中最大的角色ID
		 *	@remark : 返回0表示表示无效
		 */
		uint32 get_role_max(void);

		/**
		 *	@return : 返回最大的账号ID
		 *	@remark : 返回0表示表示无效
		 */
		uint32 get_account_max(void);

	private:
		ndb_mysql*	m_game_db;
	};

	extern nstore_center g_nexus_game_db;
} // namespace nexus

#endif	_NDB_NSTORE_CENTER_H_