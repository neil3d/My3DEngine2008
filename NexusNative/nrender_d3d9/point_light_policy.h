/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Nov, 2008
*/

#ifndef _NEXUS_D3D9_POINT_LIGHT_POLICY_H_
#define _NEXUS_D3D9_POINT_LIGHT_POLICY_H_
#include "lighting_policy.h"

namespace nexus
{
	/**
	 *	点光源的lighting policy
	*/
	class point_light_policy :
		public lighting_policy
	{
	public:
		point_light_policy(void);
		virtual ~point_light_policy(void);

		static nstring get_name(void)	{	return nstring(_T("point_light"));}
		static void modify_drawing_policy_type(light_drawing_policy_type* dp_type);
		static EDrawingPolicy  get_policy_type() { return EDP_Point_Light; }
		//virtual void set_effect_parameter(d3d9_shading_effect* effect_ptr, const nrender_light_proxy* lgt);
	};
}//namespace nexus

#endif //_NEXUS_D3D9_POINT_LIGHT_POLICY_H_