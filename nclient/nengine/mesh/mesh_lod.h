/**
*	Nexus Engine - Static Mesh data define
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_MESH_SECTION_H_
#define _NEXUS_MESH_SECTION_H_
#include "ncore.h"
#include "mesh_index_data.h"
#include "mesh_vertex_data_common.h"
#include "../material/nmaterial.h"
#include "../../../ncore/math/geom_def.h"

namespace nexus
{
	/**
	 *	引用通一个vertex buffer，但是材质不同的一块mesh
	*/
	struct nAPI nmesh_section
		: public nobject
	{
		typedef shared_ptr<nmesh_section> ptr;

		index_buffer16		m_index_buffer;
		int					m_material_id;
		enum EPrimitiveType	m_primitive_type;

		virtual void serialize(narchive& ar);
		nDECLARE_CLASS(nmesh_section)
	};

	/**
	 *	一个static mesh lod的完整数据，相当于一个完整的可渲染static mesh
	*/
	struct nAPI nstatic_mesh_lod
		: public nobject
	{
		typedef shared_ptr<nstatic_mesh_lod> ptr;

		nmesh_vertex_data::ptr		m_vert_data;
		vector<nmesh_section::ptr>	m_secton_array;
		vector<nmaterial_base::ptr>	m_mtl_array;
		box_sphere_bounds			m_bounding_box;

		size_t get_face_count() const
		{
			size_t num_indices = 0;
			for(size_t i=0; i<m_secton_array.size(); i++)
				num_indices += m_secton_array[i]->m_index_buffer.get_index_count();

			return num_indices/3;
		}

		virtual void serialize(narchive& ar);
		nDECLARE_CLASS(nstatic_mesh_lod)
	};

}//namespace nexus

#endif _NEXUS_MESH_SECTION_H_