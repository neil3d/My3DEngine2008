#include "StdAfx.h"
#include "anim_mesh.h"
#include "../resource/nresource_manager.h"

namespace nexus
{
	nDEFINE_CLASS(nanim_mesh_sequence, nobject)
	nDEFINE_CLASS(nanim_mesh, nobject)

	void nanim_mesh_sequence::serialize(narchive& ar)
	{
		nSERIALIZE(ar, m_name);	
		nSERIALIZE(ar, m_tracks);		
		nSERIALIZE(ar, m_frame_time);
		nSERIALIZE(ar, m_frame_bounds);
	}
	
	void nanim_mesh::serialize(narchive& ar)
	{
		nSERIALIZE(ar, m_secton_array);
		nSERIALIZE(ar, m_sequence_array);

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
}