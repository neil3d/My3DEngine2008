#include "StdAfx.h"
#include "render_group_world.h"
#include "drawing_policy.h"

namespace nexus
{
	render_group_world::render_group_world(void)
	{
	}

	render_group_world::~render_group_world(void)
	{
	}

	void render_group_world::clear()
	{
		for(size_t i=0; i<ETrans_Max; i++)
			m_proxy_vector[i].clear();
	}

	void render_group_world::add_obj(const nrender_proxy* obj)
	{
		/**
		 * 注意：一个obj可能被存到多个vector中，在draw的时候每个obj可能被访问多次，
		 * 但是除了mixed trans的对象被draw两个pass以外，其他的对象应该只被访问一次。
		*/
		transparent_flag trans_flag = obj->get_transparent_flag();
		nASSERT( trans_flag.flag != 0);
		for(size_t i=0; i<ETrans_Max; i++)
		{
			if( trans_flag.get((enum ETransparentType)i) )
				m_proxy_vector[i].push_back(obj);
		}
	}

	void render_group_world::draw_proxy_vector(proxy_vector& pv, const d3d_view_info* view, drawing_policy* dp, drawing_filter* filter)
	{
		for(proxy_vector::iterator iter = pv.begin();
			iter != pv.end();
			++iter)
		{
			const nrender_proxy* obj = *iter;
			if( filter )
			{
				if( !filter->should_draw(obj) )
					continue;
			}
			dp->draw_mesh(view, obj);
		}
	}

	void render_group_world::draw_all(const d3d_view_info* view, drawing_policy* dp, drawing_filter* filter)
	{
		for(size_t i=0; i<ETrans_Max; i++)
		{
			draw_proxy_vector(m_proxy_vector[i], view, dp, filter);
		}
	}

	bool render_group_world::empty() const
	{
		for(size_t i=0; i<ETrans_Max; i++)
			if( !m_proxy_vector[i].empty() )
				return false;

		return true;
	}

	void render_group_world::draw_opaque(const d3d_view_info* view, drawing_policy* dp, drawing_filter* filter)
	{
		draw_proxy_vector(m_proxy_vector[ETrans_Opaque], view, dp, filter);
		draw_proxy_vector(m_proxy_vector[ETrans_AlphaMasked], view, dp, filter);
	}

	void render_group_world::draw_unlit_trans(const d3d_view_info* view, drawing_policy* dp, drawing_filter* filter)
	{
		draw_proxy_vector(m_proxy_vector[ETrans_UnlitTrans], view, dp, filter);
	}
	
	void render_group_world::draw_mixed_trans(const d3d_view_info* view, drawing_policy* dp, drawing_filter* filter)
	{
		draw_proxy_vector(m_proxy_vector[ETrans_Mixed], view, dp, filter);		
	}

	struct _cmp_opaque
	{
		const d3d_view_info* view;
		_cmp_opaque(const d3d_view_info* _view):view(_view)
		{}

		bool operator() (const nrender_proxy* obj1, const nrender_proxy* obj2)
		{
			const box_sphere_bounds& bounds1 = obj1->get_bounds();
			const box_sphere_bounds& bounds2 = obj2->get_bounds();

			return vec_distance_sq(view->eye_pos, bounds1.origin) <
				vec_distance_sq(view->eye_pos, bounds2.origin);
		}
	};

	struct _cmp_trans
	{
		const d3d_view_info* view;
		_cmp_trans(const d3d_view_info* _view):view(_view)
		{}

		bool operator() (const nrender_proxy* obj1, const nrender_proxy* obj2)
		{
			const box_sphere_bounds& bounds1 = obj1->get_bounds();
			const box_sphere_bounds& bounds2 = obj2->get_bounds();

			return vec_distance_sq(view->eye_pos, bounds1.origin) >
				vec_distance_sq(view->eye_pos, bounds2.origin);
		}
	};

	void render_group_world::sort_draw_order(const d3d_view_info* view)
	{
		// 将对象排序，以适合渲染需求：不透明物体从前向后，透明物体从后向前
		_cmp_opaque cmp_opaque(view);
		std::sort(m_proxy_vector[ETrans_Opaque].begin(),
			m_proxy_vector[ETrans_Opaque].end(),
			cmp_opaque);

		std::sort(m_proxy_vector[ETrans_AlphaMasked].begin(),
			m_proxy_vector[ETrans_AlphaMasked].end(),
			cmp_opaque);

		_cmp_trans cmp_trans(view);
		std::sort(m_proxy_vector[ETrans_Mixed].begin(),
			m_proxy_vector[ETrans_Mixed].end(),
			cmp_trans);

		std::sort(m_proxy_vector[ETrans_UnlitTrans].begin(),
			m_proxy_vector[ETrans_UnlitTrans].end(),
			cmp_trans);
	}
}//namespace nexus