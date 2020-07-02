#include "StdAfx.h"
#include "post_bloom.h"

#include "d3d_exception.h"
#include "scene_render_targets.h"
#include "d3d_device_manager.h"
#include "util.h"


namespace nexus
{
	float ComputeGaussianValue( float x, float mean, float std_deviation )
	{
		// The gaussian equation is defined as such:
		/*    
		-(x - mean)^2
		-------------
		1.0               2*std_dev^2
		f(x,mean,std_dev) = -------------------- * e^
		sqrt(2*pi*std_dev^2)

		*/

		return ( 1.0f / sqrt( 2.0f * D3DX_PI * std_deviation * std_deviation ) ) 
			* expf( (-((x-mean)*(x-mean)))/(2.0f * std_deviation * std_deviation) );
	}

	post_bloom::post_bloom(void)
	{
	}

	post_bloom::~post_bloom(void)
	{
	}

	void post_bloom::create_resources()
	{
		// [ 0 ] GATHER NECESSARY INFORMATION
		//-----------------------------------
		const D3DSURFACE_DESC& display_desc = scene_render_targets::instance()->get_back_surface_desc();
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		HRESULT hr;

		D3DFORMAT fmt = D3DFMT_A16B16G16R16F;

		// [ 1 ] CREATE BRIGHT PASS TEXTURE
		//---------------------------------
		// Bright pass texture is 1/2 the size of the original HDR render target.
		// Part of the pixel shader performs a 2x2 downsampling. The downsampling
		// is intended to reduce the number of pixels that need to be worked on - 
		// in general, HDR pipelines tend to be fill-rate heavy.
		m_bright_pass_rt = scene_render_targets::create_item(display_desc.Width/2,
			display_desc.Height/2, fmt);

		// [ 3 ] CREATE DOWNSAMPLED TEXTURE
		//---------------------------------
		// This render target is 1/8th the size of the original HDR image (or, more
		// importantly, 1/4 the size of the bright-pass). The downsampling pixel
		// shader performs a 4x4 downsample in order to reduce the number of pixels
		// that are sent to the horizontal/vertical blurring stages.
		m_down_sampled_rt = scene_render_targets::create_item(display_desc.Width/8,
			display_desc.Height/8, fmt);

		// [ 4 ] CREATE HORIZONTAL BLOOM TEXTURE
		//--------------------------------------
		// The horizontal bloom texture is the same dimension as the down sample
		// render target. Combining a 4x4 downsample operation as well as a
		// horizontal blur leads to a prohibitively high number of texture reads.
		m_bloom_horizontal_rt = scene_render_targets::create_item(display_desc.Width/8,
			display_desc.Height/8, fmt);

		// [ 6 ] CREATE VERTICAL BLOOM TEXTURE
		//------------------------------------
		// The vertical blur texture must be the same size as the horizontal blur texture
		// so as to get a correct 2D distribution pattern.
		m_bloom_vertical_rt = scene_render_targets::create_item(display_desc.Width/8,
			display_desc.Height/8, fmt);

		//-- create effect
		std::string effect_code;
		load_shder_source(_T("shader_d3d9/post_process/bloom.fx"), effect_code);

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

	}

	void post_bloom::draw_process(const rt_item& source_rt)
	{
		float                       g_BrightThreshold           = 0.8f;             // A configurable parameter into the pixel shader
		float                       g_GaussMultiplier           = 0.4f;             // Default multiplier
		float                       g_GaussMean                 = 0.0f;             // Default mean for gaussian distribution
		float                       g_GaussStdDev               = 0.8f;             // Default standard deviation for gaussian distribution

		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		HRESULT hr;

		// [ 0 ] BRIGHT PASS
		//------------------
		hr = device->SetRenderTarget(0, m_bright_pass_rt.surf.get());
		hr = m_effect->SetTexture("g_tex", source_rt.tex.get());
		hr = m_effect->SetTechnique("techBrightPass");
		hr = m_effect->SetFloat("fBrightPassThreshold", g_BrightThreshold);

		// We need to compute the sampling offsets used for this pass.
		// A 2x2 sampling pattern is used, so we need to generate 4 offsets
		D3DXVECTOR4 offsets[4];

		// Find the dimensions for the source data
		D3DSURFACE_DESC srcDesc;
		source_rt.tex->GetLevelDesc( 0, &srcDesc );

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

		hr = m_effect->SetVectorArray("tcDownSampleOffsets", offsets, 4 );

		scene_render_targets::instance()->draw_screen_quad(m_effect.get());

		// [ 1 ] DOWN SAMPLE
		//------------------
		hr = device->SetRenderTarget(0, m_down_sampled_rt.surf.get());
		hr = m_effect->SetTexture("g_tex", m_bright_pass_rt.tex.get());
		hr = m_effect->SetTechnique("techDownSample");

		// We need to compute the sampling offsets used for this pass.
		// A 4x4 sampling pattern is used, so we need to generate 16 offsets

		// Find the dimensions for the source data
		m_bright_pass_rt.tex->GetLevelDesc( 0, &srcDesc );

		// Find the dimensions for the destination data
		D3DSURFACE_DESC destDesc;
		m_down_sampled_rt.surf->GetDesc( &destDesc );

		// Compute the offsets required for down-sampling. If constant-storage space
		// is important then this code could be packed into 8xFloat4's. The code here
		// is intentionally less efficient to aid readability...
		D3DXVECTOR4 dsOffsets[16];
		int idx = 0;
		for( int i = -2; i < 2; i++ )
		{
			for( int j = -2; j < 2; j++ )
			{
				dsOffsets[idx++] = D3DXVECTOR4( 
					(static_cast< float >( i ) + 0.5f) * ( 1.0f / static_cast< float >( destDesc.Width )), 
					(static_cast< float >( j ) + 0.5f) * ( 1.0f / static_cast< float >( destDesc.Height )), 
					0.0f, // unused 
					0.0f  // unused
					);
			}
		}

		m_effect->SetVectorArray("tcDownSampleOffsets", dsOffsets, 16 );

		scene_render_targets::instance()->draw_screen_quad(m_effect.get());

		// [ 2 ] BLUR HORIZONTALLY
		//------------------------
		hr = device->SetRenderTarget(0, m_bloom_horizontal_rt.surf.get());
		hr = m_effect->SetTexture("g_tex", m_down_sampled_rt.tex.get());
		hr = m_effect->SetTechnique("techHorizontalBlur");

		// Configure the sampling offsets and their weights
		float HBloomWeights[9];
		float HBloomOffsets[9];

		for( int i = 0; i < 9; i++ )
		{
			// Compute the offsets. We take 9 samples - 4 either side and one in the middle:
			//     i =  0,  1,  2,  3, 4,  5,  6,  7,  8
			//Offset = -4, -3, -2, -1, 0, +1, +2, +3, +4
			HBloomOffsets[i] = (static_cast< float >( i ) - 4.0f) * ( 1.0f / static_cast< float >( destDesc.Width ) );

			// 'x' is just a simple alias to map the [0,8] range down to a [-1,+1]
			float x = (static_cast< float >( i ) - 4.0f) / 4.0f;

			// Use a gaussian distribution. Changing the standard-deviation
			// (second parameter) as well as the amplitude (multiplier) gives
			// distinctly different results.
			HBloomWeights[i] = g_GaussMultiplier * ComputeGaussianValue( x, g_GaussMean, g_GaussStdDev );
		}

		// Commit both arrays to the device:
		m_effect->SetFloatArray("HBloomWeights", HBloomWeights, 9 );
		m_effect->SetFloatArray("HBloomOffsets", HBloomOffsets, 9 );


		scene_render_targets::instance()->draw_screen_quad(m_effect.get());

		// [ 3 ] BLUR VERTICALLY
		//----------------------
		hr = device->SetRenderTarget(0, m_bloom_vertical_rt.surf.get());
		hr = m_effect->SetTexture("g_tex", m_bloom_horizontal_rt.tex.get());
		hr = m_effect->SetTechnique("techVerticalBlur");

		// Configure the sampling offsets and their weights

		// It is worth noting that although this code is almost identical to the
		// previous section ('H' weights, above) there is an important difference: destDesc.Height.
		// The bloom render targets are *not* square, such that you can't re-use the same offsets in
		// both directions.
		float VBloomWeights[9];
		float VBloomOffsets[9];

		for( int i = 0; i < 9; i++ )
		{
			// Compute the offsets. We take 9 samples - 4 either side and one in the middle:
			//     i =  0,  1,  2,  3, 4,  5,  6,  7,  8
			//Offset = -4, -3, -2, -1, 0, +1, +2, +3, +4
			VBloomOffsets[i] = (static_cast< float >( i ) - 4.0f) * ( 1.0f / static_cast< float >( destDesc.Height ) );

			// 'x' is just a simple alias to map the [0,8] range down to a [-1,+1]
			float x = (static_cast< float >( i ) - 4.0f) / 4.0f;

			// Use a gaussian distribution. Changing the standard-deviation
			// (second parameter) as well as the amplitude (multiplier) gives
			// distinctly different results.
			VBloomWeights[i] = g_GaussMultiplier * ComputeGaussianValue( x, g_GaussMean, g_GaussStdDev );
		}

		// Commit both arrays to the device:
		m_effect->SetFloatArray("VBloomWeights", VBloomWeights, 9 );
		m_effect->SetFloatArray("VBloomOffsets", VBloomOffsets, 9 );

		m_bloom_horizontal_rt.surf->GetDesc(&srcDesc);
		scene_render_targets::instance()->draw_screen_quad(m_effect.get());

	}
}//namespace nexus
