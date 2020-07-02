/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_MATERIAL_SCRIPT_H_
#define _NEXUS_MATERIAL_SCRIPT_H_

#include "nmaterial_template.h"

namespace nexus
{
	/**
	 *	使用标准的HLSL代码定义材质；
	 *	@remark 分析HLSL代码中的全局变量定义，来定义材质参数；
	 *	材质参数变量定义的格式必须为：type user_name:sematic；其中sematic必须以“MTL_”开头，例如“MTL_DIFFUSE_MAP”
	*/
	class nmaterial_script :
		public nmaterial_template
	{
	public:
		nmaterial_script(const nstring& name_str);
		virtual ~nmaterial_script(void);

		virtual void load_from_file(const resource_location& loc);
		virtual bool ready() const
		{
			// todo
			return true;
		}

	private:
		void parse_material_parameter_from_HLSL();
		void prase_config_from_HLSL();
	};
}//namespace nexus
#endif //_NEXUS_MATERIAL_SCRIPT_H_