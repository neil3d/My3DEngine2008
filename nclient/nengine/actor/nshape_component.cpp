#include "StdAfx.h"
#include "nshape_component.h"
#include "../framework/nengine.h"
#include "../renderer/nrenderer_base.h"
#include "../renderer/nrender_resource_manager.h"
#include "../renderer/nrender_resource.h"
#include "../resource/nresource_manager.h"
#include "../util/shape_func.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nshape_component, nprimitive_component)

	nshape_component::nshape_component(const nstring& name_str)
		: nmesh_component(name_str)
	{
		m_trans_flag.set( ETrans_Opaque );
	}

	nshape_component::~nshape_component(void)
	{		
	}

	void nshape_component::set_material(nmaterial_base::ptr mtl_ptr)
	{
		m_mtl_ptr = mtl_ptr;
	}

	void nshape_component::create_box(float width, float height, float depth)
	{
		m_mesh_ptr.reset(
			shape_func::create_box_rich(width, height, depth)
			);

		// todo:
		m_bounds.box_extent = vector3(width, height, depth);
		m_bounds.origin = vector3(0,0,0);
		m_bounds.sphere_radius = width;
	}

	void nshape_component::create_sphere(float radius, unsigned short num_rings, unsigned short num_sections)
	{
		vector3 center(0,0,0);
		m_mesh_ptr.reset(
			shape_func::create_sphere(center, radius, num_rings, num_sections)
			);

		// todo:
		m_bounds.box_extent = vector3(radius, radius, radius);
		m_bounds.origin = vector3(0,0,0);
		m_bounds.sphere_radius = radius;
	}

	nrender_mesh* nshape_component::get_render_mesh(int lod)
	{
		(void)lod;
		return m_mesh_ptr.get();
	}
	
	nmaterial_base* nshape_component::get_material(int lod, int mtl_id)
	{
		(void)lod;
		(void)mtl_id;
		if( m_mtl_ptr)
			return m_mtl_ptr.get();
		else
			return nresource_manager::instance()->get_default_material();
	}
}//namespace nexus