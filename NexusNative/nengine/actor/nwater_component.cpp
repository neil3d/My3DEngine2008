#include "StdAfx.h"
#include "nwater_component.h"
#include "../framework/nengine.h"
#include "../renderer/nrenderer_base.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nwater_component,nprimitive_component)

	nwater_component::nwater_component(const nstring& name_str):nprimitive_component(name_str)
	,size_x(255)
	,size_y(255)
	,m_water_color(0.0,0.28,0.42,1)
	,m_water_density(0.002f)
	,m_water_soft_interact_dist(100)
	,m_foam_soft_interact_dist(80)
	,m_foam_factor(1.8f)
	{
	}

	nwater_component::~nwater_component(void)
	{
	}

	void nwater_component::_init(nactor* owner)
	{
		nprimitive_component::_init(owner);
		create_render_resource();
	}

	void nwater_component::_update_transform(const object_space& parent_space)
	{
		nprimitive_component::_update_transform(parent_space);
		
		// todo:
		m_bounds.box_extent = vector3(size_x*CYC_LENGTH/64*0.5, 10, size_y*CYC_LENGTH/64*0.5);
		m_bounds.origin = vector3(0,0,0);
		m_bounds.sphere_radius = vec_distance(m_bounds.box_extent ,vector3::zero);

		m_bounds.transform_by(get_world_matrix());
	}

	void nwater_component::render(class render_package_base* rpb)
	{
		rpb->add_water(this);
	}

	void nwater_component::create_render_resource()
	{
		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();

		m_water_mesh = rres_mgr->alloc_simple_mesh();
		nsimple_mesh simple_mesh;
		float offsetx = 0.5f*size_x/64.0f;
		float offsety = 0.5f*size_y/64.0f;
		for (unsigned int j = 0; j < size_y + 1; j ++)
		{
			for (unsigned int i = 0; i < size_x + 1; i ++)
			{
				simple_mesh.vert_buffer.push_back(vector3(i/64.0f - offsetx,0,j/64.0f - offsety));
			}
		}
		
		for (unsigned int j = 0; j < size_y; j ++)
		{
			for (unsigned int i = 0; i < size_x; i ++)
			{
				simple_mesh.index_buffer.push_back(	j*(size_x+1) + i);
				simple_mesh.index_buffer.push_back(	j*(size_x+1)  + i + 1);
				simple_mesh.index_buffer.push_back(	(j+1)*(size_x+1)  + i	);

				simple_mesh.index_buffer.push_back(	(j+1)*(size_x+1)  + i);
				simple_mesh.index_buffer.push_back(	j*(size_x+1)  + i + 1);
				simple_mesh.index_buffer.push_back(	(j+1)*(size_x+1)  + i + 1);
			}
		}
		
		m_water_mesh->create(&simple_mesh);
		EnvMap = nresource_manager::instance()->load_cube_map(resource_location(_T("demo_data:unit_test/sky_cube.dds")));
		foam_tex = nresource_manager::instance()->load_texture_2d(resource_location(_T("engine_data:resource_default/foam.dds")));
		caustic_tex = nresource_manager::instance()->load_texture_2d(resource_location(_T("engine_data:resource_default/caustic.dds")));
	}

	void nwater_component::_destroy()
	{
		nengine::instance()->get_render_res_mgr()->free_resource(m_water_mesh);
		m_water_mesh = NULL;
	}

	void nwater_component::serialize(narchive& ar)
	{
		nprimitive_component::serialize(ar);
	}

	void nwater_component::_on_device_lost(int param)
	{
		(void)param;
		EnvMap.reset(NULL);
		foam_tex.reset(NULL);
		caustic_tex.reset(NULL);
		_destroy();
	}

	bool nwater_component::_on_device_reset(int param)
	{
		(void)param;
		
		create_render_resource();
		return true;
	}
}