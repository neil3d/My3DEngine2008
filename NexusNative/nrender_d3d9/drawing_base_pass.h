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
	class drawing_base_pass :
		public drawing_policy
	{
	public:
		drawing_base_pass(void);
		virtual ~drawing_base_pass(void);

		virtual void draw_mesh(const d3d_view_info* view, const nrender_proxy* obj);
		
		static void create_type(dp_type_list& type_list);
	protected:
		static nstring	s_dp_name_str;		
	};
}//namespace nexus

#endif //_NEXUS_DRAWING_BASE_PASS_H_