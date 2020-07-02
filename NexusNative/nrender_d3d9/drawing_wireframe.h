/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_DRAWING_WIREFRAME_H_
#define _NEXUS_DRAWING_WIREFRAME_H_
#include "drawing_policy.h"

namespace nexus
{
	class drawing_wireframe :
		public drawing_policy
	{
	public:
		drawing_wireframe(void);
		virtual ~drawing_wireframe(void);

		static void create_type(dp_type_list& type_list);
		void setup_effect(d3d9_shading_effect* effect_ptr, const nprimitive_component* mesh_comp);
	protected:
		static nstring	s_dp_name_str;
	};
}//namespace nexus
#endif //_NEXUS_DRAWING_WIREFRAME_H_