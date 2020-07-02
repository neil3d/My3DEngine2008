#include "StdAfx.h"
#include "d3d9_hit_proxy_hash.h"
#include "scene_render_targets.h"
#include "d3d_device_manager.h"

namespace nexus
{
	d3d9_hit_proxy_hash::d3d9_hit_proxy_hash(void)
	{
	}

	d3d9_hit_proxy_hash::~d3d9_hit_proxy_hash(void)
	{
	}

	void d3d9_hit_proxy_hash::create(int w, int h)
	{
		m_render_target = scene_render_targets::create_item(w, h, D3DFMT_A8R8G8B8, D3DUSAGE_RENDERTARGET);
		
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		
		HRESULT hr;
		IDirect3DSurface9* new_surf = NULL;
		hr = device->CreateOffscreenPlainSurface(w, h, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM,
			&new_surf, NULL);
		m_hd_buffer.reset(new_surf);

		D3DSURFACE_DESC desc;
		hr = m_hd_buffer->GetDesc(&desc);
		D3DLOCKED_RECT lrc;
		hr = m_hd_buffer->LockRect(&lrc, NULL, D3DLOCK_READONLY);
		hr = m_hd_buffer->UnlockRect();

		alloc_buffer(desc.Width, desc.Height, lrc.Pitch);
	}

	void d3d9_hit_proxy_hash::begin_update()
	{
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		HRESULT hr;
		hr = d3d_device_manager::instance()->set_view_render_target(0, m_render_target.surf.get());
		hr = device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,255,255), 1, 0);		
	}

	void d3d9_hit_proxy_hash::end_update()
	{
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		HRESULT hr;

		//-- copy render target to texture
		hr = device->GetRenderTargetData(
			m_render_target.surf.get(),
			m_hd_buffer.get()
			);
		

		//-- update internal buffer
		D3DLOCKED_RECT lrc;
		hr = m_hd_buffer->LockRect(&lrc, NULL, D3DLOCK_READONLY);
		
		UINT buffer_size = m_buffer.size()*sizeof(hit_id);
		UINT lk_size = m_height*lrc.Pitch;
		nASSERT(buffer_size == lk_size);

		memcpy(&m_buffer[0], lrc.pBits, lk_size);

		hr = m_hd_buffer->UnlockRect();

#if 0
		hr = D3DXSaveSurfaceToFile(_T("d:\\test.bmp"),
			D3DXIFF_BMP,
			m_hd_buffer.get(),
			NULL, NULL);
#endif
	}
}//namespace nexus