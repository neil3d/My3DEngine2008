#include "StdAfx.h"
#include "nanim_mesh_component.h"
#include "../framework/nengine.h"
#include "../renderer/nrenderer_base.h"
#include "../renderer/nrender_resource_manager.h"
#include "../renderer/nrender_resource.h"

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
		m_mesh_res = res_ptr;
		
		create_dynamic_data();
		
		m_bounds = m_anim_ctrl->get_bounds();
	}

	void nanim_mesh_component::update(float delta_time, const view_info& view)
	{
		(void)view;

		if( !m_mesh_res
			|| !m_mesh_res->ready())
			return;

		m_anim_ctrl->advance_anim(delta_time);
		m_render_mesh->set_anim_status(m_anim_ctrl->get_anim_status());
	}
	
	void nanim_mesh_component::create_dynamic_data()
	{
		nmesh_anim_controller::ptr ctrl( nNew nmesh_anim_controller );
		ctrl->init(m_mesh_res, true);

		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();

		render_res_ptr<nrender_anim_mesh> new_mesh_ptr( rres_mgr->alloc_anim_mesh_indexed() );

		if( !new_mesh_ptr )
			nthrow(_T("component: create render anim mesh object failed."));

		new_mesh_ptr->create(m_mesh_res->get_mesh_data().get(),
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

		return m_mesh_res->get_material(lod, mtl_id);
	}

	void nanim_mesh_component::render(const view_info& view)
	{
		(void)view;
		if( m_render_mesh
			&& m_anim_ctrl)
		{
			m_anim_ctrl->update_render_data();
			nmesh_component::render(view);
		}
	}
}//namespace nexus