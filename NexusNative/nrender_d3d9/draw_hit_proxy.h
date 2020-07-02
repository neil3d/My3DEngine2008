#ifndef _NEXUS_DRAW_HIT_PROXY_H_
#define _NEXUS_DRAW_HIT_PROXY_H_
#include "drawing_policy.h"
#include "drawing_pre_pass.h"

namespace nexus
{
	class draw_hit_proxy :
		public drawing_policy
	{
	public:
		draw_hit_proxy(void);
		virtual ~draw_hit_proxy(void);

		static void create_type(dp_type_list& type_list);				
	protected:
		virtual void setup_effect(d3d9_shading_effect* effect_ptr, const nprimitive_component* mesh_comp);

		static nstring	s_dp_name_str;
	};
}//namespace nexus

#endif //_NEXUS_DRAW_HIT_PROXY_H_