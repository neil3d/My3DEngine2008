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
		virtual void draw_mesh(const d3d_view_info* view, const nrender_proxy* obj);

		void set_effect_tech(const std::string& tech)
		{
			m_tech = tech;
		}
	protected:
		virtual void setup_effect(d3d9_shading_effect* effect_ptr, const nrender_proxy* obj, size_t section_index);

		static nstring	s_dp_name_str;

	private:
		std::string	m_tech;
	};
}//namespace nexus

#endif //_NEXUS_DRAW_HIT_PROXY_H_