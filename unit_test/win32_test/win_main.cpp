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
#include "../../nclient/py_ngame_client/app/ngame_app_win32.h"
using namespace nexus;

#include "ntest_case.h"

nexus::ngame_engine	g_engine;

class win32_test_app;

struct neditor_render : public nrender_element
{
	virtual void draw(nrender_primitive_draw_interface* PDI);

	win32_test_app*	app;
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
		EOptMenu_RemoveLight,
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

		//-- 初始化camera
		m_camera.set_lookat(vector3(0,0,0), vector3(0, 0, 500), vector3(0,1,0));
		m_camera.set_perspective(3.14159f/4, wnd_w, wnd_h, 5, 50000);

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
		::AppendMenu(opt_menu, MF_STRING, EOptMenu_RemoveLight, _T("remove light"));
		::AppendMenu(opt_menu, MF_SEPARATOR, 0, NULL) ;
		::AppendMenu(opt_menu, MF_STRING, EOptMenu_Reset, _T("reset"));
		::AppendMenu(opt_menu, MF_SEPARATOR, 0, NULL) ;
		::AppendMenu(opt_menu, MF_STRING, EOptMenu_Exit, _T("exit"));

		
		//-- render test
		UINT menu_id = ETestMenuID;
		HMENU render_test_menu = 
			create_sub_class_test_menu( nrender_test::reflection_get_static_class(), menu_id );

		//-- physics test
		HMENU phys_test_menu = 
			create_sub_class_test_menu( nphys_test::reflection_get_static_class(), menu_id );
		
		//--
		::AppendMenu(m_menu, MF_POPUP, (UINT_PTR)opt_menu, _T("options") );
		::AppendMenu(m_menu, MF_POPUP, (UINT_PTR)render_test_menu, _T("render test") );
		::AppendMenu(m_menu, MF_POPUP, (UINT_PTR)phys_test_menu, _T("physics test") );
		::SetMenu(m_hWnd, m_menu);

		//--
		m_timer.restart();
	}

	void create_editor_render()
	{
		m_editor_render.reset(new neditor_render);
		m_editor_render->app = this;
		nengine::instance()->get_renderer()->attach_element(m_editor_render);
	}

	virtual void tick()
	{
		double delta = m_timer.elapsed()-m_run_time;
		m_run_time = m_timer.elapsed();

		m_camera_ctrl.update_camera(&m_camera);
		if( m_test )
			m_test->tick(m_run_time);

		g_engine.frame_tick(delta,
			&m_camera, m_hWnd);

		m_frame_time = delta;		

		//-- 计算20帧平均的FPS
		if(m_fps_count == 0)
			m_fps_start = m_run_time;
		m_fps_count++;

		if( m_fps_count == 20)
		{			
			double time = m_run_time-m_fps_start;
			m_fps = m_fps_count/time;
			m_fps_count = 0;
		}
	}

	void debug_draw(nrender_primitive_draw_interface* PDI)
	{
		PDI->begin_line_batch();
		PDI->draw_line(vector3(0,0,0), vector3(100, 0, 0), color4f(1, 0, 0, 1));
		PDI->draw_line(vector3(0,0,0), vector3(0, 100, 0), color4f(0, 1, 0, 1));
		PDI->draw_line(vector3(0,0,0), vector3(0, 0, 100), color4f(0, 0, 1, 1));
		PDI->end_line_batch();

		std::wostringstream ss;
		ss 	<< "resource cache:"
			<< nresource_manager::instance()->get_num_resource_cached()
			<< ", FPS : "
			<< m_fps
			;
		PDI->draw_debug_string(128, 2, ss.str(), color4ub(255,250,250,255));

		if( m_test )
			m_test->debug_draw(PDI);
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
		nengine::instance()->get_renderer()->detach_element(m_editor_render);
		m_editor_render.reset();

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
		case WM_LBUTTONDOWN:
			m_camera_ctrl.on_mouse_left_down(npoint(LOWORD(lParam), HIWORD(lParam)));
			break;
		case WM_LBUTTONUP:
			m_camera_ctrl.on_mouse_left_up();
			break;
		case WM_RBUTTONDOWN:
			m_camera_ctrl.on_mouse_right_down();
			break;
		case WM_RBUTTONUP:
			m_camera_ctrl.on_mouse_right_up();
			break;
		case WM_MOUSEMOVE:
			m_camera_ctrl.on_mouse_move( npoint(LOWORD(lParam), HIWORD(lParam)),
				(bool)(wParam&MK_CONTROL));
			break;
		case WM_MOUSEWHEEL:
			m_camera_ctrl.on_mouse_wheel( GET_WHEEL_DELTA_WPARAM(wParam) );
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
			renderer->set_render_mode(nexus::ERM_Wireframe);
			break;
		case EOptMenu_UnLit:
			renderer->set_render_mode(nexus::ERM_Unlit);
			break;
		case EOptMenu_Lit:
			renderer->set_render_mode(nexus::ERM_Lit);
			break;
		case EOptMenu_AddPointLight:
			if(m_test)
				m_test->add_point_light();
			break;
		case EOptMenu_AddDirectLight:
			if(m_test)
				m_test->add_direct_light(m_camera.get_view_z());
			break;
		case EOptMenu_RemoveLight:
			if(m_test)
				m_test->remove_last_light();
			break;
		case EOptMenu_Reset:
			if(m_test)
				m_test->close();
			m_test.reset();
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
	ncamera			m_camera;
	editor_camera_controller
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
	
	shared_ptr<neditor_render>	m_editor_render;
};

win32_test_app		g_app;

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
	cfg.width = 800;
	cfg.height = 600;
		
	g_app.create(cfg.width, cfg.height, _T("nengine client win32 test"), false);
	cfg.window_handle = g_app.get_wnd_handle();
	g_engine.init_modules(cfg);

	g_app.create_editor_render();
	g_app.main_loop();

	g_engine.close();	
	return 0;
}

void neditor_render::draw(nrender_primitive_draw_interface* PDI)
{
	app->debug_draw(PDI);
}