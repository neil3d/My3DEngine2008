
#include "nlog_mt.h"

#include "nstore_center.h"
#include "ncharacter.h"


namespace nexus{

ncharacter::ncharacter(void)
{
	m_info = NULL;
}


ncharacter::~ncharacter(void)
{
}

bool ncharacter::cache_from_db(uint64 owner_id)
{
	set_owner_id(owner_id);
	return g_nexus_game_db.cache_character(m_info, m_owner_id);
}

void ncharacter::unload_cache(void)
{
	if( cache_policy(EROCPolicyType_OffLine)  && get_unload_store() )
		store();

	ncharacter_cache::free_character_data(m_info);

}

ncharacter_cache* ncharacter::get_data(void)
{
	return m_info;
}

void ncharacter::insert(ncharacter_cache* data_ptr)
{
	if( UNVALID_POINT(data_ptr) ) return;
	m_info = data_ptr;
}

void ncharacter::destroy(void)
{
	ncharacter_cache::free_character_data(m_info);
	g_nexus_game_db.delete_character(m_owner_id);
}


void ncharacter::store(void)
{
#ifndef	ENFORCE_SAVE_INHERIT_FROM_ROLE_OBJ
	if( !m_info->change ) return;
#endif

	g_nexus_game_db.update_character(m_info);
}

} //namespace nexus