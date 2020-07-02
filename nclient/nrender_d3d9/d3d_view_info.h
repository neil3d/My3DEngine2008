/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_D3D_VIEW_INFO_H_
#define _NEXUS_D3D_VIEW_INFO_H_

namespace nexus
{
	struct d3d_view_info
	{
		vector3		eye_pos;
		matrix44	mat_view;
		matrix44	mat_project;
		matrix44	mat_view_project;
	};
}//namespace nexus

#endif //_NEXUS_D3D_VIEW_INFO_H_