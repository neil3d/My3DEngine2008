#include "StdAfx.h"
#include "nmtl_base.h"
#include "../resource/nresource_manager.h"
#include "../engine_all.h"

namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(nmtl_base, nobject)

		nmtl_base::nmtl_base(const nstring& name_str):
		m_name(name_str),
		is_two_side(false),
		m_blend_type(EOpaque)
	{
	}

	nmtl_base::~nmtl_base(void)
	{
	}

	void nmtl_base::set_render_state(nshading_effect* effect_ptr) 
	{
		if (get_two_side())
		{
			effect_ptr->set_render_state(ERS_CullMode,ECM_None);
		}
		else
		{
			effect_ptr->set_render_state(ERS_CullMode,ECM_CCW);
		}

		switch ( m_blend_type)
		{
		case EOpaque:
			{
				effect_ptr->set_render_state(ERS_AlphaBlendEnable, false);
				effect_ptr->set_render_state(ERS_ZWriteEnable,true);
			}
			break;
		case ETranslucent:
			{
				effect_ptr->set_render_state(ERS_AlphaBlendEnable, true);
				effect_ptr->set_render_state(ERS_SrcBlend, EAB_SrcAlpha);
				effect_ptr->set_render_state(ERS_DestBlend, EAB_InvSrcAlpha);
				effect_ptr->set_render_state(ERS_ZWriteEnable,false);
			}
			break;
		case EAdditive:
			{
				effect_ptr->set_render_state(ERS_AlphaBlendEnable, true);
				effect_ptr->set_render_state(ERS_SrcBlend, EAB_One);
				effect_ptr->set_render_state(ERS_DestBlend, EAB_One);
				effect_ptr->set_render_state(ERS_ZWriteEnable,false);
			}
			break;
		}
	}

	void nmtl_base::create_from_hlsl(const nstring& name_str, const resource_location& shader_loc)
	{
		//-- load shader
		m_shader_loc=shader_loc;
		m_shader = nresource_manager::instance()->load_mtl_shader(shader_loc, EIO_Block);

		//-- 绑定所有的Shader参数
		bind_shader_param(m_shader);
	}

	nmtl_tech_shader::ptr nmtl_base::get_shader()
	{
		return m_shader;
	}

	void nmtl_base::serialize(narchive& ar)
	{
		nSERIALIZE(ar,is_two_side);
		nSERIALIZE(ar,m_shader_loc);
		nserialize_enum_as_int(ar, m_blend_type, _T("m_blend_type"));

		resource_location shader_loc;
		if(ar.is_loading())
		{
			nSERIALIZE(ar, shader_loc);
			if (shader_loc.file_name == _T(""))
			{
				shader_loc = resource_location(
					_T("engine_data"),
					_T("material/standard.hlsl"));
			}

			m_shader = nresource_manager::instance()->load_mtl_shader(shader_loc, EIO_Block);
		}
		else
		{
			shader_loc = m_shader->get_location();
			nSERIALIZE(ar, shader_loc);
		}
	}

	nmtl_base::ptr nmtl_base::create_from_file( const resource_location& loc )
	{
		if(!loc.is_valid())
		{
			return nmtl_base::ptr();
		}

		narchive::ptr ar_ptr = narchive::open_xml_archive(nengine::instance()->get_file_sys(),EFileRead, loc.pkg_name, loc.file_name);	

		nmtl_base::ptr Material;
		nSERIALIZE(*ar_ptr.get(),Material);
		Material->set_name(loc.to_string());
		ar_ptr->close();

		return Material;
	}

	void nmtl_base::save_to_file( const resource_location& loc )
	{
		narchive::ptr ar_ptr = narchive::open_xml_archive(nengine::instance()->get_file_sys(),EFileWrite, loc.pkg_name, loc.file_name );	

		nmtl_base& Material=*this;
		nSERIALIZE(*ar_ptr.get(),Material);

		ar_ptr->close();
	}
}//namespace nexus