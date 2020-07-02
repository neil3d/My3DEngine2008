/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NRENDER_STATIC_MESH_INDEXED_H_
#define _NRENDER_STATIC_MESH_INDEXED_H_
#include "nrender_mesh.h"
#include "../mesh/mesh_lod.h"

namespace nexus
{
	/**
	 *	包含vertex buffer和index buffer的static mesh对象
	*/
	class nAPI nrender_static_mesh_indexed :
		public nrender_mesh
	{
	public:
		nrender_static_mesh_indexed(void)	{	}
		virtual ~nrender_static_mesh_indexed(void)	{	}

		virtual void create(nstatic_mesh_lod* mesh) = 0;
		virtual void create(enum EPrimitiveType prim_type,
			const nmesh_vertex_data* vert_data, const index_buffer16* index_data, int mtl_id) = 0;

		nDECLARE_VIRTUAL_CLASS(nrender_static_mesh_indexed)
	};
}//namespace nexus
#endif //_NRENDER_STATIC_MESH_INDEXED_H_