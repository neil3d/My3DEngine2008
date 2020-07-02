#ifndef _NEXUS_RENDER_SIMPLE_MESH_H_
#define _NEXUS_RENDER_SIMPLE_MESH_H_
#include "nrender_resource.h"
#include "../mesh/simple_mesh.h"

namespace nexus
{
	class nAPI nrender_simple_mesh
		: public nrender_resource
	{
	public:
		nrender_simple_mesh(void)	{	}
		virtual ~nrender_simple_mesh(void)	{	}

		virtual void create(const nsimple_mesh* mesh) = 0;

		nDECLARE_VIRTUAL_CLASS(nrender_simple_mesh)
	};
}//namespace nexus
#endif //_NEXUS_RENDER_SIMPLE_MESH_H_