#include "StdAfx.h"
#include <boost/algorithm/string.hpp>
#include <sstream>
#include "shader_compile.h"
#include "nrenderer_d3d9.h"
#include "drawing_policy.h"
#include "util.h"

namespace nexus
{
	shader_compile_environment::shader_compile_environment(void)
		: m_dp_type(NULL)
	{
	}

	shader_compile_environment::~shader_compile_environment(void)
	{
	}

	D3DXMACRO _convert_macor(const shader_define& engine_def)
	{
		D3DXMACRO d3d_def = {
			engine_def.name.c_str(),
			engine_def.value.c_str() 
		}; // 这里引用指针，外部必须保证engine_macros在shader编译期间具有生存期

		return d3d_def;
	}

	void _convert_shader_macros(const shader_macro_array& engine_macros, vector<D3DXMACRO>& d3d_macros)
	{
		for( size_t i=0; i<engine_macros.size(); i++)
		{
			d3d_macros.push_back(_convert_macor(engine_macros[i]));
		}

	}

	void shader_compile_environment::init(drawing_policy_type* dp_type, vertex_factory_type* vf_type, 
		const nmaterial_base* mtl, const nshader_modifier* mod)
	{
		m_dp_type = dp_type;
		m_include.set_data(dp_type, vf_type, mtl);

		//-- make D3DX macro
		m_macro_array.clear();

		// vertex factory macros
		_convert_shader_macros(vf_type->get_shader_macro_array(), m_macro_array);
		
		// drawing policy macros
		_convert_shader_macros(m_dp_type->macros, m_macro_array);

		if( mod )
		{
			for(size_t i=0; i<mod->get_num_macro(); i++)
			{
				m_macro_array.push_back(_convert_macor(mod->get_macro(i)));
			}
		}
		
		// terminate the macros list.
		if(!m_macro_array.empty())
		{
			D3DXMACRO end_def={NULL,NULL};
			m_macro_array.push_back(end_def);
		}

		//-- make name string
		wostringstream ss;
		ss << dp_type->dp_name.name_str
			<< _T('_')
			<< vf_type->get_name()
			<< _T('_');
		if( mod )
			ss << mod->get_name();
		ss << mtl->get_template_name();			
		

		m_name = boost::replace_all_copy(ss.str(), g_engine_package_name, _T("_"));
		m_name = boost::replace_all_copy(m_name, _T("/"), _T("_"));	// file name ready
		m_name = boost::replace_all_copy(m_name, _T(":"), _T("_"));	// file name ready
	}

	bool shader_include::search_include_file(const nstring& file_name)
	{
		std::vector<nstring> include_paths;
		include_paths.push_back(_T("material/"));
		include_paths.push_back(_T("common/"));

		for(size_t i=0; i<include_paths.size(); i++)
		{
			nstring full_path_str = include_paths[i]+file_name;

			if( g_file_sys->file_exists(g_engine_package_name, full_path_str) )
			{
				load_shder_source(full_path_str, m_include_code);
				return true;
			}
		}
		return false;
	}
}//namespace nexus