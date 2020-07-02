#ifndef _NEXUS_SKELETAL_MESH_H_
#define _NEXUS_SKELETAL_MESH_H_

#include "mesh_lod.h"
#include "mesh_vertex_data_skin.h"

namespace nexus
{
	class nAPI nskeletal_mesh 	
		: public nobject
	{
	public:
		typedef shared_ptr<nskeletal_mesh> ptr;

		nskeletal_mesh(void);
		virtual ~nskeletal_mesh(void);

		nmesh_vertex_data_skin::ptr	m_vert_data;
		vector<nmesh_section::ptr>	m_secton_array;
		vector<nmtl_base::ptr>		m_mtl_array;
		box_sphere_bounds			m_bounding_box;

		size_t get_face_count() const
		{
			size_t num_indices = 0;
			for(size_t i=0; i<m_secton_array.size(); i++)
				num_indices += m_secton_array[i]->m_index_buffer.get_index_count();

			return num_indices/3;
		}

		virtual void serialize(narchive& ar);

		nDECLARE_CLASS(nskeletal_mesh)
	};
}//namespace nexus

#endif //_NEXUS_SKELETAL_MESH_H_