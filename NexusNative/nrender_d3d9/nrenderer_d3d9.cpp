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
#include "d3d9_hit_proxy_hash.h"
#include "draw_hit_proxy.h"
#include "dynamic_vb_pool.h"
#include "global_shader_lib.h"
#include "d3d_exception.h"
#include "drawing_extend.h"
#include "drawing_policy.h"
#include "ambent_light_policy.h"
#include "ssao.h"
#include "..\nengine\actor\nlight_component.h"
#include "FFTWave.h"

#include "..\nengine\speed_tree\nspeed_tree_component.h"
#include "..\nengine\actor\nmesh_component.h"
#include "..\nengine\util\hud_info.h"

#ifdef _DEBUG
#	pragma comment(lib,"../nengine/SPT_SDK/lib/vc9/debug/SpeedTreeRT_d.lib")
#else
#	pragma comment(lib,"../nengine/SPT_SDK/lib/vc9/release/SpeedTreeRT.lib")
#endif

namespace nexus
{
	void draw_stencil_volume(
							const vector<vector3>& positons,
							const UINT* indexes,
							UINT prim_count,
							const matrix44&world,
							const matrix44&view,
							const matrix44&proj
							)
	{
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		device->Clear(0, NULL, D3DCLEAR_STENCIL, 0, 0, 0);
		device->SetVertexShader(NULL);	
		device->SetPixelShader(NULL);
		device->SetFVF(D3DFVF_XYZ);
		device->SetTransform(D3DTS_WORLD,(D3DXMATRIX*)&world);
		device->SetTransform(D3DTS_VIEW,(D3DXMATRIX*)&view);
		device->SetTransform(D3DTS_PROJECTION,(D3DXMATRIX*)&proj);
		device->SetVertexShader(NULL);	
		device->SetPixelShader(NULL);
		device->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
		device->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);

		device->SetRenderState(D3DRS_STENCILENABLE,true);
		device->SetRenderState(D3DRS_STENCILFUNC,D3DCMP_ALWAYS);
		device->SetRenderState(D3DRS_STENCILFAIL,D3DSTENCILOP_KEEP);
		device->SetRenderState(D3DRS_STENCILZFAIL,D3DSTENCILOP_DECR);
		device->SetRenderState(D3DRS_STENCILPASS,D3DSTENCILOP_KEEP);
		device->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE,true);
		device->SetRenderState(D3DRS_CCW_STENCILFUNC,D3DCMP_ALWAYS);
		device->SetRenderState(D3DRS_CCW_STENCILFAIL,D3DSTENCILOP_KEEP);
		device->SetRenderState(D3DRS_CCW_STENCILZFAIL,D3DSTENCILOP_INCR);
		device->SetRenderState(D3DRS_CCW_STENCILPASS,D3DSTENCILOP_KEEP);
		device->SetRenderState(D3DRS_STENCILMASK,0xff);
		device->SetRenderState(D3DRS_STENCILWRITEMASK,0xff);
		device->SetRenderState(D3DRS_STENCILREF,0);

		device->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
		device->SetRenderState(D3DRS_ZWRITEENABLE,false);
		device->SetRenderState(D3DRS_ZENABLE,true);

		device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0,positons.size(),prim_count,indexes,D3DFMT_INDEX32,&positons[0],sizeof(vector3));

		device->SetRenderState(D3DRS_STENCILENABLE,true);
		device->SetRenderState(D3DRS_STENCILFUNC,D3DCMP_NOTEQUAL);
		device->SetRenderState(D3DRS_STENCILFAIL,D3DSTENCILOP_KEEP);
		device->SetRenderState(D3DRS_STENCILZFAIL,D3DSTENCILOP_KEEP);
		device->SetRenderState(D3DRS_STENCILPASS,D3DSTENCILOP_KEEP);
		device->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE,false);
		device->SetRenderState(D3DRS_CCW_STENCILFUNC,D3DCMP_ALWAYS);
		device->SetRenderState(D3DRS_CCW_STENCILFAIL,D3DSTENCILOP_KEEP);
		device->SetRenderState(D3DRS_CCW_STENCILZFAIL,D3DSTENCILOP_KEEP);
		device->SetRenderState(D3DRS_CCW_STENCILPASS,D3DSTENCILOP_KEEP);
		device->SetRenderState(D3DRS_STENCILMASK,0xff);
		device->SetRenderState(D3DRS_STENCILWRITEMASK,0xff);
		device->SetRenderState(D3DRS_STENCILREF,0);
		device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORALL);
	}

	class  occlusion_query
	{
	public:
		occlusion_query()
		{
			d3d_device_manager::instance()->get_device()->CreateQuery(D3DQUERYTYPE_OCCLUSION,&m_query);
		}

		~occlusion_query()
		{
			m_query->Release();
		}

		void begin( )
		{
			m_query->Issue( D3DISSUE_BEGIN );
		}

		void end()
		{
			m_query->Issue( D3DISSUE_END );
		}

		bool get_result()
		{
			DWORD pixelsVisible = 100;
			while (m_query->GetData((void *)&pixelsVisible, sizeof(DWORD), D3DGETDATA_FLUSH) == S_FALSE);

			if( pixelsVisible >9 )
				return false;
			else
			{
				return true;
			}	
		}

	private:
		IDirect3DQuery9* m_query;
	};

	nDEFINE_CLASS(nrenderer_d3d9, nrenderer_base)

	nfile_system::ptr g_file_sys;
	nstring g_engine_package_name(_T("engine_data"));
	
	static nrenderer_d3d9* g_instance = NULL;

	nrenderer_d3d9::nrenderer_d3d9(void)
	{
		g_instance = this;		
		m_device_lost = 0;
		m_ssao = NULL;
		bocclusion_query = false;
		m_render_mode = ERM_Lit;
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
		d3d_device_manager::instance()->create_device(cfg);
		scene_render_targets::create_instance(cfg);

		shading_effect_lib::instance()->init();
		dynamic_vb_pool::instance()->init();

		if (cfg.bEnableSSAO)
		{
			m_ssao = new ssao(high);
		}	

		m_PDI.init(&m_view_info);
		
		// todo : 使用vfs
		nstring global_shader_path = _T("shader_d3d9/global");
		global_shader_lib::instance()->load(g_engine_package_name, global_shader_path);

		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		HRESULT hr = D3DXCreateVolumeTextureFromFile(device,_T("../nexus_game/engine_data/resource_default/Jitter.dds"),(LPDIRECT3DVOLUMETEXTURE9*)&m_engine_textures[ETexture_Jitter]);
		hr = D3DXCreateTextureFromFile(device,_T("../nexus_game/engine_data/resource_default/rotrandomCM.dds"),(LPDIRECT3DTEXTURE9*)&m_engine_textures[ETexture_Rot]);
		hr = device->CreateTexture(NX,NY,0,D3DUSAGE_DYNAMIC,D3DFMT_A32B32G32R32F,D3DPOOL_DEFAULT,(LPDIRECT3DTEXTURE9*)&m_engine_textures[ETexture_FFT],NULL);
		hr = device->CreateTexture(NX,NY,0,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,(LPDIRECT3DTEXTURE9*)&m_engine_textures[ETexture_FFT_Normal],NULL);

		m_timer.restart();
	}

	void nrenderer_d3d9::frame_view_info(const nviewport& view)
	{
		m_view_info.frustum = view.frustum;
		const ncamera* cam = &view.camera;
		m_view_info.look_at = cam->get_lookat();
		m_view_info.aspect = cam->get_viewport_aspect();
		m_view_info.eye_pos = cam->get_eye_pos();
		m_view_info.mat_view = cam->get_view();
		m_view_info.time = get_run_time();
		m_view_info.znear = cam->get_znear();
		m_view_info.zfar = cam->get_zfar();
		m_view_info.fov = cam->get_fov();
		m_view_info.mat_project = cam->get_project();

		m_view_info.mat_view_project = m_view_info.mat_view*m_view_info.mat_project;
		m_view_info.enable_dynamic_shadow = view.enable_dynamic_shadow;
	}

	void nrenderer_d3d9::frame_light(const nlight_component* lgt)
	{
		nrender_light_proxy * light_proxy = NULL;

		switch(lgt->m_type)
		{
		case ELight_Point:
			{
				light_proxy = new npoint_light_proxy((npoint_light_component*)lgt);
			}
			break;
		case ELight_Directional:
			{
				light_proxy = new ndirectional_light_proxy((ndirectional_light_component*)lgt);
			}
			break;
		case Elight_Spot:
			{
				light_proxy = new nspot_light_proxy((nspot_light_component*)lgt);
			}
			break;
		default:
			assert(0);  //LightType ndef
		}

		m_lights.push_back(light_proxy);
	}

	void nrenderer_d3d9::update_hit_proxy_hash(nhit_proxy_hash* hit_map)
	{
		d3d9_hit_proxy_hash* d3d9_hm = dynamic_cast<d3d9_hit_proxy_hash*>(hit_map);
		d3d9_hm->begin_update();
		
		draw_hit_proxy dp;		
		m_render_package.draw_for_hit_hash(&m_view_info, &dp);
		
		if(m_view_widgets)
			m_view_widgets->update_hit_proxy(&m_PDI);
			
		d3d9_hm->end_update();
	}
	
	void nrenderer_d3d9::resolve_target(nrender_target* target)
	{
		scene_render_targets::instance()->copy_render_target(target);
	}

	void nrenderer_d3d9::present(void* hWindow)
	{
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		HRESULT hr;

		//-- 处理device lost，尝试恢复
		if( m_device_lost )
		{
			hr = device->TestCooperativeLevel();
			
			if( hr == D3DERR_DEVICENOTRESET )
			{
				// 设备就绪，可以reset了
				D3DPRESENT_PARAMETERS pp = d3d_device_manager::instance()->get_present_param();
				hr = device->Reset(&pp);

				if ( SUCCEEDED(hr) )
				{
					// 成功恢复设备，通知所有handler创建资源
					if( !d3d_device_manager::instance()->on_device_reset(0 ))
					{
						// 资源创建失败
						THROW_D3D_EXCEPTION(_T("D3D device reset handler return false."));
					}	

					hr = D3DXCreateVolumeTextureFromFile(device,_T("../nexus_game/engine_data/resource_default/Jitter.dds"),(LPDIRECT3DVOLUMETEXTURE9*)&m_engine_textures[ETexture_Jitter]);
					hr = D3DXCreateTextureFromFile(device,_T("../nexus_game/engine_data/resource_default/rotrandomCM.dds"),(LPDIRECT3DTEXTURE9*)&m_engine_textures[ETexture_Rot]);
					hr = device->CreateTexture(NX,NY,0,D3DUSAGE_DYNAMIC,D3DFMT_A32B32G32R32F,D3DPOOL_DEFAULT,(LPDIRECT3DTEXTURE9*)&m_engine_textures[ETexture_FFT],NULL);
					hr = device->CreateTexture(NX,NY,0,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,(LPDIRECT3DTEXTURE9*)&m_engine_textures[ETexture_FFT_Normal],NULL);

					::InterlockedExchange((LONG*)&m_device_lost, 0L);					
				}// end of if
				else
				{
					THROW_D3D_HRESULT(hr, _T("D3D Device Reset Failed!"));
				}
			}// end of if
			else if( hr ==  D3DERR_DEVICELOST )
			{
				::Sleep(200); // 为了更好的与其他应用程序协作
			}
			return;
		}

		//-- 处理正常渲染		
#if 0
		hr = device->Present(NULL, NULL, (HWND)hWindow,	NULL);
#else
		//-- 为了在editor模式下，将部分back buffer不拉伸的显示到窗口，需要指定present的rect	
		const D3DVIEWPORT9& vp = *(D3DVIEWPORT9*)&m_view_info.m_view_port;

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
		if(hr == D3DERR_DEVICELOST)
		{			
			// 由正常状态切换到Lost状态，通知所有handler释放资源
			::InterlockedExchange((LONG*)&m_device_lost, 1L);					
			d3d_device_manager::instance()->on_device_lost(0);

			for ( occlusion_map::iterator it = m_occlusion_map.begin();
				it != m_occlusion_map.end();
				++it)
			{
				delete it->second;
			}

			m_occlusion_map.clear();

			m_render_package.clear();
			m_lights.clear();

			for (int i = 0; i < ETexture_Max; i ++)
			{
				m_engine_textures[i]->Release();
				m_engine_textures[i] = NULL;
			}
		}
		
		//hr = D3DXSaveSurfaceToFile(_T("d:\\test.bmp"),
		//	D3DXIFF_BMP, 
		//	scene_render_targets::instance()->get_back_surface().get(),
		//	NULL,
		//	NULL);
	}

	void nrenderer_d3d9::close()
	{
		m_PDI.destroy();
		m_render_package.clear();

		delete m_ssao;
		m_ssao = NULL;
		for (size_t i = 0; i < m_postprocess_array.size(); i ++)
		{
			delete m_postprocess_array[i];
		}
		m_postprocess_array.clear();

		for(int i = 0; i < ETexture_Max; i ++)
		{
			m_engine_textures[i]->Release();
			m_engine_textures[i] = NULL;
		}

		shading_effect_lib::instance()->close();

		global_shader_lib::instance()->close();
		dynamic_vb_pool::instance()->destroy();
		scene_render_targets::destroy_instance();

		vertex_factory_type_lib::instance()->destroy();
		shading_effect_lib::instance()->destroy();

		d3d_device_manager::instance()->destroy_device();
		g_file_sys.reset();
		nLog_Info(_T("nexus d3d9 renderer closed.\r\n"));
	}

	void nrenderer_d3d9::draw_background()
	{
	}

	void nrenderer_d3d9::draw_foreground()
	{
	}

	void nrenderer_d3d9::make_occlusion_query( const nview_info&view_info,node_ptr node,box_sphere_bounds bound )
	{
		vector3 box_max = bound.origin+bound.box_extent;
		vector3 box_min = bound.origin - bound.box_extent;

		if ( view_info.eye_pos.x > box_min.x 
			&&view_info.eye_pos.x < box_max.x 
			&&view_info.eye_pos.y > box_min.y 
			&&view_info.eye_pos.y < box_max.y 
			&&view_info.eye_pos.z > box_min.z 
			&&view_info.eye_pos.z < box_max.z)
		{
			return;
		}

		static vector3 positons[8];
		positons[0] = vector3(box_min.x,box_max.y,box_min.z); 
		positons[1] = box_min;  
		positons[2] = vector3(box_max.x,box_min.y,box_min.z); 
		positons[3] = vector3(box_max.x,box_max.y,box_min.z);

		positons[4] = vector3(box_min.x,box_max.y,box_max.z);	
		positons[5] = vector3(box_min.x,box_min.y,box_max.z);
		positons[6] = vector3(box_max.x,box_min.y,box_max.z);
		positons[7] =  box_max;	

		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		occlusion_node_map::iterator it = m_occlusion_node_map.find(node);
		occlusion_query* query = NULL;
		if( it != m_occlusion_node_map.end())
			query = it->second;
		else
		{
			query = new occlusion_query();
			m_occlusion_node_map.insert(pair<node_ptr,occlusion_query*>(node,query));
		}

		query->begin();
		device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0,8,12,cube_index,D3DFMT_INDEX32,positons,sizeof(vector3));
		query->end();
	}

	void nrenderer_d3d9::make_occlusion_query( const nview_info&view_info,nprimitive_component* prim )
	{
		const box_sphere_bounds& bound = prim->get_bounds();
		vector3 box_max = bound.origin+bound.box_extent + vector3(5,5,5);
		vector3 box_min = bound.origin - bound.box_extent - vector3(5,5,5);

		if ( view_info.eye_pos.x > box_min.x 
			&&view_info.eye_pos.x < box_max.x 
			&&view_info.eye_pos.y > box_min.y 
			&&view_info.eye_pos.y < box_max.y 
			&&view_info.eye_pos.z > box_min.z 
			&&view_info.eye_pos.z < box_max.z)
		{
			return;
		}

		static vector3 positons[8];
		positons[0] = vector3(box_min.x,box_max.y,box_min.z); 
		positons[1] = box_min;  
		positons[2] = vector3(box_max.x,box_min.y,box_min.z); 
		positons[3] = vector3(box_max.x,box_max.y,box_min.z);

		positons[4] = vector3(box_min.x,box_max.y,box_max.z);	
		positons[5] = vector3(box_min.x,box_min.y,box_max.z);
		positons[6] = vector3(box_max.x,box_min.y,box_max.z);
		positons[7] =  box_max;	

		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		occlusion_map::iterator it = m_occlusion_map.find(prim);
		occlusion_query* query = NULL;
		if( it != m_occlusion_map.end())
			query = it->second;
		else
		{
			query = new occlusion_query();
			m_occlusion_map.insert(pair<nprimitive_component*,occlusion_query*>(prim,query));
		}
			
		query->begin();
		device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0,8,12,cube_index,D3DFMT_INDEX32,positons,sizeof(vector3));
		query->end();
	}

	void nrenderer_d3d9::render_scene(const nviewport& view,nscene_manager::ptr scene_ptr,const render_setting& setting)
	{
		// 加载level失败时可能导致scene_ptr为空
		if (is_device_lost() || !scene_ptr)
		{
			return;
		}

		//-- device
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		if( !device )
			return;

		D3DVIEWPORT9& vp = *(D3DVIEWPORT9*)&m_view_info.m_view_port;
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
			return;
		}

		//hr = device->Clear(0, NULL, D3DCLEAR_TARGET, 0x000000, 1, 0);
		m_render_mode = view.render_mode;
		m_view_widgets = view.widgets_render;

		m_lights.clear();
		m_render_package.clear();
		
		frame_view_info(view);
		m_scene_ptr = scene_ptr;
		m_render_setting = setting;
		
		//init_visible();	
		vector<node_ptr> cur_frame_nodes;
		vector<box_sphere_bounds> cur_frame_node_bounds;
		
		in_frustum_prims.clear();
		in_frustum_prims.reserve(1024);

		scene_render_targets::instance()->begin_scene_color();

		if (bocclusion_query)
		{
			for ( nscene_manager::prim_octree_type::const_iterator pri_it( *m_scene_ptr->m_prim_octrant );
				pri_it.has_pending_node(); pri_it.advance() )
			{
				const nscene_manager::prim_octree_type::octreenode_ptr	curr_node	= pri_it.get_curr_node();
				const octree_node_bounds&				curr_bound	= pri_it.get_curr_boud();
				box_sphere_bounds node_bound(curr_bound.center,vector3(curr_bound.extent,curr_bound.extent,curr_bound.extent));

				if( m_view_info.frustum.intersect_bounds( node_bound ))
				{
					cur_frame_nodes.push_back(curr_node);
					cur_frame_node_bounds.push_back(node_bound);
					bool node_occlude = false;
					occlusion_node_map::iterator it = m_occlusion_node_map.find(curr_node);
					if (it != m_occlusion_node_map.end())
					{
						node_occlude = it->second->get_result();
					}

					if(!node_occlude)
					{
						const std::set<nprimitive_component*>& curr_elements = curr_node->get_curr_contents();
						for ( std::set<nprimitive_component*>::const_iterator comp_it = curr_elements.begin();
							comp_it != curr_elements.end(); ++comp_it)
						{
							// 再进行一次视锥裁剪
							nprimitive_component* prim = *comp_it; 
							if( prim->get_visible())
							{
								const box_sphere_bounds& bound = prim->get_bounds();
								if(m_view_info.frustum.intersect_bounds(bound))
								{
									bool bocclude = false;
									in_frustum_prims.push_back(prim);
									occlusion_map::iterator it = m_occlusion_map.find(prim);
									if (it != m_occlusion_map.end())
									{
										bocclude = it->second->get_result();
									}

									if(!bocclude)
									{
										g_hud_info.visible_primitive_count ++;
										prim->render(&m_render_package);
									}
									else
									{
										g_hud_info.primitive_occlude_count ++;
									}
								}
							}
						}

						// push children
						for ( int i=0; i< 8; ++i )
						{
							if( curr_node->has_child(i) )
							{
								pri_it.push_children( i );
							}
						}
					}
				}
			}
		}	
		else
		{
			for ( nscene_manager::prim_octree_type::const_iterator pri_it( *m_scene_ptr->m_prim_octrant );
				pri_it.has_pending_node(); pri_it.advance() )
			{
				const nscene_manager::prim_octree_type::octreenode_ptr	curr_node	= pri_it.get_curr_node();
				const octree_node_bounds&				curr_bound	= pri_it.get_curr_boud();
				box_sphere_bounds node_bound(curr_bound.center,vector3(curr_bound.extent,curr_bound.extent,curr_bound.extent));

				if(  m_view_info.frustum.intersect_bounds( node_bound ))
				{
					const std::set<nprimitive_component*>& curr_elements = curr_node->get_curr_contents();
					for ( std::set<nprimitive_component*>::const_iterator comp_it = curr_elements.begin();
						comp_it != curr_elements.end(); ++comp_it)
					{
						// 再进行一次视锥裁剪
						nprimitive_component* prim = *comp_it; 
						if( prim->get_visible())
						{
							const box_sphere_bounds& bound = prim->get_bounds();
							if(m_view_info.frustum.intersect_bounds(bound))
							{
								g_hud_info.visible_primitive_count ++;
								prim->render(&m_render_package);
							}
						}
					}

					// push children
					for ( int i=0; i< 8; ++i )
					{
						if( curr_node->has_child(i) )
						{
							pri_it.push_children( i );
						}
					}
				}
			}
		}

		int num = g_hud_info.visible_primitive_count;

		vector<nlight_component*> visble_light;
		m_scene_ptr->get_visible_light(m_view_info.frustum,visble_light);
		for (size_t i = 0; i < visble_light.size(); i++)
		{
			frame_light(visble_light[i]);
		}
	
//----------------------------------------------------------rendering---------------------------------------------------------------------------
		m_render_package.begin(true);
		draw_background();
		draw_world();
		draw_foreground();

		if(view.hit_hash)
			update_hit_proxy_hash(view.hit_hash);

		m_render_package.end();
		//-- draw view widgets		
		scene_render_targets::instance()->draw_final_scene(false);
		if(m_view_widgets)		
		{
			scene_render_targets::instance()->begin_back_buffer();
			m_view_widgets->draw(&m_PDI);
			scene_render_targets::instance()->end_back_buffer();
		}		

		if (bocclusion_query)
		{
			device->SetVertexShader(NULL);	
			device->SetPixelShader(NULL);
			device->SetFVF(D3DFVF_XYZ);
			device->SetTransform(D3DTS_WORLD,(D3DXMATRIX*)&matrix44::identity);
			device->SetTransform(D3DTS_VIEW,(D3DXMATRIX*)&m_view_info.mat_view);
			device->SetTransform(D3DTS_PROJECTION,(D3DXMATRIX*)&m_view_info.mat_project);
			device->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);
			device->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);

			device->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
			device->SetRenderState(D3DRS_ZWRITEENABLE,false);
			device->SetRenderState(D3DRS_ZENABLE,true);
			device->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);

			for (size_t i =0; i < cur_frame_nodes.size(); i ++)
			{
				make_occlusion_query(m_view_info,cur_frame_nodes[i],cur_frame_node_bounds[i]);
			}

			for(size_t i = 0; i < in_frustum_prims.size(); i ++)
			{
				make_occlusion_query(m_view_info, in_frustum_prims[i]);
			}

			device->SetRenderState(D3DRS_COLORWRITEENABLE,D3DCOLORALL);
		}
		
		//-- end d3d scene
		device->EndScene();
	}

	class drawing_unlit_filter : public drawing_filter
	{
	public:
		static bool should_draw(const nprimitive_component* obj)
		{
			return !obj->accept_dynamic_light();
		}
	};

	void nrenderer_d3d9::draw_world()
	{
		switch(m_render_mode)
		{
		case ERM_Wireframe:
			{
				scene_render_targets::instance()->begin_scene_color(true);
				drawing_wireframe dp;

				m_render_package.draw_all(&m_view_info,&dp);
				scene_render_targets::instance()->end_scene_color(true);
			}
			break;
		case ERM_Unlit:
			{
				scene_render_targets::instance()->begin_scene_color(true);
				drawing_unlit dp;
				
				m_render_package.draw_all(&m_view_info,&dp);
				
				scene_render_targets::instance()->end_scene_color(true);
			}
			break;
		case ERM_Lit:
			{
				//!!! 只有World Group接受光照				
				//-- 渲染自发光、静态光（Light map）
				// 注意：没有自发光，静态光，则需要渲染base color为0，因为后续灯光渲染执行的是颜色累加
				scene_render_targets::instance()->begin_scene_color(true,true);
				hemisphere_policy::context context_data;
				context_data.high_color = m_render_setting.skylight_high_color;
				context_data.low_color = m_render_setting.skylight_low_color;
				drawing_base_pass<hemisphere_policy> dp(context_data);		
				m_render_package.draw_Opaque(&m_view_info,&dp);
				
				scene_render_targets::instance()->end_scene_color();

				if (m_ssao)
				{
					m_ssao->m_occlustion_radius = m_render_setting.ssao_occlustion_radius;
					m_ssao->m_occlustion_power = m_render_setting.ssao_occlustion_power;
					m_ssao->m_blocker_power = m_render_setting.ssao_blocker_power;
					m_ssao->render();
				}

				//-- 渲染不透明物体的动态光照
				scene_render_targets::instance()->begin_scene_color();
				render_lights();
				scene_render_targets::instance()->end_scene_color(true);
			
				scene_render_targets::instance()->begin_scene_color();	
				m_render_package.draw_Translucent(&m_view_info,&dp);
				m_render_package.render_env_effect(&m_view_info,&dp);
				scene_render_targets::instance()->end_scene_color(true);
			}
			break;
		default:
			nASSERT(0 && "unsupport render mode");
			break;
		}//end of switch()		
	}
	
	void nrenderer_d3d9::render_postprocess()
	{
		for (size_t i = 0; i < m_postprocess_array.size(); i ++)
		{
			m_postprocess_array[i]->render();
		}
	}

	void nrenderer_d3d9::render_lights()
	{
		// 只有world group计算光照
		size_t num_lights = m_lights.size();

		//-- 渲染“不透明物体”，每个light一个pass
		for(size_t i=0; i<num_lights; i++)
		{
			nrender_light_proxy* lgt = m_lights[i];
			lgt->render(this);				
		}
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

	void nrenderer_d3d9::screen_shot(nimage* blank_img, size_t width, size_t height)
	{
		HRESULT hr;

		nASSERT(blank_img != NULL);
		int Bpp = 4;
		blank_img->create(width, height, EPF_A8R8G8B8);


		d3d_surface_ptr mem_surface = _do_d3d_screen_shot();
		//-- copy surface to image
		D3DLOCKED_RECT lrc;
		hr = mem_surface->LockRect(&lrc, NULL, D3DLOCK_READONLY);

		BYTE* img_pixel = (BYTE*)blank_img->get_pixel();
		BYTE* surf_data = (BYTE*)lrc.pBits;
		int img_pitch = width * Bpp;
		img_pixel += img_pitch*(height - 1);//从最后一行向上,倒着存放
		nASSERT( img_pitch <= lrc.Pitch);

		for(UINT y=0; y<height; y++)
		{
			memcpy(img_pixel, surf_data, img_pitch);

			img_pixel -= img_pitch;
			surf_data += lrc.Pitch;
		}	

		hr = mem_surface->UnlockRect();
	}

	void nrenderer_d3d9::viewport_shot(nimage* blank_img)
	{
		D3DVIEWPORT9& vp = *(D3DVIEWPORT9*)&m_view_info.m_view_port;
		screen_shot(blank_img, vp.Width, vp.Height);
	}

	void nrenderer_d3d9::back_surface_shot( nimage* blank_img )
	{
		const D3DSURFACE_DESC& rt_desc = scene_render_targets::instance()->get_back_surface_desc();
		nASSERT(rt_desc.Format == D3DFMT_A8R8G8B8
			||rt_desc.Format == D3DFMT_X8R8G8B8);
		screen_shot(blank_img, rt_desc.Width, rt_desc.Height);
	}

	int nrenderer_d3d9::register_device_handler(handler_device_lost hlost, handler_device_reset hreset)
	{
		return d3d_device_manager::instance()->register_device_handler(hlost, hreset);
	}

	void nrenderer_d3d9::unregister_device_handler(int id)
	{
		d3d_device_manager::instance()->unregister_device_handler(id);
	}

	IDirect3DBaseTexture9*	nrenderer_d3d9::get_texture(EEngineTexture ETex)
	{
		return m_engine_textures[ETex];
	}
}//namespace nexus