/**
*	Nexus Engine - Win32 GUI测试程序
*
*	Author: Neil
*	Date:	Feb, 2009
*/

#include "stdafx.h"
#include <sstream>
#include <boost/timer.hpp>
#include "ncore.h"
#include "ngame_app_win32.h"
using namespace nexus;

#include "ntest_case.h"

#include <time.h>

nexus::ngame_engine	g_engine;

class win32_test_app;

class nwin32_test_render : public nrender_element
{
public:
	virtual void draw(nrender_primitive_draw_interface* PDI);

	nui_canvas*		m_canvas;
	win32_test_app*	m_app;
};

/**
 *	测试application
 *	@remarks 自动检索test case派生类，添加菜单，自动创建
*/
class win32_test_app 
	: public ngame_app_win32
{
	enum member_const
	{
		EOptMenu_Wireframe = 1000,
		EOptMenu_UnLit,
		EOptMenu_Lit,
		EOptMenu_AddPointLight,
		EOptMenu_AddDirectLight,
		EOptMenu_AddSpotLight,
		EOptMenu_RemoveLight,
		EOptMenu_AddRigidBody,
		EOptMenu_ScreenShot,
		EOptMenu_Reset,
		EOptMenu_Exit,

		ETestMenuID = 2000,
	};
public:
	win32_test_app(void) : m_menu(NULL),m_run_time(0)
	{
		m_fps_count = 0;
		m_fps_start = 0;
		m_fps = 0;
	}
	virtual ~win32_test_app(void)	{}

	virtual void create(int wnd_w, int wnd_h, const nstring& title, bool full_scr_style)
	{
		ngame_app_win32::create(wnd_w, wnd_h, title, full_scr_style);

		m_view.width = wnd_w;
		m_view.height = wnd_h;
		m_view.handle_wnd = m_hWnd;		

		//-- 初始化camera
		m_view.camera.set_lookat(vector3(0,0,0), vector3(0, 0, 500), vector3(0,1,0));
		m_view.camera.set_perspective(3.14159f/4, wnd_w, wnd_h, 1, 50000);				

		//-- 创建菜单
		m_menu = ::CreateMenu();

		//-- options
		HMENU opt_menu = ::CreateMenu();
		::AppendMenu(opt_menu, MF_STRING, EOptMenu_Wireframe, _T("render wireframe"));
		::AppendMenu(opt_menu, MF_STRING, EOptMenu_UnLit, _T("render unlit"));
		::AppendMenu(opt_menu, MF_STRING, EOptMenu_Lit, _T("render lit"));
		::AppendMenu(opt_menu, MF_SEPARATOR, 0, NULL) ;
		::AppendMenu(opt_menu, MF_STRING, EOptMenu_AddPointLight, _T("add point light"));
		::AppendMenu(opt_menu, MF_STRING, EOptMenu_AddDirectLight, _T("add direct light"));
		::AppendMenu(opt_menu, MF_STRING, EOptMenu_AddSpotLight, _T("add spot light"));
		::AppendMenu(opt_menu, MF_STRING, EOptMenu_RemoveLight, _T("remove light"));
		::AppendMenu(opt_menu, MF_SEPARATOR, 0, NULL) ;
		::AppendMenu(opt_menu, MF_STRING, EOptMenu_AddRigidBody, _T("add &rigid body"));
		::AppendMenu(opt_menu, MF_SEPARATOR, 0, NULL) ;
		::AppendMenu(opt_menu, MF_STRING, EOptMenu_Reset, _T("reset"));
		::AppendMenu(opt_menu, MF_STRING, EOptMenu_ScreenShot, _T("screen shot"));
		::AppendMenu(opt_menu, MF_SEPARATOR, 0, NULL) ;
		::AppendMenu(opt_menu, MF_STRING, EOptMenu_Exit, _T("exit"));

		
		//-- render test
		UINT menu_id = ETestMenuID;
		HMENU render_test_menu = 
			create_sub_class_test_menu( nrender_test::reflection_get_static_class(), menu_id );

		//-- resource test
		HMENU resource_test_menu = 
			create_sub_class_test_menu( nresource_test::reflection_get_static_class(), menu_id );

		//-- physics test
		HMENU phys_test_menu = 
			create_sub_class_test_menu( nphys_test::reflection_get_static_class(), menu_id );
		
		//--
		::AppendMenu(m_menu, MF_POPUP, (UINT_PTR)opt_menu, _T("options") );
		::AppendMenu(m_menu, MF_POPUP, (UINT_PTR)render_test_menu, _T("render test") );
		::AppendMenu(m_menu, MF_POPUP, (UINT_PTR)resource_test_menu, _T("resource test") );
		::AppendMenu(m_menu, MF_POPUP, (UINT_PTR)phys_test_menu, _T("physics test") );
		::SetMenu(m_hWnd, m_menu);

		//--
		m_timer.restart();
		srand(clock());
		m_view.far_lod = 10000;
		m_view.render_mode = nexus::ERM_Wireframe;
	}

	void create_editor_render()
	{
		if( !m_view.widgets_render )
		{
			shared_ptr<nwin32_test_render> editor_render(new nwin32_test_render);
			editor_render->m_app = this;
			m_view.widgets_render = editor_render;

			nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();
			editor_render->m_canvas = rres_mgr->alloc_ui_canvas();
		}

#if 0	// hit proxy对性能影响比较大，平时都关闭这个测试
		//-- 为了测试hit prorxy的渲染，这里也建立一个
		m_view.hit_hash = g_engine.get_render_res_mgr()->alloc_hit_proxy_hash();
		m_view.hit_hash->create(m_view.width,m_view.height);
#endif
	}

	virtual void tick()
	{
		double delta = m_timer.elapsed()-m_run_time;
		m_run_time = m_timer.elapsed();

		m_camera_ctrl.update_camera(&m_view.camera);
		if( m_test )
			m_test->tick(m_run_time,delta);

		m_view.update();
		g_engine.render(m_view);
		g_engine.update(delta, m_view);

		m_frame_time = delta;		

		//计算每秒绘制的帧数
		
		double e=m_run_time-m_fps_start;
		if(e>1.0f)
		{
			m_fps = m_fps_count/e;
			m_fps_count = 0;
			m_fps_start=m_run_time;
		}
		else
		{
			++m_fps_count;
		}
	}

	void debug_draw(nrender_primitive_draw_interface* PDI)
	{
		PDI->begin_line_batch();

		PDI->draw_line(vector3(0,0,0), vector3(100, 0, 0), color4f(1, 0, 0, 1));
		PDI->draw_line(vector3(0,0,0), vector3(0, 100, 0), color4f(0, 1, 0, 1));
		PDI->draw_line(vector3(0,0,0), vector3(0, 0, 100), color4f(0, 0, 1, 1));

		std::wostringstream ss;
		ss 	<< "resource cache:"
			<< nresource_manager::instance()->get_num_resource_cached()
			<< ", FPS : "
			<< m_fps
			<< ", Visible Primitive : "
			<< g_hud_info.visible_primitive_count
			<< ", DrawCall : "
			<<(g_hud_info.draw_call_count)
			;

		g_hud_info.reset();
		PDI->draw_debug_string(6, 2, ss.str(), color4ub(255,250,250,255));

		if( m_test )
			m_test->debug_draw(PDI);
		
		PDI->end_line_batch();
	}

	void debug_canvas(nui_canvas* canvas)
	{
		canvas->begin(true);
		
		// do render
		if( m_test )
		{
			m_test->debug_canvas( canvas );
		}

		canvas->end();
	}

	void open_test(const nstring& test_class_name)
	{
		if(m_test)
			m_test->close();
		m_test.reset(
			nconstruct<ntest_case>(test_class_name)
			);
		m_test->open(&m_camera_ctrl);
	}

	virtual void close()
	{
		ngame_app_win32::close();

		if(m_test)
			m_test->close();
		m_test.reset();

		::DestroyMenu(m_menu);
		m_menu = NULL;
	}

	virtual LRESULT CALLBACK _WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch( message )
		{
		case WM_COMMAND:
			on_menu( LOWORD(wParam) );
			break;
		case WM_SIZE:
			{
				RECT rc;
				GetClientRect(hWnd,&rc);
				int width=rc.right-rc.left;
				int height=rc.bottom-rc.top;
				m_view.width = width;
				m_view.height = height;
				m_view.camera.set_perspective(m_view.camera.get_fov(), width, height, 1, m_view.camera.get_zfar());
				break;
			}
		case WM_KEYDOWN:
			{
				m_camera_ctrl.on_key_down(wParam);

				if( m_test )
				{
					m_test->on_key_down( wParam );
				}
			}
			break;
		case WM_LBUTTONDOWN:
			m_camera_ctrl.on_mouse_left_down(npoint(LOWORD(lParam), HIWORD(lParam)));
			break;
		case WM_LBUTTONUP:
			m_camera_ctrl.on_mouse_left_up();
			break;
		case WM_RBUTTONDOWN:
			m_camera_ctrl.on_mouse_right_down(npoint(LOWORD(lParam), HIWORD(lParam)));
			break;
		case WM_RBUTTONUP:
			m_camera_ctrl.on_mouse_right_up();
			break;
		case WM_MOUSEMOVE:
			{
				npoint pt = npoint((short)LOWORD(lParam), (short)HIWORD(lParam));
				m_camera_ctrl.on_mouse_move( pt, 0!=(wParam&MK_CONTROL) );

				if( m_test )
				{
					m_test->on_mouse_move( pt );
				}
			}
			break;
		case WM_MOUSEWHEEL:
			float scale=1.0f;
			if(wParam&MK_CONTROL)
			{
				scale=20.0f;
			}
			if(wParam&MK_SHIFT)
			{
				scale=0.2f;
			}
			m_camera_ctrl.on_mouse_wheel( (int)(scale*GET_WHEEL_DELTA_WPARAM(wParam)) );
			break;
		}

		return ngame_app_win32::_WndProc(hWnd, message, wParam, lParam);
	}
private:
	void on_menu(UINT menu_id)
	{
		nexus::nrenderer_base* renderer = nexus::nengine::instance()->get_renderer();

		switch( menu_id )
		{
		case EOptMenu_Wireframe:
			m_view.render_mode = nexus::ERM_Wireframe;
			break;
		case EOptMenu_UnLit:
			m_view.render_mode = nexus::ERM_Unlit;
			break;
		case EOptMenu_Lit:
			m_view.render_mode = nexus::ERM_Lit;
			break;
		case EOptMenu_AddPointLight:
			if(m_test)
				m_test->add_point_light();
			break;
		case EOptMenu_AddDirectLight:
			if(m_test)
				m_test->add_direct_light(m_view.camera.get_view_z());
			break;
		case EOptMenu_AddSpotLight:
			if(m_test)
				m_test->add_spot_light();
			break;
		case EOptMenu_RemoveLight:
			if(m_test)
				m_test->remove_last_light();
			break;
		case EOptMenu_AddRigidBody:
			if( m_test )
				m_test->add_rigid_body();
			break;
		case EOptMenu_Reset:
			if(m_test)
				m_test->close();
			m_test.reset();
			break;
		case EOptMenu_ScreenShot:
			{
				nimage img;
				nengine::instance()->get_renderer()->viewport_shot(&img);
				img.save_to_file(_T("ScreenShot.bmp"));
			}
			break;
		case EOptMenu_Exit:
			::SendMessage(m_hWnd, WM_CLOSE, 0, 0) ;
			break;
		default:
			if( menu_id >= ETestMenuID
				&& menu_id < ETestMenuID+m_menu_classes.size())
			{
				open_test( m_menu_classes[menu_id-ETestMenuID] );
			}
		}
	}
private:
	HMENU create_sub_class_test_menu(const nclass* parent_class, UINT& menu_id)
	{
		HMENU hmenu = ::CreateMenu();

		for(size_t i=0; i<parent_class->get_num_children(); i++)
		{
			const nclass* child = parent_class->get_child(i);
			::AppendMenu(hmenu, MF_STRING, menu_id++,
				child->get_name());
			m_menu_classes.push_back(child->get_name());
		}
		
		return hmenu;
	}
private:
	nviewport	m_view;
	perspective_camera_controller
					m_camera_ctrl;

	ntest_case::ptr	m_test;
	HMENU			m_menu;
	vector<nstring>	m_menu_classes;
	boost::timer	m_timer;
	double			m_run_time;
	double			m_frame_time;

	double	m_fps_start;
	int		m_fps_count;
	double	m_fps;
};

win32_test_app		g_app;

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	log_output_ptr to_stderr( new nstderr_listener() );
	nlog::instance()->add_log_output(to_stderr);

	log_output_ptr to_file( new nfile_listener(_T("win32_test.log")) );
	nlog::instance()->add_log_output(to_file);

	log_output_ptr to_console( new nconsole_listener() );
	nlog::instance()->add_log_output(to_console);

	g_engine.init_core();
	
	engine_config cfg;	
	cfg.renderer_class = _T("nrenderer_d3d9");	
	cfg.file_system_class = _T("nstd_file_system");
	cfg.engine_data_pkg = _T("engine_data");
	cfg.resource_cache_class = _T("nresource_cache_basic");
	cfg.resource_io_class = _T("nresource_io_basic");
	//cfg.resource_io_class = _T("nresource_async_io");
	cfg.bWindowed	= true;
	cfg.bEnableHDR	= false;
	cfg.bEnableSSAO	= false;
	cfg.color_bits	= 32;
	cfg.width = 1440;
	cfg.height = 900;
		
	g_app.create(cfg.width, cfg.height+20, _T("nengine client win32 test"), false);
	cfg.window_handle = g_app.get_wnd_handle();
	g_engine.init_modules(cfg);

	g_app.create_editor_render();	
	g_app.main_loop();

	g_app.close();

	g_engine.close();	
	return 0;
}

void nwin32_test_render::draw(nrender_primitive_draw_interface* PDI)
{
	m_app->debug_canvas(m_canvas);
	m_app->debug_draw(PDI);
}