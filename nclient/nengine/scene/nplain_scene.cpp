#include "StdAfx.h"
#include "nplain_scene.h"

namespace nexus
{
	nDEFINE_CLASS(nplain_scene, nscene_manager)

	nplain_scene::nplain_scene(void)
	{
	}

	nplain_scene::~nplain_scene(void)
	{
	}

	void nplain_scene::add_primitive(nprimitive_component* prim)
	{
		m_prim_set.insert(prim);
	}

	void nplain_scene::remove_primitive(nprimitive_component* prim)
	{
		m_prim_set.erase(prim);
	}

	void nplain_scene::move_primitive(nprimitive_component* prim)
	{
		(void)prim;
		// do nothing
	}

	void nplain_scene::add_light(nlight_component* lgt)
	{
		m_lgt_set.insert(lgt);
	}

	void nplain_scene::remove_light(nlight_component* lgt)
	{
		m_lgt_set.erase(lgt);
	}

	void nplain_scene::move_light(nlight_component* lgt)
	{
		(void)lgt;
		// do nothing
	}

	void nplain_scene::render_visible(const view_info& view)
	{
		const nfrustum& m_frustum = view.frustum;

		for( std::set<nprimitive_component*>::iterator iter = m_prim_set.begin();
			iter != m_prim_set.end();
			++iter)
		{
			nprimitive_component* prim = *iter;

			if(prim->get_owner()->get_visible()
				&& m_frustum.intersect_bounds(prim->get_bounds())
				)
			{
				prim->render(view);
			}
		}

		for( std::set<nlight_component*>::iterator iter = m_lgt_set.begin();
			iter != m_lgt_set.end();
			++iter)
		{
			nlight_component* lgt = *iter;
			
			if(lgt->get_owner()->get_visible())
			{
				lgt->render(view);
			}
		}
	}
}//namespace nexus