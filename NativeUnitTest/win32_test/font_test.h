#pragma once
#include "ntest_case.h"

class font_test :
	public nrender_test
{
public:
	font_test(void);
	virtual ~font_test(void);

	virtual void open(perspective_camera_controller* cam);
	virtual void debug_canvas(nui_canvas* canvas);

private:
	nrender_font*	m_Font;
	UINT			m_Color[20];

	nDECLARE_CLASS(font_test)
};
