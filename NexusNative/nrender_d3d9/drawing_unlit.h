/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_DRAWING_UNLIT_H_
#define _NEXUS_DRAWING_UNLIT_H_

#include "drawing_policy.h"

namespace nexus
{
	class drawing_unlit :
		public drawing_policy
	{
	public:
		drawing_unlit(void);
		virtual ~drawing_unlit(void);

		virtual void draw_mesh(const d3d_view_info* view, const nrender_proxy* obj);
		virtual void setup_effect(d3d9_shading_effect* effect_ptr, const nrender_proxy* obj, size_t section_index);

		void set_effect_tech(const std::string& tech)
		{
			m_tech = tech;
		}
	private:
		std::string	m_tech;

	public:
		static void create_type(dp_type_list& type_list);
	protected:
		static nstring	s_dp_name_str;
	};
}//namespace nexus
#endif //_NEXUS_DRAWING_UNLIT_H_