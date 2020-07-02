#ifndef _NEXUS_MATERIAL_TECH_SHADER_HLSL_H_
#define _NEXUS_MATERIAL_TECH_SHADER_HLSL_H_
#include "nmtl_tech_shader.h"

namespace nexus
{
	/**
	*	使用标准的HLSL代码定义材质Pass；
	*	@remark 分析HLSL代码中的全局变量定义，来定义材质参数；
	*	材质参数变量定义的格式必须为：type user_name:sematic；其中sematic必须开头为“MTL_”，例如“MTL_DIFFUSE_MAP”
	*/
	class nAPI nmtl_tech_shader_hlsl : public nmtl_tech_shader
	{
	public:
		nmtl_tech_shader_hlsl(const nstring& name_str);
		virtual ~nmtl_tech_shader_hlsl(void);

		virtual void load_from_file(const resource_location& loc);

	private:
		void parse_parameter_from_hlsl();		
		void parse_annotation(const std::string text, material_parameter& param);
	};
}//namespace nexus
#endif //_NEXUS_MATERIAL_TECH_SHADER_HLSL_H_