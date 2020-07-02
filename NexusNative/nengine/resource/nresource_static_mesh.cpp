#include "StdAfx.h"
#include "nresource_manager.h"
#include "nresource_static_mesh.h"
#include "../framework/nengine.h"
#include "../renderer/nrender_resource_manager.h"
#include "resource_importer_manager.h"

#pragma warning(disable:4100)
#include "../../ConvexDecomposition/ConvexDecomposition.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(nresource_static_mesh, nresource)

		nresource_static_mesh::nresource_static_mesh(const nstring& name_str):nresource(name_str)
	{
	}

	nresource_static_mesh::~nresource_static_mesh(void)
	{
		for (size_t i=0; i<m_lod_array.size(); i++)
		{
			nstatic_mesh_lod::ptr lod = m_lod_array[i];
			for(size_t j=0; j<lod->m_mtl_array.size(); j++)
			{
				nmaterial_base::ptr mtl = lod->m_mtl_array[j];
				if(mtl)
					mtl->_destroy();
			}
		}
	}

	nrender_mesh* nresource_static_mesh::get_render_mesh(int lod) const
	{
		if( m_render_mesh_array.empty() )
			return NULL;
		if( lod<0 || lod >= (int)m_render_mesh_array.size() )
			return NULL;

		return m_render_mesh_array[0];
	}

	void nresource_static_mesh::import_lod(int lod, const nstring& full_path)
	{
		nstatic_mesh_lod::ptr new_lod_mesh = 
			resource_importer_manager::instance()->import_static_mesh(full_path);
		if(new_lod_mesh)
		{
			//-- make NULL material
			new_lod_mesh->m_mtl_array.resize(new_lod_mesh->m_secton_array.size());			

			//-- create render mesh
			nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();

			render_res_ptr<nrender_static_mesh_indexed> new_render_mesh(
				rres_mgr->alloc_static_mesh_indexed() );

			new_render_mesh->create(new_lod_mesh.get());

			//-- add to array
			if( lod==-1
				|| lod >= m_lod_array.size() )
			{
				m_lod_array.push_back(new_lod_mesh);
				m_render_mesh_array.push_back(new_render_mesh.release());
			}
			else
			{
				m_lod_array[lod] = new_lod_mesh;
				m_render_mesh_array.set_at(lod, new_render_mesh.release() );
			}

			//-- 取最高级lod的bounding box
			if( m_lod_array.size() == 1)
				m_bounding_box = new_lod_mesh->m_bounding_box;
		}
	}

	nmaterial_base::ptr nresource_static_mesh::get_material(int lod, int mtl_id) const
	{
		nASSERT(lod < (int)m_lod_array.size());
		nstatic_mesh_lod::ptr mesh_lod  = m_lod_array[lod];

		nmaterial_base::ptr mtl_ret;
		if( mtl_id < (int)mesh_lod->m_mtl_array.size() )
			mtl_ret = mesh_lod->m_mtl_array[mtl_id];
		return mtl_ret;
	}

	void nresource_static_mesh::import_set_material(int lod, int mtl_id, nmaterial_base::ptr mtl_ptr)
	{
		nASSERT(lod < (int)m_lod_array.size());
		nstatic_mesh_lod::ptr mesh_lod  = m_lod_array[lod];

		nASSERT(mtl_id < (int)mesh_lod->m_mtl_array.size());
		mesh_lod->m_mtl_array[mtl_id] = mtl_ptr;

		//-- transparent flag
		m_trans_flag.clear();		

		for(size_t i=0; i<m_lod_array.size(); i++)
		{
			nstatic_mesh_lod::ptr mesh_lod  = m_lod_array[i];	

			for(size_t j=0; j<mesh_lod->m_mtl_array.size(); j++)
			{
				nmaterial_base::ptr mtl_ptr = mesh_lod->m_mtl_array[j];

				if( mtl_ptr)
					m_trans_flag.set(mtl_ptr->get_trans_type());										
				else				
					m_trans_flag.set(ETrans_Opaque);										
			}
		}
	}

	void nresource_static_mesh::serialize(narchive& ar)
	{
		nresource::serialize(ar);

		nSERIALIZE(ar, m_bounding_box);
		nSERIALIZE(ar, m_trans_flag);
		nSERIALIZE(ar, m_lod_array);

		if( ar.is_loading() )
		{
			//-- create render resouce
			nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();
			for(size_t i=0; i<m_lod_array.size(); i++)
			{
				nstatic_mesh_lod::ptr mesh_lod = m_lod_array[i];
				render_res_ptr<nrender_static_mesh_indexed> new_render_mesh(
					rres_mgr->alloc_static_mesh_indexed() );

				new_render_mesh->create(mesh_lod.get());

				//-- add to array			
				m_render_mesh_array.push_back(new_render_mesh.release());
			}
		}// end of if
	}

	void nresource_static_mesh::generate_simple_mesh()
	{
		m_simple_mesh.reset();
		m_render_simple_mesh.reset();

		if( m_lod_array.empty() )
			nthrow(_T("generate simple mesh failed: no LOD mesh."));

		nstatic_mesh_lod::ptr lod_mesh = m_lod_array[0];

		struct posd
		{
			double x,y,z;
			posd(const vector3& v)
				:x(v.x), y(v.y), z(v.z)
			{}
		};
		std::vector<posd>			vert_buffer;
		std::vector<unsigned int>	index_buffer;

		nmesh_vertex_data::ptr src_vert = lod_mesh->m_vert_data;
		vertex_stream* pos_stream = src_vert->get_stream(0);
		vector3* pos_vec = (vector3*)pos_stream->get_data();

		for(size_t i=0; i<pos_stream->get_num_vert(); i++)
		{
			vert_buffer.push_back( posd(pos_vec[i]) );
		}

		for(size_t i=0; i<lod_mesh->m_secton_array.size(); i++)
		{
			nmesh_section::ptr sec = lod_mesh->m_secton_array[i];

			for(size_t j=0; j<sec->m_index_buffer.get_index_count(); j++)
			{
				index_buffer.push_back( sec->m_index_buffer.data[j] );
			}
		}

		class NDecompCallback
			: public ConvexDecomposition::ConvexDecompInterface
		{
		public:			
			nsimple_mesh::ptr	m_mesh;

			NDecompCallback()
				: m_mesh(nNew nsimple_mesh)
			{
			}
			virtual void ConvexDecompResult(ConvexDecomposition::ConvexResult &result)
			{
				boost::scoped_ptr<ConvexDecomposition::FConvexResult>
					fresult( nNew ConvexDecomposition::FConvexResult(result) );
				
				merge_result(fresult.get());
			}
		private:
			void merge_result(const ConvexDecomposition::FConvexResult* result)
			{
				unsigned short base_index = m_mesh->vert_buffer.size();

				//-- copy vertices
				const vector3* vert = (const vector3*)result->mHullVertices;
				for(size_t i=0; i<result->mHullVcount; i++)
				{
					m_mesh->vert_buffer.push_back(vert[i]);
				}

				//-- copy indices
				size_t index_count = result->mHullTcount*3;
				unsigned int* indices = result->mHullIndices;

				for(size_t i=0; i<index_count; i++)
				{
					unsigned short index = base_index+indices[i];
					nASSERT(index < std::numeric_limits<unsigned short>::max() );
					m_mesh->index_buffer.push_back( index );
				}
			}
		};

		NDecompCallback callback;

		ConvexDecomposition::DecompDesc desc;
		desc.mVcount = vert_buffer.size();
		desc.mVertices = (const double*)(&vert_buffer[0]);
		desc.mTcount = index_buffer.size()/3;
		desc.mIndices = &index_buffer[0];
		desc.mCallback = &callback;
		desc.mSkinWidth = 0.5f;
		desc.mDepth = 3;

		unsigned int r = ConvexDecomposition::performConvexDecomposition(desc);

		if( r > 0 )
		{
			// todo : 如果生成的simple mesh比原始mesh更复杂的情况，则使用原始mesh			
			m_simple_mesh = callback.m_mesh;			

			//-- create render resouce
			nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();
			m_render_simple_mesh.reset( rres_mgr->alloc_simple_mesh() );
			m_render_simple_mesh->create( m_simple_mesh.get() );		

#if 1
			nLog_Info(_T("simple mesh generated for lod mesh, source vert count =%d, source face count=%d, simple mesh vert count =%d, face count=%d.\r\n"),
				lod_mesh->m_vert_data->get_num_vert(), lod_mesh->get_face_count(),
				m_simple_mesh->vert_buffer.size(), m_simple_mesh->index_buffer.size()/3);
#endif
		}// end of if
	}

	size_t nresource_static_mesh::get_num_importer()
	{
		return resource_importer_manager::instance()->get_num_static_mesh_importer();
	}
	
	nstring nresource_static_mesh::get_importer_type(size_t i)
	{
		return resource_importer_manager::instance()->get_static_mesh_importer_type(i);
	}

	nstring nresource_static_mesh::get_importer_desc(size_t i)
	{
		return resource_importer_manager::instance()->get_static_mesh_importer_desc(i);
	}

	size_t nresource_static_mesh::get_num_section(size_t i) const	
	{	
		return m_lod_array[i]->m_secton_array.size();
	}

	void nresource_static_mesh::get_lod_info(size_t i, int& num_vert, int& num_sec, int& num_tri) const
	{
		if( i>=m_lod_array.size() )
		{
			num_vert = num_sec = num_tri = -1;
		}
		else
		{
			nstatic_mesh_lod::ptr lod = m_lod_array[i];

			num_vert = lod->m_vert_data->get_num_vert();
			num_sec = lod->m_secton_array.size();
			num_tri = lod->get_face_count();
		}
	}
}//namespace nexus