#ifndef _NEXUS_CPU_SKIN_MESH_
#define _NEXUS_CPU_SKIN_MESH_
#include "nrender_mesh.h"
#include "../mesh/mesh_vertex_data_skin.h"
#include "../mesh/skeletal_mesh.h"
#include "../mesh/skeletal_anim.h"
#include "../animation/anim_define.h"

namespace nexus
{
	class nAPI nrender_cpu_skin_mesh
		: public nrender_mesh
	{
	public:
		nrender_cpu_skin_mesh(void);
		virtual ~nrender_cpu_skin_mesh(void);

		virtual void create(nskeletal_mesh::ptr mesh_ptr);
		void update_skin(const matrix_palette& mat_pal);
	
	protected:
		nskeletal_mesh::ptr				m_source_mesh;
		nmesh_vertex_data_skin::ptr		m_skined_vert_data;

		nDECLARE_VIRTUAL_CLASS(nrender_cpu_skin_mesh)
	};
}//namespace nexus

#endif //_NEXUS_CPU_SKIN_MESH_