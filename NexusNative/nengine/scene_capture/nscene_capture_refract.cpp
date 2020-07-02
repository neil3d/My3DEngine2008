#include "StdAfx.h"
#include "nscene_capture_refract.h"
#include "../framework/nengine.h"

namespace nexus
{
	nscene_capture_refract::nscene_capture_refract(void)
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

	nscene_capture_refract::~nscene_capture_refract(void)
	{
	}

	bool nscene_capture_refract::update_required(const nviewport& view)
	{
		m_view.camera = view.camera;
		//--
		m_uv_project = m_view.camera.get_view()*m_view.camera.get_project()*m_uv_tranfo;

		m_view.camera.set_clip_plane( m_plane );
		return true;
	}

	void nscene_capture_refract::capture_scene(nlevel* lv)
	{
		nrenderer_base* rnd = nengine::instance()->get_renderer();		
		
		m_view.update();	

		lv->render(m_view);
		
		rnd->resolve_target( m_rt.get() );

#if 0
		m_rt->save_to_file( _T("d:/test.bmp") );
#endif
	}

	nrender_target* nscene_capture_refract::get_texture_target()
	{
		return m_rt.get();
	}

	void nscene_capture_refract::create(int render_target_w, int render_target_h)
	{
		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();

		m_rt.reset( rres_mgr->alloc_render_target() );
		m_rt->create(render_target_w, render_target_h, EPF_A8R8G8B8);

		//--
		m_view.width = render_target_w;
		m_view.height = render_target_h;
		m_view.show_flags |= EShow_InSceneCapture;
	}

	void nscene_capture_refract::_on_device_lost(int param)
	{
		m_rt.reset();
	}

	bool nscene_capture_refract::_on_device_reset(int param)
	{
		create(m_view.width, m_view.height);
		return true;
	}
}//namespace nexus