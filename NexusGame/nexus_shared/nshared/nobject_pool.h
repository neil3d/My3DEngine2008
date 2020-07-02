/**
 *	nexus nshared - nobject_pool
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NSHARED_NOBJECT_POOL_H_
#define _NSHARED_NOBJECT_POOL_H_

#include "nsafe_mem_pool.h"

namespace nexus {

	/**
	 *	nobject_pool
	 */
	template<typename TMUTEX>
	class nobject_pool : private nnoncopyable
	{
	public:
		explicit nobject_pool(uint32 max_pool_size = 16 * 1024 * 1024) : m_mem_pool(max_pool_size) {}
		virtual ~nobject_pool() {}

		template<typename T>
		T* construct()
		{
			T* const object_ptr = reinterpret_cast<T* const>(m_mem_pool.alloc(sizeof(T)));

			if (NULL == object_ptr)
			{
				return object_ptr;
			}

			new (object_ptr)T();
			return object_ptr;
		}

		template<typename T>
		void destroy(T* const object_ptr)
		{
			object_ptr->~T();
			m_mem_pool.free(object_ptr);
		}

	private:
		nmem_pool<TMUTEX>		m_mem_pool;
	};

} // namespace nexus

#endif _NSHARED_NOBJECT_POOL_H_
