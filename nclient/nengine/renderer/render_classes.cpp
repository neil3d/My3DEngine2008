#include "StdAfx.h"
#include "renderer_framework.h"
#include "../framework/nengine.h"
#include "../material/nmaterial.h"

namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(nrenderer_base, nsubsystem);
	nDEFINE_VIRTUAL_CLASS(nrender_resource_manager, nsubsystem)

	nDEFINE_VIRTUAL_CLASS(nrender_primitive_draw_interface, nobject)
	nDEFINE_VIRTUAL_CLASS(nrender_element, nobject)

	nDEFINE_VIRTUAL_CLASS(nrender_resource, nobject)
	
	nDEFINE_VIRTUAL_CLASS(nvertex_factory, nrender_resource)
	nDEFINE_VIRTUAL_CLASS(nshading_effect, nrender_resource)
	nDEFINE_VIRTUAL_CLASS(nrender_simple_mesh, nrender_resource)

	nDEFINE_VIRTUAL_CLASS(nrender_mesh_section, nrender_resource)
	nDEFINE_VIRTUAL_CLASS(nrender_mesh, nrender_resource)
	nDEFINE_VIRTUAL_CLASS(nrender_static_mesh, nrender_mesh)
	nDEFINE_VIRTUAL_CLASS(nrender_static_mesh_indexed, nrender_mesh)
	nDEFINE_VIRTUAL_CLASS(nrender_anim_mesh, nrender_mesh)
	nDEFINE_VIRTUAL_CLASS(nrender_cpu_skin_mesh, nrender_mesh)
	nDEFINE_VIRTUAL_CLASS(nrender_dynamic_mesh_indexed, nrender_mesh)

	nDEFINE_VIRTUAL_CLASS(nrender_texture, nrender_resource)
	nDEFINE_VIRTUAL_CLASS(nrender_texture2D, nrender_texture)
	nDEFINE_VIRTUAL_CLASS(nrender_heightmap, nrender_texture)
	nDEFINE_VIRTUAL_CLASS(nrender_alphamap, nrender_texture)

	void nrender_resource::release()
	{
		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();
		rres_mgr->free_resource(this);
	}

	enum ETransparentType nrender_proxy::get_transparent_type(int lod, int mesh_sec) const
	{
		return get_material(lod, mesh_sec)->get_trans_type();
	}
}