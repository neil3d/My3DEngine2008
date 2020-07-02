#include "StdAfx.h"
#include "navatar_skeletal_mesh.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(navatar_skeletal_mesh, nmesh_component)

	navatar_skeletal_mesh::navatar_skeletal_mesh(const nstring& name_str):nskeletal_mesh_component(name_str)
	{
	}

	navatar_skeletal_mesh::~navatar_skeletal_mesh(void)
	{
	}

	void navatar_skeletal_mesh::update(float delta_time, const nviewport& view)
	{
		nmesh_component::update(delta_time, view);

		// anim ctrl已经由owner统一tick，所以这里不需要再tick
	}
}//namespace nexus