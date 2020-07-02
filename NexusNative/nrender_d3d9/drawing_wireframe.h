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

		virtual void draw_mesh(const d3d_view_info* view, const nrender_proxy* obj);
	
		static void create_type(dp_type_list& type_list);
	protected:
		static nstring	s_dp_name_str;
	};
}//namespace nexus
#endif //_NEXUS_DRAWING_WIREFRAME_H_