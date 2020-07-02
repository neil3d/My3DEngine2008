/**
*	Nexus Engine
*
*	Author: mengrui
*	Date:	april,2010
*/


#ifndef _NEXUS_D3D9_SPOT_LIGHT_POLICY_H_
#define _NEXUS_D3D9_SPOT_LIGHT_POLICY_H_
#include "lighting_policy.h"

namespace nexus
{
	/**
	*	¾Û¹âµÆµÄpolicy
	*/
	class spot_light_policy :
		public lighting_policy
	{
	public:
		spot_light_policy(void)
		{}
		virtual ~spot_light_policy(void)
		{}

		static nstring get_name(void)	{	return nstring(_T("spot_light"));}
		static void modify_drawing_policy_type(light_drawing_policy_type* dp_type)
		{
			wostringstream ss;
			ss	<< _T("shader_d3d9/lighting/")
				<< get_name()
				<< _T(".hlsl");

			dp_type->m_light_policy = ss.str();
		}

		static EDrawingPolicy  get_policy_type() { return EDP_Spot_Light; }
	};
}//namespace nexus
#endif //_NEXUS_D3D9_SPOT_LIGHT_POLICY_H_