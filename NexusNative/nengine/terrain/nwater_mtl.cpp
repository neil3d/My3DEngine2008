#include "StdAfx.h"
#include "nwater_mtl.h"
#include "../resource/nresource_manager.h"
#include "terrain_water.h"
#include "../framework/nlevel.h"

namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(nwater_mtl, nmaterial_base)
	nDEFINE_NAMED_CLASS(nwater_mtl_basic, nwater_mtl)
	nDEFINE_NAMED_CLASS(nwater_mtl_natural, nwater_mtl)

	////////////////////////////////////////////////////////////////////////////
	//	class nwater_mtl
	////////////////////////////////////////////////////////////////////////////
	nwater_mtl::nwater_mtl(const nstring& name_str):nmaterial_base(name_str)
	{
	}

	nwater_mtl::~nwater_mtl(void)
	{
	}

	////////////////////////////////////////////////////////////////////////////
	//	class nwater_mtl_basic
	////////////////////////////////////////////////////////////////////////////
	nwater_mtl_basic::nwater_mtl_basic(const nstring& name_str):nwater_mtl(name_str)
	{}

	void nwater_mtl_basic::create(const resource_location& texture_loc)
	{
		m_template = nresource_manager::instance()->load_material_template_script(
			resource_location(_T("engine_data:material/TRN_mtl_basic.hlsl"))
			);

		m_diffuse_map = nresource_manager::instance()->load_texture_2d(texture_loc);
	}

	void nwater_mtl_basic::draw_effect_param(nshading_effect* effect_ptr) const
	{
		effect_ptr->set_texture("MTL_DiffuseMap", m_diffuse_map->get_render_texture());
	}

	void nwater_mtl_basic::serialize(narchive& ar)
	{
		nwater_mtl::serialize(ar);

		resource_location diffuse_tex;
		if (ar.is_loading())
		{
			nSERIALIZE(ar, diffuse_tex);
			this->create(diffuse_tex);
		}
		else
		{
			diffuse_tex = m_diffuse_map->get_location();
			nSERIALIZE(ar, diffuse_tex);
		}
	}

	////////////////////////////////////////////////////////////////////////////
	//	class nwater_mtl_natural
	////////////////////////////////////////////////////////////////////////////
	nwater_mtl_natural::nwater_mtl_natural(const nstring& name_str):nwater_mtl(name_str),
		m_owner(NULL), m_rt_size(0,0)
	{}

	void nwater_mtl_natural::create(nterrain_water* owner, int render_target_w, int render_target_h,
		const resource_location& detail_map, const resource_location& bump_map)
	{
		m_owner = owner;

		//--
		m_template = nresource_manager::instance()->load_material_template_script(
			resource_location(_T("engine_data:material/TRN_water_natural.hlsl"))
			);

		m_detail_map = nresource_manager::instance()->load_texture_2d(detail_map);
		m_bump_map = nresource_manager::instance()->load_texture_2d(bump_map);

		//--
		float water_h = m_owner->get_water_height_scaled();
		nscene_capture_reflect::ptr reflect_cap( new nscene_capture_reflect );
		reflect_cap->create(render_target_w, render_target_h);
		reflect_cap->set_plane( plane(vector3(0,1,0), -water_h) );

		nscene_capture_refract::ptr refract_cap( new nscene_capture_refract );
		refract_cap->create(render_target_w, render_target_h);
		refract_cap->set_plane( plane(vector3(0,-1,0), water_h+128) );//waterH向上移动一点,防止水面边缘出现黑边

		owner->get_owner()->get_owner()->add_scene_capture(reflect_cap);
		owner->get_owner()->get_owner()->add_scene_capture(refract_cap);

		//--
		m_reflection = reflect_cap;
		n_refraction = refract_cap;		

		m_rt_size.x = render_target_w;
		m_rt_size.y = render_target_h;
		m_detail_res = detail_map;
		m_bump_res = bump_map;
	}

	void nwater_mtl_natural::destroy()
	{
		nlevel* lv = m_owner->get_owner()->get_owner();
		
		lv->remove_scene_capture(m_reflection);
		m_reflection.reset();

		lv->remove_scene_capture(n_refraction);
		n_refraction.reset();
	}

	void nwater_mtl_natural::draw_effect_param(nshading_effect* effect_ptr) const
	{
		effect_ptr->set_matrix("TRN_WATER_MatReflection", m_reflection->get_uv_project_matrix());
		effect_ptr->set_texture("MTL_ReflectMap", m_reflection->get_texture_target());

		effect_ptr->set_matrix("TRN_WATER_MatRefract", n_refraction->get_uv_project_matrix());
		effect_ptr->set_texture("MTL_RefractMap", n_refraction->get_texture_target());

		effect_ptr->set_texture("MTL_DetailMap", m_detail_map->get_render_texture());
		effect_ptr->set_texture("MTL_BumpMap", m_bump_map->get_render_texture());
	}

	void nwater_mtl_natural::serialize(narchive& ar)
	{
		nwater_mtl::serialize(ar);

		nSERIALIZE(ar, m_rt_size);
		nSERIALIZE(ar, m_detail_res);
		nSERIALIZE(ar, m_bump_res);
	}

	void nwater_mtl_natural::_level_loaded(nterrain_water* owner)
	{
		nwater_mtl::_level_loaded(owner);
		this->create(owner, m_rt_size.x, m_rt_size.y,
			m_detail_res,
			m_bump_res
			);
	}
}//namespace nexus