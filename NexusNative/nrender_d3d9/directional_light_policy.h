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
		static EDrawingPolicy  get_policy_type() { return EDP_Directional_Light; }
	};
}//namespace nexus
#endif //_NEXUS_D3D9_DIRECTIONAL_LIGHT_POLICY_H_