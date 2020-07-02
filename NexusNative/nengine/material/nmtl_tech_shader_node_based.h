#ifndef _NEXUS_MATERIAL_TECH_SHADER_NODE_BASED_H_
#define _NEXUS_MATERIAL_TECH_SHADER_NODE_BASED_H_
#include "nmtl_tech_shader.h"

namespace nexus
{
	/**
	 *	基于表达式节点方式编辑的材质Shader
	*/
	class nmtl_tech_shader_node_based : public nmtl_tech_shader
	{
	public:
		nmtl_tech_shader_node_based(const nstring& name_str);
		virtual ~nmtl_tech_shader_node_based(void);
	};
}//namespace nexus
#endif //_NEXUS_MATERIAL_TECH_SHADER_NODE_BASED_H_