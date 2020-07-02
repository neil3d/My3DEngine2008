#ifndef _NEXUS_SHAPE_FUNC_H_
#define _NEXUS_SHAPE_FUNC_H_

#include "ncore.h"
#include "../renderer/nrender_resource.h"
#include "../renderer/nrender_static_mesh.h"
#include "../renderer/nrender_static_mesh_indexed.h"

namespace nexus
{
	class shape_func
	{
	public:
		shape_func(void);
		~shape_func(void);

		static nrender_static_mesh* create_box(float width, float height, float depth);
		static nrender_static_mesh* create_box_rich(float width, float height, float depth);
		static nrender_static_mesh_indexed* create_sphere(vector3& vCenter, float fRadius, unsigned short wNumRings, unsigned short wNumSections);
	};
}//namespace nexus
#endif //_NEXUS_SHAPE_FUNC_H_