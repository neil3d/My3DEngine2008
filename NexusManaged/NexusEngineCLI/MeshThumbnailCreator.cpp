#include "StdAfx.h"
#include "MeshThumbnailCreator.h"
#include <il/il.h>
#include <il/ilu.h>
#include <il/ilut.h>

namespace NexusEngine
{
	MeshThumbnailCreator::MeshThumbnailCreator(void)
	{
		m_fileExtList = gcnew System::Collections::ArrayList();
		m_fileExtList->Add("nmdl");	// static mesh
		m_fileExtList->Add("nam");	// anim mesh
		m_fileExtList->Add("spt");	// SpeedTree		
	}

	HBITMAP MeshThumbnailCreator::CreateThumbnail(NResourceLoc loc, int w, int h)
	{
		resource_location nloc;
		loc.ToNative(nloc);
		neditor_engine* engine = (neditor_engine*)nengine::instance();

		//-- make a level
		const TCHAR* lv_name = _T("MeshThumbnailCreator_TempLevel");
		nlevel::ptr tmp_lv = engine->create_level(lv_name,
			_T("nlevel") );
		tmp_lv->init(_T("nplain_scene"));
		nactor::ptr tmp_ac = tmp_lv->create_actor_t<nactor>(_T("TempActor"));

		nstring file_ext = nloc.get_file_ext();
		AABBox box;

		try
		{
			if(file_ext == _T("nmdl"))
			{
				nstatic_mesh_component::ptr tmp_comp = 
					tmp_ac->create_component_t<nstatic_mesh_component>(_T("MeshComp"));
				tmp_comp->reset_resource(nloc);
				box = tmp_comp->get_AABBox();
			}
			else if(file_ext == _T("nam"))
			{
				nanim_mesh_component::ptr tmp_comp = 
					tmp_ac->create_component_t<nanim_mesh_component>(_T("MeshComp"));
				tmp_comp->reset_resource(nloc);
				box = tmp_comp->get_AABBox();
			}
			else if(file_ext == _T("spt"))
			{
				nspeed_tree_component::ptr tmp_comp = 
					tmp_ac->create_component_t<nspeed_tree_component>(_T("MeshComp"));
				tmp_comp->create(nloc);
				box = tmp_comp->get_AABBox();
			}
		}
		catch(nexus::nexception& ex)
		{
			(void)ex;
			engine->destroy_level(tmp_lv);
			return NULL;
		}

		//-- render and screen shot
		nviewport vp;
		vp.render_mode = ERM_Unlit;		
		vp.width = w;
		vp.height = h;
		
		vp.camera.set_lookat(vector3(0,0,0), vector3(0, 0, 500), vector3(0,1,0));
		vp.camera.set_perspective(3.14159f/4, vp.width , vp.height, 5, 40000);				

		perspective_camera_controller cam_ctrl;		
		cam_ctrl.zoom_extents(box, &vp.camera);		
		cam_ctrl.update_camera(&vp.camera);
		vp.update();

		engine->render_level(lv_name, vp, false);

		nimage screen_img;
		engine->get_renderer()->viewport_shot(&screen_img);
		//screen_img.save_to_file(_T("d:\\test.bmp"));

		//!! hack code -- bind image name
		screen_img.get_width();
		iluScale(w, h, 1);
		HBITMAP ret = ilutConvertToHBitmap(m_hdc);

		//-- clean up
		engine->destroy_level(tmp_lv);

		return ret;
	}

	void MeshThumbnailCreator::Init()
	{
		HWND hwnd = static_cast<HWND>( nexus::nengine::instance()->get_engine_config().window_handle );
		HDC wndDC = ::GetDC(hwnd);

		m_hdc = wndDC;
		//m_hdc = ::CreateCompatibleDC(wndDC);

		//::ReleaseDC(hwnd, wndDC);
	}

	void MeshThumbnailCreator::Close()
	{
		HWND hwnd = static_cast<HWND>( nexus::nengine::instance()->get_engine_config().window_handle );
		::ReleaseDC(hwnd, m_hdc);
	}
}//namespace NexusEngine