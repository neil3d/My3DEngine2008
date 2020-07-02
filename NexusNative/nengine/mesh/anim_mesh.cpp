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
		nSERIALIZE(ar, m_mtl_array);
		nSERIALIZE(ar, m_sequence_array);
	}
}