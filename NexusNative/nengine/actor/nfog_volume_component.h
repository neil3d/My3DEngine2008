/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NFOG_VOLUME_COMPONENT_H_
#define _NFOG_VOLUME_COMPONENT_H_
#include "nprimitive_component.h"

namespace nexus
{
	/**
	 *	各种可见图元的基类
	*/

	class nAPI nfog_volume_component :
		public nprimitive_component
	{	
	public:
		enum eshape
	    {
			e_sphere,
			e_box
	    };
		
		eshape shape_type;  //形状
		nfog_volume_component(const nstring& name_str,eshape shaptype = e_box);
		virtual ~nfog_volume_component(void);
		virtual void _init(nactor* owner);
		virtual void _update_transform(const object_space& parent_space);
		virtual void render(class render_package_base* rpb);
		virtual void serialize(narchive& ar);

	public:
		// rigid body interface
		float	global_density;
		color4f	color;
		vector3 base_point;
		vector3 dir_scaled;
		vector2 soft_edges;
		float fog_start_distance;

	private:	
		nDECLARE_NAMED_CLASS(nfog_volume_component)
	};
}//namespace nexus
#endif 