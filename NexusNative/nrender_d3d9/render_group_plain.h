#ifndef _NEXUS_RENDER_GROUP_PLAIN_H_
#define _NEXUS_RENDER_GROUP_PLAIN_H_
#include "render_group.h"
#include "drawing_policy.h"

namespace nexus
{
	class render_group_plain
		: public render_group
	{
	public:
		render_group_plain(void)	{}
		virtual ~render_group_plain(void)	{}

		virtual bool empty() const	{	return m_objs.empty();}
		virtual void clear()		{	return m_objs.clear(); }
		virtual void add_obj(const nrender_proxy* obj)
		{
			m_objs.push_back( obj );
		}
		virtual void draw_all(const d3d_view_info* view, drawing_policy* dp, drawing_filter* filter=NULL)
		{
			for(proxy_vector::iterator iter = m_objs.begin();
				iter != m_objs.end();
				++iter)
			{
				const nrender_proxy* obj = *iter;
				if(filter)
				{
					if( !filter->should_draw(obj) )
						continue;
				}				
				dp->draw_mesh(view, obj);
			}
		}

	private:
		proxy_vector	m_objs;
	};
}//namespace nexus
#endif //_NEXUS_RENDER_GROUP_PLAIN_H_