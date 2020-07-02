#ifndef _NEXUS_SHADOW_BUFFER_H_
#define _NEXUS_SHADOW_BUFFER_H_
#include "shadow_policy_base.h"
#include "drawing_pre_pass.h"
#include "d3d_ptr.h"
#include "shading_effect.h"

namespace nexus
{
	class shadow_buffer_gen_filter : public drawing_filter
	{
	public:
		virtual bool should_draw(const nrender_proxy* obj)
		{
			return obj->cast_dynamic_shadow();
		}
	};

	class shadow_buffer :
		public shadow_policy_base
	{
	public:
		shadow_buffer(void);
		virtual ~shadow_buffer(void);

		void create(size_t rt_size);
		void begin_generate(const ndirectional_light_proxy* lgt, const d3d_view_info* view);
		drawing_policy* get_drawing_policy()	{	return &m_dp; }
		d3d_view_info* get_view()	{	return &m_lgt_view; }
		void end_generate();

		static nstring get_name(void)	{	return nstring(_T("shadow_buffer"));}
		static void modify_drawing_policy_type(light_drawing_policy_type* dp_type);
		virtual void set_effect_parameter(d3d9_shading_effect* effect_ptr);

	public:
		drawing_pre_pass	m_dp;
		d3d_view_info		m_lgt_view;
		ncamera				m_camera;

		d3d_texture_ptr		m_shadow_tex;
		d3d_surface_ptr		m_surface;

		d3d_texture_ptr		m_color_tex;
		d3d_surface_ptr		m_color_surface;	

		d3d_surface_ptr		m_old_target,
							m_old_zbuffer;

		D3DVIEWPORT9	m_old_viewport;
		size_t			m_rt_size;
	};
}//namespace nexus

#endif //_NEXUS_SHADOW_BUFFER_H_