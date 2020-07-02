
#include "nlog_mt.h"

#include "nstore_center.h"
#include "nskill.h"


namespace nexus{


nskill::nskill(void)
{
}


nskill::~nskill(void)
{
}

void nskill::cache_from_db(uint64 owner_id)
{
	set_owner_id(owner_id);
	g_nexus_game_db.cache_skill(m_owner_id, m_skilles);
}

void nskill::unload_cache(void)
{
#ifdef	NSKILL_UNLOAD_CACHE
	float elapse_time = get_time();
#endif

	if( cache_policy(EROCPolicyType_OffLine)  && get_unload_store() )
		store();

#ifdef	NSKILL_UNLOAD_CACHE
	elapse_time = get_time() - elapse_time;
	NLOG_INFO(_T("%s, elapse time is %f"), __TFUNCTION__, elapse_time);
#endif

	for( SkillMapIt it=m_skilles.begin(); m_skilles.end()!=it; ++it )
	{
		nskill_cache::free_skill_data(it->second);
	}
	m_skilles.clear();
}

nskill_cache* nskill::get_data(uint32 skill_id)
{
	SkillMapIt it = m_skilles.find(skill_id);
	if( m_skilles.end() != it )
	{
		return it->second;
	}
	return NULL;
}

void nskill::insert(nskill_cache* data_ptr)
{
	if( UNVALID_POINT(data_ptr) ) return;

	m_skilles.insert(std::make_pair(data_ptr->id, data_ptr));
	g_nexus_game_db.insert_skill(m_owner_id, data_ptr);
}

void nskill::destroy(uint32 skill_id)
{
	SkillMapIt it = m_skilles.find(skill_id);
	if(m_skilles.end() == it) return;

	g_nexus_game_db.delete_skill(m_owner_id, it->second->id);
	nskill_cache::free_skill_data(it->second);
	m_skilles.erase(it);	
}

void nskill::store(void)
{
	for( SkillMapIt it = m_skilles.begin(); m_skilles.end()!=it; ++it )
	{
		nskill_cache* data_ptr = it->second;

		if( UNVALID_POINT(data_ptr) ) continue;

#ifndef ENFORCE_SAVE_INHERIT_FROM_ROLE_OBJ
		if( !data_ptr->change ) continue;
#endif

		g_nexus_game_db.update_skill(m_owner_id, data_ptr);

		data_ptr->change = false;
	}
}

} //namespace nexus