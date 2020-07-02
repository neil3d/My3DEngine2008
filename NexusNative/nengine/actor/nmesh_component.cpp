#include "StdAfx.h"
#include "nmesh_component.h"
#include "../framework/nengine.h"
#include "../renderer/nrenderer_base.h"

namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(nmesh_component, nprimitive_component)

	nmesh_component::nmesh_component(const nstring& name_str):nprimitive_component(name_str)
	{
		m_trans_flag.set(ETrans_Opaque);
		m_render_proxy.init(this);
	}

	nmesh_component::~nmesh_component(void)
	{
	}

	nmaterial_base* nmesh_component::get_material(int lod, int mtl_id)
	{
		std::pair<int, int> mtl_key(lod, mtl_id);
		material_map::iterator iter = m_replace_mtl.find( mtl_key );
		if(iter != m_replace_mtl.end())
			return iter->second.get();
		else
			return NULL;
	}

	transparent_flag nmesh_component::get_transparent_flag() const
	{
		return m_trans_flag;
	}

	void nmesh_component::replace_material(int lod, int mtl_id, nmaterial_base::ptr mtl_ptr)
	{
		m_replace_mtl.insert(make_pair(make_pair(lod, mtl_id), mtl_ptr));

		// todo : update m_trans_flag
	}

	void nmesh_component::render(const nviewport& view)
	{
		(void)view;

		if( get_render_mesh(get_render_lod()) )
		{
			nrenderer_base* rnd = nengine::instance()->get_renderer();
			rnd->frame_object(&m_render_proxy);
		}
	}

	void nmesh_component::_destroy()
	{
		for (material_map::iterator iter = m_replace_mtl.begin();
			iter != m_replace_mtl.end();
			++iter)
		{
			iter->second->_destroy();
		}
		m_replace_mtl.clear();
		nprimitive_component::_destroy();
	}

	void nmesh_component::serialize(narchive& ar)
	{
		nprimitive_component::serialize(ar);

		// todo : replace mtl serialize
		nSERIALIZE(ar, m_trans_flag);
	}
}//namespace nexus