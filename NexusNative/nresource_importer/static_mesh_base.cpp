#include "StdAfx.h"
#include <sstream>
#include "static_mesh_base.h"
#include "nv_meshmender/NVMeshMender.h"
#include "../F3DFileHeader/StaticMeshFile.h"
#include "../F3DFileHeader/SubMesh.h"
#include "../F3DFileHeader/MtlDesc.h"
#include "../nengine/engine_define.h"
#include "../nengine/mesh/nmesh_importer.h"
#include "F3D_util.h"

namespace nexus
{
	static_mesh_base::static_mesh_base(void)
	{
	}

	static_mesh_base::~static_mesh_base(void)
	{
	}

	box_sphere_bounds static_mesh_base::compute_bounding_box()
	{
		float fm = std::numeric_limits<float>::max();
		vector3 vec_m(fm, fm, fm);

		AABBox box( vec_m, -vec_m );

		for(size_t i=0; i<m_vertex_buffer.size(); i++)
		{
			const vector3& pos = m_vertex_buffer[i].pos;

			if(pos.x > box.m_max.x)
				box.m_max.x = pos.x;
			if(pos.y > box.m_max.y)
				box.m_max.y = pos.y;
			if(pos.z > box.m_max.z)
				box.m_max.z = pos.z;

			if(pos.x < box.m_min.x)
				box.m_min.x = pos.x;
			if(pos.y < box.m_min.y)
				box.m_min.y = pos.y;
			if(pos.z < box.m_min.z)
				box.m_min.z = pos.z;
		}

		box_sphere_bounds ret;
		ret.origin = (box.m_max+box.m_min)*0.5f;
		ret.box_extent = (box.m_max-box.m_min)*0.5f;
		ret.sphere_radius = vec_length(ret.box_extent);

		return ret;
	}

	static bool error_normal(const vector3& v)
	{
		float f = vec_length(v);
		if( f*f < 0)
			return true;

		if( fabs(f-1) > 1e-5 )
			return true;

		return false;
	}


	void static_mesh_base::generate_tangent(nstatic_mesh_lod::ptr lod)
	{
		if(m_vertex_buffer.empty())
			return;
		if( !lod )
			return;
		if( lod->m_secton_array.empty() )
			return;

		//-- Step 1
		//	 fill up the generic structures that meshmender can work on.
		//	 fill up the vertex data
		std::vector<MeshMender::Vertex>	md_vb;
		md_vb.resize(m_vertex_buffer.size());

		for(size_t i=0; i<m_vertex_buffer.size(); ++i)
		{
			MeshMender::Vertex& md_vert = md_vb[i];
			const vertex& v = m_vertex_buffer[i];

			memcpy(&md_vert.pos, &v.pos, sizeof(vector3));
			memcpy(&md_vert.normal, &v.normal, sizeof(vector3));
			md_vert.s = v.tex.x;
			md_vert.t = v.tex.y;
		}

		for(size_t i=0; i<lod->m_secton_array.size(); i++)
		{
			nmesh_section::ptr sec = lod->m_secton_array[i];

			//-- Step 2
			//	fill up the index buffer
			std::vector<unsigned int> md_indices;
			md_indices.resize(sec->m_index_buffer.get_index_count());
			for(size_t j=0; j<md_indices.size(); j++)
				md_indices[j] = sec->m_index_buffer.data[j];

			//STEP 3:
			//		perform the actual mend
			std::vector< unsigned int > mappingNewToOldVert;

			MeshMender theMender;
			bool success = theMender.Mend(md_vb,
				md_indices, mappingNewToOldVert,
				0.5f, 0.5f, 0.5f, 1.0f,
				MeshMender::DONT_CALCULATE_NORMALS,	// note: 使用“CALCULATE_NORMALS”会造成per-pixel lighting的bug
				MeshMender::DONT_RESPECT_SPLITS,
				MeshMender::DONT_FIX_CYLINDRICAL);

			//STEP 4:
			//		update the mesh with the mended data
			/*if( success )
			{
				nASSERT(md_indices.size() == sec->m_index_buffer.get_index_count());
				for(size_t j=0; j<md_indices.size(); j++)
					sec->m_index_buffer.data[j] = md_indices[j];
			}*/
		}

		// fill in our verts
		for(size_t i=0; i<m_vertex_buffer.size(); ++i)
		{
			const MeshMender::Vertex& md_vert = md_vb[i];
			vertex& v = m_vertex_buffer[i];

			memcpy(&v.pos,		&md_vert.pos, sizeof(vector3));
			memcpy(&v.normal,	&md_vert.normal,sizeof(vector3));
			memcpy(&v.tangent,	&md_vert.tangent, sizeof(vector3));

			// test data
			if( error_normal(v.normal) 
				|| error_normal(v.tangent))
			{
				v.normal= vector3(0,1,0);			
				v.tangent = vector3(1,0,0);
			}

			v.tex.x = md_vert.s;
			v.tex.y = md_vert.t;
		}
	}

	nmesh_vertex_data::ptr static_mesh_base::create_vertex_data()
	{
		// pos, normal, tangent, uv
		shared_ptr<nmesh_vertex_data_common> vert_data(nNew nmesh_vertex_data_common);

		vert_data->init(m_vertex_buffer.size(), 2, 1);

		vertex_stream* pos_stream = vert_data->get_stream(0);
		vertex_stream* second_stream = vert_data->get_stream(1);

		for(size_t i=0; i<m_vertex_buffer.size(); i++)
		{
			vertex& vt = m_vertex_buffer[i];

			pos_stream->set_element(i, 0, vt.pos);
			second_stream->set_element(i, 0, vt.normal);
			second_stream->set_element(i, 1, vt.tangent);
			second_stream->set_element(i, 2, vt.tex);
		}

		return dynamic_pointer_cast<nmesh_vertex_data>(vert_data);
	}

	nstatic_mesh_lod::ptr static_mesh_base::_load_f3d_static_mesh(const F3D::StaticMeshHeader& header, FILE* fp)
	{
		nstatic_mesh_lod::ptr ret_lod( nNew nstatic_mesh_lod );		

		//-- load vertex buffer
		fseek(fp, header.offsetVert, SEEK_SET);

		if( header.vertType == F3D::EVType_PNT)
		{
			scoped_array<F3D::Vert_PNT> f3d_vb( nNew F3D::Vert_PNT[header.numVert] );
			fread(f3d_vb.get(), sizeof(F3D::Vert_PNT), header.numVert, fp);

			//-- copy
			m_vertex_buffer.resize(header.numVert);
			for(DWORD i=0; i<header.numVert; i++)
			{
				vertex& v = m_vertex_buffer[i];
				F3D::Vert_PNT& fv = f3d_vb[i];
				memcpy(&v.pos,		&fv.pos, sizeof(vector3));
				memcpy(&v.normal,	&fv.normal, sizeof(vector3));
				memcpy(&v.tex,		&fv.uv, sizeof(vector2));
			}
		}
		else
		{
			nASSERT( 0 && "unknown F3D Vertex Type");
		}

		//-- load mesh section
		fseek(fp, header.offsetMesh, SEEK_SET);
		size_t total_face = 0;
		for( DWORD i=0; i<header.numMesh; i++)
		{
			shared_ptr<F3D::LodSubMesh> fm(nNew F3D::LodSubMesh);
			fm->ReadFile(fp);					

			ret_lod->m_secton_array.push_back( convert_f3d_sub_mesh(fm) );
			total_face += fm->baseFaces.size();
		}

		//--
		ret_lod->m_bounding_box = compute_bounding_box();
		//--
		nLog_Info(_T("\t\ttotal vert=%d, total section = %d, total face = %d\r\n"), 
			m_vertex_buffer.size(), ret_lod->m_secton_array.size(), total_face);


		//--
		generate_tangent(ret_lod);
		ret_lod->m_vert_data = create_vertex_data();

		//-- out material
		fseek(fp, header.offsetMtl, SEEK_SET);
		for(DWORD i=0; i<header.numMtl; i++)
		{
			F3D::tagMtlDesc mtl;
			int r = fread(&mtl, sizeof(F3D::tagMtlDesc), 1, fp);
			if( r > 0 )
			{
				std::wostringstream os;

				os << _T("\t\tDiffuse : ") << mtl.m_map[F3D::EMap_Diffuse] << std::endl;
				nLog_Info( os.str().c_str() );
			}
		}

		fclose(fp);
		return ret_lod;
	}
}//namespace nexus