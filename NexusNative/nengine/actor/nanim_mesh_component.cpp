#include "StdAfx.h"
#include "nanim_mesh_component.h"
#include "../framework/nengine.h"
#include "../resource/nresource_manager.h"
#include "../renderer/nrenderer_base.h"
#include "../renderer/nrender_resource_manager.h"
#include "../renderer/nrender_resource.h"
#include "../resource/nresource_manager.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nanim_mesh_component, nmesh_component)

	nanim_mesh_component::nanim_mesh_component(const nstring& name_str):nmesh_component(name_str)
	{
		nresource_manager::instance()->register_event_target(this);
	}

	nanim_mesh_component::~nanim_mesh_component(void)
	{
		nresource_manager::instance()->unregister_event_target(this);
	}

	void nanim_mesh_component::reset_resource(nresource_anim_mesh::ptr res_ptr)
	{
		//-- free old
		m_mesh_res.reset();
		m_render_mesh.reset();
		m_anim_ctrl.reset();

		//--
		m_mesh_res = res_ptr;		
		if( m_mesh_res->ready() )
			on_resource_ready();		
	}

	void nanim_mesh_component::reset_resource(const resource_location& loc)
	{
		reset_resource(
			nresource_manager::instance()->load_anim_mesh(loc)
			);
	}

	void nanim_mesh_component::update(float delta_time, const nviewport& view)
	{		
		nmesh_component::update(delta_time, view);

		if( !m_mesh_res
			|| !m_mesh_res->ready())
			return;

		if(m_anim_ctrl )
			m_anim_ctrl->tick_anim(delta_time);

		if(m_render_mesh) // 可能是在device lost状态，render mesh为空
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

	nmtl_base* nanim_mesh_component::get_material(int lod, int mtl_id)
	{
		nmtl_base* ret = nmesh_component::get_material(lod, mtl_id);
		if( ret != NULL )
			return ret;

		nmtl_base::ptr res_mtl;
		if( m_mesh_res
			&& m_mesh_res->ready() )
		{
			res_mtl = m_mesh_res->get_material(lod, mtl_id);			
		}
		if( res_mtl)
			return res_mtl.get();
		else		
			return nresource_manager::instance()->get_default_material();
	}

	void nanim_mesh_component::render(render_package_base* rpb)
	{
		if( m_render_mesh
			&& m_anim_ctrl)
		{
			m_anim_ctrl->update_render_data();
			nmesh_component::render(rpb);
		}
	}

	void nanim_mesh_component::_destroy()
	{
		m_mesh_res.reset();
		m_render_mesh.reset();
		m_anim_ctrl.reset();

		nmesh_component::_destroy();
	}

	//void nanim_mesh_component::play_anim(const nstring& seq_name, bool loop, float start_time)
	//{
	//	if(m_anim_ctrl)
	//	{
	//		m_anim_ctrl->set_anim(seq_name);
	//		m_anim_ctrl->play_anim(loop, 1.0, start_time);
	//	}
	//}

	void nanim_mesh_component::set_anim( const nstring& seq_name )
	{
		if(m_anim_ctrl)
		{
			m_anim_ctrl->set_anim(seq_name);
		}
		else
		{
			nLog_Warning(_T("animation controller hasn't created when set_anim(%s)."), seq_name.c_str());
		}
	}

	void nanim_mesh_component::play_anim( bool loop, float rate, float start_time /*= 0.f*/ )
	{
		if(m_anim_ctrl)
		{
			m_anim_ctrl->play_anim(loop, rate, start_time);
		}
		else
		{
			nLog_Warning(_T("animation controller hasn't created when play_anim"));
		}
	}

	const TCHAR* nanim_mesh_component::get_anim_playing() const
	{
		return m_anim_ctrl ? m_anim_ctrl->get_anim_playing() : NULL;
	}

	float nanim_mesh_component::get_time_left() const
	{
		return m_anim_ctrl ? m_anim_ctrl->get_time_left() : 0.f;
	}

	float nanim_mesh_component::get_anim_length() const
	{
		return m_anim_ctrl ? m_anim_ctrl->get_anim_length() : 0.f;
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

	void nanim_mesh_component::_on_device_lost(int param)
	{
		(void)param;
		m_render_mesh.reset();
	}

	bool nanim_mesh_component::_on_device_reset(int param)
	{
		(void)param;

		on_resource_ready();
		return true;
	}

	void nanim_mesh_component::on_event(nevent_base& evt)
	{
		if( m_mesh_res
			&& is_resource_ready_event(evt, m_mesh_res->get_name() )
			)
		{
			on_resource_ready();
		}
	}

	void nanim_mesh_component::on_resource_ready()
	{
		//--注意：必须是可重复调用的
		create_dynamic_data();		
		if( m_anim_ctrl)
		{
			m_bounds = m_anim_ctrl->get_bounds();
			m_bounds.transform_by( get_world_matrix() );
		}

		nrender_mesh* mesh = m_render_mesh.get();
		m_mesh_elements.clear();
		for(size_t i=0; i<mesh->get_num_section(); i++)
		{	
			nrender_mesh_section* sec = mesh->get_section(i);
			nmtl_base* mtl = get_material(0, sec->get_material_id() );
			m_mesh_elements.push_back(mesh_element(this,mesh,sec,mtl));
		}
		if(m_owner)
		{
			nscene_event evt(ESA_MovePrimitive, this);
			m_owner->on_event(evt);		
		}
	}

}//namespace nexus