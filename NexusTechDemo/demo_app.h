#pragma once
#include <queue>
#include "ngame_app_win32.h"
#include "player_camera_ctrl.h"
#include "player_ctrl.h"

using namespace nexus;

class demo_app : public ngame_app_win32
{
public:
	demo_app(void)	{}
	virtual ~demo_app(void)	{}

	void init_load();
	void shut_down();

	void debug_draw(nrender_primitive_draw_interface* PDI);
	virtual void create(int wnd_w, int wnd_h, const nstring& title, bool full_scr_style);
	virtual void tick();
	
	virtual LRESULT CALLBACK _WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	void process_input_message();
private:
	nviewport		m_view;
	boost::timer	m_timer;
	double			m_run_time;	
	
	player_camera_ctrl		m_camera_ctrl;
	shared_ptr<player_ctrl>	m_player_ctrl;

	std::queue<MSG> m_msg_queue;
};