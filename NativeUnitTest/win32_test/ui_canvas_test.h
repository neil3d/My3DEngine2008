#pragma once
#include "ntest_case.h"

class nexus::nrender_font;
class nexus::nrender_texture2D;

class ui_canvas_test :
	public nrender_test
{
public:
	ui_canvas_test(void);
	virtual ~ui_canvas_test(void);

	virtual void open(perspective_camera_controller* cam);	
	virtual void close();

	virtual void debug_draw(nrender_primitive_draw_interface* PDI);
	virtual void debug_canvas(nui_canvas* canvas);


private:
	nexus::nrender_font::ptr				m_default_font;
	nstring									m_text_test;
	frect									m_text_draw_area;

	nexus::nresource_texture_2d::ptr		m_default_texture;
	nexus::nresource_texture_2d::ptr		m_alpha_texture;
	frect									m_texture_source_test;

	// »­Ïß
	std::vector<fpoint>						m_vec_points;

	nDECLARE_CLASS(ui_canvas_test)
};
