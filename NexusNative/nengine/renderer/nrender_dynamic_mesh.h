#ifndef _NEXUS_RENDER_DYNAMIC_MESH_H_
#define _NEXUS_RENDER_DYNAMIC_MESH_H_
#include "nrender_mesh.h"

namespace nexus
{

	class nAPI nrender_dynamic_mesh
		: public nrender_mesh
	{
	public:
		nrender_dynamic_mesh(void):m_draw_limit(-1)
		{	}
		virtual ~nrender_dynamic_mesh(void)	{	}

		virtual void create(enum EPrimitiveType prim_type,
			const nmesh_vertex_data* vert_data, int mtl_id) = 0;

		virtual void update_vertices(const nmesh_vertex_data* vert_data) = 0;		

		virtual void set_draw_limit(int primitive_count)
		{
			m_draw_limit = primitive_count;
		}
	protected:
		int	m_draw_limit;	// vertex buffer中可能保存了很多primitive，但是画的时候最多只画出这么多；指定为-1表示有多少画多少；

		nDECLARE_VIRTUAL_CLASS(nrender_dynamic_mesh)
	};
}//namespace nexus

#endif //_NEXUS_RENDER_DYNAMIC_MESH_H_