/*
*	Nexus Engine - render targets
*
*	Author: Neil
*	Date:	Nov, 2008
*/

#ifndef _NEXUS_D3D9_SCENE_RENDER_TARGETS_H_
#define _NEXUS_D3D9_SCENE_RENDER_TARGETS_H_
#include "d3d_ptr.h"
#include "post_process.h"
#include "hdr_final_pass.h"
#include "d3d_device_manager.h"

namespace nexus
{
	class nrender_target;

	const D3DCOLOR _clear_color = D3DCOLOR_ARGB(0, 125, 125, 125);
	const vector2 shadow_buffer_size(4096,4096);
	const float pixel_offset = 0.5f;

	enum ERenderTargetUsage
	{
		ERT_SceneColor = 0,
		ERT_SceneColorCopy,
		ERT_SceneDepth,	// D24S8格式的z buffer
		ERT_SceneNormalDepth,	// linear normal and depth
		ERT_ShadowColor,
		ERT_ShadowDepth,
		ERT_Fliter,
		ERT_Light,
		ERT_Num
	};

	/**
	 *	渲染管线中通用的render targets的共享管理
	*/
	class scene_render_targets
	{
	public:
		scene_render_targets(void);
		virtual ~scene_render_targets(void);

		static void create_instance(const render_config& cfg);
		static void destroy_instance();
		static scene_render_targets* instance();

		static rt_item create_item(UINT w, UINT h, D3DFORMAT fmt, DWORD usage=D3DUSAGE_RENDERTARGET);

		virtual void begin_pre_pass();
		virtual void end_pre_pass() 
		{
			d3d_device_manager::instance()->get_device()->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORALL);
		};

		virtual void begin_scene_color(bool clear = false,bool output_normal_depth = false);
		virtual void end_scene_color(bool bcommit = false);		

		virtual void draw_final_scene(bool enable_tonemap);
		virtual const rt_item& get_render_target(ERenderTargetUsage id)
		{
			return m_items[id];
		}
	
		void draw_screen_quad(ID3DXEffect* eft);		
		// void draw_screen_quad(ID3DXEffect* eft, int src_w, int src_h);		

		const D3DSURFACE_DESC& get_back_surface_desc() const	{	return m_back_desc; }
		d3d_surface_ptr get_back_surface() const		{	return m_back_surface;}

		void begin_back_buffer();
		void end_back_buffer();

		void debug_draw_target(const rt_item& rt, int index, const char* tech="techDefault");

		virtual void copy_render_target(nrender_target* out_tg);

		void on_device_lost(int param);
		bool on_device_reset(int param);

		d3d_cube_texture_ptr m_cube_texture;
		d3d_surface_ptr		 m_cube_depth;
		
		static vector4 calc_screen_scale_bias();
	protected:
		virtual void create(const render_config& cfg);				

		D3DSURFACE_DESC m_back_desc;
		d3d_surface_ptr	m_back_surface;	// back surface
		d3d_surface_ptr	m_back_depth;
	
		d3d_ptr<ID3DXEffect>	m_pass_through_effect;
		
	protected:
		struct vertex
		{
			vector3	pos;
			vector2	uv;
		};

		void adjust_screen_quad_uv(int tex_w, int tex_h, vertex vert_data[4]);

		vertex									m_quad_vert_adjust[4];
		d3d_vb_ptr								m_default_quad_vb;		// 默认texture大小与back buffer大小一致
		d3d_ptr<IDirect3DVertexDeclaration9>	m_vert_decl;				

		static scene_render_targets*			s_inst;
		hdr_final_pass							m_hdr_final_pass;
		rt_item									m_items[ERT_Num];
	};

}//namespace nexus
#endif