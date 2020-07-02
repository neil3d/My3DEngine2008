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
		m_type = -1;
		m_sphere_radius = 0;
		m_sphere_rings = 0;
		m_sphere_sections = 0;
	}

	nshape_component::~nshape_component(void)
	{		
	}

	void nshape_component::set_material(nmtl_base::ptr mtl_ptr)
	{
		m_mtl_ptr = mtl_ptr;
		m_mesh_elements.clear();
		for(size_t i=0; i<m_mesh_ptr->get_num_section(); i++)
		{	
			nrender_mesh_section* sec = m_mesh_ptr->get_section(i);
			nmtl_base* mtl = get_material(0, sec->get_material_id() );
			m_mesh_elements.push_back(mesh_element(this,m_mesh_ptr,sec,mtl));
		}
	}

	void nshape_component::create_box(float width, float height, float depth)
	{
		m_mesh_ptr = 
			shape_func::create_box_rich(width, height, depth);

		m_box_param = vector3(width/2 + 0.1, height/2 + 0.1, depth/2 + 0.1);
		// todo:
		m_bounds.box_extent = m_box_param;
		m_bounds.origin = vector3(0,0,0);
		m_bounds.sphere_radius = sqrt(m_box_param.x * m_box_param.x  
			+ m_box_param.y * m_box_param.y
			+ m_box_param.z * m_box_param.z);

		//--
		m_type = 0;
	}

	void nshape_component::create_sphere(float radius, unsigned short num_rings, unsigned short num_sections)
	{
		vector3 center(0,0,0);
		m_mesh_ptr = 
			shape_func::create_sphere(center, radius, num_rings, num_sections);

		// todo:
		m_bounds.box_extent = vector3(radius, radius, radius);
		m_bounds.origin = vector3(0,0,0);
		m_bounds.sphere_radius = radius;

		//--
		m_type = 1;
		m_sphere_radius = radius;
		m_sphere_rings = num_rings;
		m_sphere_sections = num_sections;
		
		_update_transform(m_owner->get_space());
	}

	nrender_mesh*	 nshape_component::create_sphere_mesh(float radius, unsigned short num_rings, unsigned short num_sections)
	{
		vector3 center(0,0,0);
		return shape_func::create_sphere(center, radius, num_rings, num_sections);
	}

	void nshape_component::create_sphere_with_mesh(float radius, unsigned short num_rings, unsigned short num_sections,nrender_mesh* mesh)
	{
		m_mesh_ptr = mesh;

		// todo:
		m_bounds.box_extent = vector3(radius, radius, radius);
		m_bounds.origin = vector3(0,0,0);
		m_bounds.sphere_radius = radius;

		//--
		m_type = 1;
		m_sphere_radius = radius;
		m_sphere_rings = num_rings;
		m_sphere_sections = num_sections;

		_update_transform(m_owner->get_space());
		nscene_event evt(ESA_AddPrimitive, this);
		m_owner->on_event(evt);
	}

	void nshape_component::_init(nactor* owner)
	{
		nprimitive_component::_init(owner);
	}

	void nshape_component::_update_transform(const object_space& parent_space)
	{
		nprimitive_component::_update_transform(parent_space);
		if (m_type == 0)
		{
			// todo:
			m_bounds.box_extent = m_box_param;
			m_bounds.origin = vector3(0,0,0);
			m_bounds.sphere_radius = m_box_param.x;	
		}
		else if (m_type == 1)
		{
			// todo:
			m_bounds.box_extent = vector3(m_sphere_radius, m_sphere_radius, m_sphere_radius);
			m_bounds.origin = vector3(0,0,0);
			m_bounds.sphere_radius = m_sphere_radius;
		}

		m_bounds.transform_by(get_world_matrix());
	}

	nmtl_base* nshape_component::get_material(int lod, int mtl_id)
	{
		(void)lod;
		(void)mtl_id;
		if( m_mtl_ptr)
			return m_mtl_ptr.get();
		else
			return nresource_manager::instance()->get_default_material();
	}

	void nshape_component::_destroy()
	{
		m_mesh_ptr = NULL;
		m_mtl_ptr.reset();

		nmesh_component::_destroy();
	}

	void nshape_component::serialize(narchive& ar)
	{
		nmesh_component::serialize(ar);

		nSERIALIZE(ar, m_type);
		nSERIALIZE(ar, m_box_param);
		nSERIALIZE(ar, m_sphere_radius);
		nSERIALIZE(ar, m_sphere_rings);
		nSERIALIZE(ar, m_sphere_sections);
		nSERIALIZE(ar, m_mtl_ptr);

		if( ar.is_loading() )
		{
			create_render_mesh_by_data();
		}
	}

	void nshape_component::create_render_mesh_by_data()
	{
		switch(m_type)
		{
		case 0:
			create_box(m_box_param.x,m_box_param.y, m_box_param.z);
			break;
		case 1:
			create_sphere(m_sphere_radius, m_sphere_rings, m_sphere_sections);
			break;
		}
	}

	void nshape_component::_on_device_lost(int param)
	{
		(void)param;
		m_mesh_ptr->release();
		m_mesh_ptr = NULL;
	}

	bool nshape_component::_on_device_reset(int param)
	{
		(void)param;
		create_render_mesh_by_data();
		set_material(m_mtl_ptr);
		return true;
	}
}//namespace nexus