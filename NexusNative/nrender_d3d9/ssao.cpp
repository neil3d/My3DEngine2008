#include "StdAfx.h"
#include "ssao.h"
#include "nrenderer_d3d9.h"
#include "global_shader_lib.h"
#include "d3d_device_manager.h"
#include "scene_render_targets.h"

namespace nexus
{
	ssao::ssao(
		EQuality quality
		):m_quality(quality)
	 ,m_occlustion_radius(20)
	 ,m_occlustion_power(3.5)
	 ,m_blocker_power(1.5f)
	{
		scene_render_targets* scene_rt = scene_render_targets::instance();
		D3DSURFACE_DESC desc = scene_rt->get_back_surface_desc();
		UINT factor = 0;
		if (m_quality == low)
		{
			factor = 4;
		}
		else if (m_quality == medium)
		{
			factor = 2;
		}
		else if (m_quality == high)
		{
			factor = 1;
		}
		
		m_items[0] = scene_render_targets::create_item(desc.Width/factor, desc.Height/factor, desc.Format);
		m_items[1] = scene_render_targets::create_item(desc.Width/factor, desc.Height/factor,  desc.Format);
	}

	ssao::~ssao(void)
	{
		m_items[0].release();
		m_items[1].release();
	}

#define sampler_count 32
	struct random_normal
	{
		vector<vector4> verts;

		random_normal()
		{
			verts.reserve(sampler_count);
			for (int i = 0; i < sampler_count; i ++)
			{
				vector4 vertex(
					(float)rand()/RAND_MAX*2 - 1,
					(float)rand()/RAND_MAX*2 - 1,
					(float)rand()/RAND_MAX*2 - 1,
					1
					);
				vertex = vec_normalize(vertex);
				vertex*= (float)rand()/RAND_MAX;
				verts.push_back(vertex);
			}
		}
	};

	random_normal sampler_point;

	static d3d_texture_ptr get_radom_rot()
	{
		static d3d_texture_ptr m_rot_texture;
		if (!m_rot_texture)
		{
			IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
			LPDIRECT3DTEXTURE9 rot_texture = NULL;
			HRESULT hr = D3DXCreateTextureFromFile(device,_T("../nexus_game/engine_data/resource_default/rotrandomCM.dds"),&rot_texture);
			m_rot_texture.reset(rot_texture);
		}

		return m_rot_texture;
	}

	static d3d_volume_texture_ptr get_Jitter()
	{
		static d3d_volume_texture_ptr m_jitter_texture;
		if (!m_jitter_texture)
		{
			IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
			LPDIRECT3DVOLUMETEXTURE9 JitterTexture = NULL;
			HRESULT hr = D3DXCreateVolumeTextureFromFile(device,_T("../nexus_game/engine_data/resource_default/Jitter.dds"),&JitterTexture);
			m_jitter_texture.reset(JitterTexture);
		}

		return m_jitter_texture;
	}

	void ssao::render() const
	{
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		nrenderer_d3d9* render = nrenderer_d3d9::instance();
		scene_render_targets* scene_rt = scene_render_targets::instance();
		d3d_effect_ptr effect = global_shader_lib::instance()->find_shader(
			_T("ssao.fx"));

		assert(effect);

		device->SetRenderTarget(0, m_items[0].surf.get());
		device->Clear(0, NULL, D3DCLEAR_TARGET, 0xffffff, 1, 0);
		effect->SetTechnique("techDefault");

		const rt_item& scene_depth = scene_rt->get_render_target(ERT_SceneNormalDepth);
		effect->SetTexture("SceneNormalAndDepth",scene_depth.tex.get());
		matrix44 screen_to_view = render->get_view_info()->mat_project;
		matrix44 inv_project = mat_inverse(render->get_view_info()->mat_project);
		screen_to_view.m[0][0] = 1;
		screen_to_view.m[1][1] = 1;
		screen_to_view = screen_to_view*inv_project;

		matrix44 screen_to_world = render->get_view_info()->mat_project;
		matrix44 inv_view_project = mat_inverse(render->get_view_info()->mat_view_project);
		screen_to_world.m[0][0] = 1;
		screen_to_world.m[1][1] = 1;
		screen_to_world = screen_to_world*inv_view_project;

		effect->SetMatrix("ScreenToView",(D3DXMATRIX*)&(screen_to_view) );
		effect->SetMatrix("ScreenToWorld",(D3DXMATRIX*)&(screen_to_world) );
		effect->SetMatrix("ProjMatrix",(D3DXMATRIX*)&(render->get_view_info()->mat_project) );
		effect->SetMatrix("ViewMatrix",(D3DXMATRIX*)& (render->get_view_info()->mat_view));
		effect->SetVector("ViewPos",(D3DXVECTOR4*)&render->get_view_info()->eye_pos);
    	effect->SetVector("ScreenScaleBias",(D3DXVECTOR4*)&scene_rt->calc_screen_scale_bias());

		IDirect3DSurface9* surf;
		D3DSURFACE_DESC desc;
		device->GetRenderTarget(0,&surf);
		surf->GetDesc(&desc);
		surf->Release();

		vector2 random_uv_cale((float)desc.Width/64,(float)desc.Height/64);
		
		effect->SetVector("RandomUVScale",(D3DXVECTOR4*)&random_uv_cale);
		effect->SetVectorArray("SamplerPoints",(D3DXVECTOR4*)&sampler_point.verts[0],sampler_count);
		effect->SetTexture("RandomRot",get_radom_rot().get());

		effect->SetFloat("OcclustionRadius",m_occlustion_radius);
		effect->SetFloat("OcclustionPower",m_occlustion_power);
		effect->SetFloat("BlockerPower",m_blocker_power);
		scene_rt->draw_screen_quad(effect.get());

		// Configure the sampling offsets and their weights
		float HBloomWeights[9];
		float HBloomOffsets[9];

		static float     g_GaussMultiplier           = 0.3f;             // Default multiplier
		static float     g_GaussMean                 = 0.0f;             // Default mean for gaussian distribution
		static float     g_GaussStdDev               = 0.83f;             // Default standard deviation for gaussian distribution

		for( int i = 0; i < 9; i++ )
		{
			// Compute the offsets. We take 9 samples - 4 either side and one in the middle:
			//     i =  0,  1,  2,  3, 4,  5,  6,  7,  8
			//Offset = -4, -3, -2, -1, 0, +1, +2, +3, +4
			HBloomOffsets[i] = (static_cast< float >( i ) - 4.0f) * ( 1.0f / static_cast< float >( desc.Width ) );

			// 'x' is just a simple alias to map the [0,8] range down to a [-1,+1]
			float x = (static_cast< float >( i ) - 4.0f) / 4.0f;

			// Use a gaussian distribution. Changing the standard-deviation
			// (second parameter) as well as the amplitude (multiplier) gives
			// distinctly different results.
			HBloomWeights[i] = g_GaussMultiplier * ComputeGaussianValue( x, g_GaussMean, g_GaussStdDev );
		}

		// Commit both arrays to the device:
		device->SetRenderTarget(0, m_items[1].surf.get());
		if ( m_quality == high)
		{
			effect->SetTechnique("techHorizontalBlurFliter");
		} 
		else
		{
			effect->SetTechnique("techHorizontalBlur");
		}

		effect->SetTexture("g_tex",  m_items[0] .tex.get());
		effect->SetTexture("SceneNormalAndDepth",  scene_rt->get_render_target(ERT_SceneNormalDepth) .tex.get());
		effect->SetFloatArray("HBloomWeights", HBloomWeights, 9 );
		effect->SetFloatArray("HBloomOffsets", HBloomOffsets, 9 );
		
		scene_rt->draw_screen_quad(effect.get());

		// [ 3 ] BLUR VERTICALLY
		//----------------------
		device->SetRenderTarget(0, m_items[0].surf.get());
		effect->SetTexture("g_tex",  m_items[1] .tex.get());
		effect->SetTexture("SceneNormalAndDepth",  scene_rt->get_render_target(ERT_SceneNormalDepth) .tex.get());
		if ( m_quality == high)
		{
			effect->SetTechnique("techVerticalBlurFliter");
		} 
		else
		{
			effect->SetTechnique("techVerticalBlur");
		}

		device->GetRenderTarget(0,&surf);
		surf->GetDesc(&desc);
		surf->Release();

		float VBloomWeights[9];
		float VBloomOffsets[9];

		for( int i = 0; i < 9; i++ )
		{
			// Compute the offsets. We take 9 samples - 4 either side and one in the middle:
			//     i =  0,  1,  2,  3, 4,  5,  6,  7,  8
			//Offset = -4, -3, -2, -1, 0, +1, +2, +3, +4
			VBloomOffsets[i] = (static_cast< float >( i ) - 4.0f) * ( 1.0f / static_cast< float >( desc.Height ) );

			// 'x' is just a simple alias to map the [0,8] range down to a [-1,+1]
			float x = (static_cast< float >( i ) - 4.0f) / 4.0f;

			// Use a gaussian distribution. Changing the standard-deviation
			// (second parameter) as well as the amplitude (multiplier) gives
			// distinctly different results.
			VBloomWeights[i] = g_GaussMultiplier * ComputeGaussianValue( x, g_GaussMean, g_GaussStdDev );
		}

		// Commit both arrays to the device:
		effect->SetFloatArray("VBloomWeights", VBloomWeights, 9 );
		effect->SetFloatArray("VBloomOffsets", VBloomOffsets, 9 );
		effect->SetTexture("SceneNormalAndDepth",  scene_rt->get_render_target(ERT_SceneNormalDepth) .tex.get());
		
		scene_rt->draw_screen_quad(effect.get());

		scene_render_targets::instance()->begin_scene_color();
		effect->SetTechnique("techBlend");
		effect->SetTexture("g_tex",  m_items[0] .tex.get());
		scene_rt->draw_screen_quad(effect.get());
		scene_render_targets::instance()->end_scene_color();
	}
}