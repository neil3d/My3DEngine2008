// NexusTechDemo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "demo_app.h"

nexus::ngame_engine	g_engine;
demo_app g_app;

struct demo_wiget_render : public nrender_element
{
	virtual void draw(nrender_primitive_draw_interface* PDI)
	{
		app->debug_draw(PDI);
	}

	demo_app*	app;
};

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	g_engine.init_core();

	engine_config cfg;	
	cfg.renderer_class = _T("nrenderer_d3d9");	
	cfg.file_system_class = _T("nstd_file_system");
	cfg.engine_data_pkg = _T("engine_data");
	cfg.bWindowed	= true;
	cfg.bEnableHDR	= false;
	cfg.color_bits	= 32;
	cfg.width = 1024;
	cfg.height = 768;

	g_app.create(cfg.width, cfg.height, _T("Nexus Tech Demo 2009"), false);
	cfg.window_handle = g_app.get_wnd_handle();
	g_engine.init_modules(cfg);

	g_app.init_load();	
	g_app.main_loop();

	g_app.shut_down();
	g_engine.close();	
	return 0;
}

void demo_app::init_load()
{
	m_timer.restart();
	m_run_time = 0;

	//-- load level
	nlevel::ptr main_lv = g_engine.load_level(
		resource_location(_T("demo_data:level"))
		);

	vector3 start_pos(0,0,0);
	//nactor::ptr find_trn = main_lv->find_actor(_T("DefaultTerrainName"));
	//if(find_trn)
	//{
	//	nterrain_actor::ptr trn = boost::dynamic_pointer_cast<nterrain_actor>(find_trn);
	//	if(trn)
	//	{
	//		start_pos = trn->tile2world(243,138);
	//		//start_pos.y += 100;
	//	}
	//}


	//-- create direct light
	vector3 direction(0,-500,-500);
	direction = vec_normalize(direction);
	nactor::ptr lgt_actor = main_lv->create_actor(_T("main_light"));			
	shared_ptr<nlight_component> lgt(
		lgt_actor->create_component_t<nlight_component>(_T("direct_light"))
		);	
	lgt->create_directional_light();
	ndirectional_light_proxy* dir_lgt = lgt->get_directional_light();
	dir_lgt->m_diffuse_color = color4f(0.7f, 0.7f, 0.7f, 1);
	dir_lgt->m_ambient_color = color4f(0.3f, 0.3f, 0.3f, 0.0f);	
	dir_lgt->m_direction = direction;

	//-- create sky
#if 0
	nsky_actor::ptr sky = m_level_ptr->create_actor_t<nsky_actor>(_T("sky_dome"));
	ndynamic_sky_dome::ptr m_sky = sky->create_sky_dome(18000, 50);
	m_sky->set_sun_dir(nDegToRad(30), nDegToRad(90));
	m_sky->rebuild_colors();

	dir_lgt->m_direction = m_sky->get_sun_dir();
#endif

	//-- create player
	nactor::ptr player = main_lv->create_actor_t<nactor>(_T("demo_player"));
#if 0
	nshape_component::ptr player_shape = player->create_component_t<nshape_component>(_T("shape_mesh"));
	player_shape->create_box(60,200,60);
#else
	nanim_mesh_component::ptr player_mesh
		 = player->create_component_t<nanim_mesh_component>(_T("anim_mesh"));
	player_mesh->reset_resource(
		resource_location(_T("demo_data:player/girl.nam"))
		);	
#endif

	player->move(start_pos, vector3::zero, vector3::one);

	nactor_controller::ptr pc = player->create_controller(_T("player_ctrl"));
	// todo : 为什么player_ctrl不在namespace nexus就转换不过来
	m_player_ctrl = boost::dynamic_pointer_cast<player_ctrl>(pc);
	
	m_camera_ctrl.bind_player(player);
}

void demo_app::create(int wnd_w, int wnd_h, const nstring& title, bool full_scr_style)
{
	ngame_app_win32::create(wnd_w, wnd_h, title, full_scr_style);

	m_view.width = wnd_w;
	m_view.height = wnd_h;
	m_view.handle_wnd = m_hWnd;

	//-- 初始化camera
	m_view.camera.set_lookat(vector3(0,-500,0), vector3(0, 500, 500), vector3(0,1,0));
	m_view.camera.set_perspective(3.14159f/4, wnd_w, wnd_h, 5, 100000);				

	m_view.far_lod = 40000;
	m_view.render_mode = ERM_Lit;

	//--
	demo_wiget_render* wr = new demo_wiget_render;
	wr->app =  this;
	m_view.widgets_render.reset( wr );	
}

void demo_app::tick()
{
	double delta = m_timer.elapsed()-m_run_time;
	m_run_time = m_timer.elapsed();

	process_input_message();

	m_camera_ctrl.update_camera(&m_view.camera);
	m_view.update();
	g_engine.frame_tick((float)delta, m_view);	
}

void demo_app::shut_down()
{
	m_player_ctrl.reset();
	m_camera_ctrl.close();	
}

LRESULT CALLBACK demo_app::_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MSG msg;
	msg.hwnd = hWnd;
	msg.message = message;
	msg.wParam = wParam;
	msg.lParam = lParam;
	m_msg_queue.push(msg);

	return ngame_app_win32::_WndProc(hWnd, message, wParam, lParam);
}

void demo_app::process_input_message()
{
	while(!m_msg_queue.empty())
	{
		MSG m = m_msg_queue.front();
		m_player_ctrl->process_input_message(m);
		m_camera_ctrl.process_input_message(m);
		m_msg_queue.pop();
	}// end of while
}

void demo_app::debug_draw(nrender_primitive_draw_interface* PDI)
{
	if(m_player_ctrl)
		m_player_ctrl->debug_draw(PDI);
}