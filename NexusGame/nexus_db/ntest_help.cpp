#include <stdio.h>
#include <limits>
#include <iostream>

#include "nlog_mt.h"
#include "ntest_help.h"

namespace nexus{

void ntest_help::out_nchar(nchar* info)
{
	char* cur_locale = ::setlocale(LC_ALL, "chs");
	::wprintf(L"%s\n", info);
	::setlocale(LC_ALL, cur_locale);
}

void ntest_help::out_ncharacter_cache(ncharacter_cache* data_ptr)
{
	if(!data_ptr) return;

	NLOG_INFO(_T("role_id = %d, account_id = %d\n"), data_ptr->role_id, data_ptr->account_id);
	out_nchar(data_ptr->name);
}

void ntest_help::out_nitem_cache(nitem_cache* data_ptr)
{
	if(!data_ptr) return;

	NLOG_INFO(_T("guid = %d, info_id = %d, count = %d\n"), data_ptr->item.guid, data_ptr->item.info_id, data_ptr->item.count);
}

void ntest_help::out_nequip_cache(nequip_cache* data_ptr)
{
	if(!data_ptr) return;

	NLOG_INFO(_T("guid = %d, info_id = %d, count = %d\n"), data_ptr->equip.guid, data_ptr->equip.info_id, data_ptr->equip.count);
	NLOG_INFO(_T("enchantment = %d, creator = %d"), data_ptr->equip.enchantment, data_ptr->equip.creator);
}

void ntest_help::out_nskill_cache(nskill_cache* data_ptr)
{
	if(!data_ptr) return;

	NLOG_INFO(_T("id = %d, cd = %d"), data_ptr->id, data_ptr->cd);
}

//输出角色的物品信息
void ntest_help::out_role_item(nrole_info* role_ptr)
{
	if(!role_ptr) return;

	NLOG_INFO(_T("the info of item: "));
	ItemMap& items = role_ptr->get_items().get_items();
	for( ItemMapIt it=items.begin(); items.end()!=it; ++it )
	{
		out_nitem_cache(it->second);
	}
}

//输出角色的装备信息
void ntest_help::out_role_equip(nrole_info* role_ptr)
{
	if(!role_ptr) return;

	NLOG_INFO(_T("the info of equip: "));
	EquipMap& equips = role_ptr->get_equips().get_equips();
	for( EquipMapIt it=equips.begin(); equips.end()!=it; ++it )
	{
		out_nequip_cache(it->second);
	}
}

//输出角色的技能信息
void ntest_help::out_role_skill(nrole_info* role_ptr)
{
	if(!role_ptr) return;

	NLOG_INFO(_T("the info of skill: "));
	SkillMap& skilles = role_ptr->get_skilles().get_skilles();
	for( SkillMapIt it=skilles.begin(); skilles.end()!=it; ++it )
	{
		out_nskill_cache(it->second);
	}
}

/**
 *	修改角色的指定物品信息
 *	@remark : 增加物品数量(增加2个)，检测物品数据的变化
 */
void ntest_help::change_role_item(nrole_info* role_ptr)
{
	if(!role_ptr) return;
	ItemMap& items = role_ptr->get_items().get_items();
	for( ItemMapIt it=items.begin(); items.end()!=it; ++it )
	{
		it->second->item.count += 2;
		it->second->change = true;
	}
	role_ptr->get_items().store();
}

/**
 *	修改角色的指定装备信息 
 *	@remark : 增加装备数量(增加2个)，检测装备数据的变化
 */
//修改角色的装备信息
void ntest_help::change_role_equip(nrole_info* role_ptr)
{
	if(!role_ptr) return;

	EquipMap& equips = role_ptr->get_equips().get_equips();
	for( EquipMapIt it=equips.begin(); equips.end()!=it; ++it )
	{
		it->second->equip.count += 2;
		it->second->change = true;
	}
	role_ptr->get_equips().store();
}

/**
 *	修改角色的指定技能信息
 *	@remark : 增加技能CD(增加2个单位时间)，检测技能数据的变化
 */
void ntest_help::change_role_skill(nrole_info* role_ptr)
{
	if(!role_ptr) return;

	SkillMap& skilles = role_ptr->get_skilles().get_skilles();
	for( SkillMapIt it=skilles.begin(); skilles.end()!=it; ++it )
	{
		it->second->cd += 2;
		it->second->change = true;
	}
	role_ptr->get_skilles().store();
}

}