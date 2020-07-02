#include "StdAfx.h"
#include "d3d9_render_target.h"
#include "d3d_device_manager.h"
#include "d3d_exception.h"

namespace nexus
{
	D3DFORMAT conv_pixel_format(enum EPixelFormat fmt)
	{
		switch(fmt)
		{
		case EPF_A8R8G8B8:
			return D3DFMT_A8R8G8B8;
			break;
		case EPF_R8G8B8:
			return D3DFMT_R8G8B8;
			break;
		case EPF_X8R8G8B8:
			return D3DFMT_X8R8G8B8;
			break;
		}

		return D3DFMT_UNKNOWN;
	}

	d3d9_render_target::d3d9_render_target(void)
	{
	}

	d3d9_render_target::~d3d9_render_target(void)
	{
	}

	void d3d9_render_target::create(size_t w, size_t h, EPixelFormat fmt)
	{
		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();

		//-- create color target
		IDirect3DTexture9* new_tex = NULL;
		HRESULT hr = D3DXCreateTexture(d3d_device, w, h,
			1,//num mip lvs
			D3DUSAGE_RENDERTARGET,//usage
			conv_pixel_format(fmt),//format
			D3DPOOL_DEFAULT,
			&new_tex);
		if( FAILED(hr) )
		{
			nLog_Error(_T("create render target texture failed\r\n"));
			THROW_D3D_HRESULT(hr, _T("create render target texture failed"));
		}

		m_texture.reset(new_tex);

		//--
		IDirect3DSurface9* surface = NULL;
		m_texture->GetSurfaceLevel(0, &surface);
		m_surface.reset(surface);
	}

	void d3d9_render_target::save_to_file(const nstring& file_name)
	{
		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();

		HRESULT hr = D3DXSaveSurfaceToFile(file_name.c_str(),
			D3DXIFF_BMP,
			m_surface.get(),
			NULL, NULL);
	}

}//namespace nexus