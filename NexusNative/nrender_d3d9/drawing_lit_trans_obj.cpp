#include "StdAfx.h"
#include "drawing_lit_trans_obj.h"
#include "scene_render_targets.h"
#include "drawing_light.h"
#include "util.h"

namespace nexus
{
	drawing_lit_trans_obj::drawing_lit_trans_obj(const light_vector& lights):m_lights(lights)
	{		
	}

	drawing_lit_trans_obj::~drawing_lit_trans_obj(void)
	{
	}

	void drawing_lit_trans_obj::draw_mesh(const d3d_view_info* view, const nrender_proxy* obj)
	{
		scene_render_targets::instance()->begin_transparency_buffer();

		size_t num_lights = m_lights.size();
		for(size_t i=0; i<num_lights; i++)
		{
			const nrender_light_proxy* lgt = m_lights[i];

			//-- render to scene color
			switch(lgt->m_type)
			{
			case ELight_Point:
				{
					drawing_light<point_light_policy, noshadow> dp(lgt);					
					dp.set_tech(EDrawLight_HighTransPass);
					dp.draw_mesh(view, obj);
				}
				break;
			case ELight_Directional:				
				{
					
					drawing_light<directional_light_policy, noshadow> dp(lgt);					
					dp.set_tech(EDrawLight_HighTransPass);
					dp.draw_mesh(view, obj);
				}
				break;
			default:
				nASSERT(0 && "unknown light type");
			}
		}

		scene_render_targets::instance()->end_transparency_buffer();
	}

	void light_drawing_policy_type::load_shader_file()
	{			
		nstring dp_name = _T("shader_d3d9/drawing_light.fx");
		load_shder_source(dp_name, shader_source);
		load_shder_source(m_light_policy, m_light_code);
		load_shder_source(m_shadow_policy, m_shadow_code);
	}
}//namespace nexus