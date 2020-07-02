#include "StdAfx.h"
#include "nmtl_tech_shader.h"

namespace nexus
{
	nmtl_tech_shader::nmtl_tech_shader(const nstring& name_str):nresource(name_str)
	{
	}

	nmtl_tech_shader::~nmtl_tech_shader(void)
	{
	}

	const material_parameter& nmtl_tech_shader::get_param(size_t i) const
	{
		nASSERT( i<m_param_array.size() );
		return m_param_array[i];
	}
}//namespace nexus