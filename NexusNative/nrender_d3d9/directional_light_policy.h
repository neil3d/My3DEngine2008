/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Nov, 2008
*/


#ifndef _NEXUS_D3D9_DIRECTIONAL_LIGHT_POLICY_H_
#define _NEXUS_D3D9_DIRECTIONAL_LIGHT_POLICY_H_
#include "lighting_policy.h"

namespace nexus
{
	/**
	 *	方向光的policy
	*/
	class directional_light_policy :
		public lighting_policy
	{
	public:
		directional_light_policy(void);
		virtual ~directional_light_policy(void);

		static nstring get_name(void)	{	return nstring(_T("directional_light"));}
		static void modify_drawing_policy_type(light_drawing_policy_type* dp_type);

		virtual void set_effect_parameter(d3d9_shading_effect* effect_ptr, const nrender_light_proxy* lgt);
	};
}//namespace nexus
#endif //_NEXUS_D3D9_DIRECTIONAL_LIGHT_POLICY_H_