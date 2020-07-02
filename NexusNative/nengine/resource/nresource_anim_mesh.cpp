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
		for (size_t i=0; i<m_lod_array.size(); i++)
		{
			nanim_mesh::ptr lod = m_lod_array[i];
			for(size_t j=0; j<lod->m_mtl_array.size(); j++)
			{
				nmaterial_base::ptr mtl = lod->m_mtl_array[j];
				if(mtl)
					mtl->_destroy();
			}
		}
	}

	nmaterial_base::ptr nresource_anim_mesh::get_material(int lod, int mtl_id) const
	{
		nmaterial_base::ptr mtl_ret;
		if( lod < 0
			|| lod >= (int)m_lod_array.size() )
			return mtl_ret;

		nanim_mesh::ptr lod_mesh = m_lod_array[lod];		

		if(lod_mesh->m_mtl_array.empty()
			|| (size_t)mtl_id >= lod_mesh->m_mtl_array.size()
			|| !lod_mesh->m_mtl_array[mtl_id])
			return mtl_ret;
		else
			return lod_mesh->m_mtl_array[mtl_id];
	}

	void nresource_anim_mesh::import_lod(int lod, const nstring& full_path)
	{
		nASSERT( lod >= 0 );
		nanim_mesh::ptr lod_mesh 
			= resource_importer_manager::instance()->import_anim_mesh(full_path);
		nASSERT( lod_mesh );

		if( lod >= (int)m_lod_array.size() )
			m_lod_array.push_back(lod_mesh);
		else
			m_lod_array[lod] = lod_mesh;
	}

	void nresource_anim_mesh::import_set_material(int lod, int mtl_id, nmaterial_base::ptr mtl_ptr)
	{
		nASSERT(lod>=0 && lod<(int)m_lod_array.size());
		nanim_mesh::ptr lod_mesh = m_lod_array[lod];		

		nASSERT(mtl_id>=0 && (size_t)mtl_id<lod_mesh->m_mtl_array.size());
		lod_mesh->m_mtl_array[mtl_id] = mtl_ptr;
	}

	size_t nresource_anim_mesh::get_num_section(size_t lod) const
	{
		if( lod >= m_lod_array.size() )
			return 0;
		else
			return m_lod_array[lod]->m_secton_array.size();
	}

	void nresource_anim_mesh::get_lod_info(size_t i, int& num_vert, int& num_sec, int& num_tri) const
	{
		if( i >= m_lod_array.size() )
		{
			num_vert = num_sec = num_tri = 0;
			return;
		}

		nanim_mesh::ptr lod_mesh = m_lod_array[i];
		if( lod_mesh )
		{
			num_sec = lod_mesh->m_secton_array.size();
			num_tri = lod_mesh->get_face_count();

			if( lod_mesh->m_sequence_array.size() > 0)
			{
				nanim_mesh_sequence::ptr seq = lod_mesh->m_sequence_array[0];
				num_vert = seq->m_tracks.size();
			}
		}
	}

	size_t nresource_anim_mesh::get_num_sequence() const
	{
		if (m_lod_array.empty())
		{
			return 0;
		}

		nanim_mesh::ptr lod_mesh = m_lod_array[0];
		return lod_mesh->m_sequence_array.size();
	}

	void nresource_anim_mesh::get_sequence_info(size_t i, nstring& name, float& length) const
	{
		if (!m_lod_array.empty())
		{
			nanim_mesh::ptr lod_mesh = m_lod_array[0];
			if(i < lod_mesh->m_sequence_array.size())
			{
				nanim_mesh_sequence::ptr seq = lod_mesh->m_sequence_array[i];
				name = seq->m_name;
				length = seq->get_length();
				return;
			}
		}// end of if

		name = _T("none");
		length = 0;			
	}
}//namespace nexus