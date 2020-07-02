#include "StdAfx.h"
#include "hdr_final_pass.h"
#include "d3d_exception.h"
#include "scene_render_targets.h"
#include "d3d_device_manager.h"
#include "util.h"


namespace nexus
{
	hdr_final_pass::hdr_final_pass(void)
	{
	}

	hdr_final_pass::~hdr_final_pass(void)
	{
	}

	void hdr_final_pass::create_resources()
	{
		//--
		m_lum.create_resources();
		m_bloom.create_resources();

		//--
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		HRESULT hr;

		const D3DSURFACE_DESC& display_desc = scene_render_targets::instance()->get_back_surface_desc();
		m_tex_final = scene_render_targets::create_item(display_desc.Width, display_desc.Height,
			D3DFMT_A16B16G16R16F);

		//--
		std::string effect_code;
		load_shder_source(_T("shader_d3d9/post_process/hdr_final.fx"), effect_code);

		ID3DXEffect* d3d_eft = NULL;
		hr = D3DXCreateEffect(device,
			effect_code.c_str(),
			effect_code.length(),
			NULL, NULL,
			0,
			NULL, &d3d_eft, NULL);

		if( FAILED(hr) )
			THROW_D3D_HRESULT(hr, _T("create hdr final effect failed."));

		m_effect.reset(d3d_eft);
	}

	void hdr_final_pass::draw_process(const rt_item& hdr_scene_rt)
	{
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		HRESULT hr;

		//-- test code
		//static rt_item hdr_scene_rt;
		//static bool test = true;
		//if( test )
		//{
		//	test = false;
		//	
		//	IDirect3DTexture9* d3d_tex = NULL;
		//	hr = D3DXCreateTextureFromFile(device, _T("HDR_Scene.dds"), &d3d_tex);
		//	hdr_scene_rt.tex.reset(d3d_tex);

		//	IDirect3DSurface9* d3d_surf = NULL;
		//	hr = hdr_scene_rt.tex->GetSurfaceLevel(0, &d3d_surf);
		//	hdr_scene_rt.surf.reset(d3d_surf);
		//}
		////--

		m_lum.measure_luminance(hdr_scene_rt);		

		m_bloom.draw_process(hdr_scene_rt);

		//--
		float	g_fExposure			= 0.50f;    // The exposure bias fed into the FinalPass.psh shader (OnFrameRender )
		float	g_GaussMultiplier	= 0.40f;     // Default multiplier

		hr = device->SetRenderTarget(0, m_tex_final.surf.get());

		const rt_item& lum_result_rt = m_lum.get_result();
		const rt_item& bloom_result_rt = m_bloom.get_result();

		hr = m_effect->SetTexture("g_texSceneColor", hdr_scene_rt.tex.get());
		hr = m_effect->SetTexture("g_texLum", lum_result_rt.tex.get());
		hr = m_effect->SetTexture("g_texBloom", bloom_result_rt.tex.get());

		D3DSURFACE_DESC d;
		hr = bloom_result_rt.tex->GetLevelDesc( 0, &d );

		hr = m_effect->SetFloat("g_rcp_bloom_tex_w", 1.0f / static_cast< float >( d.Width ) );
		hr = m_effect->SetFloat("g_rcp_bloom_tex_h", 1.0f / static_cast< float >( d.Height ) );
		hr = m_effect->SetFloat("fExposure", g_fExposure );
		hr = m_effect->SetFloat("fGaussianScalar", g_GaussMultiplier );

		
		scene_render_targets::instance()->draw_screen_quad(m_effect.get());

#if 0 // for test 
		for(size_t i=0; i<m_lum.get_num_rt(); i++)
		{
			scene_render_targets::instance()->debug_draw_target(m_lum.get_render_item(i), i+1, "techRed");
		}
#endif
	}
}//namespace nexus