#include "StdAfx.h"
#include "nspeed_tree_component.h"
#include "../framework/nengine.h"
#include "../renderer/nrenderer_base.h"
#include "../resource/nresource_manager.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nspeed_tree_component, nprimitive_component)

	nspeed_tree_component::nspeed_tree_component(const nstring& name_str):nprimitive_component(name_str)
	{
		m_spt.owner = this;
	}

	nspeed_tree_component::~nspeed_tree_component(void)
	{
	}

	void nspeed_tree_component::create(const resource_location& spt_file)
	{
		m_spt.base_spt = nresource_manager::instance()->load_speed_tree(spt_file);

		m_bounds = m_spt.base_spt->get_bounds();
		m_spt_file_loc = spt_file;
	}

	void nspeed_tree_component::render(render_package_base* rpb)
	{
		if( m_spt.base_spt )
		{
			rpb->add_spt_tree(this);
		}
	}

	void nspeed_tree_component::_destroy()
	{
		m_spt.base_spt.reset();
	}

	void nspeed_tree_component::update(float delta_time, const nviewport& view)
	{
		nprimitive_component::update(delta_time,view);

		//-- update lod
		float lod = view.get_lod(m_spt.pos);
		lod = 1;// temp
		if( m_spt.base_spt )
		{
			m_spt.update_lod( lod );
		}
	}

	void nspeed_tree_component::_update_transform(const object_space& parent_space)
	{
		m_spt.pos = parent_space.location;
		m_spt.scale = parent_space.scale.x;
		m_spt.set_rotation(parent_space.rotation.y);

		m_bounds = m_spt.base_spt->get_bounds();

		float rotation = parent_space.rotation.y;

		matrix44 mat;
		mat_set_scaling(mat, parent_space.scale.x, parent_space.scale.x, parent_space.scale.x);
		mat_rotate_Y(mat, rotation);
		mat_translate(mat, m_spt.pos);		

		m_bounds.transform_by( mat );
		m_spt.bounds = m_bounds;
		m_spt.mat_world = mat;
	}

	void nspeed_tree_component::serialize(narchive& ar)
	{
		nprimitive_component::serialize(ar);

		nSERIALIZE(ar, m_spt_file_loc);
		nstring class_name(_T("nspt_instance"));
		ar.object_begin(_T("spt_instance"), class_name);
		m_spt.serialize(ar);
		ar.object_end();
	}
	
	void nspeed_tree_component::_level_loaded(nactor* owner)
	{
		nactor_component::_level_loaded(owner);
		this->_init(owner);
		create(m_spt_file_loc);		
		this->_update_transform(owner->get_space());		
	}
}//namespace nexus