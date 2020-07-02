
#include "nlog_mt.h"

#include "ncache_utility.h"

namespace nexus{


	//非线程安全的内存池
	nobject_pool<nnull_mutex>  g_pool(500 * 1024 * 1024);	//池的最大容量500M

	////线程安全的内存池
	//nobject_pool<nfast_mutex>  g_pool(500 * 1024 * 1024);	//池的最大容量500M


	// 分配一个 ncharacter_cache 对象
	ncharacter_cache* ncharacter_cache::alloc_character_data(void)
	{
		return g_pool.construct<ncharacter_cache>();
	}
	// 回收一个 ncharacter_cache 对象
	void ncharacter_cache::free_character_data(ncharacter_cache* character_cache_ptr)
	{
		if(character_cache_ptr) g_pool.destroy<ncharacter_cache>(character_cache_ptr);
	}



	// 分配一个 nitem_cache 对象
	nitem_cache* nitem_cache::alloc_item_data(void)
	{
		return g_pool.construct<nitem_cache>();
	}
	// 回收一个 nitem_cache 对象
	void nitem_cache::free_item_data(nitem_cache* item_cache_ptr)
	{
		if(item_cache_ptr) g_pool.destroy<nitem_cache>(item_cache_ptr);
	}



	// 分配一个 nequip_cache 对象
	nequip_cache* nequip_cache::alloc_equip_data(void)
	{
		return g_pool.construct<nequip_cache>();
	}
	// 回收一个 nequip_cache 对象
	void nequip_cache::free_equip_data(nequip_cache* equip_cache_ptr)
	{
		if(equip_cache_ptr) g_pool.destroy<nequip_cache>(equip_cache_ptr);
	}



	// 分配一个 nskill_cache 对象
	nskill_cache* nskill_cache::alloc_skill_data(void)
	{
		return g_pool.construct<nskill_cache>();
	}
	// 回收一个 nskill_cache 对象
	void nskill_cache::free_skill_data(nskill_cache* skill_cache_ptr)
	{
		if(skill_cache_ptr) g_pool.destroy<nskill_cache>(skill_cache_ptr);
	}

}