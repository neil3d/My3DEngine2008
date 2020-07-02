/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Nov, 2008
*/

#ifndef _NEXUS_DRAWING_BASE_PASS_H_
#define _NEXUS_DRAWING_BASE_PASS_H_
#include "drawing_policy.h"

namespace nexus
{
	template<typename TAmbentLightPolicy>
	class drawing_base_pass :
		public drawing_policy
	{
		typedef typename TAmbentLightPolicy::context context;
	public:
		drawing_base_pass(const context& context_data):
		context_data(context_data)
		{
			m_type = shading_effect_lib::instance()->get_draw_policy_type(
				get_type_name()
				);
			m_policy_tech = EDP_BaseColor;
		}

		virtual ~drawing_base_pass(void)
		{

		}

		static nstring get_type_name(void)
		{
			wostringstream ss;
			ss << _T("draw_light_")
				<< TAmbentLightPolicy::get_name();

			return ss.str();
		}

		static void create_type(dp_type_list& type_list)
		{
			//default_create_type(type_list, s_dp_name_str);	
			shared_ptr<base_pass_policy_type> new_type(nNew base_pass_policy_type(get_type_name()));

			TAmbentLightPolicy::modify_drawing_policy_type(new_type.get());
			new_type->load_shader_file();

			type_list.add_drawing_policy_type(new_type);
		}

		virtual void setup_effect( d3d9_shading_effect* effect_ptr, const nprimitive_component* prim_comp )
		{
			TAmbentLightPolicy::set_effect_parameter(effect_ptr,context_data);
		}

	protected:
		context context_data;
	};
}//namespace nexus

#endif //_NEXUS_DRAWING_BASE_PASS_H_