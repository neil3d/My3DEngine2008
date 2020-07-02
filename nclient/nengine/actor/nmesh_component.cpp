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

	void nmesh_component::render(const view_info& view)
	{
		(void)view;
		nrenderer_base* rnd = nengine::instance()->get_renderer();
		rnd->frame_object(&m_render_proxy);
	}
}//namespace nexus