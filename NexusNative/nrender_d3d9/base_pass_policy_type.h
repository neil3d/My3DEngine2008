/**
*	Nexus Engine
*
*	Author: mr
*	Date:	April, 2010
*/
#pragma once
#include "util.h"
#include "drawing_policy_type.h"

namespace nexus
{
	class base_pass_policy_type :
		public drawing_policy_type
	{
	public:
		base_pass_policy_type(const nstring& _dp_name):drawing_policy_type(_dp_name)
		{	}
		virtual ~base_pass_policy_type(void)	{	}

		virtual bool get_include_code(const char* include_name, std::string& out_code)
		{	
			if(_stricmp(include_name, "ambent_light.hlsl") == 0)
			{
				out_code = m_AmbentCode;
				return true;
			}

			return false; 
		}

		virtual void load_shader_file()
		{
			nstring dp_name = _T("shader_d3d9/drawing_base_pass.fx");
			load_shader_source(dp_name, shader_source);
			load_shader_source(m_Ambent_policy, m_AmbentCode);
		}


	public:
		nstring	m_Ambent_policy;

	private:
		std::string	m_AmbentCode;
	};
}//namespace nexus
