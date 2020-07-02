#include "StdAfx.h"
#include "ui_canvas_test.h"

nDEFINE_CLASS(ui_canvas_test, nrender_test)

ui_canvas_test::ui_canvas_test(void)
{
}

ui_canvas_test::~ui_canvas_test(void)
{
}


void ui_canvas_test::open(nexus::perspective_camera_controller *cam)
{
	nrender_test::open(cam);
	//// 创建缺省字体
	//nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();
	//m_default_font.reset(rres_mgr->alloc_render_font());
	//m_default_font->create( _T("宋体"),14,0 );

	//m_text_test += _T("宋体 14 0 \r\n");

	// 创建缺省测试纹理
	resource_location tex_loc(_T("demo_data"),_T("test/bg.jpg"));
	m_default_texture = nresource_manager::instance()->load_texture_2d( tex_loc,EIO_Block );

	resource_location alpha_loc(_T("demo_data"),_T("test/alpha.png"));
	m_alpha_texture = nresource_manager::instance()->load_texture_2d( alpha_loc,EIO_Block );

	m_text_test += _T("Test texture: ") + tex_loc.to_string();
	m_text_draw_area.left = 100.f;
	m_text_draw_area.right = 700.f;
	m_text_draw_area.top = 100.f;
	m_text_draw_area.bottom = 500.f;

	m_texture_source_test.left = 0.0f;
	m_texture_source_test.right = 1.0f;
	m_texture_source_test.top = 0.0f;
	m_texture_source_test.bottom = 1.0f;

	m_vec_points.push_back( fpoint(0,0) );
	m_vec_points.push_back( fpoint(100,100) );
	m_vec_points.push_back( fpoint(100,200) );
	m_vec_points.push_back( fpoint(200,100) );
	m_vec_points.push_back( fpoint(700,500) );
}

void ui_canvas_test::close()
{
	nrender_test::close();
	m_default_font.reset();
}

void ui_canvas_test::debug_draw(nexus::nrender_primitive_draw_interface *PDI)
{
	
}

void ui_canvas_test::debug_canvas(nexus::nui_canvas *canvas)
{
	// 测试纹理绘制
	frect tex_target_area = m_texture_source_test;
	for ( int i=0; i<20; i++ )
	{
		for (int j=0; j<20; j++)
		{
			tex_target_area.top = 32 * j + 3;
			tex_target_area.bottom = tex_target_area.top + 32;
			tex_target_area.left = 32 * i + 3;
			tex_target_area.right = tex_target_area.left + 32;
			canvas->draw_texture( m_default_texture->get_render_texture2d(),m_texture_source_test,tex_target_area,color_rect(0x44FFFFFF,0xAAFF0000,0x4400FF00,0xAA0000FF));
		}
	}

	tex_target_area = frect(0,0,256,256);
	canvas->draw_texture( m_default_texture->get_render_texture2d(),m_texture_source_test,m_alpha_texture->get_render_texture2d(),frect(0,0,1,1),tex_target_area,0xFFFFFFFF);

	// 画矩形块测试
	canvas->draw_rect(m_text_draw_area,color_rect(0x44FFFFFF,0xAAFF0000,0x4400FF00,0xAA0000FF));
	//canvas->draw_rect(m_text_draw_area,0xFFFFFFFF);
	// 测试字体绘制
	//canvas->draw_string( m_default_font.get(),m_text_draw_area,m_text_test.c_str(),0xFF000000 );

	// 画线测试
	canvas->draw_line( &(*m_vec_points.begin()), m_vec_points.size(), 0xFFAA33AA );
}