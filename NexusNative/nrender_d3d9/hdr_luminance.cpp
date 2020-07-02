#include "StdAfx.h"
#include "hdr_luminance.h"
#include "d3d_exception.h"
#include "scene_render_targets.h"
#include "d3d_device_manager.h"
#include "util.h"

namespace nexus
{
	hdr_luminance::hdr_luminance(void)
	{
	}

	hdr_luminance::~hdr_luminance(void)
	{
	}

	void hdr_luminance::create_resources()
	{
		const D3DSURFACE_DESC& display_desc = scene_render_targets::instance()->get_back_surface_desc();
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		HRESULT hr;

		D3DFORMAT fmt = find_best_fromat();

		//-- create lum render targets
		UINT tex_size = 1;
		const int MAX_LUM_TEX = 6;

		for( int i=0; i<MAX_LUM_TEX; i++)
		{
			rt_item new_rt = scene_render_targets::create_item(tex_size, tex_size, fmt);
			m_lum_tex_array.push_back(new_rt);

			// Increment for the next texture
			tex_size *= 3;

			if( tex_size > display_desc.Width
				|| tex_size > display_desc.Height)
				break;
		}

		m_lum_adapted = scene_render_targets::create_item(1, 1, fmt);
		m_last_adapted = scene_render_targets::create_item(1, 1, fmt);

		hr = device->ColorFill(m_lum_tex_array[0].surf.get(), NULL, 0);
		hr = device->ColorFill(m_lum_adapted.surf.get(), NULL, 0);
		hr = device->ColorFill(m_last_adapted.surf.get(), NULL, 0);
		

		//-- create effect
		std::string effect_code;
		load_shader_source(_T("shader_d3d9/post_process/hdr_luminance.fx"), effect_code);

		ID3DXEffect* d3d_eft = NULL;
		hr = D3DXCreateEffect(device,
			effect_code.c_str(),
			(UINT)effect_code.length(),
			NULL, NULL,
			0,
			NULL, &d3d_eft, NULL);

		if( FAILED(hr) )
			THROW_D3D_HRESULT(hr, _T("create hdr luminance effect failed."));

		m_effect.reset(d3d_eft);

		//--
		m_timer.restart();
	}

	D3DFORMAT hdr_luminance::find_best_fromat()
	{
		D3DFORMAT fmt = D3DFMT_G16R16F;

		// todo : check device
		return fmt;
	}

	//--------------------------------------------------------------------------------------
	//      DESC:
	//          This is the core function for this particular part of the application, it's
	//          job is to take the previously rendered (in the 'HDRScene' namespace) HDR
	//          image and compute the overall luminance for the scene. This is done by
	//          repeatedly downsampling the image until it is only 1x1 in size. Doing it
	//          this way (pixel shaders and render targets) keeps as much of the work on
	//          the GPU as possible, consequently avoiding any resource transfers, locking
	//          and modification.
	//--------------------------------------------------------------------------------------
	void hdr_luminance::measure_luminance(const rt_item& scene_color_rt)
	{
		size_t num_lum_tex = m_lum_tex_array.size();
		if( num_lum_tex <= 0)
			return;

		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		HRESULT hr;

		//[ 0 ] DECLARE VARIABLES AND ALIASES
		//-----------------------------------
		rt_item	source_rt;
		rt_item	dest_rt;

		//[ 1 ] SET THE DEVICE TO RENDER TO THE HIGHEST RESOLUTION LUMINANCE MAP.
		//---------------------------------------------		
		dest_rt = m_lum_tex_array[num_lum_tex-1];

		hr = device->SetRenderTarget(0, dest_rt.surf.get());
		hr = m_effect->SetTexture("g_texLum", scene_color_rt.tex.get());

		//[ 2 ] RENDER AND DOWNSAMPLE THE HDR TEXTURE TO THE LUMINANCE MAP.
		//-------------------------------------------

		// Set which shader we're going to use. g_pLum1PS corresponds
		// to the 'GreyScaleDownSample' entry point in 'Luminance.psh'.
		hr = m_effect->SetTechnique("lumFirstPass");

		// We need to compute the sampling offsets used for this pass.
		// A 2x2 sampling pattern is used, so we need to generate 4 offsets.
		//
		// NOTE: It is worth noting that some information will likely be lost
		//       due to the luminance map being less than 1/2 the size of the
		//       original render-target. This mis-match does not have a particularly
		//       big impact on the final luminance measurement. If necessary,
		//       the same process could be used - but with many more samples, so as
		//       to correctly map from HDR->Luminance without losing information.
		D3DXVECTOR4 offsets[4];

		// Find the dimensions for the source data
		D3DSURFACE_DESC srcDesc;
		scene_color_rt.tex->GetLevelDesc( 0, &srcDesc );

		// Because the source and destination are NOT the same sizes, we
		// need to provide offsets to correctly map between them.
		float sU = (1.0f / static_cast< float >(srcDesc.Width));
		float sV = (1.0f / static_cast< float >(srcDesc.Height));

		// The last two components (z,w) are unused. This makes for simpler code, but if
		// constant-storage is limited then it is possible to pack 4 offsets into 2 float4's
		offsets[0] = D3DXVECTOR4( -0.5f * sU,  0.5f * sV, 0.0f, 0.0f );
		offsets[1] = D3DXVECTOR4(  0.5f * sU,  0.5f * sV, 0.0f, 0.0f );
		offsets[2] = D3DXVECTOR4( -0.5f * sU, -0.5f * sV, 0.0f, 0.0f );
		offsets[3] = D3DXVECTOR4(  0.5f * sU, -0.5f * sV, 0.0f, 0.0f );

		// Set the offsets to the constant table
		hr = m_effect->SetVectorArray("tcLumOffsets", offsets, 4 );

		// With everything configured we can now render the first, initial, pass
		// to the luminance textures.
		scene_render_targets::instance()->draw_screen_quad(m_effect.get());

		//[ 3 ] SCALE EACH RENDER TARGET DOWN
		//      The results ("dest") of each pass feeds into the next ("src")
		//-------------------------------------------------------------------
		hr = m_effect->SetTechnique("lumDownSample");
		for(int i=int(num_lum_tex-1); i>0; i--)
		{
			// Configure the render targets for this iteration
			source_rt = m_lum_tex_array[i];
			dest_rt =m_lum_tex_array[i-1];

			hr = device->SetRenderTarget(0, dest_rt.surf.get());
			hr = m_effect->SetTexture("g_texLum", source_rt.tex.get());

			// Because each of these textures is a factor of 3
			// different in dimension, we use a 3x3 set of sampling
			// points to downscale.
			D3DSURFACE_DESC srcTexDesc;
			hr = source_rt.tex->GetLevelDesc( 0, &srcTexDesc );

			// Create the 3x3 grid of offsets
			D3DXVECTOR4 DSoffsets[9];
			int idx = 0;
			for( int x = -1; x < 2; x++ )
			{
				for( int y = -1; y < 2; y++ )
				{
					DSoffsets[idx++] = D3DXVECTOR4(
						static_cast< float >( x ) / static_cast< float >( srcTexDesc.Width ),
						static_cast< float >( y ) / static_cast< float >( srcTexDesc.Height ),
						0.0f,   //unused
						0.0f    //unused
						);
				}
			}

			// Set them to the current pixel shader
			hr = m_effect->SetVectorArray("tcDSOffsets", DSoffsets, 9 );						

			// Render the display to this texture
			scene_render_targets::instance()->draw_screen_quad(m_effect.get());
		}

		// =============================================================
		//    At this point, the g_pTexLuminance[0] texture will contain
		//    a 1x1 texture that has the downsampled luminance for the
		//    scene as it has currently been rendered.
		// =============================================================		


		//-----------------------------------------------------------------------------
		// Name: CalculateAdaptation()
		// Desc: Increment the user's adapted luminance
		//-----------------------------------------------------------------------------
		// Swap current & last luminance
		std::swap(m_lum_adapted, m_last_adapted);

		// This simulates the light adaptation that occurs when moving from a 
		// dark area to a bright area, or vice versa. The g_pTexAdaptedLuminance
		// texture stores a single texel cooresponding to the user's adapted 
		// level.
		hr = m_effect->SetTechnique("lumAdapted");
		hr = m_effect->SetFloat("g_fElapsedTime", (float)m_timer.elapsed());
		m_timer.restart();

		hr = device->SetRenderTarget(0, m_lum_adapted.surf.get());
		hr = m_effect->SetTexture("g_texLastLum", m_lum_tex_array[0].tex.get());
		hr = m_effect->SetTexture("g_texLum", m_last_adapted.tex.get());

		scene_render_targets::instance()->draw_screen_quad(m_effect.get());
	}
}//namespace nexus