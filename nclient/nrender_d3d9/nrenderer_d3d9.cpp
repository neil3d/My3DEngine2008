#include "StdAfx.h"
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
#include "dynamic_vb_pool.h"
#include "global_shader_lib.h"

namespace nexus
{
	nDEFINE_CLASS(nrenderer_d3d9, nrenderer_base)

	nfile_system::ptr g_file_sys;
	nstring g_engine_package_name(_T("engine_data"));

	nrenderer_d3d9::nrenderer_d3d9(void)
	{
		m_render_mode = ERM_Lit;
		m_render_group[EDG_Background]	= NULL;
		m_render_group[EDG_World]		= &m_world_group;
		m_render_group[EDG_Foreground]	= NULL;
	}

	nrenderer_d3d9::~nrenderer_d3d9(void)
	{
		close();
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

		shading_effect_lib::instance()->init_drawing_policy_type();

		dynamic_vb_pool::instance()->init();

		m_PDI.init(&m_view_info);

		// todo : 使用vfs
		nstring global_shader_path = _T("../engine_data/shader_d3d9/global");
		global_shader_lib::instance()->load(global_shader_path);
	}

	void nrenderer_d3d9::frame_view_info(const ncamera* cam)
	{
		m_view_info.eye_pos = cam->get_eye_pos();
		m_view_info.mat_view = cam->get_view();
		m_view_info.mat_project = cam->get_project();
		m_view_info.mat_view_project = cam->get_view()*cam->get_project();
	}

	bool nrenderer_d3d9::frame_begin(const ncamera* cam)
	{
		if( cam == NULL)
			return false;

		frame_view_info(cam);

		//--
		m_lights.clear();
		m_post_handle_array.clear();

		//-- render group;
		for(int i=0; i<EDG_Max; i++)
			if(m_render_group[i] != NULL)
				m_render_group[i]->clear();

		//-- device
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		if( !device )
			return false;

		if( FAILED( device->BeginScene() ) )
		{
			return false;
		}

		//HRESULT hr = device->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x000000, 1, 0);

		return true;
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

	void nrenderer_d3d9::frame_end()
	{
		//-- render main scene
		m_world_group.sort_draw_order(&m_view_info);
		frame_render();

		//-- draw all render elements
		for(std::list<nrender_element::ptr>::iterator iter=m_elements.begin();
			iter != m_elements.end();
			++iter)
		{
			(*iter)->draw(&m_PDI);
		}

		//-- end d3d scene
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		HRESULT hr = device->EndScene();

		//-- frame data clear
		for(render_mesh_vector::iterator iter = m_post_handle_array.begin();
			iter != m_post_handle_array.end();
			++iter)
		{
			nrender_mesh* mesh = *iter;
			mesh->post_render();
		}
	}

	void nrenderer_d3d9::present(void* hWindow)
	{
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		HRESULT hr = device->Present(NULL, NULL,
			(HWND)hWindow,
			NULL);
	}

	void nrenderer_d3d9::close()
	{
		global_shader_lib::instance()->close();
		dynamic_vb_pool::instance()->destroy();
		scene_render_targets::destroy_instance();

		vertex_factory_type_lib::instance()->destroy();
		shading_effect_lib::instance()->destroy();

		d3d_device_manager::instance()->destroy_device();
		g_file_sys.reset();
	}

	void nrenderer_d3d9::draw_pass(drawing_policy* dp)
	{
		for(int i=0; i<EDG_Max; i++)
		{
			if(m_render_group[i] != NULL
				&& !m_render_group[i]->empty())
				m_render_group[i]->draw_all(&m_view_info, dp);
		}
	}

	void nrenderer_d3d9::frame_render()
	{
		//-- render depth pass
		{
			scene_render_targets::instance()->begin_pre_pass();
			drawing_pre_pass dp;
			draw_pass(&dp);
			scene_render_targets::instance()->end_pre_pass();
		}

		bool enable_tonemap = false;
		scene_render_targets::instance()->begin_scene_color();
		switch(m_render_mode)
		{
		case ERM_Wireframe:
			{
				drawing_wireframe dp;
				draw_pass(&dp);
			}
			break;
		case ERM_Unlit:
			{
				drawing_unlit dp;
				draw_pass(&dp);
			}
			break;
		case ERM_Lit:
			{
				enable_tonemap = true;
				//-- draw base pass
				{
					drawing_base_pass dp;
					draw_pass(&dp);
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
		scene_render_targets::instance()->draw_final_scene(enable_tonemap);
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
					drawing_light<point_light_policy> dp(lgt);
					m_world_group.draw_opaque(&m_view_info, &dp);					

					dp.set_tech(EDrawLight_LowTransPass);
					m_world_group.draw_mixed_trans(&m_view_info, &dp);					
				}
				break;
			case ELight_Directional:
				{
					drawing_light<directional_light_policy> dp(lgt);
					m_world_group.draw_opaque(&m_view_info, &dp);

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
}//namespace nexus