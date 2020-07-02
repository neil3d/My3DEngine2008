#include "StdAfx.h"
#include "WxMainFrame.h"

const int TIMER_ID = 1234;

enum WxMainFrameMenuID
{
	Menu_Begin = 1000,
	
	//-- file
	Menu_FileOpenPackage,
	Menu_FileClosePackage,
	Menu_FileExit,

	//-- render
	Menu_RenderWireframe,
	Menu_RenderUnlit,
	Menu_RenderLit,
	Menu_ForceVertexLight,

	//-- light
	Menu_AddPointLight,
	Menu_AddDirectionalLight,
	Menu_RemoveLight,

	//-- test
	Menu_Test1,
	Menu_Test2,
	Menu_Test3,
	Menu_Test4,

	//-- help
	Menu_About,
};

BEGIN_EVENT_TABLE(WxMainFrame, wxFrame)
	EVT_TIMER(TIMER_ID, WxMainFrame::on_timer)
	EVT_CLOSE(on_window_close)
	
	EVT_LEFT_DOWN(on_mouse_left_down)
	EVT_LEFT_UP(on_mouse_left_up)
	EVT_RIGHT_DOWN(on_mouse_right_down)
	EVT_RIGHT_UP(on_mouse_right_up)
	EVT_MOTION(on_mouse_move)
	EVT_MOUSEWHEEL(on_mouse_wheel)	

	EVT_MENU(Menu_RenderWireframe, WxMainFrame::on_menu_render_mode)
	EVT_MENU(Menu_RenderUnlit, WxMainFrame::on_menu_render_mode)
	EVT_MENU(Menu_RenderLit, WxMainFrame::on_menu_render_mode)

	EVT_MENU(Menu_AddPointLight, WxMainFrame::on_menu_light)
	EVT_MENU(Menu_AddDirectionalLight, WxMainFrame::on_menu_light)
	EVT_MENU(Menu_RemoveLight, WxMainFrame::on_menu_light)

END_EVENT_TABLE()

WxMainFrame::WxMainFrame(void) : m_tick_timer(this, TIMER_ID)
{
	m_left_drag = false;
	m_right_drag = false;
	m_last_drag_pt.x = 0;
	m_last_drag_pt.y = 0;
	m_pause = false;
	m_run_time = 0;

	m_light_anim_radius = 200;
}

WxMainFrame::~WxMainFrame(void)
{
}

void WxMainFrame::create_frame(int width, int height)
{
	wxFrame::Create(NULL,		// parent
		wxID_ANY,				// id
		_("Nexus Viewer"),		// title
		wxDefaultPosition,		// position
		wxSize(width, height)	// size
		);

	wxStatusBar* sb = CreateStatusBar();
	SetStatusText(_T("Welcome to nexus viewer"));

	create_all_menu();

	//--
	m_timer.restart();
	m_cam_ctrl.update_camera(&m_camera);
	m_camera.set_perspective(3.14159f/4, width, height, 5, 50000);

	m_tick_timer.Start(1000/50);
}

void WxMainFrame::create_all_menu()
{
	//-- create file menu
	wxMenu* fileMenu = new wxMenu;
	fileMenu->Append(Menu_FileOpenPackage, _T("open package ..."));
	fileMenu->Append(Menu_FileClosePackage, _T("close package ..."));
	fileMenu->AppendSeparator();
	fileMenu->Append(Menu_FileExit, _T("E&xit"));

	//-- create render menu
	wxMenu* renderMenu = new wxMenu;
	renderMenu->AppendRadioItem(Menu_RenderWireframe, _T("render wireframe"));
	renderMenu->AppendRadioItem(Menu_RenderUnlit, _T("render unlit"));
	renderMenu->AppendRadioItem(Menu_RenderLit, _T("render lit"));
	renderMenu->Check(Menu_RenderLit,true);
	renderMenu->AppendSeparator();
	renderMenu->AppendCheckItem(Menu_ForceVertexLight,_T("force vertex light"));

	//-- create light menu
	wxMenu* lightMenu = new wxMenu;
	lightMenu->Append(Menu_AddPointLight, _T("add point light"));
	lightMenu->Append(Menu_AddDirectionalLight, _T("add directional light"));
	lightMenu->Append(Menu_RemoveLight, _T("remove light"));		

	//-- help menu
	wxMenu* helpMenu = new wxMenu;
	helpMenu->Append(Menu_About, _T("about ..."));

	//-- create menu bar
	wxMenuBar* menuBar = new wxMenuBar( wxMB_DOCKABLE );
	menuBar->Append(fileMenu,	_T("&File"));
	menuBar->Append(renderMenu,	_T("&Render"));
	menuBar->Append(lightMenu,	_T("&Light"));
	menuBar->Append(helpMenu,	_T("&Help"));

	// associate the menu bar with the frame
	SetMenuBar(menuBar);
}

void WxMainFrame::create_point_light()
{
	wostringstream oss;
	oss << _("light_actor_")
		<< m_light_array.size();

	nactor::ptr new_actor = m_level_ptr->create_actor(oss.str());	
	new_actor->set_static(false);

	// 使用一个shape component显示light的位置
	shared_ptr<nshape_component> box(
		new_actor->create_component_t<nshape_component>(_T("box"))
		);	
	//box->create_box(10,10,10);
	box->create_sphere(5, 10,20);

	// 创建light component
	shared_ptr<nlight_component> lgt(
		new_actor->create_component_t<nlight_component>(_T("point_light"))
		);	
	lgt->create_point_light();
	npoint_light_proxy* lgt_proxy = lgt->get_point_light();
	lgt_proxy->m_color = color4f(1, 1, 1, 1);
	lgt_proxy->m_position = vector3(0, 0, 0);	// actor space position
	lgt_proxy->m_attenuation = vector3(0.5f, 0.0001f,0);
	
	//--
	m_light_array.push_back(new_actor);
}

void WxMainFrame::create_direct_light()
{
	wostringstream oss;
	oss << _("light_actor_")
		<< m_light_array.size();

	nactor::ptr new_actor = m_level_ptr->create_actor(oss.str());		
	new_actor->set_static(false);

	//-- 创建light component
	shared_ptr<nlight_component> lgt(
		new_actor->create_component_t<nlight_component>(_T("point_light"))
		);	
	lgt->create_directional_light();
	ndirectional_light_proxy* dir_lgt = lgt->get_directional_light();
	
#if 0
	vector3 r(rand()%1000, rand()%1000, rand()%1000);
	dir_lgt->m_direction = vec_normalize(r);	
#else
	dir_lgt->m_direction = m_camera.get_view_z();
#endif

	//--
	m_light_array.push_back(new_actor);
}

void WxMainFrame::create_main_level()
{
	m_editor_render.reset(new neditor_render);
	m_editor_render->pFrame = this;
	nengine::instance()->get_renderer()->attach_element(m_editor_render);

	//
	m_level_ptr = nengine::instance()->create_level();
	m_level_ptr->init(_T("nplain_scene"));

	// 创建一个默认的光源
	create_point_light();
	//create_direct_light();
}

void WxMainFrame::on_timer(wxTimerEvent& evt)
{
	if( !m_pause )
		m_run_time += m_timer.elapsed();
	
	anim_lights();
	m_cam_ctrl.update_camera(&m_camera);
	nengine::instance()->frame_tick(m_timer.elapsed(), &m_camera, NULL);

	m_timer.restart();
}

void WxMainFrame::anim_lights()
{
	if( m_light_array.empty() )
		return;

	float step = 20;
	for(size_t i=0; i<m_light_array.size(); i++)
	{
		nactor::ptr lgt_actor = m_light_array[i];

		float t = m_run_time*0.6f+step*i;
		float r = m_light_anim_radius;

		vector3 pos;
		switch(i%3)
		{
		case 0:
			pos = vector3(sinf(t)*r, cosf(t)*r, sinf(t)*cosf(t)*r);
			break;
		case 1:
			pos = vector3(sinf(t)*r, cosf(t)*r, 0);
			break;
		case 2:
			pos = vector3(sinf(t)*r, 0, cosf(t)*r);
			break;
		}

		matrix44 mat;
		mat_set_translation(mat,pos.x, pos.y, pos.z);
		lgt_actor->move(mat);
	}
}

void WxMainFrame::on_window_close(wxCloseEvent& evt)
{
	m_skel_mesh.reset();
	nengine::instance()->get_renderer()->detach_element(m_editor_render);
	m_editor_render.reset();

	m_tick_timer.Stop();

	m_light_array.clear();
	m_level_ptr.reset();
	nengine::instance()->close();

	// destroy window
	Destroy();
}

void WxMainFrame::on_menu_render_mode(wxCommandEvent& evt)
{
	nexus::nrenderer_base* renderer = nexus::nengine::instance()->get_renderer();
	switch(evt.GetId())
	{
	case Menu_RenderWireframe:
		renderer->set_render_mode(nexus::ERM_Wireframe);
		break;
	case Menu_RenderUnlit:
		renderer->set_render_mode(nexus::ERM_Unlit);
		break;
	case Menu_RenderLit:
		renderer->set_render_mode(nexus::ERM_Lit);
		break;
	};
}

void WxMainFrame::on_menu_light(wxCommandEvent& evt)
{
	switch(evt.GetId())
	{
	case Menu_AddPointLight:
		create_point_light();
		break;
	case Menu_AddDirectionalLight:
		create_direct_light();
		break;
	case Menu_RemoveLight:
		{
			if( !m_light_array.empty() )
			{
				nactor::ptr rm_actor = m_light_array[m_light_array.size()-1];
				m_light_array.pop_back();
				m_level_ptr->free_actor(rm_actor->get_name());

			}
		}
		break;
	}
}

void neditor_render::draw(nrender_primitive_draw_interface* PDI)
{
	pFrame->editor_draw(PDI);
}

void WxMainFrame::editor_draw(nrender_primitive_draw_interface* PDI)
{
	PDI->begin_line_batch();
	PDI->draw_line(vector3(0,0,0), vector3(100, 0, 0), color4f(1, 0, 0, 1));
	PDI->draw_line(vector3(0,0,0), vector3(0, 100, 0), color4f(0, 1, 0, 1));
	PDI->draw_line(vector3(0,0,0), vector3(0, 0, 100), color4f(0, 0, 1, 1));
	PDI->end_line_batch();

	if( m_skel_mesh )
		m_skel_mesh->draw_skeleton(PDI);

	if( m_level_ptr.get()  
		&& m_level_ptr->get_scene_manager().get() )
	{
		m_level_ptr->get_scene_manager()->draw(PDI);
	}
}