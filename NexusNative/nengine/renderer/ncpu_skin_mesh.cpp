#include "StdAfx.h"
#include "ncpu_skin_mesh.h"

namespace nexus
{
	nrender_cpu_skin_mesh::nrender_cpu_skin_mesh(void)
	{
	}

	nrender_cpu_skin_mesh::~nrender_cpu_skin_mesh(void)
	{
	}

	void nrender_cpu_skin_mesh::create(nskeletal_mesh::ptr mesh_ptr)
	{
		m_source_mesh = mesh_ptr;

		//--
		nmesh_vertex_data_skin::ptr source_vert = m_source_mesh->m_vert_data;

		m_skined_vert_data.reset( nNew nmesh_vertex_data_skin );
		m_skined_vert_data->init(source_vert->get_num_vert(), source_vert->has_tangent());

		vertex_stream* pos_stream = m_skined_vert_data->get_stream(0);
		vertex_stream* sec_stream = m_skined_vert_data->get_stream(1);

		memcpy(pos_stream->get_data(), source_vert->get_stream(0)->get_data(), pos_stream->get_data_size());
		memcpy(sec_stream->get_data(), source_vert->get_stream(1)->get_data(), sec_stream->get_data_size());
	}

	void nrender_cpu_skin_mesh::update_skin(const matrix_palette& mat_pal)
	{
		if(mat_pal.empty())
			return;

		//-- update local skin vertex data
		size_t num_vert = m_skined_vert_data->get_num_vert();

		vertex_stream* pos_stream = m_skined_vert_data->get_stream(0);
		vertex_stream* sec_stream = m_skined_vert_data->get_stream(1);

		struct skin_vert
		{
			vector3			normal;
			vector3			tangent;
			vector2			uv;
			unsigned char	bone_index[4];
			float			bone_weight[4];
		};

		size_t skin_vert_t = sizeof(skin_vert);
		nASSERT(skin_vert_t == sec_stream->get_vertex_size());

		vector3* dst_pos_vert = (vector3*)pos_stream->get_data();
		//skin_vert* dst_sk_vert = (skin_vert*)sec_stream->get_data();

		nmesh_vertex_data_skin::ptr source_vert = m_source_mesh->m_vert_data;
		pos_stream = source_vert->get_stream(0);
		sec_stream = source_vert->get_stream(1);

		vector3* src_pos_vert = (vector3*)pos_stream->get_data();
		skin_vert* src_sk_vert = (skin_vert*)sec_stream->get_data();

		for(size_t i=0; i<num_vert; i++)
		{
			dst_pos_vert[i] = vector3(0, 0, 0);
			//dst_sk_vert[i].uv = src_sk_vert[i].uv;
			for(int b=0; b<4; b++)
			{
				if( src_sk_vert[i].bone_index[b] == 255)
					break;

				const matrix44& bone_mat = mat_pal[ src_sk_vert[i].bone_index[b] ];				
				
				dst_pos_vert[i] += (src_pos_vert[i]*bone_mat)*src_sk_vert[i].bone_weight[b];
				//dst_pos_vert[i] = src_pos_vert[i];
			}
		}
	}
}//namespace nexus