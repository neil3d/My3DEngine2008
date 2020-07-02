#include "StdAfx.h"
#include "nscene_capture_reflect.h"
#include "../framework/nengine.h"

namespace nexus
{
	nscene_capture_reflect::nscene_capture_reflect(void)
	{
		mat_set_identity(m_uv_tranfo);

		/**
			0.5		0		0	0
			0		¨C0.5	0	0
			0		0		0	0
			0.5		0.5		1	1
		*/
		
		m_uv_tranfo._11 = 0.5f;
		m_uv_tranfo._22 = -0.5f;
		m_uv_tranfo._33 = 0.0f;
		m_uv_tranfo._41 = 0.5f;
		m_uv_tranfo._42 = 0.5f;
		m_uv_tranfo._43 = 1.0f;
		m_uv_tranfo._44 = 1.0f;
	}

	nscene_capture_reflect::~nscene_capture_reflect(void)
	{
	}

	bool nscene_capture_reflect::update_required(const nviewport& view)
	{
		m_view.camera = view.camera;				
		
		ncamera& cam = m_view.camera; // for short
		matrix44 reflect_mat;
		mat_reflect(reflect_mat, m_plane);
		
		matrix44 view_mat = cam.get_view();
		view_mat = reflect_mat*view_mat;

		cam.set_view( view_mat );		
		cam.set_clip_plane( m_plane );
		
		//--
		m_uv_project = view_mat*cam.get_project();
		m_uv_project = m_uv_project*m_uv_tranfo;
		return true;
	}

	void nscene_capture_reflect::capture_scene(nlevel* lv)
	{		
		m_view.update();
		lv->render(m_view, NULL);
		nengine::instance()->get_renderer()->resolve_target( m_rt.get() );

#if 0
		m_rt->save_to_file( _T("d:/test.bmp") );
#endif
	}

	nrender_target* nscene_capture_reflect::get_texture_target()
	{
		return m_rt.get();
	}

	void nscene_capture_reflect::create(int render_target_w, int render_target_h)
	{
		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();
		
		m_rt.reset( rres_mgr->alloc_render_target() );
		m_rt->create(render_target_w, render_target_h, EPF_A8R8G8B8);

		//--
		m_view.width = render_target_w;
		m_view.height = render_target_h;
		m_view.show_flags |= EShow_InSceneCapture;
	}
}//namespace nexus