#include "StdAfx.h"
#include "nlight_component.h"
#include "nprimitive_component.h"
#include "../framework/nengine.h"
#include "../renderer/nrenderer_base.h"


namespace nexus
{
	nDEFINE_NAMED_CLASS(nlight_component, nactor_component)

	nlight_component::nlight_component(const nstring& name_str):nactor_component(name_str)
	,m_type(ELight_None)
	,m_color(1,1,1,1)
	,m_shadow_softness(1)
	,project_shadow(true)
	{
		m_bounds.box_extent = vector3(HALF_WORLD_MAX,HALF_WORLD_MAX,HALF_WORLD_MAX);
		m_bounds.origin = vector3(0,0,0);
		m_bounds.sphere_radius = HALF_WORLD_MAX;
	}

	nlight_component::~nlight_component(void)
	{
	}

	void nlight_component::render(const nviewport& view)
	{
		(void)view;
	}

	void nlight_component::_init(nactor* owner)
	{
		nactor_component::_init(owner);

		_update_transform(owner->get_space());
		nscene_event evt(ESA_AddLight, this);
		owner->on_event(evt);
	}

	void nlight_component::_remove_from_level()
	{
		nscene_event evt(ESA_RemoveLight, this);
		m_owner->on_event(evt);
	}

	void nlight_component::_destroy()
	{
		_remove_from_level();
	}

	bool nlight_component::get_visible() const 
	{
		return m_owner? (m_owner->get_visible()) : false; 
	}

	void nlight_component::serialize(narchive& ar)
	{
		nactor_component::serialize(ar);

		nSERIALIZE(ar, m_bounds);
		nserialize_enum_as_int(ar, m_type, _T("m_type"));
		nSERIALIZE(ar, m_color);
		nSERIALIZE(ar, m_shadow_softness);
		nSERIALIZE(ar,project_shadow);
	}

	void nlight_component::update_transform(const object_space& parent_space)
	{
		_update_transform(parent_space);
		nscene_event evt(ESA_MoveLight, this);
		m_owner->on_event(evt);
	}

	void nlight_component::_level_loaded(nactor* owner)
	{
		nactor_component::_level_loaded(owner);
		this->_update_transform(owner->get_space());
		this->_init(owner);
	}

	nDEFINE_NAMED_CLASS(npoint_light_component,nlight_component)
	npoint_light_component::npoint_light_component(const nstring& name_str):nlight_component(name_str)
	,m_attenuation(4000,2,2)
	{
		m_type = ELight_Point;
	}

	npoint_light_component::~npoint_light_component(void)
	{
	}

	void npoint_light_component::_update_transform(const object_space& parent_space)
	{
		m_position = parent_space.location;
		m_bounds.box_extent = vector3(m_attenuation.x,m_attenuation.x,m_attenuation.x);
		m_bounds.sphere_radius =  m_attenuation.x;
		m_bounds.origin = vector3::zero;
		nlight_component::_update_transform(parent_space);

		for( size_t faceIndex = 0; faceIndex < 6; faceIndex++ )
		{
			nview_info& view_info = view_infos[faceIndex];
			nexus::mat_set_perspective_LH( view_info.mat_project, nPI / 2, 1.0f, 1.0f, get_radius());

			// Standard view that will be overridden below
			vector3 vEnvEyePt = m_position;
			vector3 vLookatPt, vUpVec;

			switch( faceIndex )
			{
			case POSITIVE_X:
				vLookatPt = vector3( 1.0f, 0.0f, 0.0f );
				vUpVec = vector3( 0.0f, 1.0f, 0.0f );
				break;
			case NEGATIVE_X:
				vLookatPt = vector3( -1.0f, 0.0f, 0.0f );
				vUpVec = vector3( 0.0f, 1.0f, 0.0f );
				break;
			case POSITIVE_Y:
				vLookatPt = vector3( 0.0f, 1.0f, 0.0f );
				vUpVec = vector3( 0.0f, 0.0f, -1.0f );
				break;
			case NEGATIVE_Y:
				vLookatPt = vector3( 0.0f, -1.0f, 0.0f );
				vUpVec = vector3( 0.0f, 0.0f, 1.0f );
				break;
			case POSITIVE_Z:
				vLookatPt = vector3( 0.0f, 0.0f, 1.0f );
				vUpVec = vector3( 0.0f, 1.0f, 0.0f );
				break;
			case NEGATIVE_Z:
				vLookatPt = vector3( 0.0f, 0.0f, -1.0f );
				vUpVec = vector3( 0.0f, 1.0f, 0.0f );
				break;
			}

			vector3 vDir = ( vEnvEyePt + vLookatPt );
			mat_set_lookat_LH( view_info.mat_view, vEnvEyePt, vDir, vUpVec );
			view_info.mat_view_project = view_info.mat_view * view_info.mat_project;			
			view_info.fov = 1;

			view_info.frustum.extract( view_info.mat_view,view_info.mat_project);
		}
	}

	void npoint_light_component::serialize(narchive& ar)
	{
		nlight_component::serialize(ar);
		nSERIALIZE(ar,m_position);
		nSERIALIZE(ar,m_attenuation);
	}

	void npoint_light_component::draw_bounds( nrender_primitive_draw_interface* PDI, const color4f& color )
	{
		PDI->draw_wired_box(m_bounds.get_AABBox(), color);
	}

	void npoint_light_component::add_affect_prim(nprimitive_component* prim)
	{
		if (!prim->accept_dynamic_light())
		{
			return;
		}

		for( size_t faceIndex = 0; faceIndex < 6; faceIndex++ )
		{
			if(view_infos[faceIndex].frustum.intersect_bounds(prim->get_bounds()))
			{
				affect_prims[faceIndex].insert(prim);
				affect_prim_set.insert(prim);
			}
		}
	}

	void npoint_light_component::remove_affect_prim(nprimitive_component* prim)
	{
		if (affect_prim_set.find(prim) != affect_prim_set.end())
		{
			affect_prim_set.erase(prim);
			for( size_t faceIndex = 0; faceIndex < 6; faceIndex++ )
			{
				affect_prims[faceIndex].erase(prim);
			}
		}
	}

	nDEFINE_NAMED_CLASS(ndirectional_light_component,nlight_component)
	ndirectional_light_component::ndirectional_light_component(const nstring& name_str):nlight_component(name_str)
	{
		m_type = ELight_Directional;
		m_bounds.box_extent = vector3(HALF_WORLD_MAX,HALF_WORLD_MAX,HALF_WORLD_MAX);
		m_bounds.origin = vector3(0,0,0);
		m_bounds.sphere_radius = HALF_WORLD_MAX;
	}

	ndirectional_light_component::~ndirectional_light_component(void)
	{
	}

	void ndirectional_light_component::serialize(narchive& ar)
	{
		nlight_component::serialize(ar);
	}

	nDEFINE_NAMED_CLASS(nspot_light_component,nlight_component)
	nspot_light_component::nspot_light_component(const nstring& name_str):nlight_component(name_str)
	,m_attenuation(4000,2,2)
	,inner_cone_angle(30)
	,outer_cone_angle(44)
	{
		m_type = Elight_Spot;
	}

	nspot_light_component::~nspot_light_component(void)
	{
	}

	void nspot_light_component::_update_transform(const object_space& parent_space)
	{
		m_bounds.box_extent = vector3(m_attenuation.x,m_attenuation.x,m_attenuation.x);
		m_bounds.sphere_radius = m_attenuation.x;
		m_bounds.origin = vector3::zero;
		local_to_world = parent_space.local_to_world;
		nlight_component::_update_transform(parent_space);

		view_info.eye_pos = local_to_world.get_origin();
		nexus::mat_set_perspective_LH(view_info.mat_project,outer_cone_angle*nPI/180.0f,1,1, m_attenuation.x);
		view_info.mat_view = mat_inverse(local_to_world);
		view_info.mat_view_project = view_info.mat_view*view_info.mat_project;

		view_info.frustum.extract(view_info.mat_view,view_info.mat_project);
	}

	void nspot_light_component::add_affect_prim(nprimitive_component* prim)
	{
		affect_prim_set.insert(prim);
	}

	void nspot_light_component::remove_affect_prim(nprimitive_component* prim)
	{
		affect_prim_set.erase(prim);
	}

	void nspot_light_component::serialize(narchive& ar)
	{
		nlight_component::serialize(ar);
		nSERIALIZE(ar,m_attenuation);
		nSERIALIZE(ar,inner_cone_angle);
		nSERIALIZE(ar,outer_cone_angle);
	}

	void nspot_light_component::draw_bounds( nrender_primitive_draw_interface* PDI, const color4f& color )
	{
		PDI->draw_wired_box(m_bounds.get_AABBox(), color);
	}
}//namespace nexus