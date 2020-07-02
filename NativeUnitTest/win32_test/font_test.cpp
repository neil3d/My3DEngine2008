#include "StdAfx.h"
#include "font_test.h"

nDEFINE_CLASS(font_test, nrender_test)

font_test::font_test(void)
{
	m_Font=0;
}

font_test::~font_test(void)
{
}

void font_test::open( perspective_camera_controller* cam )
{
	nrender_test::open(cam);
	srand(100);

	for(int i=0;i<20;++i)
	{
		m_Color[i]=((rand()&0xffff)<<16)|(rand()&0xffff);
	}
	nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();
	m_Font=rres_mgr->alloc_render_font();
	m_Font->create( _T("ÀŒÃÂ"),0,32,800);
}

void font_test::debug_canvas( nui_canvas* canvas )
{
	frect rect(0,0,200,30);
	int j=0;
	for(float i=0.0f;i<30.0f;i+=3.0f)
	{
		++j;
		canvas->draw_string(m_Font,L"≤‚ ‘",100+i,100,m_Color[j],0);
	}
}