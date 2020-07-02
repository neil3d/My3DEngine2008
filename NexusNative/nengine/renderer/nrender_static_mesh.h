/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_STATIC_MESH_H_
#define _NEXUS_STATIC_MESH_H_
#include "nrender_mesh.h"
#include "../mesh/mesh_vertex_data.h"

namespace nexus
{
	/**
	 *	只使用vertex数据的static mesh
	*/
	class nAPI nrender_static_mesh 
		: public nrender_mesh
	{
	public:
		nrender_static_mesh(void)	{	}
		virtual ~nrender_static_mesh(void)	{	}

		virtual void create(enum EPrimitiveType prim_type, const nmesh_vertex_data* vb) = 0;

		nDECLARE_VIRTUAL_CLASS(nrender_static_mesh)
	};
}//namespace nexus
#endif //_NEXUS_STATIC_MESH_H_