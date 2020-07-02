#include "StdAfx.h"
#include "static_mesh_base.h"
#include "nv_meshmender/NVMeshMender.h"

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

}//namespace nexus