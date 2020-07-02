#include "StdAfx.h"
#include "nfog_volume_component.h"
#include "../framework/nengine.h"
#include "../renderer/nrenderer_base.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nfog_volume_component,nprimitive_component)

	nfog_volume_component::nfog_volume_component(const nstring& name_str,eshape shaptype)
	:nprimitive_component(name_str),
	shape_type(shaptype),
	global_density(0.01f),
	color(1,1,1,1),
	base_point(0,0,0),
	dir_scaled(0,0.002f,0),
	soft_edges(0.1f,0),
	fog_start_distance(0)
	{	
		m_accept_dynamic_light = false;
		m_cast_dynamic_shadow = false;
		m_disable_all_rigidbody = true;	
		m_bounds.box_extent=vector3(1,1,1);
		m_bounds.sphere_radius = 1.0f;
	}

	nfog_volume_component::~nfog_volume_component(void)
	{
	}

	void nfog_volume_component::render(render_package_base* package)
	{
		package->add_fog(this);
	}

	void nfog_volume_component::_init( nactor* owner )
	{
		nprimitive_component::_init(owner);
		move(vector3::zero,vector3::zero,vector3(100,100,100));
	}

	void nfog_volume_component::_update_transform( const object_space& parent_space )
	{
		nprimitive_component::_update_transform(parent_space);
		m_bounds.box_extent = vector3(1,1,1);
		m_bounds.sphere_radius =  1.0f;
		m_bounds.origin = vector3::zero;
		m_bounds.transform_by(get_world_matrix());
	}

	void nfog_volume_component::serialize( narchive& ar )
	{
		nprimitive_component::serialize(ar);

		nserialize_enum_as_int(ar, shape_type, _T("shape_type"));
		nSERIALIZE(ar, global_density);
		nSERIALIZE(ar, color);
		nSERIALIZE(ar, base_point);
		nSERIALIZE(ar, dir_scaled);
		nSERIALIZE(ar, soft_edges);
		nSERIALIZE(ar, fog_start_distance);
	}
}//namespace nexus