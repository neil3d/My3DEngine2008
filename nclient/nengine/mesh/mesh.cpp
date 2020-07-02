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
		nSERIALIZE(ar, m_mtl_array);
		nSERIALIZE(ar, m_bounding_box);
	}

	void nmesh_section::serialize(narchive& ar)
	{
		nserialize(ar, m_index_buffer.data, _T("index_buffer"));
		nSERIALIZE(ar, m_material_id);
		nserialize_enum_as_int(ar, m_primitive_type, _T("m_primitive_type"));
	}
	
}//namespace nexus