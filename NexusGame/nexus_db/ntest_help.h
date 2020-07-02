/**
 *	nexus db - ntest_help
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: LeoChen
 *	Date:	June, 2010
 */

#ifndef _NDB_NTEST_HELP_H_
#define _NDB_NTEST_HELP_H_

#include "nstore_center.h"
#include "ncache_center.h"

namespace nexus{

	class ntest_help
	{
	public:
		//输出指定角色信息
		static void out_nchar(nchar* info);

		//输出角色账号缓存信息
		static void out_ncharacter_cache(ncharacter_cache* data_ptr);

		//输出指定物品信息
		static void out_nitem_cache(nitem_cache* data_ptr);

		//输出指定装备信息
		static void out_nequip_cache(nequip_cache* data_ptr);

		//输出指定技能信息
		static void out_nskill_cache(nskill_cache* data_ptr);

		//输出角色的所有物品信息
		static void out_role_item(nrole_info* role_ptr);

		//输出角色的所有装备信息
		static void out_role_equip(nrole_info* role_ptr);

		//输出角色的所有技能信息
		static void out_role_skill(nrole_info* role_ptr);

		/**
		 *	修改角色的指定物品信息
		 *	@remark : 增加物品数量(增加2个)，检测物品数据的变化
		 */
		static void change_role_item(nrole_info* role_ptr);

		/**
		 *	修改角色的指定装备信息 
		 *	@remark : 增加装备数量(增加2个)，检测装备数据的变化
		 */
		static void change_role_equip(nrole_info* role_ptr);

		/**
		 *	修改角色的指定技能信息
		 *	@remark : 增加技能CD(增加2个单位时间)，检测技能数据的变化
		 */
		static void change_role_skill(nrole_info* role_ptr);
	};
}

#endif _NDB_NTEST_HELP_H_