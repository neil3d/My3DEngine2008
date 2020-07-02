#pragma once

class WxMainFrame;

struct neditor_render : public nrender_element
{
	virtual void draw(nrender_primitive_draw_interface* PDI);

	WxMainFrame*	pFrame;
};

class WxMainFrame : public wxFrame
{
public:
	WxMainFrame(void);
	virtual ~WxMainFrame(void);

	void create_frame(int width, int height);
	void create_main_level();

	void editor_draw(nrender_primitive_draw_interface* PDI);

private:
	void create_point_light();
	void create_direct_light();
	void anim_lights();

	void create_all_menu();

	void on_timer(wxTimerEvent& evt);
	void on_window_close(wxCloseEvent& evt);	
	
	void on_mouse_left_down(wxMouseEvent& evt)
	{
		m_cam_ctrl.on_mouse_left_down(npoint(evt.GetX(), evt.GetY()));
	}
	void on_mouse_left_up(wxMouseEvent& evt)
	{
		m_cam_ctrl.on_mouse_left_up();
	}

	void on_mouse_right_down(wxMouseEvent& evt)
	{
		m_cam_ctrl.on_mouse_right_down();
	}

	void on_mouse_right_up(wxMouseEvent& evt)
	{
		m_cam_ctrl.on_mouse_right_up();
	}

	void on_mouse_move(wxMouseEvent& evt)
	{
		m_cam_ctrl.on_mouse_move(npoint(evt.GetX(), evt.GetY()),
			evt.ControlDown());
	}

	void on_mouse_wheel(wxMouseEvent& evt)
	{
		m_cam_ctrl.on_mouse_wheel(evt.GetWheelDelta());
	}

	void on_menu_render_mode(wxCommandEvent& evt);
	void on_menu_light(wxCommandEvent& evt);

	wxTimer m_tick_timer;

	boost::timer	m_timer;
	float			m_run_time;
	ncamera			m_camera;
	editor_camera_controller
					m_cam_ctrl;
	
	nlevel::ptr		m_level_ptr;
	std::vector<nactor::ptr>	m_light_array;
	float	m_light_anim_radius;

	bool			m_pause;
	bool			m_left_drag;
	bool			m_right_drag;
	wxPoint			m_last_drag_pt;

	nskeletal_mesh_component::ptr	m_skel_mesh;

	shared_ptr<neditor_render>	m_editor_render;

	DECLARE_EVENT_TABLE()
};
