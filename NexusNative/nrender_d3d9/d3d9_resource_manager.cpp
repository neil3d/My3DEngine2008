#include "StdAfx.h"
#include "d3d9_resource_manager.h"
#include "d3d9_static_mesh.h"
#include "d3d9_static_mesh_indexed.h"
#include "d3d9_anim_mesh.h"
#include "d3d9_cpu_skin_mesh.h"
#include "d3d9_texture.h"
#include "shading_effect.h"
#include "d3d9_dynamic_mesh_indexed.h"
#include "d3d9_simple_mesh.h"
#include "d3d9_render_target.h"
#include "d3d9_hit_proxy_hash.h"

namespace nexus
{
	nDEFINE_CLASS(d3d9_resource_manager, nrender_resource_manager)

	d3d9_resource_manager::d3d9_resource_manager(void)
	{
	}

	d3d9_resource_manager::~d3d9_resource_manager(void)
	{
	}

	nrender_static_mesh* d3d9_resource_manager::alloc_static_mesh()
	{
		return nNew d3d9_static_mesh;
	}

	nrender_static_mesh_indexed* d3d9_resource_manager::alloc_static_mesh_indexed()
	{
		return nNew d3d9_static_mesh_indexed;
	}

	nrender_anim_mesh* d3d9_resource_manager::alloc_anim_mesh_indexed()
	{
		return nNew d3d9_anim_mesh_indexed;
	}

	nrender_cpu_skin_mesh* d3d9_resource_manager::alloc_cup_skin_mesh()
	{
		return nNew d3d9_cpu_skin_mesh;
	}

	nrender_texture2D* d3d9_resource_manager::alloc_texture_2d()
	{
		return nNew d3d9_texture2D;
	}

	nrender_cube_map* d3d9_resource_manager::alloc_cube_map()
	{
		return nNew d3d9_cube_map;
	}

	void d3d9_resource_manager::free_resource(nrender_resource* res)
	{
		delete res;
	}

	void d3d9_resource_manager::set_default_material(nmaterial_base* mtl, nmaterial_base* mtl_two_sided)
	{
		shading_effect_lib::instance()->set_default_material(mtl, mtl_two_sided);
	}

	nrender_dynamic_mesh_indexed* d3d9_resource_manager::alloc_dynamic_mesh_indexed()
	{
		return nNew d3d9_dynamic_mesh_indexed;
	}

	nrender_simple_mesh* d3d9_resource_manager::alloc_simple_mesh()
	{
		return nNew d3d9_simple_mesh;
	}

	nrender_heightmap* d3d9_resource_manager::alloc_heightmap()
	{
		return nNew d3d9_heightmap;
	}

	nrender_alphamap* d3d9_resource_manager::alloc_alphamap()
	{
		return nNew d3d9_alphamap;
	}

	nrender_target* d3d9_resource_manager::alloc_render_target()
	{
		return nNew d3d9_render_target;
	}

	nhit_proxy_hash* d3d9_resource_manager::alloc_hit_proxy_hash()
	{
		return nNew d3d9_hit_proxy_hash;
	}
}//namespace nexus