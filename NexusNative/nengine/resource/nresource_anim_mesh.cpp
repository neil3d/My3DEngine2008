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
				nmtl_base::ptr mtl = lod->m_mtl_array[j];
				if(mtl)
					mtl->_destroy();
			}
		}
	}

	nmtl_base::ptr nresource_anim_mesh::get_material(int lod, int mtl_id) const
	{
		nmtl_base::ptr mtl_ret;
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

	void nresource_anim_mesh::import_set_material(int lod, int sec, nmtl_base::ptr mtl_ptr)
	{
		if((size_t)lod >= m_lod_array.size())
		{
			nLog_Error(_T("bad lod in [nresource_anim_mesh::import_set_material()]"));
			return;
		}
		
		nanim_mesh::ptr lod_mesh = m_lod_array[lod];		
		if((size_t)sec >= lod_mesh->m_secton_array.size())
		{
			nLog_Error(_T("bad section in [nresource_anim_mesh::import_set_material()]"));
			return;
		}
		nmesh_section::ptr mesh_sec = lod_mesh->m_secton_array[sec];

		//-- 找到对应的mtl id
		vector<nmtl_base::ptr>& mtl_array = lod_mesh->m_mtl_array;
		if(sec < (int)mtl_array.size())
		{
			mtl_array[sec] = mtl_ptr;
		}
		else
		{
			mtl_array.resize(sec+1);
			mtl_array[sec] = mtl_ptr;
		}
		mesh_sec->m_material_id = sec;

		// delete by zewu: 下面的代码会导致m_mtl_array不断在增大
		//int mtl_id = -1;
		//vector<nmtl_base::ptr>& mtl_array = lod_mesh->m_mtl_array;
		//for (int i=0; i<(int)mtl_array.size(); i++)
		//{
		//	if(mtl_array[i] == mtl_ptr)
		//	{
		//		mtl_id = i;
		//		break;
		//	}
		//}
		//if(mtl_id == -1)
		//{
		//	mtl_id = mtl_array.size();
		//	mtl_array.push_back(mtl_ptr);
		//}

		//-- 设置相应的section
		//mesh_sec->m_material_id = mtl_id;
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

	bool nresource_anim_mesh::get_sequence_info(size_t i, nstring& name, float& length) const
	{
		if (!m_lod_array.empty())
		{
			nanim_mesh::ptr lod_mesh = m_lod_array[0];
			if(i < lod_mesh->m_sequence_array.size())
			{
				nanim_mesh_sequence::ptr seq = lod_mesh->m_sequence_array[i];
				name = seq->m_name;
				length = seq->get_length();
				return true;
			}
		}// end of if

		name = _T("none");
		length = 0;			
		return false;
	}

	bool nresource_anim_mesh::ready() const
	{
		if( !nresource::ready() )
			return false;

		boost::mutex::scoped_lock lock(m_mutex);

		for(size_t i=0; i<m_lod_array.size(); i++)
		{
			nanim_mesh::ptr mesh_lod = m_lod_array[i];

			for (size_t j=0; j<mesh_lod->m_mtl_array.size(); j++)
			{
				nmtl_base::ptr mtl = mesh_lod->m_mtl_array[j];
				if( !mtl )
					continue; // 在资源编辑导入的时候，运行材质为空
				if( !mtl->resource_ready() )
					return false;
			}
		}

		return true;
	}


}//namespace nexus