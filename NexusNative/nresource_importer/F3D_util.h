#pragma once
#include "../nengine/engine_define.h"
#include "../nengine/mesh/nmesh_importer.h"
#include "../F3DFileHeader/SubMesh.h"

namespace nexus
{
	nmesh_section::ptr convert_f3d_sub_mesh(shared_ptr<F3D::LodSubMesh> fm);


	inline vector3 _MAX_TO_D3D(const vector3& max_pos)
	{
		return vector3(max_pos.x, max_pos.z, max_pos.y);		
	}

	inline quat _MAX_TO_D3D(const quat& max_rot)
	{
		return quat(max_rot.x, max_rot.z, max_rot.y, max_rot.w);		
	}

}//namespace nexus