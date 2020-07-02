#include "StdAfx.h"
#include "mesh_lod.h"
#include "../resource/nresource_manager.h"

namespace nexus
{
	nDEFINE_CLASS(nstatic_mesh_lod, nobject)
	nDEFINE_CLASS(nmesh_section, nobject)

	void nstatic_mesh_lod::serialize(narchive& ar)
	{
		nSERIALIZE(ar, m_vert_data);
		nSERIALIZE(ar, m_secton_array);	
		nSERIALIZE(ar, m_bounding_box);

		vector<resource_location> m_mtl_loc_array;
		if(ar.is_loading())
		{
			nSERIALIZE(ar, m_mtl_loc_array);
			m_mtl_array.resize(m_mtl_loc_array.size());
			for(size_t i=0;i<m_mtl_loc_array.size();++i)
			{
				m_mtl_array[i]=nmtl_base::create_from_file(m_mtl_loc_array[i]);
			}
		}
		else
		{
			m_mtl_loc_array.resize(m_mtl_array.size());
			for(size_t i=0;i<m_mtl_array.size();++i)
			{
				if(m_mtl_array[i]!=NULL)
				{
					m_mtl_loc_array[i]=resource_location(m_mtl_array[i]->get_name());
				}
			}
			nSERIALIZE(ar, m_mtl_loc_array);
		}
	}

	void nmesh_section::serialize(narchive& ar)
	{
		nserialize(ar, m_index_buffer.data, _T("index_buffer"));
		nSERIALIZE(ar, m_material_id);
		nserialize_enum_as_int(ar, m_primitive_type, _T("m_primitive_type"));
		nSERIALIZE(ar, m_base_vertex_index);
	}
	
}//namespace nexus