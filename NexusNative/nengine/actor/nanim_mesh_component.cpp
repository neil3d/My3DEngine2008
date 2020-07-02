#include "StdAfx.h"
#include "nanim_mesh_component.h"
#include "../framework/nengine.h"
#include "../renderer/nrenderer_base.h"
#include "../renderer/nrender_resource_manager.h"
#include "../renderer/nrender_resource.h"
#include "../resource/nresource_manager.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nanim_mesh_component, nmesh_component)

	nanim_mesh_component::nanim_mesh_component(const nstring& name_str):nmesh_component(name_str)
	{
	}

	nanim_mesh_component::~nanim_mesh_component(void)
	{
	}

	void nanim_mesh_component::reset_resource(nresource_anim_mesh::ptr res_ptr)
	{
		//-- free old
		m_mesh_res.reset();
		m_render_mesh.reset();
		m_anim_ctrl.reset();

		//--
		m_mesh_res = res_ptr;		
		create_dynamic_data();		
		if( m_anim_ctrl)
			m_bounds = m_anim_ctrl->get_bounds();
	}

	void nanim_mesh_component::reset_resource(const resource_location& loc)
	{
		reset_resource(
			nresource_manager::instance()->load_anim_mesh(loc)
			);
	}

	void nanim_mesh_component::update(float delta_time, const nviewport& view)
	{		
		(void)view;

		if( !m_mesh_res
			|| !m_mesh_res->ready())
			return;
		if( !m_anim_ctrl )
			return;

		m_anim_ctrl->advance_anim(delta_time);
		m_render_mesh->set_anim_status(m_anim_ctrl->get_anim_status());
	}
	
	void nanim_mesh_component::create_dynamic_data()
	{
		if( !m_mesh_res )
			return;
		if( m_mesh_res->get_num_lod() <= 0)
			return;

		//--
		nmesh_anim_controller::ptr ctrl( nNew nmesh_anim_controller );
		ctrl->init(m_mesh_res, true);

		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();

		render_res_ptr<nrender_anim_mesh> new_mesh_ptr( rres_mgr->alloc_anim_mesh_indexed() );

		if( !new_mesh_ptr )
			nthrow(_T("component: create render anim mesh object failed."));

		new_mesh_ptr->create(m_mesh_res->get_lod_mesh(0).get(),
			ctrl->get_anim_status());		

		//--
		m_anim_ctrl = ctrl;
		m_render_mesh.reset( new_mesh_ptr.release() );
		m_render_mesh->set_anim_status(m_anim_ctrl->get_anim_status());
	}

	nrender_mesh* nanim_mesh_component::get_render_mesh(int lod)
	{
		(void)lod;
		return m_render_mesh.get();
	}

	nmaterial_base* nanim_mesh_component::get_material(int lod, int mtl_id)
	{
		nmaterial_base* ret = nmesh_component::get_material(lod, mtl_id);
		if( ret != NULL )
			return ret;

		return m_mesh_res->get_material(lod, mtl_id).get();
	}

	void nanim_mesh_component::render(const nviewport& view)
	{
		(void)view;
		if( m_render_mesh
			&& m_anim_ctrl)
		{
			m_anim_ctrl->update_render_data();
			nmesh_component::render(view);
		}
	}

	void nanim_mesh_component::_destroy()
	{
		m_mesh_res.reset();
		m_render_mesh.reset();
		m_anim_ctrl.reset();

		nmesh_component::_destroy();
	}

	void nanim_mesh_component::play_anim(const nstring& seq_name, bool loop, float start_time)
	{
		if(m_anim_ctrl)
		{
			m_anim_ctrl->play_anim(seq_name, loop, start_time);
		}
	}

	void nanim_mesh_component::_update_transform(const object_space& parent_space)
	{
		nmesh_component::_update_transform(parent_space);

		if( m_anim_ctrl)
		{
			m_bounds = m_anim_ctrl->get_bounds();
		}
		else
		{
			// default
			m_bounds.origin = vector3::zero;
			m_bounds.box_extent = vector3(100, 200, 100);
			m_bounds.sphere_radius = 400;
		}

		m_bounds.transform_by( get_world_matrix() );

		nscene_event evt(ESA_MovePrimitive, this);
		m_owner->on_event(evt);		
	}

	void nanim_mesh_component::serialize(narchive& ar)
	{
		nmesh_component::serialize(ar);

		resource_location mesh_loc;
		if( m_mesh_res )
			mesh_loc = m_mesh_res->get_location();

		nSERIALIZE(ar, mesh_loc);

		if( ar.is_loading() 
			&& mesh_loc.is_valid() )
		{
			reset_resource( mesh_loc );
		}
	}
}//namespace nexus