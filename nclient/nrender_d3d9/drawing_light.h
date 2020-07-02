/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Nov, 2008
*/

#ifndef _NEXUS_D3D9_DRAWING_LIGHT_H_
#define _NEXUS_D3D9_DRAWING_LIGHT_H_
#include <sstream>
#include "drawing_policy.h"
#include "light_drawing_policy_type.h"
#include "point_light_policy.h"
#include "directional_light_policy.h"
#include "shading_effect.h"

namespace nexus
{
	enum EDrawingLightTech
	{
		EDrawLight_Opaque,
		EDrawLight_LowTransPass,
		EDrawLight_HighTransPass,
	};

	/**
	 *	动态灯光绘制所使用的drawing policy
	*/
	template<
		typename TLightPolicy,
		bool VertexLighting=false,
		bool LightOnly=false
	>
	class drawing_light
		: public drawing_policy
	{
	public:
		drawing_light(const nrender_light_proxy* lgt):m_light(lgt)
		{	
			m_tech = EDrawLight_Opaque;

			m_type = shading_effect_lib::instance()->get_draw_policy_type(
				get_type_name()
				);
		}
		virtual ~drawing_light(void)	{	}

		void set_tech(enum EDrawingLightTech tech)
		{
			m_tech = tech;
		}

		static nstring get_type_name(void)
		{
			wostringstream ss;
			ss << _T("draw_light_")
				<< TLightPolicy::get_name();
			return ss.str();
		}

		static void create_type(dp_type_list& type_list)
		{
			wostringstream ss;
			ss << _T("shader_d3d9/")
				<< _T("drawing_light")
				<< _T(".fx");
			nstring dp_name = ss.str();

			shared_ptr<light_drawing_policy_type> new_type(nNew light_drawing_policy_type(get_type_name()));
			load_shder_source(dp_name, new_type->shader_source);

			if(VertexLighting)
				new_type->macros.push_back(shader_define("VERTEX_LIGHTING", ""));

			TLightPolicy::modify_drawing_policy_type(new_type.get());

			type_list.add_drawing_policy_type(new_type);
		}

		virtual void draw_mesh(const d3d_view_info* view, const nrender_proxy* obj)
		{
			nrender_mesh* res_ptr = obj->get_render_mesh(obj->get_render_lod());
			if(res_ptr == NULL)
				return;

			for(size_t i=0; i<res_ptr->get_num_section(); i++)
			{
				draw_shared_material(view, obj, i);
			}
		}

	protected:
		virtual void setup_effect(d3d9_shading_effect* effect_ptr, const nrender_proxy* obj, size_t section_index)
		{
			ID3DXEffect* d3d_eft = effect_ptr->get_d3d_effect();
			HRESULT hr;

			switch(m_tech)
			{
			case EDrawLight_Opaque:
				hr = d3d_eft->SetTechnique("techOpaque");
				break;
			case EDrawLight_LowTransPass:
				hr = d3d_eft->SetTechnique("techLowTransPass");
				break;
			case EDrawLight_HighTransPass:
				hr = d3d_eft->SetTechnique("techHighTransPass");
				break;
			default:
				nASSERT(0 && "unknown lighting tech");
				break;
			}

			m_light_policy.set_effect_parameter(effect_ptr, m_light);
		}
	private:
		EDrawingLightTech		m_tech;
		TLightPolicy			m_light_policy;
		const nrender_light_proxy*	m_light;
	};
}//namespace nexus
#endif //_NEXUS_D3D9_DRAWING_LIGHT_H_