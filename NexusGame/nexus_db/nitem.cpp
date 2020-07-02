
#include "nlog_mt.h"

#include "nstore_center.h"
#include "nitem.h"


namespace nexus{


nitem::nitem(void)
{
}


nitem::~nitem(void)
{
}

void nitem::cache_from_db(uint64 owner_id)
{
	set_owner_id(owner_id);
	g_nexus_game_db.cache_item(m_owner_id, m_items);
}

void nitem::unload_cache(void)
{
#ifdef	NITEM_UNLOAD_CACHE
	float elapse_time = get_time();
#endif

	if( cache_policy(EROCPolicyType_OffLine) && get_unload_store() )
		store();

#ifdef	NITEM_UNLOAD_CACHE
	elapse_time = get_time()-elapse_time;
	NLOG_INFO(_T("%s, elapse time is %f"), __TFUNCTION__, elapse_time);
#endif


	for( ItemMapIt it=m_items.begin(); m_items.end()!=it; ++it )
	{
		nitem_cache::free_item_data(it->second);
	}
	m_items.clear();
}

nitem_cache* nitem::get_data(uint64 guid)
{
	ItemMapIt it = m_items.find(guid);
	if( m_items.end() != it )
	{
		return it->second;
	}

	return NULL;
}

void nitem::insert(nitem_cache* data_ptr)
{
	if( UNVALID_POINT(data_ptr) ) return;

	m_items.insert(std::make_pair(data_ptr->item.guid, data_ptr));

	g_nexus_game_db.insert_item(m_owner_id, data_ptr);
}

void nitem::destroy(uint64 guid)
{
	ItemMapIt it = m_items.find(guid);
	if(m_items.end() == it) return;

	g_nexus_game_db.delete_item(guid);
	nitem_cache::free_item_data(it->second);
	m_items.erase(guid);
}

void nitem::store(void)
{
	for( ItemMapIt it = m_items.begin(); m_items.end()!=it; ++it )
	{
		nitem_cache* data_ptr = it->second;

		if( UNVALID_POINT(data_ptr) ) continue;

#ifndef ENFORCE_SAVE_INHERIT_FROM_ROLE_OBJ
		if( !data_ptr->change ) continue;
#endif

		g_nexus_game_db.update_item(m_owner_id, data_ptr);

		data_ptr->change = false;
	}
}

} //namespace nexus