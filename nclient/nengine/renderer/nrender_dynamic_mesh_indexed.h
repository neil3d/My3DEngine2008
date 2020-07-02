#ifndef _NEXUS_RENDER_DYNAMIC_MESH_INDEXED_H_
#define _NEXUS_RENDER_DYNAMIC_MESH_INDEXED_H_
#include "nrender_mesh.h"

namespace nexus
{
	class nAPI nrender_dynamic_mesh_indexed
		: public nrender_mesh
	{
	public:
		nrender_dynamic_mesh_indexed(void)	{	}
		~nrender_dynamic_mesh_indexed(void)	{	}

		virtual void create(enum EPrimitiveType prim_type,
			const nmesh_vertex_data* vert_data, const index_buffer16* index_data, int mtl_id) = 0;

		virtual void update_vertices(const nmesh_vertex_data* vert_data) = 0;
		virtual void update_indices(const index_buffer16* index_data, size_t valid_count) = 0;

		nDECLARE_VIRTUAL_CLASS(nrender_dynamic_mesh_indexed)
	};
}//namespace nexus
#endif