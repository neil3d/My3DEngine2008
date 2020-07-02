/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Nov, 2008
*/

#ifndef _NEXUS_D3D9_LIGHT_DRAWING_POLICY_TYPE_H_
#define _NEXUS_D3D9_LIGHT_DRAWING_POLICY_TYPE_H_

#include "drawing_policy_type.h"

namespace nexus
{
	/**
	 *	光照渲染所使用的drawing policy type
	 *	@remark 在标准dp type上增加了“lighting.hlsl”的组合处理
	*/
	class light_drawing_policy_type :
		public drawing_policy_type
	{
	public:
		light_drawing_policy_type(const nstring& _dp_name):drawing_policy_type(_dp_name)
		{	}
		virtual ~light_drawing_policy_type(void)	{	}

		virtual bool get_include_code(const char* include_name, std::string& out_code)
		{	
			if(_stricmp(include_name, "lighting.hlsl") == 0)
			{
				out_code = m_light_code;
				return true;
			}

			return false; 
		}

	public:
		std::string	m_light_code;
	};
}//namespace nexus

#endif //_NEXUS_D3D9_LIGHT_DRAWING_POLICY_TYPE_H_