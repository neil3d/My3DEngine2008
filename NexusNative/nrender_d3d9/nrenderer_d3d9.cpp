#include "StdAfx.h"
#include "../nengine/image/nimage.h"
#include "nrenderer_d3d9.h"
#include "d3d_device_manager.h"
#include "shading_effect.h"
#include "drawing_pre_pass.h"
#include "drawing_wireframe.h"
#include "drawing_unlit.h"
#include "drawing_base_pass.h"
#include "vertex_factory_type.h"
#include "drawing_light.h"
#include "scene_render_targets.h"
#include "drawing_lit_trans_obj.h"
#include "drawing_unlit_trans.h"
#include "d3d9_hit_proxy_hash.h"
#include "draw_hit_proxy.h"
#include "dynamic_vb_pool.h"
#include "global_shader_lib.h"

#ifdef _DEBUG
#	pragma comment(lib,"../nengine/SPT_SDK/lib/vc9/debug/SpeedTreeRT_d.lib")
#else
#	pragma comment(lib,"../nengine/SPT_SDK/lib/vc9/release/SpeedTreeRT.lib")
#endif

namespace nexus
{
	nDEFINE_CLASS(nrenderer_d3d9, nrenderer_base)

	nfile_system::ptr g_file_sys;
	nstring g_engine_package_name(_T("engine_data"));
	
	static nrenderer_d3d9* g_instance = NULL;

	nrenderer_d3d9::nrenderer_d3d9(void)
	{
		g_instance = this;

		m_render_mode = ERM_Lit;
		m_render_group[EDG_Background]	= &m_bg_group;
		m_render_group[EDG_World]		= &m_world_group;
		m_render_group[EDG_Foreground]	= &m_fore_group;
	}

	nrenderer_d3d9::~nrenderer_d3d9(void)
	{		
		g_instance = NULL;
	}

	nrenderer_d3d9* nrenderer_d3d9::instance()
	{
		return g_instance;
	}

	void nrenderer_d3d9::set_file_system(nfile_system::ptr file_sys_ptr, const nstring& engine_pkg_name)	
	{
		g_file_sys = file_sys_ptr;
		g_engine_package_name = engine_pkg_name;
	}

	void nrenderer_d3d9::init(render_config& cfg)
	{
		d3d_device_manager::instance()->create_device((HWND)cfg.window_handle,
			cfg.width, cfg.height,
			cfg.color_bits,
			cfg.bWindowed);

		if( cfg.bEnableHDR )
			scene_render_targets::create_instance(scene_render_targets::HDR, cfg);
		else
			scene_render_targets::create_instance(scene_render_targets::LDR, cfg);

		shading_effect_lib::instance()->init();

		dynamic_vb_pool::instance()->init();

		m_PDI.init(&m_view_info);

		m_shadow_buffer.reset( new shadow_buffer );
		m_shadow_buffer->create(2048);

		// todo : 使用vfs
		nstring global_shader_path = _T("../engine_data/shader_d3d9/global");
		global_shader_lib::instance()->load(global_shader_path);

		m_timer.restart();
	}

	void nrenderer_d3d9::frame_view_info(const ncamera* cam)
	{
		m_view_info.look_at = cam->get_lookat();
		m_view_info.eye_pos = cam->get_eye_pos();
		m_view_info.mat_view = cam->get_view();
		m_view_info.mat_project = cam->get_project();
		m_view_info.mat_view_project = cam->get_view()*cam->get_project();
		m_view_info.time = get_run_time();
	}

	bool nrenderer_d3d9::frame_begin(const nviewport& view)
	{
		//-- device
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		if( !device )
			return false;
		
		D3DVIEWPORT9& vp = m_view_info.d3d_view;
		vp.X      = view.x;
		vp.Y      = view.y;
		vp.Width  = view.width;
		vp.Height = view.height;
		vp.MinZ   = view.min_z;
		vp.MaxZ   = view.max_z;

		HRESULT hr;
		hr = device->SetViewport(&vp);


		if( FAILED( device->BeginScene() ) )
		{
			return false;
		}

		hr = device->Clear(0, NULL, D3DCLEAR_TARGET, 0x000000, 1, 0);
		m_render_mode = view.render_mode;
		m_view_widgets = view.widgets_render;
		return true;
	}

	void nrenderer_d3d9::frame_begin_level(const ncamera* cam)
	{
		frame_view_info(cam);

		//--
		m_lights.clear();
		m_post_handle_array.clear();
		m_spt_renderer.clear();

		//-- render group;
		for(int i=0; i<EDG_Max; i++)
			if(m_render_group[i] != NULL)
				m_render_group[i]->clear();
	}

	void nrenderer_d3d9::frame_object(const nrender_proxy* obj)
	{
		enum EDepthGroup dg = obj->get_depth_group();

		nASSERT(dg>=0 && dg<EDG_Max);
		m_render_group[dg]->add_obj(obj);

		//--
		nrender_mesh* mesh = obj->get_render_mesh(
			obj->get_render_lod() );
		if( mesh->pre_render() )
		{
			m_post_handle_array.push_back(mesh);
		}
	}

	void nrenderer_d3d9::frame_light(const nrender_light_proxy* lgt)
	{
		m_lights.push_back(lgt);
	}

	void nrenderer_d3d9::frame_end_level(nhit_proxy_hash* hit_map)
	{
		//-- render main scene
		m_world_group.sort_draw_order(&m_view_info);
		frame_render();

		if( hit_map )
			update_hit_proxy_hash(hit_map);

		//-- frame data clear
		for(render_mesh_vector::iterator iter = m_post_handle_array.begin();
			iter != m_post_handle_array.end();
			++iter)
		{
			nrender_mesh* mesh = *iter;
			mesh->post_render();
		}
	}

	void nrenderer_d3d9::update_hit_proxy_hash(nhit_proxy_hash* hit_map)
	{
		d3d9_hit_proxy_hash* d3d9_hm = dynamic_cast<d3d9_hit_proxy_hash*>(hit_map);
		d3d9_hm->begin_update();
		
		draw_hit_proxy dp;
		m_world_group.draw_opaque(&m_view_info, &dp);
		m_world_group.draw_mixed_trans(&m_view_info, &dp);
		m_spt_renderer.draw_all_trees(&m_view_info, &dp);

		m_view_widgets->update_hit_proxy(&m_PDI);
		d3d9_hm->end_update();
	}
	
	void nrenderer_d3d9::resolve_target(nrender_target* target)
	{
		scene_render_targets::instance()->copy_render_target(target);
	}

	void nrenderer_d3d9::frame_end()
	{
		//-- draw view widgets		
		scene_render_targets::instance()->draw_final_scene( m_render_mode==ERM_Lit );
		if(m_view_widgets)		
		{
			scene_render_targets::instance()->begin_back_buffer();
			m_view_widgets->draw(&m_PDI);
			scene_render_targets::instance()->end_back_buffer();
		}		

		//-- end d3d scene
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		HRESULT hr = device->EndScene();
	}

	void nrenderer_d3d9::present(void* hWindow)
	{
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();

		HRESULT hr;
#if 0
		hr = device->Present(NULL, NULL, (HWND)hWindow,	NULL);
#else
		//-- 为了在editor模式下，将部分back buffer不拉伸的显示到窗口，需要指定present的rect	
		const D3DVIEWPORT9& vp = m_view_info.d3d_view;

		RECT src_rc, dst_rc;
		src_rc.left = vp.X;
		src_rc.right = vp.X+vp.Width;
		src_rc.top = vp.Y;
		src_rc.bottom = vp.Y+vp.Height;

		dst_rc.left = dst_rc.top = 0;
		dst_rc.right = vp.Width;
		dst_rc.bottom = vp.Height;

		hr = device->Present(&src_rc, &dst_rc,
			(HWND)hWindow,
			NULL);
#endif

		//hr = D3DXSaveSurfaceToFile(_T("d:\\test.bmp"),
		//	D3DXIFF_BMP, 
		//	scene_render_targets::instance()->get_back_surface().get(),
		//	NULL,
		//	NULL);
	}

	void nrenderer_d3d9::close()
	{
		shading_effect_lib::instance()->close();

		global_shader_lib::instance()->close();
		dynamic_vb_pool::instance()->destroy();
		scene_render_targets::destroy_instance();

		vertex_factory_type_lib::instance()->destroy();
		shading_effect_lib::instance()->destroy();

		d3d_device_manager::instance()->destroy_device();
		g_file_sys.reset();
	}

	void nrenderer_d3d9::draw_background()
	{
		scene_render_targets::instance()->begin_scene_color(true);

		if( m_render_mode == ERM_Wireframe )
		{
			drawing_wireframe dp;
			m_bg_group.draw_all(&m_view_info, &dp);
		}
		else
		{
			drawing_unlit dp;
			m_bg_group.draw_all(&m_view_info, &dp);
		}
		scene_render_targets::instance()->end_scene_color();
	}

	void nrenderer_d3d9::draw_foreground()
	{
		scene_render_targets::instance()->begin_scene_color(false);

		if( m_render_mode == ERM_Wireframe )
		{
			drawing_wireframe dp;
			m_fore_group.draw_all(&m_view_info, &dp);
		}
		else
		{
			drawing_unlit dp;
			dp.set_effect_tech("techForgroundBlend");
			m_fore_group.draw_all(&m_view_info, &dp);
		}
		scene_render_targets::instance()->end_scene_color();
	}

	void nrenderer_d3d9::frame_render()
	{
		draw_background();
		draw_world();
		draw_foreground();
	}

	class drawing_unlit_filter : public drawing_filter
	{
	public:
		virtual bool should_draw(const nrender_proxy* obj)
		{
			return !obj->accept_dynamic_light();
		}
	};

	void nrenderer_d3d9::draw_world()
	{
		//-- render depth pass
		{
			scene_render_targets::instance()->begin_pre_pass();
			drawing_pre_pass dp;
			m_world_group.draw_all(&m_view_info, &dp);
			m_spt_renderer.draw_all_trees(&m_view_info, &dp);
			scene_render_targets::instance()->end_pre_pass();
		}
		
		scene_render_targets::instance()->begin_scene_color(false);
		switch(m_render_mode)
		{
		case ERM_Wireframe:
			{
				drawing_wireframe dp;
				m_world_group.draw_all(&m_view_info, &dp);
				m_spt_renderer.draw_all_trees(&m_view_info, &dp);
			}
			break;
		case ERM_Unlit:
			{
				drawing_unlit dp;
				m_world_group.draw_all(&m_view_info, &dp);
				m_spt_renderer.draw_all_trees(&m_view_info, &dp);
			}
			break;
		case ERM_Lit:
			{
				//!!! 只有World Group接受光照				
				//-- draw base pass
				{
					drawing_base_pass dp;					
					m_world_group.draw_all(&m_view_info, &dp);
					m_spt_renderer.draw_all_trees(&m_view_info, &dp);
				}

				//-- 画出那些不受灯光影响的对象
				{
					drawing_unlit_filter filter;
					drawing_unlit dp;
					m_world_group.draw_all(&m_view_info, &dp, &filter);
				}

				//-- render dynamic lights
				render_lights();
			}
			break;
		default:
			nASSERT(0 && "unsupport render mode");
			break;
		}//end of switch()
		scene_render_targets::instance()->end_scene_color();		
	}

	void nrenderer_d3d9::render_lights()
	{
		// 只有world group计算光照
		size_t num_lights = m_lights.size();

		//-- 渲染“不透明物体”，每个light一个pass
		for(size_t i=0; i<num_lights; i++)
		{
			const nrender_light_proxy* lgt = m_lights[i];

			//-- create shadow buffer

			//-- render to scene color
			switch(lgt->m_type)
			{
			case ELight_Point:
				{
					drawing_light<point_light_policy, noshadow> dp(lgt);
					m_world_group.draw_opaque(&m_view_info, &dp);					
					m_spt_renderer.draw_all_trees(&m_view_info, &dp);

					dp.set_tech(EDrawLight_LowTransPass);
					m_world_group.draw_mixed_trans(&m_view_info, &dp);					
				}
				break;
			case ELight_Directional:
				{
					const ndirectional_light_proxy* dir_lgt = 
						dynamic_cast<const ndirectional_light_proxy*>(lgt);
					generate_shadow_buffer(dir_lgt);

					drawing_light<directional_light_policy, shadow_buffer> dp(lgt);
					dp.set_shadow(m_shadow_buffer.get());
					m_world_group.draw_opaque(&m_view_info, &dp);
					
					//-- shadow map在speed tree的自身阴影上质量不佳，暂时关闭
					drawing_light<directional_light_policy, noshadow> dp_noshadow(lgt);
					m_spt_renderer.draw_all_trees(&m_view_info, &dp_noshadow);

					dp.set_tech(EDrawLight_LowTransPass);
					m_world_group.draw_mixed_trans(&m_view_info, &dp);					
				}
				break;
			default:
				nASSERT(0 && "unknown light type");
			}
		}

		//-- 渲染“透明物体”，对于每个透明物体：每个light一个pass计算出最终颜色，然后blend到scene color buffer
		{
			drawing_lit_trans_obj dp(m_lights);
			m_world_group.draw_mixed_trans(&m_view_info, &dp);
		}

		//-- 渲染“不需要光照的透明物体”
		{
			drawing_unlit_trans dp;
			m_world_group.draw_unlit_trans(&m_view_info, &dp);
		}
	}

	void nrenderer_d3d9::generate_shadow_buffer(const ndirectional_light_proxy* lgt)
	{
		shadow_buffer_gen_filter filter;

		m_shadow_buffer->begin_generate(lgt, &m_view_info);
		m_world_group.draw_opaque(m_shadow_buffer->get_view(), m_shadow_buffer->get_drawing_policy(), &filter);
		m_world_group.draw_mixed_trans(m_shadow_buffer->get_view(), m_shadow_buffer->get_drawing_policy(), &filter);
		m_spt_renderer.draw_all_trees(m_shadow_buffer->get_view(), m_shadow_buffer->get_drawing_policy());
		m_shadow_buffer->end_generate();
	}

	void nrenderer_d3d9::frame_tree(const nspt_instance* spt)
	{
		m_spt_renderer.attach_tree(spt);
	}

	static d3d_surface_ptr _do_d3d_screen_shot()
	{
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		HRESULT hr;

		const D3DSURFACE_DESC& rt_desc = scene_render_targets::instance()->get_back_surface_desc();
		rt_item color_rt = scene_render_targets::instance()->get_render_target(ERT_SceneColor);
		d3d_surface_ptr rt_surf = color_rt.surf;
		
		//-- get render target data to a system memory surface
		d3d_surface_ptr mem_surface;
		IDirect3DSurface9* new_surf = NULL;
		hr = device->CreateOffscreenPlainSurface(rt_desc.Width,
			rt_desc.Height, rt_desc.Format, D3DPOOL_SYSTEMMEM,
			&new_surf, NULL);

		mem_surface.reset(new_surf);

		hr = device->GetRenderTargetData(
			rt_surf.get(),
			mem_surface.get()
			);
		nASSERT(SUCCEEDED(hr));

		//hr = D3DXSaveSurfaceToFile(_T("d://test.bmp"), D3DXIFF_BMP, rt_surf.get(), NULL, NULL);
		return mem_surface;
	}

	void nrenderer_d3d9::screen_shot(nimage* blank_img)
	{
		HRESULT hr;

		nASSERT(blank_img != NULL);
		
		const D3DSURFACE_DESC& rt_desc = scene_render_targets::instance()->get_back_surface_desc();
		nASSERT(rt_desc.Format == D3DFMT_A8R8G8B8
			||rt_desc.Format == D3DFMT_X8R8G8B8);
		int Bpp = 4;
		blank_img->create(rt_desc.Width, rt_desc.Height, EPF_A8R8G8B8);

		
		d3d_surface_ptr mem_surface = _do_d3d_screen_shot();
		//-- copy surface to image
		D3DLOCKED_RECT lrc;
		hr = mem_surface->LockRect(&lrc, NULL, D3DLOCK_READONLY);

		BYTE* img_pixel = (BYTE*)blank_img->get_pixel();
		BYTE* surf_data = (BYTE*)lrc.pBits;
		int img_pitch = rt_desc.Width*Bpp;
		img_pixel += img_pitch*(rt_desc.Height-1);//从最后一行向上,倒着存放
		nASSERT( img_pitch <= lrc.Pitch);
		
		for(UINT y=0; y<rt_desc.Height; y++)
		{
			memcpy(img_pixel, surf_data, img_pitch);

			img_pixel -= img_pitch;
			surf_data += lrc.Pitch;
		}	

		hr = mem_surface->UnlockRect();
	}

	void nrenderer_d3d9::viewport_shot(nimage* blank_img)
	{
		HRESULT hr;

		nASSERT(blank_img != NULL);
		D3DVIEWPORT9& vp = m_view_info.d3d_view;
		int Bpp = 4;
		blank_img->create(vp.Width, vp.Height, EPF_A8R8G8B8);

		d3d_surface_ptr mem_surface = _do_d3d_screen_shot();
		//-- copy surface to image
		D3DLOCKED_RECT lrc;
		hr = mem_surface->LockRect(&lrc, NULL, D3DLOCK_READONLY);
		BYTE* surf_data = (BYTE*)lrc.pBits;
		int line_start = vp.X*Bpp;

		BYTE* img_pixel = (BYTE*)blank_img->get_pixel();		
		int img_pitch = vp.Width*Bpp;
		img_pixel += img_pitch*(vp.Height-1);//从最后一行向上,倒着存放
		nASSERT( img_pitch <= lrc.Pitch);

		for(UINT y=0; y<vp.Height; y++)
		{
			int line = vp.Y+y;			

			memcpy(img_pixel-y*img_pitch,
				surf_data+line*lrc.Pitch+line_start, 
				img_pitch);			
		}	

		hr = mem_surface->UnlockRect();
	}
}//namespace nexus