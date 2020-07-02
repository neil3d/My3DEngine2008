#include "StdAfx.h"
#include "d3d9_texture.h"
#include "d3d_device_manager.h"
#include "d3d_exception.h"

namespace nexus
{
	d3d9_texture2D::d3d9_texture2D(void)
	{
	}

	d3d9_texture2D::~d3d9_texture2D(void)
	{
	}

	d3d_texture_ptr load_d3d9_texture_file(nfile_system* fs, const nstring& pkg_name, const nstring& file_name)
	{
		//-- read file to memory
		size_t file_size = fs->get_file_size(pkg_name, file_name);
		if( file_size==0
			|| file_size==-1)
		{
			nLog_Error(_T("load texture failed, file=%s.%s\r\n"), pkg_name.c_str(), file_name.c_str());
			THROW_D3D_EXCEPTION(_T("load texture failed."));
		}

		nfile::ptr file_ptr = fs->open_file(pkg_name, file_name, EFileRead);
		boost::scoped_array<unsigned char> file_lump(nNew unsigned char[file_size]);

		file_ptr->read_buffer(file_lump.get(), file_size);

		//-- create from memory
		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();

		IDirect3DTexture9* d3d_tex = NULL;
		// 使用D3DX_DEFAULT设置filter时，nvPerfHud失败
		HRESULT hr = D3DXCreateTextureFromFileInMemoryEx(
			d3d_device,			// pDevice
			file_lump.get(),	// pSrcData
			(UINT)file_size,	// SrcDataSize			
			D3DX_DEFAULT,		// Width
			D3DX_DEFAULT,		// Height
			D3DX_DEFAULT,		// MipLevels
			0,					// Usage
			D3DFMT_UNKNOWN,		// Format
			D3DPOOL_MANAGED,	// Pool
			D3DX_FILTER_LINEAR|D3DX_FILTER_DITHER,	// Filter
			D3DX_FILTER_LINEAR,	// MipFilter
			0,					// ColorKey
			NULL,				// pSrcInfo
			NULL,				// pPalette
			&d3d_tex			// ppTexture
			);

		if( FAILED(hr) )
		{
			nLog_Error(_T("create texture failed, file=%s.%s\r\n"), pkg_name.c_str(), file_name.c_str());
			THROW_D3D_HRESULT(hr, _T("create texture failed"));
		}

		return d3d_texture_ptr(d3d_tex);
	}

	void d3d9_texture2D::load_from_file(nfile_system* fs, const nstring& pkg_name, const nstring& file_name)
	{
		m_texture = load_d3d9_texture_file(fs, pkg_name, file_name);
	}

	void d3d9_cube_map::load_from_file(nfile_system* fs, const nstring& pkg_name, const nstring& file_name)
	{
		//-- read file to memory
		size_t file_size = fs->get_file_size(pkg_name, file_name);
		if( file_size==0
			|| file_size==-1)
		{
			nLog_Error(_T("load texture failed, file=%s.%s\r\n"), pkg_name.c_str(), file_name.c_str());
			THROW_D3D_EXCEPTION(_T("load texture failed."));
		}

		nfile::ptr file_ptr = fs->open_file(pkg_name, file_name, EFileRead);
		boost::scoped_array<unsigned char> file_lump(nNew unsigned char[file_size]);

		file_ptr->read_buffer(file_lump.get(), file_size);

		//-- create from memory
		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();

		IDirect3DCubeTexture9* d3d_tex = NULL;
		HRESULT hr = D3DXCreateCubeTextureFromFileInMemory(d3d_device,
			file_lump.get(),
			(UINT)file_size,
			&d3d_tex);

		if( FAILED(hr) )
		{
			nLog_Error(_T("create texture failed, file=%s.%s\r\n"), pkg_name.c_str(), file_name.c_str());
			THROW_D3D_HRESULT(hr, _T("create texture failed"));
		}

		//--
		m_texture.reset(d3d_tex);		
	}

	////////////////////////////////////////////////////////////////////////////
	//	class d3d9_heightmap
	////////////////////////////////////////////////////////////////////////////
	d3d9_heightmap::d3d9_heightmap(void)
	{}

	d3d9_heightmap::~d3d9_heightmap(void)
	{}

	IDirect3DTexture9* _create_d3d_texture(size_t w, size_t h, size_t mip_lv, D3DFORMAT fmt)
	{
		nASSERT( w>0 && h>0 );

		IDirect3DDevice9* d3d_device = d3d_device_manager::instance()->get_device();
		IDirect3DTexture9* d3d_tex = NULL;

		DWORD usage = 0;
		HRESULT hr = d3d_device->CreateTexture(w, h, mip_lv, usage, fmt, 
			D3DPOOL_MANAGED, &d3d_tex, NULL);

		return d3d_tex;
	}

	void d3d9_heightmap::create(size_t w, size_t h)
	{
		IDirect3DTexture9* d3d_tex = _create_d3d_texture(w, h, 1, D3DFMT_R32F);

		if( !d3d_tex )
		{
			nLog_Error(_T("create heightmap texture failed, size=(%d, %d)\r\n"), w, h);
			THROW_D3D_EXCEPTION(_T("create height map texture failed"));
		}

		m_texture.reset(d3d_tex);
	}

	inline void _convert_rect(const nrect& rc, RECT& d3d_rc)
	{
		d3d_rc.left = rc.left;
		d3d_rc.top = rc.top;
		d3d_rc.right = rc.right;
		d3d_rc.bottom = rc.bottom;
	}

	void d3d9_heightmap::copy_heightmap(const nrect& rc, nheight_map16* src)
	{
		// todo : lock rect instead of entire texure
		nASSERT( src );

		HRESULT hr;
		
		RECT d3d_rc;
		_convert_rect(rc, d3d_rc);
		
		nsize tex_size = get_size();
		nASSERT( tex_size.x = src->get_width() );
		nASSERT( tex_size.y = src->get_height() );

		D3DLOCKED_RECT lrc;
		//hr = m_texture->LockRect(0, &lrc, &d3d_rc, 0);
		hr = m_texture->LockRect(0, &lrc, NULL, 0);

		if( FAILED(hr) )
		{
			THROW_D3D_HRESULT(hr, _T("lock height map failed."));
		}

		UINT w = tex_size.x; //rc.get_width();
		UINT h = tex_size.y; //rc.get_height();
		BYTE* line = static_cast<BYTE*>(lrc.pBits);

		for(UINT y=0; y<h; y++)
		{
			float* texel = (float*)(line);
			for( UINT x=0; x<w; x++ )
			{
				texel[x] = (float)( src->get_value(x,y) );
			}
			line += lrc.Pitch;
		}


		hr = m_texture->UnlockRect(0);
	}

	void d3d9_heightmap::update_rect(const nrect& rc, nheight_map16* src)
	{
		nASSERT( src );
	}

	nsize d3d9_heightmap::get_size() const
	{
		D3DSURFACE_DESC desc;
		HRESULT hr = m_texture->GetLevelDesc(0, &desc);
		nASSERT( SUCCEEDED(hr) );

		return nsize(desc.Width, desc.Height);
	}

	////////////////////////////////////////////////////////////////////////////
	//	class d3d9_alphamap
	////////////////////////////////////////////////////////////////////////////
	d3d9_alphamap::d3d9_alphamap(void)
	{}

	d3d9_alphamap::~d3d9_alphamap(void)
	{}

	void d3d9_alphamap::create(size_t w, size_t h)
	{
		IDirect3DTexture9* d3d_tex = _create_d3d_texture(w, h, 1, D3DFMT_A8R8G8B8);

		if( !d3d_tex )
		{
			nLog_Error(_T("create alphamap texture failed, size=(%d, %d)\r\n"), w, h);
			THROW_D3D_EXCEPTION(_T("create alphamap texture failed"));
		}

		m_texture.reset(d3d_tex);
	}

	void d3d9_alphamap::copy_alpha(size_t channel, const nrect& rc, nalpha_map* src)
	{
		nASSERT( channel < 4);
		nASSERT( src );		

		HRESULT hr;

		RECT d3d_rc;
		_convert_rect(rc, d3d_rc);

		nsize tex_size = get_size();
		nASSERT( tex_size.x = src->get_width() );
		nASSERT( tex_size.y = src->get_height() );

		D3DLOCKED_RECT lrc;
		hr = m_texture->LockRect(0, &lrc, NULL, 0);

		if( FAILED(hr) )
		{
			THROW_D3D_HRESULT(hr, _T("lock alpha map failed."));
		}

		UINT w = tex_size.x; //rc.get_width();
		UINT h = tex_size.y; //rc.get_height();
		BYTE* line = static_cast<BYTE*>(lrc.pBits);

		struct pixel
		{
			unsigned char channel[4];
		};
		

		channel = 3-channel;

		for(UINT y=0; y<h; y++)
		{
			pixel* texel = (pixel*)(line);
			for( UINT x=0; x<w; x++ )
			{
				texel[x].channel[channel] = src->get_value(x,y);
			}
			line += lrc.Pitch;
		}

		hr = m_texture->UnlockRect(0);
	}

	nsize d3d9_alphamap::get_size() const
	{
		D3DSURFACE_DESC desc;
		HRESULT hr = m_texture->GetLevelDesc(0, &desc);
		nASSERT( SUCCEEDED(hr) );

		return nsize(desc.Width, desc.Height);
	}
}//namespace nexus