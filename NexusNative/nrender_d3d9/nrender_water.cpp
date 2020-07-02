#include "StdAfx.h"
#include "nrender_water.h"
#include "drawing_policy.h"
#include "nrenderer_d3d9.h"
#include "..\nengine\actor\nwater_component.h"
#include "global_shader_lib.h"
#include "d3d_device_manager.h"
#include "scene_render_targets.h"
#include "d3d9_simple_mesh.h"
#include "FFTWave.h"

namespace nexus
{
	HANDLE g_simulate_start;
	HANDLE g_simulate_end;
	float g_simulate_time = 0;

	FFTWave*						Wave = NULL;
	class wave_thread
	{
	public:
		wave_thread();
		~wave_thread() 
		{
		    TerminateThread(m_handle,exit_code);	
			WaitForSingleObject(m_handle,INFINITE);
			CloseHandle(m_handle);
			CloseHandle(g_simulate_start);
			CloseHandle(g_simulate_end);
			delete Wave;
			Wave = NULL;
		}

	private:
		HANDLE						m_handle;
		DWORD						exit_code;
	};

	wave_thread g_wave_thread;

	DWORD WINAPI do_work(LPVOID lpParameter)
	{
		while(1)
		{
			WaitForSingleObject(g_simulate_start,INFINITE);
			Wave->idle(g_simulate_time*2.5);
			SetEvent(g_simulate_end);
		}
		
		return S_OK;
	}

	wave_thread::wave_thread()
	{
		Wave = new FFTWave(CYC_LENGTH);
		g_simulate_start = CreateEvent(NULL,false,true,NULL);
		g_simulate_end = CreateEvent(NULL,false,false,NULL);
		m_handle = CreateThread(NULL,0,do_work,NULL,CREATE_SUSPENDED,NULL);
		
		ResumeThread(m_handle);
		GetExitCodeThread(m_handle,&exit_code);
	}

	void fft_simulation(float time)
	{
		WaitForSingleObject(g_simulate_end,INFINITE);

		D3DLOCKED_RECT LockedRect;
		LPDIRECT3DTEXTURE9 wave_height = (LPDIRECT3DTEXTURE9)nrenderer_d3d9::instance()->get_texture(ETexture_FFT);
		wave_height->LockRect(0,&LockedRect,NULL,D3DLOCK_DISCARD);
		float *data = (float *)LockedRect.pBits;
		memcpy(data,Wave->sea,NX*NY*4*sizeof(float));
		wave_height->UnlockRect(0);

		d3d_effect_ptr effect = global_shader_lib::instance()->find_shader(
			_T("water_surface.fx"));
		nASSERT(effect);
		
		effect->SetTechnique("techNormal");
		effect->SetTexture("VertexTexture",wave_height);
		effect->SetVector("worldSize",&D3DXVECTOR4(Wave->WorldSizeX,Wave->WorldSizeY,0,1));
		IDirect3DTexture9* normal_tex = (LPDIRECT3DTEXTURE9)nrenderer_d3d9::instance()->get_texture(ETexture_FFT_Normal);
		IDirect3DSurface9* old_surf = NULL;
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		device->GetRenderTarget(0, &old_surf);

		IDirect3DSurface9* surf =NULL;
		normal_tex->GetSurfaceLevel(0,&surf);
		device->SetRenderTarget(0,surf);
		scene_render_targets::instance()->draw_screen_quad(effect.get());
		surf->Release();
		device->SetRenderTarget(0,old_surf);
		old_surf->Release();
		SetEvent(g_simulate_start);
		g_simulate_time = time;
	}

	nwater_render_proxy::nwater_render_proxy(const nwater_component* water)
	{
		m_owner = water;
	}

	nwater_render_proxy::~nwater_render_proxy(void)
	{
	}

	void nwater_render_proxy::render(const nview_info* view,drawing_policy* dp) const
	{
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();

		d3d_effect_ptr effect = global_shader_lib::instance()->find_shader(
			_T("water_surface.fx"));

		nASSERT(effect);

		HRESULT hr;
		const matrix44& world = m_owner->get_world_matrix();

		matrix44 worldIT = mat_inverse(world);
		const nview_info& view_info = *view;

		matrix44 viewIT = mat_inverse(view_info.mat_view);
		vector4 eye_pos;
		eye_pos = view_info.eye_pos;
		vector4 eyePosInOS = eye_pos * worldIT;
		
		if (dp->get_policy_tech() == EDP_Wireframe)
		{
			effect->SetTechnique("techWireframe");
		}
		else
			effect->SetTechnique("techDefault");

		hr = effect->SetMatrix("g_matWorldViewPrj", (const D3DXMATRIX*)&(world*view_info.mat_view_project));
		effect->SetTexture("VertexTexture",nrenderer_d3d9::instance()->get_texture(ETexture_FFT));
		effect->SetTexture("WaveNormal",nrenderer_d3d9::instance()->get_texture(ETexture_FFT_Normal));
		effect->SetTexture("EnvMap",(LPDIRECT3DBASETEXTURE9)m_owner->EnvMap->get_render_texture()->get_handle());
		effect->SetTexture("g_texFoam",(LPDIRECT3DBASETEXTURE9)m_owner->foam_tex->get_render_texture()->get_handle());
		effect->SetTexture("g_texCaustic",(LPDIRECT3DBASETEXTURE9)m_owner->caustic_tex->get_render_texture()->get_handle());
		effect->SetVector("worldSize",&D3DXVECTOR4(Wave->WorldSizeX,Wave->WorldSizeY,0,1));
		effect->SetVector("ViewPos",(D3DXVECTOR4*)&view_info.eye_pos);
		effect->SetVector("waterColor",(D3DXVECTOR4*)&m_owner->m_water_color);
		effect->SetFloat("waterDensity",m_owner->m_water_density);
		effect->SetFloat("waterSoftInteractDist",m_owner->m_water_soft_interact_dist);
		effect->SetFloat("foamSoftInteractDist",m_owner->m_foam_soft_interact_dist);
		effect->SetFloat("foamFactor",m_owner->m_foam_factor);

		hr = effect->SetMatrix("g_matWorld",(const D3DXMATRIX*)&(world));
		hr = effect->SetMatrix("g_matViewPrj",(const D3DXMATRIX*)&(view_info.mat_view_project));
	//	hr = effect->SetMatrix("g_matWorldIT",(const D3DXMATRIX*)&(worldIT));

		const rt_item& scene_depth = scene_render_targets::instance()->get_render_target(ERT_SceneNormalDepth);
		vector4 screen_scale_bias = scene_render_targets::calc_screen_scale_bias();

		effect->SetVector("ScreenScaleBias",(D3DXVECTOR4*)&(screen_scale_bias));
		IDirect3DTexture9* SceneNormalAndDepth = scene_depth.tex.get();
		effect->SetTexture("g_sceneDepth",SceneNormalAndDepth);
		effect->SetTexture("g_sceneColor",scene_render_targets::instance()->get_render_target(ERT_SceneColor).tex.get());

		d3d9_simple_mesh* simpler_mesh  = (d3d9_simple_mesh*)m_owner->m_water_mesh;
		simpler_mesh->draw_index_primitive(effect);
	}
}