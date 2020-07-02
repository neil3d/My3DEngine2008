#ifndef _NEXUS_CORE_PATH_RESULT_CACHE_H_
#define _NEXUS_CORE_PATH_RESULT_CACHE_H_
#include "../ncore.h"
#include "ngraph_navigation_map.h"

namespace nexus
{
	/**
	* 定义路径结果缓存，提供迭代器接口
	*/
	class nCoreAPI path_result_cache
	{
	private:
		DISALLOW_COPY_AND_ASSIGN(path_result_cache);
	public:
		path_result_cache(){}

		size_t get_num_paths() const
		{
			return m_paths.size();
		}

		npoint get_path(size_t index)
		{
			nASSERT(index<m_paths.size());
			return m_paths[index];
		}

		void clear_paths()
		{
			m_paths.clear();
			m_path_iterator = m_paths.end();
		}

		ngraph_navigation_map::path_result_type& get_path_vector() { return m_paths; }

	private:
		ngraph_navigation_map::path_result_type	m_paths;
		ngraph_navigation_map::path_result_type::iterator m_path_iterator;
	};
}

#endif // end of _NEXUS_CORE_PATH_RESULT_CACHE_H_