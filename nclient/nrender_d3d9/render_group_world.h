/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_RENDER_GROUP_FULL_H_
#define _NEXUS_RENDER_GROUP_FULL_H_
#include "render_group.h"

namespace nexus
{
	// 把mesh打成包，使用instance一次draw call绘制
	struct mesh_packet
	{};

	class render_group_world :
		public render_group
	{
	public:
		render_group_world(void);
		virtual ~render_group_world(void);

		virtual bool empty() const;
		virtual void clear();
		virtual void add_obj(const nrender_proxy* obj);
		
		virtual void draw_all(const d3d_view_info* view, drawing_policy* dp);
		
		void sort_draw_order(const d3d_view_info* view);
		// draw all opaque objects and alpha masked opaque part of mixed transparent objects
		void draw_opaque(const d3d_view_info* view, drawing_policy* dp);
		// draw unlit transparent objects
		void draw_unlit_trans(const d3d_view_info* view, drawing_policy* dp);
		// draw lighting transparent objects
		void draw_mixed_trans(const d3d_view_info* view, drawing_policy* dp);

	private:
		typedef std::vector<const nrender_proxy*>	proxy_vector;
		
		void draw_proxy_vector(proxy_vector& pv, const d3d_view_info* view, drawing_policy* dp);		

		proxy_vector	m_proxy_vector[ETrans_Max];
	};
}//namespace nexus

#endif //_NEXUS_RENDER_GROUP_FULL_H_