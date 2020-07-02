#include "StdAfx.h"
#include "nresource_anim_mesh.h"
#include "../framework/nengine.h"
#include "../renderer/nrender_resource_manager.h"
#include "resource_importer_manager.h"
#include "nresource_manager.h"


namespace nexus
{
	nDEFINE_NAMED_CLASS(nresource_anim_mesh, nresource)

	nresource_anim_mesh::nresource_anim_mesh(const nstring& name_str):nresource(name_str)
	{
	}

	nresource_anim_mesh::~nresource_anim_mesh(void)
	{
	}

	nmaterial_base* nresource_anim_mesh::get_material(int lod, int mtl_id) const
	{
		(void)lod;

		if(m_mesh->m_mtl_array.empty()
			|| m_mesh->m_mtl_array.size() < (size_t)mtl_id
			|| !m_mesh->m_mtl_array[mtl_id])
			return nresource_manager::instance()->get_default_material();
		else
			return m_mesh->m_mtl_array[mtl_id].get();
	}

	void nresource_anim_mesh::import_mesh(const nstring& full_path)
	{
		m_mesh = resource_importer_manager::instance()->import_anim_mesh(full_path);
	}

	void nresource_anim_mesh::import_set_material(int mtl_id, nmaterial_base::ptr mtl_ptr)
	{
		nASSERT(mtl_id>=0 && (size_t)mtl_id<m_mesh->m_mtl_array.size());
		m_mesh->m_mtl_array[mtl_id] = mtl_ptr;
	}
}//namespace nexus