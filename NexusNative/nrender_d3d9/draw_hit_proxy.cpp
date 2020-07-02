#include "StdAfx.h"
#include "draw_hit_proxy.h"
#include "shading_effect.h"
#include "d3d9_vertex_factory.h"
#include "../nengine/actor/nmesh_component.h"

namespace nexus
{
	nstring	draw_hit_proxy::s_dp_name_str(_T("draw_hit_proxy"));

	draw_hit_proxy::draw_hit_proxy(void)
	{
		m_type = shading_effect_lib::instance()->get_draw_policy_type(
			s_dp_name_str
			);
		m_policy_tech = EDP_Hit_Proxy;
	}

	draw_hit_proxy::~draw_hit_proxy(void)
	{
	}

	void draw_hit_proxy::create_type(dp_type_list& type_list)
	{
		default_create_type(type_list, s_dp_name_str);		
	}

	void draw_hit_proxy::setup_effect(d3d9_shading_effect* effect_ptr, const nprimitive_component* mesh_comp)
	{
		if( !mesh_comp )
			return;

		hit_id hid = mesh_comp->get_hit_proxy_id();
		color4ub c;
		memcpy(&c, &hid, sizeof(hid));
		vector4 v(c.B/255.0f, c.G/255.0f, c.R/255.0f, c.A/255.0f);
		effect_ptr->set_vector("Sys_HitProxyID", v);
	}
}//namespace nexus