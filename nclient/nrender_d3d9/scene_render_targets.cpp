#include "StdAfx.h"
#include "scene_render_targets.h"
#include "d3d_device_manager.h"
#include "d3d_exception.h"
#include "util.h"

namespace nexus
{
	///////////////////////////////////////////////////////////////////////////////////////
	//	class scene_render_targets
	///////////////////////////////////////////////////////////////////////////////////////
	scene_render_targets* scene_render_targets::s_inst = NULL;

	scene_render_targets::scene_render_targets(void)
	{
	}

	scene_render_targets::~scene_render_targets(void)
	{
	}

	void scene_render_targets::create_instance(enum ERenderType type, const render_config& cfg)
	{
		nASSERT(s_inst == NULL);
		switch(type)
		{
		case HDR:
			s_inst = nNew scene_render_targets_HDR;
			break;
		case LDR:
			s_inst = nNew scene_render_targets_LDR;
			break;
		default:
			nASSERT(0 && "unknown render targets type");
		}

		if(s_inst)
			s_inst->create(cfg);
	}

	void scene_render_targets::destroy_instance()
	{
		if( s_inst )
		{
			delete s_inst;
			s_inst = NULL;
		}
	}

	scene_render_targets* scene_render_targets::instance()
	{
		return s_inst;
	}

	rt_item scene_render_targets::create_item(UINT w, UINT h, D3DFORMAT fmt, DWORD usage)
	{
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();

		rt_item result;
		IDirect3DTexture9* tex = NULL;
		IDirect3DSurface9* surf = NULL;

		//-- create texture
		HRESULT hr;
		hr = device->CreateTexture(w, h, 1,
			usage, fmt, 
			D3DPOOL_DEFAULT, &tex, NULL);
		if( FAILED(hr) )
			THROW_D3D_HRESULT(hr, _T("create render target failed"));

		result.tex.reset(tex);

		//-- get surface
		hr = tex->GetSurfaceLevel(0, &surf);

		if( FAILED(hr) )
			THROW_D3D_HRESULT(hr, _T("get render texture surface failed"));

		result.surf.reset(surf);

		//
		D3DSURFACE_DESC desc;
		hr = surf->GetDesc(&desc);

		return result;
	}

	void scene_render_targets::adjust_screen_quad_uv(int tex_w, int tex_h, vertex vert_data[4])
	{
		// account for DirectX 9's texel center standard:
		float u_adjust = 0.5f / tex_w;
		float v_adjust = 0.5f / tex_h;

		vert_data[0].uv	= vector2(0+u_adjust, 1+v_adjust);
		vert_data[1].uv	= vector2(0+u_adjust, 0+v_adjust);
		vert_data[2].uv	= vector2(1+u_adjust, 0+v_adjust);
		vert_data[3].uv	= vector2(1+u_adjust, 1+v_adjust);
	}

	void scene_render_targets::create(const render_config& cfg)
	{
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		HRESULT hr;

		//-- create vertex declaration
		D3DVERTEXELEMENT9 decl[] =
		{
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }, 
			{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
		};

		IDirect3DVertexDeclaration9* d3d_decl = NULL;
		hr = device->CreateVertexDeclaration(decl, &d3d_decl);

		if( FAILED(hr) )
			THROW_D3D_HRESULT(hr, _T("create screen quad vertex declaration failed"));

		//-- create vertex buffer
		m_quad_vert_adjust[0].pos	= vector3(-1, -1, 0);
		m_quad_vert_adjust[1].pos	= vector3(-1, 1, 0);
		m_quad_vert_adjust[2].pos	= vector3(1, 1, 0);
		m_quad_vert_adjust[3].pos	= vector3(1, -1, 0);
		
		adjust_screen_quad_uv((int)cfg.width, (int)cfg.height, m_quad_vert_adjust);
		
		IDirect3DVertexBuffer9* d3d_vb = NULL;
		hr = device->CreateVertexBuffer(sizeof(m_quad_vert_adjust),
			D3DUSAGE_WRITEONLY, 0,
			D3DPOOL_MANAGED, &d3d_vb, NULL);

		if( FAILED(hr) )
			THROW_D3D_HRESULT(hr, _T("create screen quad vertex buffer failed"));
		
		d3d_vb_ptr smart_vb_ptr(d3d_vb);

		void* lock_vb = NULL;
		hr = d3d_vb->Lock(0, 0, &lock_vb, 0);

		if( FAILED(hr) )
			THROW_D3D_HRESULT(hr, _T("lock screen quad vertex buffer failed"));

		memcpy(lock_vb, &m_quad_vert_adjust, sizeof(m_quad_vert_adjust));
		hr = d3d_vb->Unlock();

		//--
		D3DSURFACE_DESC desc;
		
		IDirect3DSurface9* d3d_surf = NULL;
		hr = device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &d3d_surf);

		if( FAILED(hr) )
			THROW_D3D_HRESULT(hr, _T("get back surface failed"));

		hr = d3d_surf->GetDesc(&m_back_desc);
		
		IDirect3DSurface9* d3d_depth = NULL;
		hr = device->GetDepthStencilSurface(&d3d_depth);

		if( FAILED(hr) )
			THROW_D3D_HRESULT(hr, _T("get back depth surface failed"));

		hr = d3d_depth->GetDesc(&desc);
		
		m_back_surface.reset(d3d_surf);
		m_back_depth.reset(d3d_depth);

		//-- load debug draw effect
		std::string effect_code;
		load_shder_source(_T("shader_d3d9/post_process/pass_through.fx"), effect_code);
		
		ID3DXEffect* debug_effect = NULL;
		hr = D3DXCreateEffect(device,
			effect_code.c_str(),
			effect_code.length(),
			NULL, NULL,
			0,
			NULL, &debug_effect, NULL);
		
		if( SUCCEEDED(hr) )
			m_pass_through_effect.reset(debug_effect);

		//--
		m_vert_decl.reset(d3d_decl);
		m_default_quad_vb = smart_vb_ptr;
	}

	void scene_render_targets::draw_screen_quad(ID3DXEffect* eft)
	{
#if 0
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		HRESULT hr;

		hr = device->SetVertexDeclaration(m_vert_decl.get());
		hr = device->SetStreamSource(0, m_default_quad_vb.get(), 0, sizeof(vertex));

		UINT num_pass = 0;
		hr = eft->Begin(&num_pass, 0);

		if( FAILED(hr) )
			return;

		for(UINT i=0; i<num_pass; i++)
		{
			eft->BeginPass(0);

			hr = device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

			eft->EndPass();
		}

		eft->End();
#else
		struct TLVertex
		{
			D3DXVECTOR4 p;
			D3DXVECTOR2 t;
		};

		static const DWORD FVF_TLVERTEX = D3DFVF_XYZRHW | D3DFVF_TEX1;

		IDirect3DDevice9* pDev = d3d_device_manager::instance()->get_device();
		HRESULT hr;

		D3DSURFACE_DESC         desc;
		LPDIRECT3DSURFACE9      pSurfRT;

		pDev->GetRenderTarget(0, &pSurfRT);
		pSurfRT->GetDesc(&desc);
		pSurfRT->Release();

		// To correctly map from texels->pixels we offset the coordinates
		// by -0.5f:
		float fWidth = static_cast< float >( desc.Width ) - 0.5f;
		float fHeight = static_cast< float >( desc.Height ) - 0.5f;

		// Now we can actually assemble the screen-space geometry
		TLVertex v[4];

		v[0].p = D3DXVECTOR4( -0.5f, -0.5f, 0.0f, 1.0f );
		v[0].t = D3DXVECTOR2( 0.0f, 0.0f );

		v[1].p = D3DXVECTOR4( fWidth, -0.5f, 0.0f, 1.0f );
		v[1].t = D3DXVECTOR2( 1.0f, 0.0f );

		v[2].p = D3DXVECTOR4( -0.5f, fHeight, 0.0f, 1.0f );
		v[2].t = D3DXVECTOR2( 0.0f, 1.0f );

		v[3].p = D3DXVECTOR4( fWidth, fHeight, 0.0f, 1.0f );
		v[3].t = D3DXVECTOR2( 1.0f, 1.0f );

		// Configure the device and render..
		pDev->SetVertexShader( NULL );
		pDev->SetFVF( FVF_TLVERTEX );


		UINT num_pass = 0;
		hr = eft->Begin(&num_pass, 0);

		if( FAILED(hr) )
			return;

		for(UINT i=0; i<num_pass; i++)
		{
			eft->BeginPass(i);

			pDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, v, sizeof( TLVertex ) );

			eft->EndPass();
		}

		eft->End();
#endif
	}

	//void scene_render_targets::draw_screen_quad(ID3DXEffect* eft, int src_w, int src_h)
	//{
	//	adjust_screen_quad_uv(src_w, src_h, m_quad_vert_adjust);

	//	IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
	//	HRESULT hr;

	//	hr = device->SetVertexDeclaration(m_vert_decl.get());
	//	
	//	UINT num_pass = 0;
	//	hr = eft->Begin(&num_pass, 0);

	//	if( FAILED(hr) )
	//		return;

	//	for(UINT i=0; i<num_pass; i++)
	//	{
	//		eft->BeginPass(0);

	//		hr = device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, &m_quad_vert_adjust[0], sizeof(vertex));
	//		
	//		eft->EndPass();
	//	}

	//	eft->End();
	//}

	void scene_render_targets::begin_back_buffer()
	{
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();

		HRESULT hr;
		hr = device->SetRenderTarget(0, m_back_surface.get());
	}

	void scene_render_targets::end_back_buffer()
	{
	}

	void scene_render_targets::debug_draw_target(const rt_item& rt, int index, const char* tech)
	{
		if( !m_pass_through_effect )
			return;

		m_pass_through_effect->SetTechnique(tech);
		HRESULT hr;
		
		//-- make memory vertex buffer
		D3DSURFACE_DESC surf_desc;
		hr = rt.surf->GetDesc(&surf_desc);

		vertex quad_vert[4];

		float quad_size = 0.66f;
		float quad_corner = 1.0f-quad_size;
		quad_vert[0].pos	= vector3(-1, quad_corner, 0);
		quad_vert[1].pos	= vector3(-1, 1, 0);
		quad_vert[2].pos	= vector3(-quad_corner, 1, 0);
		quad_vert[3].pos	= vector3(-quad_corner, quad_corner, 0);

		float tab_size = quad_size+4.0f/m_back_desc.Width;
		int num_col = int(2.0f/tab_size);
		
		int row = index / num_col;
		int col = index % num_col;
		
		for(int i=0; i<4; i++)
		{
			quad_vert[i].pos += vector3(col*tab_size, -row*tab_size, 0);
		}

		adjust_screen_quad_uv(surf_desc.Width, surf_desc.Height, quad_vert);

		//-- draw
		// 请调节fx文件的pixel shader改变需要观察的颜色通道
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		hr = device->SetVertexDeclaration(m_vert_decl.get());

		hr = m_pass_through_effect->SetTexture("g_texSource", rt.tex.get());
		UINT num_pass = 0;
		hr = m_pass_through_effect->Begin(&num_pass, 0);

		if( FAILED(hr) )
			return;

		for(UINT i=0; i<num_pass; i++)
		{
			m_pass_through_effect->BeginPass(0);

			hr = device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, &quad_vert[0], sizeof(vertex));			

			m_pass_through_effect->EndPass();
		}

		m_pass_through_effect->End();
	}

	///////////////////////////////////////////////////////////////////////////////////////
	//	class scene_render_targets_HDR
	///////////////////////////////////////////////////////////////////////////////////////
	scene_render_targets_HDR::scene_render_targets_HDR(void)
	{
	}

	scene_render_targets_HDR::~scene_render_targets_HDR(void)
	{
	}

	void scene_render_targets_HDR::create(const render_config& cfg)
	{
		scene_render_targets::create(cfg);

		//--
		D3DFORMAT scene_color_fmt = D3DFMT_A16B16G16R16F;
		m_items[EItem_SceneColor] = create_item(cfg.width, cfg.height, scene_color_fmt);

		D3DFORMAT scene_depth_fmt = D3DFMT_D24S8;
		m_items[EItem_SceneDepth] = create_item(cfg.width, cfg.height, scene_depth_fmt, D3DUSAGE_DEPTHSTENCIL);

		//--
		m_hdr_final_pass.create_resources();
	}

	void scene_render_targets_HDR::begin_pre_pass()
	{
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();

		HRESULT hr;
		hr = device->SetDepthStencilSurface(m_items[EItem_SceneDepth].surf.get());
		hr = device->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1, 0);
	}
	
	void scene_render_targets_HDR::end_pre_pass()
	{}
	
	void scene_render_targets_HDR::begin_scene_color()
	{
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();

		HRESULT hr;
		hr = device->SetRenderTarget(0, m_items[EItem_SceneColor].surf.get());				
		hr = device->Clear(0, NULL, D3DCLEAR_TARGET, _clear_color, 1, 0);		
	}

	void scene_render_targets_HDR::end_scene_color()
	{
#if 0
		//HRESULT hr = D3DXSaveSurfaceToFile(_T("RT_SceneColor.dds"), D3DXIFF_DDS, 
		//	m_items[EItem_SceneColor].surf.get(), NULL, NULL);
#endif
	}

	void scene_render_targets_HDR::begin_transparency_buffer()
	{}
	void scene_render_targets_HDR::end_transparency_buffer()
	{}

	void scene_render_targets_HDR::draw_final_scene(bool enable_tonemap)
	{
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		HRESULT hr;
		hr = device->SetDepthStencilSurface(NULL);		

		if( enable_tonemap )
		{
			m_hdr_final_pass.draw_process(m_items[EItem_SceneColor]);
			hr = device->SetRenderTarget(0, m_back_surface.get());
			hr = m_pass_through_effect->SetTexture("g_texSource", m_hdr_final_pass.get_result().tex.get());			
			m_pass_through_effect->SetTechnique("techDefault");
			draw_screen_quad(m_pass_through_effect.get());
		}
		else
		{
			hr = device->SetRenderTarget(0, m_back_surface.get());			
			hr = m_pass_through_effect->SetTexture("g_texSource", m_items[EItem_SceneColor].tex.get());
			m_pass_through_effect->SetTechnique("techDefault");
			draw_screen_quad(m_pass_through_effect.get());
		}

		//-- for debug
		//debug_draw_target(m_items[EItem_SceneColor], 0);
	}

	void scene_render_targets_LDR::begin_transparency_buffer()
	{
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();

		HRESULT hr;
		hr = device->SetRenderTarget(0, m_trans_buffer.surf.get());
		hr = device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1, 0);
	}

	void scene_render_targets_LDR::end_transparency_buffer()
	{
		// blend trans buffer to back buffer
		begin_back_buffer();
		
		HRESULT hr;
		hr = m_pass_through_effect->SetTexture("g_texSource", m_trans_buffer.tex.get());
		m_pass_through_effect->SetTechnique("techAlphaBlend");
		draw_screen_quad(m_pass_through_effect.get());

		end_back_buffer();
	}

	
	void scene_render_targets_LDR::create(const render_config& cfg)
	{
		scene_render_targets::create(cfg);

		m_dummy_rt = create_item(1, 1, m_back_desc.Format);
		m_trans_buffer = create_item(m_back_desc.Width, m_back_desc.Height, D3DFMT_A8R8G8B8);
	}
}//namespace nexus