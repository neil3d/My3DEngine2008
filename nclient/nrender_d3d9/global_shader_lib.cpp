#include "StdAfx.h"
#include "global_shader_lib.h"
#define BOOST_FILESYSTEM_DYN_LINK 
#include <boost/filesystem.hpp>
#include "d3d_device_manager.h"
#include "util.h"

namespace nexus
{
	global_shader_lib::global_shader_lib(void)
	{
	}

	global_shader_lib::~global_shader_lib(void)
	{
	}

	global_shader_lib* global_shader_lib::instance()
	{
		static global_shader_lib g_global_shaders;
		return &g_global_shaders;
	}

	void global_shader_lib::load(const nstring& folder_name)
	{
		boost::filesystem::wpath base_path(folder_name);

		boost::filesystem::wdirectory_iterator end_iter;
		for(boost::filesystem::wdirectory_iterator iter(base_path);
			iter != end_iter;
			++iter)
		{
			if(boost::filesystem::is_regular( iter->status() ) )
			{
				boost::filesystem::wpath path = *iter;
				create_effect_from_file(path.leaf(), path.string() );				
			}
		}// end of for
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
		load_shder_source(file_name, shader_code);

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
}//namespace nexus