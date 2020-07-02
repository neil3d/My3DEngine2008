#include "StdAfx.h"
#include "nresource_cache_basic.h"

namespace nexus
{
	nDEFINE_CLASS(nresource_cache_basic, nresource_cache)

	nresource_cache_basic::nresource_cache_basic(void)
	{
	}

	nresource_cache_basic::~nresource_cache_basic(void)
	{
	}

	void nresource_cache_basic::cache_resource(nresource* res_ptr)
	{
		nASSERT(res_ptr != NULL);
		
		mutex::scoped_lock lk(m_mutex);
		m_resource_map.insert(make_pair(res_ptr->get_name(), res_ptr));
	}

	bool nresource_cache_basic::release_resource(nresource* res_ptr)
	{
		nASSERT(res_ptr != NULL);

		mutex::scoped_lock lk(m_mutex);
		m_resource_map.erase(res_ptr->get_name());

		return true;
	}

	nresource* nresource_cache_basic::active_resource(const nname& res_name)
	{
		return find_resource(res_name);
	}

	nresource* nresource_cache_basic::find_resource(const nname& res_name)
	{
		mutex::scoped_lock lk(m_mutex);
		resource_map::iterator iter = m_resource_map.find(res_name);
		
		if( iter != m_resource_map.end() )
			return iter->second;
		return NULL;
	}

	size_t nresource_cache_basic::get_num_resource()
	{
		mutex::scoped_lock lk(m_mutex);
		return m_resource_map.size();
	}

	void nresource_cache_basic::accept_visitor(nresource_cache_visitor* v, int param)
	{
		mutex::scoped_lock lk(m_mutex);
		v->begin();
		for (resource_map::iterator iter=m_resource_map.begin();
			iter != m_resource_map.end();
			++iter)
		{
			v->handle_resource(iter->second, param);
		}
		v->end();
	}
}//namespace nexus