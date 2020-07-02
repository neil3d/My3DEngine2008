/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_DRAWING_POLICY_H_
#define _NEXUS_DRAWING_POLICY_H_
#include "d3d_view_info.h"
#include "drawing_policy_type.h"

namespace nexus
{
	class d3d9_shading_effect;

	class drawing_filter
	{
	public:
		virtual bool should_draw(const nrender_proxy* obj) = 0;
	};

	/**
	 *	渲染策略的基类
	*/
	class drawing_policy 		
	{
	public:
		drawing_policy(void);
		virtual ~drawing_policy(void)	{	}

		virtual void draw_mesh(const d3d_view_info* view, const nrender_proxy* obj) = 0;
		// hardware instance drawing
		//virtual void draw_mesh_packet();

	protected:
		virtual void setup_effect(d3d9_shading_effect* effect_ptr, const nrender_proxy* obj, size_t section_index);

		// 不使用材质的draw，即可以将mesh的所有section在一个effect pass中画出
		void draw_shared_no_material(d3d9_shading_effect* shader, const d3d_view_info* view, const nrender_proxy* obj, size_t section_index);		

		void draw_shared_material(const d3d_view_info* view, const nrender_proxy* obj, size_t section_index);		

		static void default_create_type(dp_type_list& type_list, const nstring& dp_name);

		drawing_policy_type::ptr	m_type;
	};
}//namespace nexus

#endif //_NEXUS_DRAWING_POLICY_H_