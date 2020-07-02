#include "StdAfx.h"
#include "drawing_pre_pass.h"
#include "d3d9_vertex_factory.h"
#include "shading_effect.h"
#include "d3d_device_manager.h"
#include "d3d9_mesh_section.h"
#include "nrenderer_d3d9.h"
#include "util.h"

namespace nexus
{	
	nstring	drawing_pre_pass::s_dp_name_str(_T("drawing_pre_pass"));

	drawing_pre_pass::drawing_pre_pass(void)
	{
		m_type = shading_effect_lib::instance()->get_draw_policy_type(
			s_dp_name_str
			);
	}

	drawing_pre_pass::~drawing_pre_pass(void)
	{
	}

	void drawing_pre_pass::draw_mesh(const d3d_view_info* view, const nrender_proxy* obj)
	{
		int lod = obj->get_render_lod();
		nrender_mesh* res_ptr = obj->get_render_mesh(lod);
		if(res_ptr == NULL)
			return;

		// 找到无需material的shader
		d3d9_vertex_factory* vf = static_cast<d3d9_vertex_factory*>(res_ptr->get_vertex_factory());		
		
		for(size_t i=0; i<res_ptr->get_num_section(); i++)
		{
			enum ETransparentType trans = obj->get_transparent_type(lod, i);

			// 透明物体不影响z buffer
			if( trans == ETrans_UnlitTrans)
				continue;

			if(trans == ETrans_Opaque)
			{
				nrender_mesh_section* sec = res_ptr->get_section(i);
				nmaterial_base* mtl = obj->get_material( obj->get_render_lod(), sec->get_material_id() );
				bool two_sided = false;
				if( mtl )
					two_sided = mtl->is_two_sided();

				d3d9_shading_effect* shader = shading_effect_lib::instance()->acquire_shader_default_mtl(
					m_type.get(), vf->get_type(), 
					res_ptr->get_shader_mod(),
					two_sided );

				//-- 简单的不透明物体使用默认材质画出
				draw_shared_no_material(shader, view, obj, i);
			}
			else if(trans == ETrans_AlphaMasked
				|| trans == ETrans_Mixed)
			{
				//-- 使用Alpha Mask材质的对象需要访问材质的opacity通道
				// 使用mixed transparent材质的对象，将不透明部分的z画出
				draw_shared_material(view, obj, i);
			}			
		}
	}

	void drawing_pre_pass::create_type(dp_type_list& type_list)
	{
		default_create_type(type_list, s_dp_name_str);		
	}

	void drawing_pre_pass::setup_effect(d3d9_shading_effect* effect_ptr, const nrender_proxy* obj, size_t section_index)
	{
		enum ETransparentType trans_type = obj->get_transparent_type(obj->get_render_lod(), section_index);

		ID3DXEffect* d3d_eft = effect_ptr->get_d3d_effect();
		HRESULT hr;

		if(trans_type == ETrans_Mixed)
		{
			hr = d3d_eft->SetTechnique("techMixedTrans");
		}
		else
		{
			hr = d3d_eft->SetTechnique("techDefault");
		}
	}
}//namespace nexus