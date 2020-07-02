#ifndef _NEXUS_D3D9_MESH_UTIL_H_
#define _NEXUS_D3D9_MESH_UTIL_H_
#include "d3d9_mesh_section.h"

namespace nexus
{
	void _create_mesh_section_array(const vector<nmesh_section::ptr>& nsec_array, vector<d3d9_mesh_section::ptr>& d3d_array, D3DPOOL pool);
}//namespace nexus

#endif //_NEXUS_D3D9_MESH_UTIL_H_