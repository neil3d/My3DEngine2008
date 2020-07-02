/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Nov, 2008
*/

#ifndef _NEXUS_D3D9_LIGHTING_POLICY_H_
#define _NEXUS_D3D9_LIGHTING_POLICY_H_
#include "light_drawing_policy_type.h"

namespace nexus
{
	class light_drawing_policy_type;
	class d3d9_shading_effect;
	/**
	*	光照渲染策略
	*	@remark 完成以下工作：
	*		1.一段实现光照的HLSL代码；
	*		2.控制drawing policy代码生成的macro；
	*		3.知道自己有哪些effect参数，以及如何设置它们；
	*/
	class lighting_policy
	{
	public:
		lighting_policy(void){	}
		virtual ~lighting_policy(void)	{	}

		static nstring get_name(void)	{	return nstring(_T("light_none"));}
		static void modify_drawing_policy_type(light_drawing_policy_type* dp_type)	{}
	};
}//namespace nexus
#endif //_NEXUS_D3D9_LIGHTING_POLICY_H_