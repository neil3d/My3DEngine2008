/**
 *	nexus network - nsafe_mem_pool 线程安全内存池
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: Y & D
 *	Date:	Feb, 2010
 */

#ifndef _NNETWORK_NSAFE_MEM_POOL_H_
#define _NNETWORK_NSAFE_MEM_POOL_H_

#include "ncommon.h"
#include "nmutex.h"
#include "nguard.h"

namespace nexus {

	/**
	 *	内存池(注意：内存池的大小表示:内存池当前空闲内存的大小)
	 *	初始化时：
	 *	1. 外部设定内存池的最大允许大小
	 *	2. 内存池不做任何内存预分配
	 *
	 *	外部申请内存时,依次判断：
	 *	1. 如果内存池有对应尺寸的空闲块,使用空闲块返回
	 *	2. 没有空闲块时,直接申请内存并在内存前加入tagMemItem,然后返回
	 *
	 *	外部释放内存时,依次判断:
	 *	1. 如果释放内存块大小>内存池最大大小,直接释放到系统内存
	 *	2. 如果当前内存池大小+释放内存块大小<内存池最大大小,直接放入内存池
	 *	3. 进行垃圾收集：垃圾收集后重新检查上面的第2步，如果不通过，则进行第4步
	 *	4. 否则直接释放入系统内存
	 */
	template<typename TMUTEX>
	class nmem_pool : private nnoncopyable
	{
	public:
		explicit nmem_pool(uint32 max_pool_size = 16 * 1024 * 1024);
		~nmem_pool();

		NINLINE void*	alloc(uint32 bytes);
		NINLINE void	free(void* const mem_ptr);
		NINLINE void*	try_alloc(uint32 bytes);					// 尝试进入锁定区域
		NINLINE bool	try_free(void* const mem_ptr);				// 尝试进入锁定区域
		NINLINE void	set_max_size(uint32 size) { m_max_size = size; }
		NINLINE uint32	get_size()		const { return m_current_size; }
		NINLINE uint32	get_malloc()	const { return m_malloc; }
		NINLINE uint32	get_gc()		const { return m_gc_times; }

	private:
		// 垃圾收集
		NINLINE void _garbage_collect(uint32 expect_size, uint32 use_time);
		// 返回最匹配的大小
		NINLINE int32 _size2index(uint32 size, uint32& real_size);

	private:
		// 内存块头描述
		struct nnode
		{
			nnode*			next_ptr;
			nnode*			pre_ptr;
			int32			index;
			uint32			size;
			uint32			use_time;
			uint32			mem_ptr[1];							// 实际内存空间
		};

		struct
		{
			nnode*			first_ptr;
			nnode*			last_ptr;
		}					m_pool[16];

		uint32				m_max_size;						// 外部设定的最大允许空闲内存
		uint32				m_malloc;						// 统计用，实际Malloc次数
		uint32				m_gc_times;						// 统计用，实际垃圾收集次数

		uint32 volatile 	m_current_size;					// 内存池中空闲内存总数

		TMUTEX				m_mutex;
	};

	typedef nmem_pool<nfast_mutex> nsafe_mem_pool;

	//-----------------------------------------------------------------------------
	// construction
	//-----------------------------------------------------------------------------
	template<typename TMUTEX>
	nmem_pool<TMUTEX>::nmem_pool(uint32 max_pool_size) : m_max_size(max_pool_size), m_malloc(0), m_gc_times(0), m_current_size(0)
	{
		ZeroMemory(m_pool, sizeof(m_pool));
	}

	//-----------------------------------------------------------------------------
	// destruction
	//-----------------------------------------------------------------------------
	template<typename TMUTEX>
	nmem_pool<TMUTEX>::~nmem_pool()
	{
		for (int32 i = 0; i < 16; ++i)
		{
			while (m_pool[i].first_ptr)
			{
				nnode* node_ptr = m_pool[i].first_ptr;
				m_pool[i].first_ptr = m_pool[i].first_ptr->next_ptr;
				::free(node_ptr);
			}
		}
	}

	//-----------------------------------------------------------------------------
	// 分配
	//-----------------------------------------------------------------------------
	template<typename TMUTEX>
	void* nmem_pool<TMUTEX>::alloc(uint32 bytes)
	{
		uint32 real_size = 0;
		int32 index = _size2index(bytes, real_size);

		if (-1 != index)
		{
			// 提前尝试
			if (m_pool[index].first_ptr)	
			{
				nguard<TMUTEX> guard(m_mutex);

				// 池里有，就从池里分配
				if (m_pool[index].first_ptr)
				{
					nnode* node_ptr = m_pool[index].first_ptr;
					m_pool[index].first_ptr = m_pool[index].first_ptr->next_ptr;

					if (m_pool[index].first_ptr)
					{
						m_pool[index].first_ptr->pre_ptr = NULL;
					}
					else
					{
						m_pool[index].last_ptr = NULL;
					}

					m_current_size -= real_size;
					++node_ptr->use_time;
					return node_ptr->mem_ptr;	
				}
			}
		}

		++m_malloc;
		nnode* node_ptr = reinterpret_cast<nnode*>(::malloc(real_size + sizeof(nnode) - sizeof(uint32)));

		if (!node_ptr)
		{
			return NULL;
		}

		node_ptr->index		= index;
		node_ptr->size		= real_size;
		node_ptr->next_ptr	= NULL;
		node_ptr->pre_ptr	= NULL;
		node_ptr->use_time	= 0;
		return node_ptr->mem_ptr;			// 从实际内存中分配
	}

	//-----------------------------------------------------------------------------
	// 释放
	//-----------------------------------------------------------------------------
	template<typename TMUTEX>
	void nmem_pool<TMUTEX>::free(void* const mem_ptr)
	{
		nnode* node_ptr = reinterpret_cast<nnode*>(((uint8*)mem_ptr) - sizeof(nnode) + sizeof(uint32));

		if (-1 != node_ptr->index)
		{
			nguard<TMUTEX> guard(m_mutex);

			if (node_ptr->size + m_current_size > m_max_size && node_ptr->use_time > 0)
			{
				// 垃圾收集
				_garbage_collect(node_ptr->size * 2, node_ptr->use_time);	
			}

			// 内存池可以容纳
			if (node_ptr->size + m_current_size <= m_max_size) 
			{
				node_ptr->pre_ptr = NULL;
				node_ptr->next_ptr = m_pool[node_ptr->index].first_ptr;

				if (m_pool[node_ptr->index].first_ptr)
				{
					m_pool[node_ptr->index].first_ptr->pre_ptr = node_ptr;
				}
				else
				{
					m_pool[node_ptr->index].last_ptr = node_ptr;
				}

				m_pool[node_ptr->index].first_ptr = node_ptr;
				m_current_size += node_ptr->size;
				return;
			}
		}

		::free(node_ptr);
	}

	//-----------------------------------------------------------------------------
	// 分配
	//-----------------------------------------------------------------------------
	template<typename TMUTEX>
	void* nmem_pool<TMUTEX>::try_alloc(uint32 bytes)
	{
#if _WIN32_WINNT < 0x0400
		return nmem_pool::alloc(bytes);
#else
		uint32 real_size = 0;
		int32 index = _size2index(bytes, real_size);
		
		if (-1 != index)
		{
			if (!m_mutex.try_lock())
			{
				return NULL;
			}

			// 池里有，就从池里分配
			if (m_pool[index].first_ptr)
			{
				nnode* node_ptr = m_pool[index].first_ptr;
				m_pool[index].first_ptr = m_pool[index].first_ptr->next_ptr;

				if (m_pool[index].first_ptr)
				{
					m_pool[index].first_ptr->pre_ptr = NULL;
				}
				else
				{
					m_pool[index].last_ptr = NULL;
				}

				m_current_size -= real_size;
				++node_ptr->use_time;
				m_mutex.unlock();
				return node_ptr->mem_ptr;	
			}

			m_mutex.unlock();
		}

		++m_malloc;
		nnode* node_ptr = reinterpret_cast<nnode*>(::malloc(real_size + sizeof(nnode) - sizeof(uint32)));

		if (!node_ptr)
		{
			return NULL;
		}

		node_ptr->index		= index;
		node_ptr->size		= real_size;
		node_ptr->next_ptr	= NULL;
		node_ptr->pre_ptr	= NULL;
		node_ptr->use_time	= 0;
		return node_ptr->mem_ptr;	// 从实际内存中分配
#endif
	}

	//-----------------------------------------------------------------------------
	// 释放
	//-----------------------------------------------------------------------------
	template<typename TMUTEX>
	bool nmem_pool<TMUTEX>::try_free(void* const mem_ptr)
	{
#if _WIN32_WINNT < 0x0400
		nmem_pool::free(mem_ptr);
		return true;
#else
		nnode* node_ptr = reinterpret_cast<nnode*>(((uint8*)mem_ptr) - sizeof(nnode) + sizeof(uint32));
	
		if (-1 != node_ptr->index)
		{
			if (!m_mutex.try_lock())
			{
				return false;
			}

			if (node_ptr->size + m_current_size > m_max_size && node_ptr->use_time > 0)
			{
				// 垃圾收集
				_garbage_collect(node_ptr->size * 2, node_ptr->use_time);
			}

			// 内存池可以容纳
			if (node_ptr->size + m_current_size <= m_max_size)
			{
				node_ptr->pre_ptr = NULL;
				node_ptr->next_ptr = m_pool[node_ptr->index].first_ptr;

				if (m_pool[node_ptr->index].first_ptr)
				{
					m_pool[node_ptr->index].first_ptr->pre_ptr = node_ptr;
				}
				else
				{
					m_pool[node_ptr->index].last_ptr = node_ptr;
				}

				m_pool[node_ptr->index].first_ptr = node_ptr;
				m_current_size += node_ptr->size;
				m_mutex.unlock();
				return true;
			}

			m_mutex.unlock();
		}

		::free(node_ptr);
		return true;
#endif
	}

	//-----------------------------------------------------------------------------
	// 垃圾收集
	//-----------------------------------------------------------------------------
	template<typename TMUTEX>
	void nmem_pool<TMUTEX>::_garbage_collect(uint32 expect_size, uint32 use_time)
	{
		++m_gc_times;
		uint32 free_size = 0;

		// 从最大的开始回收
		for (int32 i = 15; i >= 0; --i)
		{
			if (!m_pool[i].first_ptr)
			{
				continue;
			}

			// 从最后开始释放，因为后面的Node使用次数少
			nnode* node_ptr = m_pool[i].last_ptr;

			while (node_ptr)
			{
				nnode* temp_node_ptr = node_ptr;
				node_ptr = node_ptr->pre_ptr;

				// 释放此节点
				if (temp_node_ptr->use_time < use_time)
				{
					if (node_ptr)
					{
						node_ptr->next_ptr = temp_node_ptr->next_ptr;
					}

					if (temp_node_ptr->next_ptr)
					{
						temp_node_ptr->next_ptr->pre_ptr = node_ptr;
					}

					if (m_pool[i].last_ptr == temp_node_ptr)
					{
						m_pool[i].last_ptr = node_ptr;
					}

					if (m_pool[i].first_ptr == temp_node_ptr)
					{
						m_pool[i].first_ptr = temp_node_ptr->next_ptr;
					}

					m_current_size -= temp_node_ptr->size;
					free_size += temp_node_ptr->size;
					::free(temp_node_ptr);
				}

				if (free_size >= expect_size)
				{
					return;
				}
			}
		}
	}

	//-----------------------------------------------------------------------------
	// 返回最匹配的大小
	//-----------------------------------------------------------------------------
	template<typename TMUTEX>
	int32 nmem_pool<TMUTEX>::_size2index(uint32 size, uint32& real_size)
	{
		if (size <= 32)				{ real_size = 32;			return 0; }
		if (size <= 64)				{ real_size = 64;			return 1; }
		if (size <= 128)			{ real_size = 128;			return 2; }
		if (size <= 256)			{ real_size = 256;			return 3; }
		if (size <= 512)			{ real_size = 512;			return 4; }
		if (size <= 1024)			{ real_size = 1024;			return 5; }
		if (size <= 2 * 1024)		{ real_size = 2 * 1024;		return 6; }
		if (size <= 4 * 1024 )		{ real_size = 4 * 1024;		return 7; }
		if (size <= 8 * 1024 )		{ real_size = 8 * 1024;		return 8; }
		if (size <= 16 * 1024 )		{ real_size = 16 * 1024;	return 9; }
		if (size <= 32 * 1024 )		{ real_size = 32 * 1024;	return 10; }
		if (size <= 64 * 1024 )		{ real_size = 64 * 1024;	return 11; }
		if (size <= 128 * 1024 )	{ real_size = 128 * 1024;	return 12; }
		if (size <= 256 * 1024 )	{ real_size = 256 * 1024;	return 13; }
		if (size <= 512 * 1024 )	{ real_size = 512 * 1024;	return 14; }
		if (size <= 1024 * 1024 )	{ real_size = 1024 * 1024;	return 15; }
		real_size = size;
		return -1;
	}

} // namespace nexus

#endif _NNETWORK_NSAFE_MEM_POOL_H_
