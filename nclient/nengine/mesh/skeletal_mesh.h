#ifndef _NEXUS_SKELETAL_MESH_H_
#define _NEXUS_SKELETAL_MESH_H_

#include "mesh_lod.h"
#include "mesh_vertex_data_skin.h"

namespace nexus
{
	class nAPI nskeletal_mesh
	{
	public:
		typedef shared_ptr<nskeletal_mesh> ptr;

		nskeletal_mesh(void);
		virtual ~nskeletal_mesh(void);

		nmesh_vertex_data_skin::ptr	m_vert_data;
		vector<nmesh_section::ptr>	m_secton_array;
		vector<nmaterial_base::ptr>	m_mtl_array;
		box_sphere_bounds			m_bounding_box;
	};
}//namespace nexus

#endif //_NEXUS_SKELETAL_MESH_H_