#include "StdAfx.h"
#include "nmesh_component.h"
#include "../framework/nengine.h"
#include "../renderer/nrenderer_base.h"
#include "../resource/nresource_manager.h"

namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(nmesh_component, nprimitive_component)

	nmesh_component::nmesh_component(const nstring& name_str):nprimitive_component(name_str)
	,m_enable_instance(false)
	{	
	}

	nmesh_component::~nmesh_component(void)
	{
	}

	nmtl_base* nmesh_component::get_material(int lod, int mtl_id)
	{
		std::pair<int, int> mtl_key(lod, mtl_id);
		material_map::iterator iter = m_replace_mtl.find( mtl_key );
		if(iter != m_replace_mtl.end())
			return iter->second.get();
		else
			return NULL;
	}

	void nmesh_component::replace_material(int lod, int mtl_id, nmtl_base::ptr mtl_ptr)
	{
		m_replace_mtl.insert(make_pair(make_pair(lod, mtl_id), mtl_ptr));

		on_resource_ready();
		// todo : update m_trans_flag
	}

	void nmesh_component::render(render_package_base* rpb)
	{
		rpb->add_mesh(this);
	}

	void nmesh_component::_destroy()
	{
		// todo :destroy material?
		m_replace_mtl.clear();
		nprimitive_component::_destroy();
	}

	void nmesh_component::serialize(narchive& ar)
	{
		nprimitive_component::serialize(ar);

		// todo : replace mtl serialize		
	}
}//namespace nexus