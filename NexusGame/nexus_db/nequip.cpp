
#include "nlog_mt.h"

#include "nstore_center.h"
#include "nequip.h"


namespace nexus{



nequip::nequip(void)
{
}


nequip::~nequip(void)
{
}

void nequip::cache_from_db(uint64 owner_id)
{
	set_owner_id(owner_id);
	g_nexus_game_db.cache_equip(m_owner_id, m_equips);
}

void nequip::unload_cache(void)
{
#ifdef	NEQUIP_UNLOAD_CACHE
	float elapse_time = get_time();
#endif

	if( cache_policy(EROCPolicyType_OffLine)  && get_unload_store() )
		store();

#ifdef	NEQUIP_UNLOAD_CACHE
	elapse_time = get_time() - elapse_time;
	NLOG_INFO(_T("%s, elapse time is %f"), __TFUNCTION__, elapse_time);
#endif

	for( EquipMapIt it=m_equips.begin(); m_equips.end()!=it; ++it )
	{
		nequip_cache::free_equip_data(it->second);
	}
	m_equips.clear();
}

nequip_cache* nequip::get_data(uint64 guid)
{
	EquipMapIt it = m_equips.find(guid);
	if( m_equips.end() != it )
	{
		return it->second;
	}

	return NULL;
}

void nequip::insert(nequip_cache* data_ptr)
{
	if( UNVALID_POINT(data_ptr) ) return;

	m_equips.insert(std::make_pair(data_ptr->equip.guid, data_ptr));

	g_nexus_game_db.insert_equip(m_owner_id, data_ptr);
}

void nequip::destroy(uint64 guid)
{
	EquipMapIt it = m_equips.find(guid);
	if(m_equips.end() == it) return;

	nequip_cache::free_equip_data(it->second);
	m_equips.erase(guid);
	g_nexus_game_db.delete_equip(guid);
}

void nequip::store(void)
{
	for( EquipMapIt it = m_equips.begin(); m_equips.end()!=it; ++it )
	{
		nequip_cache* data_ptr = it->second;
		if( UNVALID_POINT(data_ptr) ) continue;

#ifndef ENFORCE_SAVE_INHERIT_FROM_ROLE_OBJ
		if( !data_ptr->change ) continue;
#endif

		g_nexus_game_db.update_equip(m_owner_id, data_ptr);

		data_ptr->change = false;
	}
}


} //namespace nexus