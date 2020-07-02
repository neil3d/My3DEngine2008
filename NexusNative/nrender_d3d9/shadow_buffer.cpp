#include "StdAfx.h"
#include "shadow_buffer.h"
#include "d3d_device_manager.h"
#include "d3d_exception.h"
#include "util.h"

namespace nexus
{
	matrix44 m_uv_tranfo;
	shadow_buffer::shadow_buffer(void)
	{
		mat_set_identity(m_uv_tranfo);
	}

	shadow_buffer::~shadow_buffer(void)
	{
	}

	void shadow_buffer::create(size_t rt_size)
	{
		m_rt_size = rt_size;

		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		HRESULT hr;

		IDirect3DTexture9* tex = NULL;
		hr = device->CreateTexture(rt_size, rt_size, 1,
			D3DUSAGE_DEPTHSTENCIL,
			D3DFMT_D24S8,
			D3DPOOL_DEFAULT,
			&tex, NULL);
		
		if( FAILED(hr) )
		{
			THROW_D3D_HRESULT(hr, _T("create shadow buffer failed"));
		}

		IDirect3DSurface9* surf = NULL;
		hr = tex->GetSurfaceLevel(0, &surf);

		//--
		m_shadow_tex.reset(tex);
		m_surface.reset(surf);

		//--------
		hr = device->CreateTexture(rt_size, rt_size, 1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&tex, NULL);

		if( FAILED(hr) )
		{
			THROW_D3D_HRESULT(hr, _T("create shadow buffer failed"));
		}

		hr = tex->GetSurfaceLevel(0, &surf);

		m_color_tex.reset(tex);
		m_color_surface.reset(surf);

		//--
		m_camera.set_ortho(rt_size*2.0f, rt_size*2.0f, 10, 20000);		

		//--
		//set special texture matrix for shadow mapping
		float fOffsetX = 0.5f + (0.5f / (float)rt_size);
		float fOffsetY = 0.5f + (0.5f / (float)rt_size);
		unsigned int range = 1;            //note different scale in DX9!
		float fBias    = -0.001f * (float)range;
		//float fBias    = 0.0f;
		matrix44 texScaleBiasMat( 0.5f,     0.0f,     0.0f,         0.0f,
			0.0f,    -0.5f,     0.0f,         0.0f,
			0.0f,     0.0f,     (float)range, 0.0f,
			fOffsetX, fOffsetY, fBias,        1.0f );

		m_uv_tranfo = texScaleBiasMat;
	}

	void shadow_buffer::begin_generate(const ndirectional_light_proxy* lgt, const d3d_view_info* view)
	{
		//-- update camera
		const float dist = 5000;

		vector3 pos = view->look_at;
		pos -= lgt->m_direction*dist;

		m_camera.set_lookat(pos, view->look_at, vector3(0,1,0));

		m_lgt_view.eye_pos = m_camera.get_eye_pos();
		m_lgt_view.look_at = m_camera.get_lookat();
		m_lgt_view.mat_project = m_camera.get_project();
		m_lgt_view.mat_view = m_camera.get_view();
		m_lgt_view.mat_view_project = m_lgt_view.mat_view*m_lgt_view.mat_project;

		//-- bind render targets
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		HRESULT hr;

		IDirect3DSurface9* surf = NULL;
		hr = device->GetRenderTarget(0, &surf);
		m_old_target.reset(surf);

		hr = device->GetDepthStencilSurface(&surf);
		m_old_zbuffer.reset(surf);

		hr = device->GetViewport(&m_old_viewport);
		hr = device->SetRenderTarget(0, m_color_surface.get());
		hr = device->SetDepthStencilSurface(m_surface.get());

		D3DVIEWPORT9 new_vp;
		new_vp.X = 0;
		new_vp.Y = 0;
		new_vp.Width  = m_rt_size;
		new_vp.Height = m_rt_size;
		new_vp.MinZ = 0.0f;
		new_vp.MaxZ = 1.0f;
		device->SetViewport(&new_vp);

		hr = device->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x00F0F0, 1, 0);
	}

	void shadow_buffer::end_generate()
	{		
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		HRESULT hr;

		hr = device->SetRenderTarget(0, m_old_target.get());
		hr = device->SetDepthStencilSurface(m_old_zbuffer.get());
		hr = device->SetViewport(&m_old_viewport);
	}

	void shadow_buffer::set_effect_parameter(d3d9_shading_effect* effect_ptr)
	{
		matrix44 prj = m_lgt_view.mat_view_project*m_uv_tranfo;
		effect_ptr->set_matrix("SHADOW_Mat", prj);
		effect_ptr->set_d3d_texture("SHADOW_Buffer", m_shadow_tex.get());
	}

	void shadow_buffer::modify_drawing_policy_type(light_drawing_policy_type* dp_type)	
	{
		dp_type->m_shadow_policy = _T("shader_d3d9/shadow/shadow_map.hlsl");		
	}

	void noshadow::modify_drawing_policy_type(light_drawing_policy_type* dp_type)
	{
		dp_type->m_shadow_policy = _T("shader_d3d9/shadow/no_shadow.hlsl");			
	}
}//namespace nexus