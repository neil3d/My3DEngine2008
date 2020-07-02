/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_RENDER_GROUP_H_
#define _NEXUS_RENDER_GROUP_H_
#include "../nengine/framework/view_info.h"

namespace nexus
{
	class drawing_policy;
	class drawing_filter;

	class render_group
	{
	public:
		render_group(void)	{	}
		virtual ~render_group(void)	{	}

		virtual bool empty() const = 0;
		virtual void clear() = 0;
		virtual void draw_all(const nview_info* view, drawing_policy* dp, drawing_filter* filter=NULL) = 0;
		//virtual const st_proxy_vector& get_proxys() = 0;
	};
}//namespace nexus
#endif //_NEXUS_RENDER_GROUP_H_