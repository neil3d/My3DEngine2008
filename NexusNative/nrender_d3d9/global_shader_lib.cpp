#include "StdAfx.h"
#include <boost/bind.hpp>
#define BOOST_FILESYSTEM_DYN_LINK 
#include <boost/filesystem.hpp>

#include "global_shader_lib.h"
#include "d3d_device_manager.h"
#include "util.h"

namespace nexus
{
	extern nfile_system::ptr g_file_sys;

	global_shader_lib::global_shader_lib(void)
	{
		d3d_device_manager::instance()->register_device_handler(
			boost::bind(&global_shader_lib::on_device_lost, this, _1),
			boost::bind(&global_shader_lib::on_device_reset, this, _1) );
	}

	global_shader_lib::~global_shader_lib(void)
	{
	}

	global_shader_lib* global_shader_lib::instance()
	{
		static global_shader_lib g_global_shaders;
		return &g_global_shaders;
	}

	void global_shader_lib::file_query_callback(const nfile_entity& f)
	{
		if( f.type == EFile_Regular 
			&& f.file_size > 0)
		{
			nstring filename = f.path;
			// 去掉路径的文件名作为key
			nstring::size_type ipos = filename.find_last_of(_T('/'));

			if (ipos != nstring::npos)
				filename = filename.substr(ipos+1, filename.length()-ipos-1);				
			

			create_effect_from_file(filename, f.path);
		}
	}

	void global_shader_lib::load(const nstring& pkg_name, const nstring& folder_name)
	{
		g_file_sys->query_package(pkg_name, folder_name, 
			nfile_query_bind( boost::bind(&global_shader_lib::file_query_callback, this, _1) )
			);
	}

	d3d_effect_ptr global_shader_lib::find_shader(const nstring& file_name)
	{
		d3d_effect_ptr ret;

		effect_map::iterator iter = m_effect_map.find(file_name);
		if( iter != m_effect_map.end() )
			ret = iter->second;

		return ret;
	}

	void global_shader_lib::create_effect_from_file(const nstring& key_name, const nstring& file_name)
	{
		std::string shader_code;
		load_shader_source(file_name, shader_code);

		HRESULT hr;
		IDirect3DDevice9* device = d3d_device_manager::instance()->get_device();
		ID3DXBuffer *error = NULL;
		ID3DXEffect* ret = NULL;		

		hr = D3DXCreateEffect(device,
			shader_code.c_str(),
			shader_code.length(),
			NULL,
			NULL,
			0,
			NULL,
			&ret,
			&error);

		if( FAILED(hr) )
		{
			if( error )
			{
				std::string error_string((const char*)error->GetBufferPointer());
				error->Release();
			}
			else
			{}
		}
		else
		{
			m_effect_map.insert( std::make_pair(key_name, d3d_effect_ptr(ret)) );
		}
	}

	void global_shader_lib::on_device_lost(int param)
	{
		HRESULT hr;
		for (effect_map::iterator iter = m_effect_map.begin();
			iter != m_effect_map.end();
			++iter)
		{
			d3d_effect_ptr eft = iter->second;
			hr = eft->OnLostDevice();
			nASSERT( SUCCEEDED(hr) );
		}
	}

	bool global_shader_lib::on_device_reset(int param)
	{
		HRESULT hr;
		for (effect_map::iterator iter = m_effect_map.begin();
			iter != m_effect_map.end();
			++iter)
		{
			d3d_effect_ptr eft = iter->second;
			hr = eft->OnResetDevice();
			if( FAILED(hr) )
				return false;
		}
		return true;
	}
}//namespace nexus