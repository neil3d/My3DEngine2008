#ifndef _NEXUS_MATERIAL_DYNAMIC_H_
#define _NEXUS_MATERIAL_DYNAMIC_H_
#include "nmtl_common.h"

namespace nexus
{
	/**
	 *	所有材质Shader数值型参数使用Distribution变量，自动计算其变化
	*/
	class nAPI nmtl_dynamic : public nmtl_common
	{
	public:
		explicit nmtl_dynamic(const nstring& name_str);
		virtual ~nmtl_dynamic(void);
	};
}//namespace nexus
#endif //_NEXUS_MATERIAL_DYNAMIC_H_