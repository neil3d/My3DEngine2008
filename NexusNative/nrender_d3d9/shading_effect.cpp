#include "StdAfx.h"
#include <boost/crc.hpp>  // for boost::crc_32_type
#include <fstream>
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
	void shading_effect_name::make_name_crc()
	{
		boost::crc_32_type  crc_result;
		crc_result.process_bytes(drawing_policy_name.data(), drawing_policy_name.size()*sizeof(nstring::value_type));
		crc_result.process_bytes(vertex_factory_name.data(), vertex_factory_name.size()*sizeof(nstring::value_type));
		
		if(!material_template_name.empty())
			crc_result.process_bytes(material_template_name.data(), material_template_name.size()*sizeof(nstring::value_type));

		crc_result.process_bytes(mod_name.data(), mod_name.size()*sizeof(nstring::value_type));

		name_crc = crc_result.checksum();
	}

	d3d9_shading_effect::d3d9_shading_effect(void)
	{
		memset(m_vs_parameter, 0, sizeof(m_vs_parameter) );
	}

	d3d9_shading_effect::~d3d9_shading_effect(void)
	{
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

	void d3d9_shading_effect::create_effect(shader_compile_environment* env)
	{
		HRESULT hr;
		ID3DXBuffer *error = NULL;
		//-- dump preprocess shader
#if 1
		ID3DXBuffer* shader_text = NULL;
		hr = D3DXPreprocessShader((LPCSTR)env->get_shader_source(),
			(UINT)env->get_shader_source_len(),
			env->get_macros(),
			env->get_include(),
			&shader_text,
			&error);
		const char* shader_string = NULL;
		if( SUCCEEDED(hr) )
		{
			shader_string = (const char*)shader_text->GetBufferPointer();
			
			nstring file_name = _T("shader_dump/");
			file_name += env->get_name();
			
			wofstream file(file_name.c_str());
			if( file )
				file << shader_string;

			shader_text->Release();
		}
#endif

		//-- compile effect
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		ID3DXEffect* ret = NULL;		
		
		// todo : 为什么这个函数调用会在程序退出时提示：D3DX: MEMORY LEAKS DETECTED: 16 allocations unfreed (1384 bytes)，每个effect一个
		hr = D3DXCreateEffect(device,
			env->get_shader_source(),
			(UINT)env->get_shader_source_len(),
			env->get_macros(),
			env->get_include(),
			env->get_compile_flags(),
			NULL,
			&ret,
			&error);

		if( FAILED(hr) )
		{
			std::string error_string((const char*)error->GetBufferPointer());
			error->Release();
			nstring error_string_n = conv_string(error_string);

			nLog_Error(_T("D3DXCreateEffect failed, error = %s\r\n"), 
				error_string_n.c_str());
			THROW_D3D_HRESULT(hr, _T(""));
		}

		m_d3d_effect.reset(ret);

		//-- bind vertex shader parameters
		for(int i=0; i<EVSP_Max; i++)
		{
			m_vs_parameter[i] = m_d3d_effect->GetParameterBySemantic(NULL, vertex_shader_parameter_semantic[i]);
		}
	}

	void d3d9_shading_effect::set_system_parameter(const d3d_view_info* view, const nrender_proxy* obj)
	{
		if(m_vs_parameter[EVSP_WorldViewProject])
		{
			matrix44 mat_world_view_prj = obj->get_world_matrix()*view->mat_view_project;
			HRESULT hr = m_d3d_effect->SetMatrix(m_vs_parameter[EVSP_WorldViewProject], 
				(const D3DXMATRIX*)&mat_world_view_prj);
		}

		const matrix44& world_mat = obj->get_world_matrix();
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

		if(m_vs_parameter[EVSP_EyePos])
		{
			HRESULT hr = m_d3d_effect->SetFloatArray(m_vs_parameter[EVSP_EyePos],
				(const float*)&view->eye_pos, sizeof(view->eye_pos)/sizeof(float));
		}

		if(m_vs_parameter[EVSP_RunTime])
		{
			HRESULT hr = m_d3d_effect->SetFloat(m_vs_parameter[EVSP_RunTime], view->time);
		}

		if(m_vs_parameter[EPS_RT_SceneDepth])
		{
			rt_item rt = scene_render_targets::instance()->get_render_target(ERT_SceneDepthLinear);
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
		const nmaterial_base* mtl, const nshader_modifier* mod)
	{
		shader_compile_environment env;
		env.init(dp_type, vf_type, mtl, mod);
		
		const code_buf& bin_code = m_shader_cache.acquire_shader_code(&env);

		d3d9_shading_effect::ptr new_effect(nNew d3d9_shading_effect);
		new_effect->create_effect((void*)&bin_code[0], bin_code.size());
		return new_effect;
	}

	d3d9_shading_effect* shading_effect_lib::acquire_shader(drawing_policy_type* dp_type, vertex_factory_type* vf_type
		, const nmaterial_base* mtl, const nshader_modifier* mod)
	{
		nASSERT( dp_type );
		nASSERT( vf_type );

		const nmaterial_base* _mtl = mtl;
		if( _mtl == NULL )
			_mtl = m_default_mtl;

		//-- make effect name
		shading_effect_name effect_name;
		effect_name.drawing_policy_name = dp_type->get_name_str();
		effect_name.vertex_factory_name = vf_type->get_name();	
		effect_name.material_template_name = _mtl->get_template_name();
		if( mod )
			effect_name.mod_name = mod->get_name();
		effect_name.make_name_crc();

		//-- find exist
		shader_map::iterator iter = m_shader_map.find(effect_name);
		if(iter != m_shader_map.end())
			return iter->second.get();

		//-- create on demand
		d3d9_shading_effect::ptr new_effect = create_shader(dp_type, vf_type, _mtl, mod);		
		m_shader_map.insert(make_pair(effect_name, new_effect));

		return new_effect.get();
	}

	d3d9_shading_effect* shading_effect_lib::acquire_shader_default_mtl(drawing_policy_type* dp_type, vertex_factory_type* vf_type, 
		const nshader_modifier* mod, bool two_sided)
	{
		if( two_sided )
			return acquire_shader(dp_type, vf_type, m_default_mtl_two_side, mod);
		else
			return acquire_shader(dp_type, vf_type, m_default_mtl, mod);
	}
}//namespace nexus