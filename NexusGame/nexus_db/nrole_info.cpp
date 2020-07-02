
#include "nlog_mt.h"

#include "nmsg_cluster.h"
#include "nmsg_world_login.h"
#include "nmsg_world_db.h"
#include "nmsg_login.h"
#include "nlan_session.h"

#include "nstore_center.h"
#include "nrole_info.h"

namespace nexus{

const uint32 OFFLINE_CACHE_TIME	= 5 * 60 * 1000;	//下线缓存时间(暂时为5分钟)




nrole_info* nrole_info::alloc_role_info(void)
{
	return g_pool.construct<nrole_info>();
}

void nrole_info::free_role_info(nrole_info* info_ptr)
{
	if(info_ptr) g_pool.destroy<nrole_info>(info_ptr);
}


nrole_info::nrole_info(void)
{
	m_status			= ERSType_None;
	m_offline_cache_time= OFFLINE_CACHE_TIME;
}

nrole_info::~nrole_info(void)
{
}

void nrole_info::offline_update(uint32 elapse)
{
	m_offline_cache_time -= elapse;
	if( m_offline_cache_time<0 )
		set_status(ERSType_Unloadcache);
}

bool nrole_info::cache_from_db(uint64 role_id)
{
	set_owner_id(role_id);

	set_status(ERSType_Online);
	if( !m_character.cache_from_db(m_owner_id) )
		return false;

	m_item.cache_from_db(m_owner_id);
	m_equip.cache_from_db(m_owner_id);
	m_skill.cache_from_db(m_owner_id);
	return true;
}

void nrole_info::unload_cache(void)
{
	m_character.unload_cache();
	m_item.unload_cache();
	m_equip.unload_cache();
	m_skill.unload_cache();
}

void nrole_info::destroy(void)
{
	m_character.destroy();
}

void nrole_info::set_status(ERoleStatus_Type role_status)
{
	m_status = role_status;
	if( is_status(ERSType_Online) )
		m_offline_cache_time = 0;
}

void nrole_info::set_unload_store(bool unload_store)
{
	nrole_obj_base::set_unload_store(unload_store);
	m_item.set_unload_store(unload_store);
	m_equip.set_unload_store(unload_store);
	m_skill.set_unload_store(unload_store);
}


// 角色加载发送各种角色信息
void nrole_info::send_load_msg(void)
{
	if(!m_session) return;

	//发送角色基本信息
	ncharacter_cache* cache_ptr = m_character.get_data();
	tagMsg_D2W_LoadPlayerRoleInfo role_msg;
	role_msg.account_id	= cache_ptr->account_id;
	role_msg.role_id	= cache_ptr->role_id;
	memcpy(role_msg.name, cache_ptr->name, sizeof(cache_ptr->name));
	role_msg.sex		= cache_ptr->sex;
	role_msg.race		= cache_ptr->race;
	role_msg.class_type	= cache_ptr->class_type;
	role_msg.map_id		= cache_ptr->map_id;
	role_msg.last_time	= cache_ptr->logout_time;
	m_session->send_message( &role_msg, sizeof(role_msg) );

	//发送技能信息
	tagMsg_D2W_LoadPlayerSkill skill_msg;
	SkillMap& skilles	= get_skilles().get_skilles();
	skill_msg.role_id	= m_character.get_data()->role_id;
	skill_msg.num		= 0;
	for( SkillMapIt it=skilles.begin(); skilles.end()!=it; ++it )
	{
		skill_msg.skilles[skill_msg.num++] = it->first;
	}
	m_session->send_message( &skill_msg, sizeof(skill_msg) );

	//todo : 发送物品信息
	//todo : 发送装备信息
	//todo : 发送其他相关信息
}


void nrole_info::store(void)
{
	if( m_character.cache_policy(EROCPolicyType_Update) )
		m_character.store();

	if( m_item.cache_policy(EROCPolicyType_Update))
		m_item.store();

	if( m_equip.cache_policy(EROCPolicyType_Update))
		m_equip.store();

	if( m_skill.cache_policy(EROCPolicyType_Update))
		m_skill.store();
}

} //namespace nexus