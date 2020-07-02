#include "StdAfx.h"
#include <fstream>
#include "shader_cache.h"
#include "shader_compile.h"
#include "d3d_exception.h"
#include "nrenderer_d3d9.h"

namespace nexus
{
	const unsigned char _name_mask = 138;
	const unsigned char _code_mask = 158;
	const TCHAR* _shader_cache_file_name = _T("ShaderCacheD3D9.dat");
	
	shader_cache::shader_cache(void)
	{
	}

	shader_cache::~shader_cache(void)
	{		
	}

	void shader_cache::load_from_file()
	{		
		m_code_map.clear();

#if 0 // 为了方便开发，暂时关闭shader cache功能
		//--
		nfile::ptr fp;
		try
		{
			fp = g_file_sys->open_file(g_engine_package_name,
				_shader_cache_file_name, EFileRead);
		}
		catch (nexus::nexception& ex)
		{
			(void)ex;
			nLog_Warning(_T("Shader Cache not load, build shader from source."));
			return;
		}				

		size_t num_shader = 0;
		fp->read_buffer(&num_shader, sizeof(num_shader));

		TCHAR name_buf[4096] = {0};
		for(size_t i=0; i<num_shader; i++)
		{
			size_t len = 0;

			//-- read name
			fp->read_buffer(&len, sizeof(len));
			fp->read_buffer(name_buf, len);

			//-- read code then add to map
			fp->read_buffer(&len, sizeof(len));

			std::pair<st_code_map::iterator, bool> insert_ret =		
				m_code_map.insert(
				make_pair(name_buf, code_buf())
				);
			code_buf& code = insert_ret.first->second;
			code.resize(len);

			fp->read_buffer(&code[0], len);
		}// end of for
#endif
	}

	void shader_cache::save_to_file()
	{
		nfile::ptr fp = g_file_sys->open_file(g_engine_package_name,
			_shader_cache_file_name, EFileWrite);

		size_t num_shader = m_code_map.size();
		
		fp->write_buffer(&num_shader, sizeof(num_shader));

		for(st_code_map::iterator iter = m_code_map.begin();
			iter != m_code_map.end();
			++iter)
		{
			nstring code_name =	iter->first;
			const code_buf& code = iter->second;

			//-- write name
			size_t len = code_name.size()+1;
			len *= sizeof(TCHAR);
			fp->write_buffer(&len, sizeof(len));
			fp->write_buffer((void*)code_name.c_str(), len);

			//-- write code
			len = code.size();
			fp->write_buffer(&len, sizeof(len));
			fp->write_buffer((void*)&code[0], len);
		}
	}

	const code_buf& shader_cache::acquire_shader_code(shader_compile_environment* env)
	{
		//-- find
		st_code_map::iterator find_iter = m_code_map.find(env->get_name());
		if(find_iter != m_code_map.end())
		{
			return find_iter->second;
		}

		//-- create new
		env->load_shader_source();

		HRESULT hr;
		ID3DXBuffer *error = NULL;

#if 1//-- dump preprocess shader
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
		
		ID3DXEffectCompiler* dx_cmp = NULL;
		
		hr = D3DXCreateEffectCompiler(
			env->get_shader_source(),
			(UINT)env->get_shader_source_len(),
			env->get_macros(),
			env->get_include(),
			env->get_compile_flags(),
			&dx_cmp,
			&error);

		d3d_ptr<ID3DXEffectCompiler> complier;
		complier.reset(dx_cmp);


		if( FAILED(hr) )
		{
			std::string error_string((const char*)error->GetBufferPointer());
			error->Release();
			nstring error_string_n = conv_string(error_string);

			nLog_Error(_T("D3DXCreateEffectCompiler failed, error = %s\r\n"), 
				error_string_n.c_str());
			THROW_D3D_HRESULT(hr, error_string_n.c_str());
		}


		ID3DXBuffer* dx_code = NULL;
		hr = dx_cmp->CompileEffect(env->get_compile_flags(),
			&dx_code, &error);

		if( FAILED(hr) )
		{
			std::string error_string((const char*)error->GetBufferPointer());
			error->Release();
			nstring error_string_n = conv_string(error_string);

			nLog_Error(_T("ID3DXEffectCompiler::CompileEffect failed, error = %s\r\n"), 
				error_string_n.c_str());
			THROW_D3D_HRESULT(hr, _T(""));
		}

		std::pair<st_code_map::iterator, bool> insert_ret =		
			m_code_map.insert(
			make_pair(env->get_name(), code_buf())
			);
		vector<unsigned char>& code_buf = insert_ret.first->second;
		code_buf.resize(dx_code->GetBufferSize());
		memcpy(&code_buf[0], dx_code->GetBufferPointer(), dx_code->GetBufferSize());

		dx_code->Release();

		if( m_dummy_compiler.get() == NULL )
			m_dummy_compiler = complier;

		return code_buf;
	}
}//namespace nexus