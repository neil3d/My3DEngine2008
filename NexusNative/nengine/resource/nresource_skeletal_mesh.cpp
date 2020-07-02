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

	void nresource_skeletal_mesh::import_lod(int lod, const nstring& full_path)
	{
		nASSERT( lod >= 0 );
		nskeletal_mesh::ptr new_lod_mesh =
			resource_importer_manager::instance()->import_skeletal_mesh(full_path);
		nASSERT( new_lod_mesh );

		if( lod >= (int)m_lod_array.size() )
			m_lod_array.push_back(new_lod_mesh);
		else
			m_lod_array[lod] = new_lod_mesh;
	}

	void nresource_skeletal_mesh::import_set_material(int lod, int sec, nmtl_base::ptr mtl_ptr)
	{
		if((size_t)lod >= m_lod_array.size())
		{
			nLog_Error(_T("bad lod in [nresource_skeletal_mesh::import_set_material()]"));
			return;
		}

		nskeletal_mesh::ptr lod_mesh  = m_lod_array[lod];
		if((size_t)sec >= lod_mesh->m_secton_array.size())
		{
			nLog_Error(_T("bad section in [nresource_skeletal_mesh::import_set_material()]"));
			return;
		}

		nmesh_section::ptr mesh_sec = lod_mesh->m_secton_array[sec];

		//-- 每个section对应一个mateial		
		vector<nmtl_base::ptr>& mtl_array = lod_mesh->m_mtl_array;
		if( mtl_array.empty() )
			mtl_array.resize( lod_mesh->m_secton_array.size() );

		mtl_array[sec] = mtl_ptr;
		mesh_sec->m_material_id = sec;
	}

	nmtl_base::ptr nresource_skeletal_mesh::get_material(int lod, int mtl_id) const
	{
		nmtl_base::ptr mtl_ret;

		nASSERT(lod >= 0 && lod < (int)m_lod_array.size());
		nskeletal_mesh::ptr mesh_lod  = m_lod_array[lod];

		if( mesh_lod->m_mtl_array.empty()
			|| (size_t)mtl_id >= mesh_lod->m_mtl_array.size() 
			|| !mesh_lod->m_mtl_array[mtl_id])
			return mtl_ret;
		else
			return mesh_lod->m_mtl_array[mtl_id];
	}

	void nresource_skeletal_mesh::serialize(narchive& ar)	
	{
		nresource::serialize(ar);
		nSERIALIZE(ar, m_lod_array);
	}

	bool nresource_skeletal_mesh::ready() const
	{
		if( !nresource::ready() )
			return false;

		boost::mutex::scoped_lock lock(m_mutex);

		for(size_t i=0; i<m_lod_array.size(); i++)
		{
			nskeletal_mesh::ptr mesh_lod = m_lod_array[i];

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

	void nresource_skeletal_mesh::get_lod_info( size_t i, int& num_vert, int& num_sec, int& num_tri ) const
	{
		if( i >= m_lod_array.size() )
		{
			num_vert = num_sec = num_tri = 0;
			return;
		}

		nskeletal_mesh::ptr lod_mesh = m_lod_array[i];
		if( lod_mesh )
		{
			num_sec = lod_mesh->m_secton_array.size();
			num_tri = lod_mesh->get_face_count();
		}
	}

	size_t nresource_skeletal_mesh::get_num_section(size_t lod) const
	{
		if( lod >= m_lod_array.size() )
			return 0;
		nskeletal_mesh::ptr lod_mesh = m_lod_array[lod];
		if( lod_mesh )		
			return lod_mesh->m_secton_array.size();
		else
			return 0;
	}

	void nresource_skeletal_mesh::get_bounds(box_sphere_bounds& bounds) const
	{
		if(!m_lod_array.empty())
		{
			nskeletal_mesh::ptr mesh = m_lod_array[0];
			if(mesh)
				bounds = mesh->m_bounding_box;
		}
	}
}//namespace nexus