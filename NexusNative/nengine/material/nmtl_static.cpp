#include "StdAfx.h"
#include "nmtl_static.h"
#include "../resource/nresource_manager.h"
#include "mtl_param_serialize.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nmtl_static, nmtl_common)

	nmtl_static::nmtl_static(const nstring& name_str):nmtl_common(name_str)
	{
	}

	nmtl_static::~nmtl_static(void)
	{
	}

	void nmtl_static::default_create(const resource_location& shader_loc)
	{
		nstring tech_name( _T("DefaultLightUnlit") );
		create_from_hlsl(tech_name, shader_loc);

		// 设置缺省颜色
		set_value_parameter("MTL_DiffuseColor", vector4(0.13f,0.28f,0.28f,1));	
	}

	void nmtl_static::bind_shader_param(nmtl_tech_shader::ptr mtl_shader)
	{
		for (size_t i=0; i<mtl_shader->get_num_param(); i++)
		{
			const material_parameter& pm = mtl_shader->get_param(i);
			if( have_param(pm.name) ) // 名字相同的参数只保存一次；这可以使得多个Tech共享一个参数
				continue;

			if( pm.type == EMPT_Texture )
			{
				m_texture_param_array.push_back( pm );
			}
			else
			{
				m_value_param_array.push_back( pm );
			}
		}// end of for()
	}
	
	void nmtl_static::setup_effect(nshading_effect* effect_ptr) const
	{
		for (st_param_array::const_iterator iter = m_value_param_array.begin();
			iter != m_value_param_array.end();
			++iter)
		{
			effect_ptr->set_vector(iter->name, iter->vec_value);
		}

		for (st_param_array::const_iterator iter = m_texture_param_array.begin();
			iter != m_texture_param_array.end();
			++iter)
		{
			if (iter->tex_value)
			{
				effect_ptr->set_texture(iter->name,iter->tex_value->get_render_texture());
			}
		}

		//for (size_t i=0; i<m_shader->get_num_param(); i++)
		//{
		//	const material_parameter& pm = m_shader->get_param(i);

		//	switch(pm.type)
		//	{
		//	case EMPT_Float:
		//		{
		//			vector4 vec = get_float_param(pm.name);
		//			effect_ptr->set_float(pm.name, vec.x);
		//		}
		//		break;
		//	case EMPT_Float2:
		//		{
		//			vector4 vec = get_float_param(pm.name);
		//			effect_ptr->set_vector( pm.name, vector2(vec.x, vec.y) );
		//		}
		//		break;
		//	case EMPT_Float3:
		//		{
		//			vector4 vec = get_float_param(pm.name);
		//			effect_ptr->set_vector( pm.name, vector3(vec.x, vec.y, vec.z) );
		//		}
		//		break;
		//	case EMPT_Float4:
		//		{					
		//			effect_ptr->set_vector( pm.name, get_float_param(pm.name) );
		//		}
		//		break;
		//	case EMPT_Texture:
		//		{
		//			nrender_texture* tex = get_texture_param(pm.name);
		//			if( !tex )
		//				tex = nresource_manager::instance()->get_white_texture()->get_render_texture();
		//			effect_ptr->set_texture(pm.name, tex);
		//		}
		//		break;
		//	}
		//}// end of for()
	}

	vector4 nmtl_static::get_float_param(const std::string& param_name) const
	{
		vector4 ret(0,0,0,0);

		for (st_param_array::const_iterator iter = m_value_param_array.begin();
			iter != m_value_param_array.end();
			++iter)
		{
			const material_parameter& val = (*iter);
			if(val.name == param_name)
				return val.vec_value;
		}

		return ret;
	}

	bool nmtl_static::have_param(const std::string& param_name) const
	{
		if( nmtl_common::have_param( param_name) )
			return true;

		for (st_param_array::const_iterator iter = m_value_param_array.begin();
			iter != m_value_param_array.end();
			++iter)
		{
			const material_parameter& val = (*iter);
			if(val.name == param_name)
				return true;
		}

		return false;
	}

	void nmtl_static::set_value_parameter(const std::string& param_name, const vector4& val)
	{
		for (st_param_array::iterator iter = m_value_param_array.begin();
			iter != m_value_param_array.end();
			++iter)
		{
			material_parameter& pam = (*iter);
			if(pam.name == param_name)
			{
				pam.vec_value = val;
				break;
			}
		}
	}

	void nmtl_static::set_value_parameter(const std::string& param_name, int val)
	{
		for (st_param_array::iterator iter = m_value_param_array.begin();
			iter != m_value_param_array.end();
			++iter)
		{
			material_parameter& pam = (*iter);
			if(pam.name == param_name)
			{
				pam.int_value = val;
				break;
			}
		}
	}

	void nmtl_static::serialize(narchive& ar)
	{
		nmtl_common::serialize(ar);

		nSERIALIZE(ar, m_value_param_array);
	}
}//namespace nexus