/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_DRAWING_POLICY_H_
#define _NEXUS_DRAWING_POLICY_H_
#include "drawing_policy_type.h"

namespace nexus
{
	class d3d9_shading_effect;

	class drawing_filter
	{
	public:
		static bool should_draw(const nprimitive_component* component)
		{
			return true;
		}
	};

	/**
	 *	渲染策略的基类
	*/
	class drawing_policy
	{
	public:
		drawing_policy(void);
		virtual ~drawing_policy(void)	{}
		
		drawing_policy_type* get_type()
		{
			return m_type.get();
		}

		void set_effect_tech(const std::string& tech)
		{
			m_tech = tech;
		}

		virtual void setup_effect(d3d9_shading_effect* effect_ptr, const nprimitive_component* prim_comp);
		virtual void draw(const nview_info* view,const vector<const struct mesh_element*>& mesh_elements);
		virtual void draw(const nview_info* view,const mesh_element& element);
		enum EDrawingPolicy get_policy_tech()			{   return m_policy_tech; }

	protected:
		virtual void set_technique(d3d9_shading_effect* effect_ptr);

		static void default_create_type(dp_type_list& type_list, const nstring& dp_name);

		std::string m_tech;
		drawing_policy_type::ptr		m_type;
		enum EDrawingPolicy			m_policy_tech;
	};
}//namespace nexus

#endif //_NEXUS_DRAWING_POLICY_H_