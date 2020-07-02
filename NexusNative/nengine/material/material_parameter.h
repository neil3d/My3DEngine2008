/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_MATERIAL_PARAMETER_H_
#define _NEXUS_MATERIAL_PARAMETER_H_
#include "../resource/nresource_texture.h"

namespace nexus
{
	enum EMaterialParameter
	{
		EMPT_None,
		EMPT_Float,
		EMPT_Float2,
		EMPT_Float3,
		EMPT_Float4,
		EMPT_Texture,
		EMPT_Int,
	};

	/**
	 *	材质模版中的材质参数信息
	 *	@see class nmaterial_template
	*/
	struct material_parameter
	{
		std::string			name;
		EMaterialParameter	type;

		material_parameter():type(EMPT_None)
		{}
	};

	/**
	 *	材质中的材质参数＋值/资源对象数据
	 *	@see class nmaterial
	*/
	struct material_parameter_value : public material_parameter
	{
		int						int_value;
		vector4					vec_value;
		nresource_texture::ptr	tex_value;

		material_parameter_value():int_value(0)
		{}
		material_parameter_value(const material_parameter* param)
			:int_value(0),vec_value(0,0,0,0),tex_value(NULL)
		{
			name = param->name;
			type = param->type;
		}

		void set_value(const vector4& val)
		{
			vec_value = val;
			tex_value = NULL;
		}

		void set_value(nresource_texture::ptr val)
		{
			tex_value = val;
		}

		void set_value(int val)
		{
			int_value = val;
		}
	};
}//namespace nexus

#endif //_NEXUS_MATERIAL_PARAMETER_H_