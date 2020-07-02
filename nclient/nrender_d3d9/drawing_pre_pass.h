/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_DRAWING_PRE_PASS_H_
#define _NEXUS_DRAWING_PRE_PASS_H_
#include "drawing_policy.h"

namespace nexus
{
	class drawing_pre_pass :
		public drawing_policy
	{
	public:
		drawing_pre_pass(void);
		virtual ~drawing_pre_pass(void);

		virtual void draw_mesh(const d3d_view_info* view, const nrender_proxy* obj);

		static void create_type(dp_type_list& type_list);
	protected:
		virtual void setup_effect(d3d9_shading_effect* effect_ptr, const nrender_proxy* obj, size_t section_index);

		static nstring	s_dp_name_str;
	};
}//namespace nexus
#endif //_NEXUS_DRAWING_PRE_PASS_H_