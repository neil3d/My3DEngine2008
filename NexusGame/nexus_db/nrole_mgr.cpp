
#include "nlog_mt.h"

#include "nstore_center.h"
#include "nrole_mgr.h"


namespace nexus{

const uint32	MAX_LOAD_NUM_EVERY_TICK	=	10;	//每一个tick最多load角色的数量

nrole_mgr::nrole_mgr(void)
{
}


nrole_mgr::~nrole_mgr(void)
{
}

void nrole_mgr::update(uint32 elapse)
{
	ntime_regular::update(elapse);

	for(RoleInfoMapIt it = m_role_infoes.begin(); m_role_infoes.end()!=it; ++it)
	{
		if( it->second->is_status(ERSType_Offline) )
			it->second->offline_update(elapse);
	}

	for(RoleInfoMapIt it = m_role_infoes.begin(); m_role_infoes.end()!=it; )
	{
		if( it->second->is_status(ERSType_Unloadcache) )
		{
			it->second->unload_cache();
			nrole_info::free_role_info(it->second);
			m_role_infoes.erase(it);
			it = m_role_infoes.begin();
			continue;
		}
		++it;
	}

	uint32 cur_load_num = MAX_LOAD_NUM_EVERY_TICK;
	if( ((uint32)m_load_role_list.size()) < cur_load_num )
		cur_load_num = (uint32)m_load_role_list.size();


	for( uint32 index=0; index<cur_load_num; ++index )
	{
		uint64& role_id = m_load_role_list.front();

		// to do : 发送角色登陆相关的信息
		RoleInfoMapIt it = m_role_infoes.find(role_id);
		if( m_role_infoes.end() != it )
			it->second->send_load_msg();

		m_load_role_list.pop_front();
	}
}

void nrole_mgr::unload_cache(void)
{
	for( RoleInfoMapIt it=m_role_infoes.begin(); m_role_infoes.end()!=it; ++it )
	{
		it->second->unload_cache();
		nrole_info::free_role_info(it->second);
	}

	m_role_infoes.clear();
}

bool nrole_mgr::role_online(uint64 role_id, nlan_session* session)
{
	RoleInfoMapIt it = m_role_infoes.find(role_id);
	if( m_role_infoes.end() != it )
	{
		it->second->set_status(ERSType_Online);
		return true;
	}

	nrole_info* info_ptr = nrole_info::alloc_role_info();
	if( !info_ptr )
	{
		NLOG_ERROR(_T("%s, can`t add new role!"), __TFUNCTION__);
		return false;
	}

	if( !info_ptr->cache_from_db(role_id) )
	{
		nrole_info::free_role_info(info_ptr);
		return false;
	}

	m_load_role_list.push_back(role_id);
	info_ptr->set_lan_session(session);
	m_role_infoes.insert( std::make_pair(role_id, info_ptr) );

	return true;
}

// 角色下线移除游戏数据
bool nrole_mgr::role_offline(uint64 role_id)
{
	if( m_role_infoes.end() == m_role_infoes.find(role_id) )
		return false;

	RoleInfoMapIt it = m_role_infoes.find(role_id);
	if( m_role_infoes.end() == it) return false;

	it->second->store();
	it->second->set_status(ERSType_Offline);
	it->second->set_lan_session(NULL);

	//it->second->unload_cache();
	//nrole_info::free_role_info(it->second);
	//m_role_infoes.erase(it);
	return true;
}

// 创建角色
void nrole_mgr::create_role(ncharacter_cache* data_ptr)
{
	nrole_info* info_ptr = nrole_info::alloc_role_info();
	if( !info_ptr )
	{
		NLOG_ERROR(_T("%s, can`t add new role!"), __TFUNCTION__);
		return;
	}

	m_role_infoes.insert( std::make_pair(data_ptr->role_id, info_ptr) );
	info_ptr->get_character().insert(data_ptr);
	//info_ptr->set_status(ERSType_Online);
	//to do : 根据ncharacter_cache的信息，创建角色需要的初始物品、装备、技能。。。
}


// 删除角色
void nrole_mgr::delete_role(uint64 role_id)
{
	RoleInfoMapIt it = m_role_infoes.find(role_id);
	if( m_role_infoes.end() == it) return;

	it->second->unload_cache();
	it->second->destroy();

	nrole_info::free_role_info(it->second);
	m_role_infoes.erase(it);
}

nrole_info* nrole_mgr::get_role_info(uint64 role_id)
{
	RoleInfoMapIt it = m_role_infoes.find(role_id);
	if(m_role_infoes.end() != it)
	{
		return it->second;
	}
	return NULL;
}

// 检测角色是否在线
bool nrole_mgr::is_online(uint64 role_id)
{
	return ( m_role_infoes.end() != m_role_infoes.find(role_id) );
}

void nrole_mgr::store(void)
{
	uint32 loop_num = 0;

	start_slice();

	for(RoleInfoMapIt it = m_role_infoes.begin(); m_role_infoes.end()!=it; ++it)
	{
		if( UNVALID_POINT(it->second) ) continue;

		if( it->second->loop() )
		{//已经轮询过
			++loop_num;
			continue;
		}

		it->second->store();
		it->second->loop(true);

		if( end_slice() ) return;
	}

	if( loop_num == m_role_infoes.size() )
	{//轮询完毕，执行新的轮询
		for(RoleInfoMapIt it = m_role_infoes.begin(); m_role_infoes.end()!=it; ++it)
		{
			it->second->loop(false);
		}
	}
}

ncharacter_cache* nrole_mgr::get_character_cache(uint64 role_id)
{
	RoleInfoMapIt it = m_role_infoes.find(role_id);
	if( m_role_infoes.end() == it )
		return NULL;

	return ( it->second->get_character().get_data() );
}


} //namespace nexus