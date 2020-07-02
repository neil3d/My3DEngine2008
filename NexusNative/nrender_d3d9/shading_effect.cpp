#include "StdAfx.h"
#include <boost/crc.hpp>  // for boost::crc_32_type
#include <fstream>
#include <boost/bind.hpp>
#include "shading_effect.h"
#include "drawing_policy.h"
#include "vertex_factory_type.h"
#include "shader_compile.h"
#include "d3d_device_manager.h"
#include "d3d_exception.h"
#include "scene_render_targets.h"
#include "shader_cache.h"

namespace nexus
{
	shading_effect_name::shading_effect_name(
		const nstring& drawing_policy_name,
		const nstring	& vertex_factory_name,
		const nstring	& mtl_shader_name,
		const nshader_modifier* mesh_mod,
		const nshader_modifier* mesh_mtl
		)
	{
		boost::crc_32_type  crc_result;
		static const unsigned int string_size = sizeof(nstring::value_type);
		crc_result.process_bytes(drawing_policy_name.data(), drawing_policy_name.size()*string_size);
		crc_result.process_bytes(vertex_factory_name.data(), vertex_factory_name.size()*string_size);
		crc_result.process_bytes(mtl_shader_name.data(), mtl_shader_name.size()*string_size);

		if(mesh_mod)
			crc_result.process_bytes(mesh_mod->get_name().data(), mesh_mod->get_name().size()*string_size);

		if(mesh_mtl)
			crc_result.process_bytes(mesh_mtl->get_name().data(), mesh_mtl->get_name().size()*string_size);

		name_crc = crc_result.checksum();
	}

	d3d9_shading_effect::d3d9_shading_effect(void)
	{
		memset(m_vs_parameter, 0, sizeof(m_vs_parameter) );
	}

	d3d9_shading_effect::~d3d9_shading_effect(void)
	{
	}

	static D3DRENDERSTATETYPE RENDER_STATE_CONV[ERS_End] = {
		D3DRS_FORCE_DWORD,
		D3DRS_ZENABLE,
		D3DRS_ZWRITEENABLE,
		D3DRS_ZFUNC,
		D3DRS_CULLMODE,
		D3DRS_FILLMODE,
		D3DRS_DEPTHBIAS,
		D3DRS_SLOPESCALEDEPTHBIAS,
		D3DRS_ALPHATESTENABLE,
		D3DRS_ALPHAREF,
		D3DRS_ALPHAFUNC,
		D3DRS_ALPHABLENDENABLE,
		D3DRS_SRCBLEND,
		D3DRS_DESTBLEND,
	};

	void d3d9_shading_effect::set_render_state(enum ERenderState rs, int val)
	{
		d3d_device_manager::instance()->get_device()->SetRenderState( RENDER_STATE_CONV[rs], val );
	//	m_state_mgr.set_render_state(RENDER_STATE_CONV[rs], (DWORD)val);
	}

	void d3d9_shading_effect::create_effect(void* bin_code, size_t code_size)
	{
		HRESULT hr;
		ID3DXBuffer *error = NULL;

		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		ID3DXEffect* ret = NULL;		

		hr = D3DXCreateEffect(device,
			bin_code,
			code_size,
			NULL,
			NULL,
			0,
			NULL,
			&ret,
			&error);

		if( FAILED(hr) )
			THROW_D3D_HRESULT(hr, _T("create effect from bin code failed."));

		m_d3d_effect.reset(ret);

		//-- bind vertex shader parameters
		for(int i=0; i<EVSP_Max; i++)
		{
			m_vs_parameter[i] = m_d3d_effect->GetParameterBySemantic(NULL, vertex_shader_parameter_semantic[i]);
		}
	}

	//inline bool d3d9_shading_effect::begin(const nview_info* view,const matrix44& world)
	//{
	//	//m_state_mgr.clear();
	//	set_system_parameter(view,world);

	///*	UINT num_pass = 0;
	//	HRESULT hr = m_d3d_effect->Begin(&num_pass, 0);
	//	if( FAILED(hr) || num_pass<=0 )
	//		return false;*/
	//	
	//	m_d3d_effect->BeginPass(0);
	///*	if( FAILED(hr) )
	//	{
	//		m_d3d_effect->End();
	//		return false;
	//	}*/

	//	return true;
	//}

	//inline void d3d9_shading_effect::end()
	//{
	////	m_state_mgr.restore_render_state();
	//	m_d3d_effect->EndPass();
	//	m_d3d_effect->End();
	//}

	void d3d9_shading_effect::set_system_parameter(const nview_info* view,const matrix44& world)
	{
		const matrix44& world_mat = world;
		if(m_vs_parameter[EVSP_WorldViewProject])
		{
			static D3DXMATRIX mat_world_view_prj;
			D3DXMatrixMultiply(&mat_world_view_prj,(const D3DXMATRIX*)&world_mat,(const D3DXMATRIX*)&view->mat_view_project);
			HRESULT hr = m_d3d_effect->SetMatrix(m_vs_parameter[EVSP_WorldViewProject], 
				(const D3DXMATRIX*)&mat_world_view_prj);
		}

		if(m_vs_parameter[EVSP_LocalToWorld])
		{
			HRESULT hr = m_d3d_effect->SetMatrix(m_vs_parameter[EVSP_LocalToWorld], 
				(const D3DXMATRIX*)&world_mat);
		}

		if(m_vs_parameter[EVSP_WorldToLocal])
		{
			matrix44 world_mat_inv = mat_inverse(world_mat);

			HRESULT hr = m_d3d_effect->SetMatrix(m_vs_parameter[EVSP_WorldToLocal], 
				(const D3DXMATRIX*)&world_mat_inv);
		}
	
		if(m_vs_parameter[EVSP_ViewProject])
		{
			matrix44 mat_view_prj = view->mat_view_project;
			HRESULT hr = m_d3d_effect->SetMatrix(m_vs_parameter[EVSP_ViewProject], 
				(const D3DXMATRIX*)&mat_view_prj);
		}

		if(m_vs_parameter[EVSP_EyePos])
		{
			HRESULT hr = m_d3d_effect->SetFloatArray(m_vs_parameter[EVSP_EyePos],
				(const float*)&view->eye_pos, sizeof(view->eye_pos)/sizeof(float));
		}

		if(m_vs_parameter[EVSP_ScreenScaleBias])
		{
			HRESULT hr = m_d3d_effect->SetFloatArray(m_vs_parameter[EVSP_ScreenScaleBias],
				(const float*)&scene_render_targets::calc_screen_scale_bias(),4);
		}

		if(m_vs_parameter[EVSP_RunTime])
		{
			HRESULT hr = m_d3d_effect->SetFloat(m_vs_parameter[EVSP_RunTime], view->time);
		}

		if(m_vs_parameter[EPS_RT_SceneColor])
		{
			rt_item rt = scene_render_targets::instance()->get_render_target(ERT_SceneColor);
			HRESULT hr = m_d3d_effect->SetTexture(m_vs_parameter[EPS_RT_SceneColor],
				rt.tex.get() );
		}

		if(m_vs_parameter[EPS_RT_SceneDepth])
		{
			rt_item rt = scene_render_targets::instance()->get_render_target(ERT_SceneNormalDepth);
			HRESULT hr = m_d3d_effect->SetTexture(m_vs_parameter[EPS_RT_SceneDepth],
				rt.tex.get() );
		}
	}

	shading_effect_lib* shading_effect_lib::instance()
	{
		static shading_effect_lib g_effect_lib;
		return &g_effect_lib;
	}
	

	d3d9_shading_effect::ptr shading_effect_lib::create_shader(drawing_policy_type* dp_type, vertex_factory_type* vf_type, 
		const nmtl_tech_shader* mtl_shader, const nshader_modifier* mesh_mod,const nshader_modifier* mtl_mod)
	{
		shader_compile_environment env;
		env.init(dp_type, vf_type, mtl_shader, mesh_mod,mtl_mod);
		
		const code_buf& bin_code = m_shader_cache.acquire_shader_code(&env);

		d3d9_shading_effect::ptr new_effect(nNew d3d9_shading_effect);
		new_effect->create_effect((void*)&bin_code[0], bin_code.size());
		return new_effect;
	}

	d3d9_shading_effect* shading_effect_lib::acquire_shader(drawing_policy_type* dp_type, vertex_factory_type* vf_type
		, const nmtl_tech_shader* mtl_shader, const nshader_modifier* mesh_mod,const nshader_modifier* mtl_mod)
	{
		nASSERT( dp_type );
		nASSERT( vf_type );

		if( mtl_shader==NULL )
			return NULL;

		//-- make effect name
		shading_effect_name effect_name(dp_type->get_name_str(),vf_type->get_name(),mtl_shader->get_name_str(),mesh_mod,mtl_mod);

		//-- find exist
		shader_map::iterator iter = m_shader_map.find(effect_name);
		if(iter != m_shader_map.end())
			return iter->second.get();

		//-- create on demand
		d3d9_shading_effect::ptr new_effect = create_shader(dp_type, vf_type, mtl_shader, mesh_mod,mtl_mod);		
		m_shader_map.insert(make_pair(effect_name, new_effect));

		return new_effect.get();
	}

	shading_effect_lib::shading_effect_lib(void)
	{
		d3d_device_manager::instance()->register_device_handler(
			boost::bind(&shading_effect_lib::on_device_lost, this, _1),
			boost::bind(&shading_effect_lib::on_device_reset, this, _1) );
	}

	void shading_effect_lib::on_device_lost(int param)
	{
		HRESULT hr;
		for (shader_map::iterator iter = m_shader_map.begin();
			iter != m_shader_map.end();
			++iter)
		{
			d3d9_shading_effect::ptr eft = iter->second;
			hr = eft->get_d3d_effect()->OnLostDevice();
			nASSERT( SUCCEEDED(hr) );
		}
	}

	bool shading_effect_lib::on_device_reset(int param)
	{
		HRESULT hr;
		for (shader_map::iterator iter = m_shader_map.begin();
			iter != m_shader_map.end();
			++iter)
		{
			d3d9_shading_effect::ptr eft = iter->second;
			hr = eft->get_d3d_effect()->OnResetDevice();

			if( FAILED(hr) )
				return false;
		}
		return true;
	}
}//namespace nexus