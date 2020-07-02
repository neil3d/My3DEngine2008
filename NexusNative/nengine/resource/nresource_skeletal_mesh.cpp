#include "StdAfx.h"
#include "nresource_manager.h"
#include "nresource_skeletal_mesh.h"
#include "resource_importer_manager.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nresource_skeletal_mesh, nresource)

	nresource_skeletal_mesh::nresource_skeletal_mesh(const nstring& name_str):nresource(name_str)
	{
	}

	nresource_skeletal_mesh::~nresource_skeletal_mesh(void)
	{
	}

	void nresource_skeletal_mesh::import_lod(const nstring& full_path)
	{
		nskeletal_mesh::ptr new_lod_mesh =
			resource_importer_manager::instance()->import_skeletal_mesh(full_path);

		m_lod_array.push_back(new_lod_mesh);
	}

	void nresource_skeletal_mesh::import_set_material(int lod, int mtl_id, nmaterial_base::ptr mtl_ptr)
	{
		nASSERT(lod < (int)m_lod_array.size());
		nskeletal_mesh::ptr mesh_lod  = m_lod_array[lod];

		nASSERT(mtl_id < (int)mesh_lod->m_mtl_array.size());
		mesh_lod->m_mtl_array[mtl_id] = mtl_ptr;
	}

	nmaterial_base* nresource_skeletal_mesh::get_material(int lod, int mtl_id) const
	{
		nASSERT(lod < (int)m_lod_array.size());
		nskeletal_mesh::ptr mesh_lod  = m_lod_array[lod];

		if((int)mesh_lod->m_mtl_array.size() < mtl_id
			|| !mesh_lod->m_mtl_array[mtl_id])
			return nresource_manager::instance()->get_default_material();
		else
			return mesh_lod->m_mtl_array[mtl_id].get();
	}

	void nresource_skeletal_mesh::serialize(narchive& ar)	
	{}
}//namespace nexus