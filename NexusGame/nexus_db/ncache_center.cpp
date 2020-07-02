
#include "ncache_center.h"



namespace nexus{

ncache_center g_cache_center;


ncache_center::ncache_center(void)
{
}


ncache_center::~ncache_center(void)
{
}


// 启动加载所有数据库中数据
void ncache_center::start(void)
{
}


// 刷新时间
void ncache_center::update(uint32 elapse)
{
	::timeBeginPeriod(1);

	m_role_mgr.update(elapse);

	::timeEndPeriod(1);
}


// 关闭时将内存中的数据都存储到数据库
void ncache_center::destroy(void)
{
	m_role_mgr.shut_dowm();
	m_role_mgr.unload_cache();
}


// 增加character
void ncache_center::insert_character(ncharacter_cache* data_ptr)
{
	if( UNVALID_POINT(data_ptr) ) return;

	m_role_mgr.create_role(data_ptr);
}

// 删除character
void ncache_center::destroy_character(uint64 role_id)
{
	m_role_mgr.delete_role(role_id);
}


} //namespace nexus