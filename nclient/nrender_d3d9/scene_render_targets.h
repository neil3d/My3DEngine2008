/**
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
	/**
	 *	渲染管线中通用的render targets的共享管理
	*/
	class scene_render_targets
	{
	public:
		enum ERenderType
		{
			HDR,
			LDR,
		};
	public:
		scene_render_targets(void);
		virtual ~scene_render_targets(void);

		static void create_instance(enum ERenderType type, const render_config& cfg);
		static void destroy_instance();
		static scene_render_targets* instance();

		static rt_item create_item(UINT w, UINT h, D3DFORMAT fmt, DWORD usage=D3DUSAGE_RENDERTARGET);

		virtual void begin_pre_pass() = 0;
		virtual void end_pre_pass() = 0;

		virtual void begin_scene_color() = 0;
		virtual void end_scene_color() = 0;

		virtual void begin_transparency_buffer() = 0;
		virtual void end_transparency_buffer() = 0;

		virtual void draw_final_scene(bool enable_tonemap) = 0;
		virtual const rt_item& get_render_target(int id) = 0;
	
		void draw_screen_quad(ID3DXEffect* eft);		
		// void draw_screen_quad(ID3DXEffect* eft, int src_w, int src_h);		

		const D3DSURFACE_DESC& get_back_surface_desc() const	{	return m_back_desc; }
		d3d_surface_ptr get_back_surface() const		{	return m_back_surface;}

		void begin_back_buffer();
		void end_back_buffer();

		void debug_draw_target(const rt_item& rt, int index, const char* tech="techDefault");
	protected:
		virtual void create(const render_config& cfg);				

		D3DSURFACE_DESC m_back_desc;
		d3d_surface_ptr	m_back_surface;	// back surface
		d3d_surface_ptr	m_back_depth;

		d3d_ptr<ID3DXEffect>	m_pass_through_effect;
	private:
		struct vertex
		{
			vector3	pos;
			vector2	uv;
		};

		void adjust_screen_quad_uv(int tex_w, int tex_h, vertex vert_data[4]);

		vertex									m_quad_vert_adjust[4];
		d3d_vb_ptr								m_default_quad_vb;		// 默认texture大小与back buffer大小一致
		d3d_ptr<IDirect3DVertexDeclaration9>	m_vert_decl;				

		static scene_render_targets* s_inst;
	};

	
	class scene_render_targets_HDR
		: public scene_render_targets
	{
	public:
		enum EItemUsage
		{
			EItem_SceneColor = 0,
			EItem_SceneDepth,		
			
			EItem_Num
		};
	public:
		scene_render_targets_HDR(void);
		virtual ~scene_render_targets_HDR(void);

		virtual void begin_pre_pass();
		virtual void end_pre_pass();
		virtual void begin_scene_color();
		virtual void end_scene_color();
		virtual void begin_transparency_buffer();
		virtual void end_transparency_buffer();
		
		virtual void draw_final_scene(bool enable_tonemap);

		virtual const rt_item& get_render_target(int id)
		{
			nASSERT( id>=0 && id<EItem_Num);
			enum EItemUsage u = static_cast<enum EItemUsage>(id);
			return m_items[u];
		}
	private:
		virtual void create(const render_config& cfg);
		
		rt_item					m_items[EItem_Num];
		hdr_final_pass			m_hdr_final_pass;
	};

	const D3DCOLOR _clear_color = D3DCOLOR_ARGB(0, 64, 64, 192);

	class scene_render_targets_LDR
		: public scene_render_targets
	{
	public:
		scene_render_targets_LDR(void)	{}
		virtual ~scene_render_targets_LDR(void)	{}

		virtual void begin_pre_pass()	
		{
			IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
			HRESULT hr;
			hr = device->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, _clear_color, 1, 0);
		}
		virtual void end_pre_pass()		{}

		virtual void begin_scene_color()
		{
		/*	IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
			HRESULT hr;
			hr = device->Clear(0, NULL, D3DCLEAR_TARGET, _clear_color, 1, 0);		*/
		}
		virtual void end_scene_color()	{}

		virtual void draw_final_scene(bool enable_tonemap)	{}

		virtual const rt_item& get_render_target(int id)
		{
			return m_dummy_rt;
		}

		virtual void begin_transparency_buffer();
		virtual void end_transparency_buffer();

	protected:
		virtual void create(const render_config& cfg);				
	private:
		rt_item	m_dummy_rt;
		rt_item m_trans_buffer;
	};
}//namespace nexus
#endif