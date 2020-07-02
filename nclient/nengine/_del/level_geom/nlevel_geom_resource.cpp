#include "StdAfx.h"
#include "nlevel_geom_resource.h"
#include "../framework/nengine.h"
#include "../renderer/nrenderer_base.h"
#include "../renderer/nrender_resource_manager.h"
#include "../resource/nresource_manager.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nlevel_geom_static_mesh, nlevel_geom)

	nlevel_geom_static_mesh::nlevel_geom_static_mesh(const nstring& name_str):nlevel_geom(name_str)
	{
		m_render_proxy.init(this);
	}

	nlevel_geom_static_mesh::~nlevel_geom_static_mesh(void)
	{
	}

	void nlevel_geom_static_mesh::create(nresource_ptr<nresource_static_mesh> res_ptr)
	{
		m_res_ptr = res_ptr;
	}

	void nlevel_geom_static_mesh::render()
	{
		if(m_res_ptr
			&& m_res_ptr->get_num_lod() > 0)
		{
			nrenderer_base* rnd = nengine::instance()->get_renderer();
			rnd->frame_object(&m_render_proxy);
		}
	}
}//namespace nexus