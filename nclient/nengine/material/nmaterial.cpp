#include "StdAfx.h"
#include "nmaterial.h"
#include "../resource/nresource_manager.h"

namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(nmaterial_base, nobject)
	nDEFINE_NAMED_CLASS(nmaterial, nmaterial_base)

	void nmaterial_base::serialize(narchive& ar)
	{
		nSERIALIZE(ar, m_name);
	}

	nmaterial::nmaterial(const nstring& name_str):nmaterial_base(name_str)
	{
	}

	nmaterial::~nmaterial(void)
	{
	}

	template<>
	inline narchive& nserialize(narchive& ar, material_parameter_value& data, const TCHAR* obj_name)
	{
		nstring class_name(_T("material_parameter_value"));
		ar.object_begin(obj_name, class_name);

		nserialize(ar, data.name, _T("name"));
		nserialize_enum_as_int(ar, data.type, _T("type"));	
		nserialize(ar, data.vec_value, _T("vec_value"));

		resource_location tex_loc;
		if( ar.is_loading() )
		{
			nSERIALIZE(ar, tex_loc);
			if(tex_loc.is_valid())
				data.tex_value = nresource_manager::instance()->load_texture_2d(tex_loc, EIO_Block);
		}
		else
		{
			if( data.tex_value )
				tex_loc = data.tex_value->get_location();
			nSERIALIZE(ar, tex_loc);
		}

		ar.object_end();

		return ar;
	}

	void nmaterial::serialize(narchive& ar)
	{
		//--
		nmaterial_base::serialize(ar);

		//--
		resource_location template_loc;
		if(ar.is_loading())
		{
			// 以block模式加载material template
			nSERIALIZE(ar, template_loc);
			if(template_loc.is_valid())
				m_template = nresource_manager::instance()->load_material_template_script(template_loc, EIO_Block);
		}
		else
		{
			if( m_template )
				template_loc = m_template->get_location();
			nSERIALIZE(ar, template_loc);
		}

		nSERIALIZE(ar, m_parameter_array);
	}

	void nmaterial::create(nmaterial_template::ptr mtl_tmp)
	{
		m_parameter_array.clear();
		m_template = mtl_tmp;

		if(mtl_tmp)
		{
			// 填充空白的参数值
			const vector<material_parameter>& params = m_template->get_parameter_array();
			
			for(size_t i=0; i<params.size(); i++)
			{
				m_parameter_array.push_back(material_parameter_value(&params[i]));
			}
		}
	}

	nstring nmaterial::get_template_name() const
	{
		if(m_template)
			return m_template->get_name().name_str;
		else
			return nstring(_T("none"));
	}

	const resource_location& nmaterial::get_template_loc() const
	{
		return m_template->get_location();
	}

	std::string default_mtl_code;
	const std::string& nmaterial::get_shader_code() const
	{
		if(m_template)
			return m_template->get_shader_code();
		else
			return default_mtl_code;
	}

	template<typename T>
	void nmaterial::set_parameter(const nstring& param_name, T val)
	{
		if( !m_template )
			return;

		std::string ansi_name = conv_string(param_name);

		for(size_t i=0; i<m_parameter_array.size(); i++)
		{
			material_parameter_value& param_val = m_parameter_array[i];

			if(param_val.name == ansi_name)
			{
				param_val.set_value(val);
				return;
			}
		}
	
		nLog_Error(_T("material: parameter not found, name=%s.\r\n"), param_name.c_str());
	}

	void nmaterial::set_vector_parameter(const nstring& param_name, const vector4& vec)
	{
		set_parameter(param_name, vec);
	}

	void nmaterial::set_texture_parameter(const nstring& param_name, const resource_location& tex_loc)
	{
		nresource_texture::ptr tex(
			nresource_manager::instance()->load_texture_2d(tex_loc)
			);

		set_parameter(param_name, tex);
	}

	void nmaterial::draw_effect_param(nshading_effect* effect_ptr) const
	{
		for(size_t i=0; i<m_parameter_array.size(); i++)
		{
			const material_parameter_value& param_val = m_parameter_array[i];
			
			switch(param_val.type)
			{
			case EMPT_Float:
				effect_ptr->set_float(param_val.name, param_val.vec_value.x);
				break;
			case EMPT_Float2:
				{
					const vector4& v = param_val.vec_value;
					effect_ptr->set_vector(param_val.name, vector2(v.x, v.y));
				}
				break;
			case EMPT_Float3:
				{
					const vector4& v = param_val.vec_value;
					effect_ptr->set_vector(param_val.name, vector3(v.x, v.y, v.z));
				}
				break;
			case EMPT_Float4:
				effect_ptr->set_vector(param_val.name, param_val.vec_value);
				break;
			case EMPT_Texture:
				if(param_val.tex_value)
					effect_ptr->set_texture(param_val.name, param_val.tex_value->get_render_texture());
				break;
			}
		}
	}
}//namespace nexus