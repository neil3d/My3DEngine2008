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
	 *	材质中的材质参数＋值/资源对象数据
	 *	@see class nmaterial
	*/
	struct material_parameter
	{
		std::string			name;
		EMaterialParameter	type;
		std::string			macro;	// 对应的shader宏

		int						int_value;
		vector4					vec_value;
		nresource_texture::ptr	tex_value;
		int						texture_type;	//0-普通2D贴图；1-CubeMap

		material_parameter():type(EMPT_None),int_value(0),texture_type(0)
		{}

		void set_value(const vector4& val)
		{
			vec_value = val;
			tex_value = NULL;
		}

		void set_value(nresource_texture::ptr val, char tex_type)
		{
			tex_value = val;
			texture_type = tex_type;
		}

		void set_value(int val)
		{
			int_value = val;
		}
	};
}//namespace nexus

#endif //_NEXUS_MATERIAL_PARAMETER_H_