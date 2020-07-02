#include "StdAfx.h"
#include "nmtl_common.h"
#include "nmtl_tech_shader.h"
#include "../resource/nresource_manager.h"
#include "mtl_param_serialize.h"

namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(nmtl_common, nmtl_base)

	nmtl_common::nmtl_common(const nstring& name_str):nmtl_base(name_str)
	{
	}

	nmtl_common::~nmtl_common(void)
	{
	}

	 const nshader_modifier* nmtl_common::get_shader_mod()
	 {
		s_shader_modifier.clear();
		for (size_t i=0; i<m_texture_param_array.size(); i++)
		{
			const material_parameter& param = m_texture_param_array[i];
		
			if( param.tex_value && !param.macro.empty())
				s_shader_modifier.add_macro(shader_define(param.macro,"1"));
		}

		if (get_two_side())
		{
			s_shader_modifier.add_macro(shader_define("TWO_SIDE","1"));
		}

		 return &s_shader_modifier;
	 }


	nrender_texture* nmtl_common::get_texture_param(const std::string& param_name) const
	{
		for (st_param_array::const_iterator iter = m_texture_param_array.begin();
			iter != m_texture_param_array.end();
			++iter)
		{
			const material_parameter& val = (*iter);
			if(val.name == param_name)
			{
				if(val.tex_value)
					return val.tex_value->get_render_texture();
				else
					return NULL;
				break;
			}
		}// end of for()

		return NULL;
	}

	bool nmtl_common::have_param(const std::string& param_name) const
	{
		for (st_param_array::const_iterator iter = m_texture_param_array.begin();
			iter != m_texture_param_array.end();
			++iter)
		{
			const material_parameter& val = (*iter);
			if(val.name == param_name)
				return true;
		}

		return false;
	}

	material_parameter* nmtl_common::find_texture_param(const std::string& param_name)
	{
		for (st_param_array::iterator iter = m_texture_param_array.begin();
			iter != m_texture_param_array.end();
			++iter)
		{
			material_parameter& val = (*iter);
			if(val.name == param_name)
				return &val;
		}// end of for()
		return NULL;
	}

	void nmtl_common::set_texture2d_parameter(const std::string& param_name, const resource_location& tex_loc)
	{
		material_parameter* val = find_texture_param( param_name );
		if( val)
		{
			nresource_texture::ptr tex(
				nresource_manager::instance()->load_texture_2d(tex_loc, EIO_Block)
				);
			val->set_value(tex, 0);	
		}
		else
		{
			nLog_Error(_T("material: parameter not found, name=%s.\r\n"), param_name.c_str());
		}
	}

	void nmtl_common::set_cube_map_parameter(const std::string& param_name, const resource_location& tex_loc)
	{
		material_parameter* val = find_texture_param( param_name );
		if( val)
		{
			nresource_texture::ptr tex(
				nresource_manager::instance()->load_cube_map(tex_loc, EIO_Block)
				);

			val->set_value(tex, 1);
		}
		else
		{
			nLog_Error(_T("material: parameter not found, name=%s.\r\n"), param_name.c_str());
		}
	}

	void nmtl_common::serialize(narchive& ar)
	{
		nmtl_base::serialize(ar);

		nSERIALIZE(ar, m_texture_param_array);
		if ( ar.is_loading() )
		{
			for (size_t i=0; i<m_texture_param_array.size(); i++)
			{
				const material_parameter& param = m_texture_param_array[i];
			}
		}
	}
}//namespace nexus