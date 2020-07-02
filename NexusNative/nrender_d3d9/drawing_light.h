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
#include "spot_light_policy.h"
#include "point_light_policy.h"
#include "directional_light_policy.h"
#include "shading_effect.h"
#include "nrender_light.h"

namespace nexus
{
	/**
	 *	动态灯光绘制所使用的drawing policy
	*/
	template<
		typename TLightPolicy,
		int ShadowQuality=0
	>
	class drawing_light
		: public drawing_policy
	{
	public:
		drawing_light(const nrender_light_proxy* lgt):m_light_proxy(lgt)
		{
			m_type = shading_effect_lib::instance()->get_draw_policy_type(
				get_type_name()
				);

			m_policy_tech = TLightPolicy::get_policy_type();
		}

		virtual ~drawing_light(void)	{}

		static nstring get_type_name(void)
		{
			wostringstream ss;
			ss << _T("draw_light_")
				<< TLightPolicy::get_name()
				<< _T('_')
				<< ShadowQuality;
			return ss.str();
		}

		static void create_type(dp_type_list& type_list)
		{
			shared_ptr<light_drawing_policy_type> new_type(nNew light_drawing_policy_type(get_type_name()));
			TLightPolicy::modify_drawing_policy_type(new_type.get());
			if (ShadowQuality == 0)
			{
				new_type->macros.push_back(shader_define("SHADOW_QUALITY", "0"));
			} 
			else if(ShadowQuality == 1)
			{
				new_type->macros.push_back(shader_define("SHADOW_QUALITY", "1"));
			}
			else if (ShadowQuality == 2)
			{
				new_type->macros.push_back(shader_define("SHADOW_QUALITY", "2"));
			}
			
			new_type->load_shader_file();

			type_list.add_drawing_policy_type(new_type);
		}
	protected:
		virtual void set_technique(d3d9_shading_effect* effect_ptr)
		{
			effect_ptr->get_d3d_effect()->SetTechnique("techOpaque");
		}

		virtual void setup_effect(d3d9_shading_effect* effect_ptr, const nprimitive_component* mesh_comp)
		{
			(void)mesh_comp;
			m_light_proxy->draw_setup_effect(effect_ptr);
		}
	private:		
		TLightPolicy			m_light_policy;
		const nrender_light_proxy*	m_light_proxy;
	};
}//namespace nexus
#endif //_NEXUS_D3D9_DRAWING_LIGHT_H_