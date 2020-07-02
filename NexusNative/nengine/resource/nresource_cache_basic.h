/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_RESOURCE_BASIC_H_
#define _NEXUS_RESOURCE_BASIC_H_
#include "nresource_cache.h"

namespace nexus
{
	/**
	 *	除了引用计数(由nresource类提供)以外，不提供其他任何cache机制的策略
	*/
	class nresource_cache_basic 
		: public nresource_cache
	{
	public:
		nresource_cache_basic(void);
		virtual ~nresource_cache_basic(void);

		virtual void cache_resource(nresource* res_ptr);
		virtual bool release_resource(nresource* res_ptr);
		virtual nresource* active_resource(const nname& res_name);
		virtual nresource* find_resource(const nname& res_name);
		virtual size_t get_num_resource();

		virtual void accept_visitor(nresource_cache_visitor* v, int param);

	private:
		typedef std::map<nname, nresource*>	resource_map;

		resource_map	m_resource_map;
		mutex			m_mutex;

		nDECLARE_CLASS(nresource_cache_basic)
	};
}//namespace nexus

#endif //_NEXUS_RESOURCE_BASIC_H_