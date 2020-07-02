#include "StdAfx.h"
#include "nrenderer_d3d9.h"
#include "nrender_fog.h"
#include "..\nengine\actor\nfog_volume_component.h"
#include "global_shader_lib.h"
#include "d3d_device_manager.h"
#include "scene_render_targets.h"
#include "util.h"

namespace nexus
{
	static const vector3 CubeVertex[8] = 
	{
		vector3(-1,+1,-1),
		vector3(-1,-1,-1),
		vector3(+1,-1,-1),
		vector3(+1,+1,-1),

		vector3(-1,+1,1),
		vector3(-1,-1,1),
		vector3(+1,-1,1),
		vector3(+1,+1,1)
	};

	static const UINT index[36] = {
		0,1,3,
		1,2,3,
		3,2,7,
		2,6,7,
		0,7,4,
		0,3,7,
		0,4,5,
		0,5,1,
		4,7,5,
		5,7,6,
		1,5,2,
		5,6,2
	};

	nfog_volume_proxy::nfog_volume_proxy(const nfog_volume_component* fog):
	m_owner(fog)
	{
	}

	nfog_volume_proxy::~nfog_volume_proxy(void)
	{
	}

	void nfog_volume_proxy::render(const nview_info *view,drawing_policy* dp) const
	{
		if (dp->get_policy_tech() == EDP_Wireframe
			||dp->get_policy_tech() == EDP_Unlit)
		{
			return;
		}

		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();

		d3d_effect_ptr effect = global_shader_lib::instance()->find_shader(
			_T("fog_volume.fx"));

		if( !effect )
			return;

		HRESULT hr;
		const matrix44& world = m_owner->get_world_matrix();
	/*	mat_set_scaling(world,1000,100,1000);
		mat_translate(world,vector3(0,0,0));*/
		
		matrix44 worldIT = mat_inverse(world);
		const nview_info& view_info = *view;
	
		matrix44 viewIT = mat_inverse(view_info.mat_view);
		vector4 eyePosInOS = vector4(view_info.eye_pos) * worldIT;

		if (m_owner->shape_type == nfog_volume_component::e_sphere)
		{
			effect->SetTechnique("Ellipsoid");
			effect->SetVector("outsideSoftEdgesLerp",(D3DXVECTOR4*)&(m_owner->soft_edges));
		}
		else
		{
			effect->SetTechnique("Box");
		}
		
		hr = effect->SetMatrix("g_matWorldViewPrj", (const D3DXMATRIX*)&(world*view_info.mat_view_project));
		hr = effect->SetMatrix("objMatrix",(const D3DXMATRIX*)&(world));
		hr = effect->SetMatrix("invObjSpaceMatrix",(const D3DXMATRIX*)&(worldIT));
		effect->SetVector("cameraFrontVector",(D3DXVECTOR4*)&(viewIT.get_axis_z()));
		effect->SetFloat("globalDensity",m_owner->global_density);
	
		effect->SetVector("eyePosInOS",(D3DXVECTOR4*)&(eyePosInOS));
		vector4 eyePosInWS(view_info.eye_pos);
		eyePosInWS.w = abs(eyePosInOS.x)<1&&abs(eyePosInOS.y)<1&&abs(eyePosInOS.z)<1 ? 0.0f: 1.0f;
		effect->SetVector("eyePosInWS",(D3DXVECTOR4*)&(eyePosInWS));
		effect->SetVector("fogColor",(D3DXVECTOR4*)&(m_owner->color));
		effect->SetFloat("FogStartDistance",m_owner->fog_start_distance);
		effect->SetVector("heightFallOffBasePoint",(D3DXVECTOR4*)&(m_owner->base_point));
		effect->SetVector("heightFallOffDirScaled",(D3DXVECTOR4*)&(m_owner->dir_scaled));

		const rt_item& scene_depth = scene_render_targets::instance()->get_render_target(ERT_SceneNormalDepth);
		vector4 screen_scale_bias = scene_render_targets::calc_screen_scale_bias();

		effect->SetVector("ScreenScaleBias",(D3DXVECTOR4*)&(screen_scale_bias));
		IDirect3DTexture9* SceneNormalAndDepth = scene_depth.tex.get();
		effect->SetTexture("SceneNormalAndDepth",SceneNormalAndDepth);

		UINT num_pass = 0;
		hr = effect->Begin(&num_pass, 0);

		if( FAILED(hr) )
			return;

		
		device->SetFVF(D3DFVF_XYZ);	

		for(UINT i=0; i<num_pass; i++)
		{
			hr = effect->BeginPass(i);
			if (m_owner->shape_type == nfog_volume_component::e_box)
			{
				device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0,8,12,index,D3DFMT_INDEX32,CubeVertex,sizeof(vector3));
			}
			else
			{
				device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0,g_sphere.positions.size(),g_sphere.indexes.size()/3,&g_sphere.indexes[0],D3DFMT_INDEX32,&g_sphere.positions[0],sizeof(vector3));
			}

			hr = effect->EndPass();
		}

		hr = effect->End();
	}
}
