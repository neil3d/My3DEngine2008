#include "StdAfx.h"
#include "PSK_importer.h"
#include "F3D_util.h"

namespace nexus
{
	nDEFINE_CLASS(PSK_importer, nskeletal_mesh_importer)

	bool _inf_compare_weight(const bone_inf& b1, const bone_inf& b2)
	{
		return b1.weight > b2.weight;
	}

	PSK_importer::PSK_importer(void)
	{
	}

	PSK_importer::~PSK_importer(void)
	{
	}

	nskeletal_mesh::ptr PSK_importer::import_from_file(const nstring& full_path)
	{
		m_file_data.clear();
		if( !m_file_data.load_from_file(full_path) )
			nthrow2(_T("PSK file load failed"), full_path.c_str());

		//-- 为了加速查找，建立一个表：每个VPoint对应哪几个VRawBoneInfluence
		typedef std::multimap<size_t, size_t> INF_MAP;
		INF_MAP inf_map;	// key : VPoint index, value = VRawBoneInfluence index
		
		size_t num_pt = m_file_data.m_points.size();
		size_t num_inf = m_file_data.m_raw_weights.size();

		for(size_t p=0; p<num_pt; p++)
		{
			for(size_t w=0; w<num_inf; w++)
			{
				const VRawBoneInfluence& inf = m_file_data.m_raw_weights[w];
				if( inf.PointIndex == p)
					inf_map.insert(make_pair(p, w));
			}
		}

		//-- 建立vertex buffer
		size_t num_wedge = m_file_data.m_vertices.size();
		m_vertices.resize( num_wedge );

		for(size_t w=0; w<num_wedge; w++)
		{
			const VVertex& uVert = m_file_data.m_vertices[w];
			skin_vertex& nVert = m_vertices[w];
			size_t pt_index = uVert.PointIndex;

			// 计算顶点的基本几何信息
			nVert.pos = m_file_data.m_points[pt_index].Point;
			nVert.uv = vector2(uVert.U, uVert.V);

			// 查找当前顶点相关的骨骼
			if( inf_map.count(pt_index)>0 )
			{
				std::pair<INF_MAP::const_iterator, INF_MAP::const_iterator> range = inf_map.equal_range(pt_index);
				for(INF_MAP::const_iterator iter = range.first;
					iter != range.second;
					++iter)
				{
					size_t inf_index = iter->second;
					const VRawBoneInfluence& uInf = m_file_data.m_raw_weights[inf_index];

					bone_inf nInf = {uInf.BoneIndex, uInf.Weight};
					nVert.inf_array.push_back(nInf);
				}

			}
			else
			{
				nLog_Error(_T("PSK importer error, %s : have no bone\r\n"), full_path.c_str());
			}
		}

		nskeletal_mesh::ptr	ret_mesh(new nskeletal_mesh);

		//-- 创建mesh section
		nASSERT( m_file_data.m_mtl_array.size() > 0);

		size_t num_mtl = m_file_data.m_mtl_array.size();
		ret_mesh->m_secton_array.resize( num_mtl );
		ret_mesh->m_mtl_array.resize( num_mtl );
		
		for( size_t i=0; i< num_mtl; i++)
		{
			ret_mesh->m_secton_array[i].reset( new nmesh_section );
			ret_mesh->m_secton_array[i]->m_material_id = (int)i;
			ret_mesh->m_secton_array[i]->m_primitive_type = EDraw_TriangleList;
		}

		size_t num_tri = m_file_data.m_faces.size();

		for(size_t i=0; i<num_tri; i++)
		{
			const VTriangle& uTri = m_file_data.m_faces[i];
			nmesh_section::ptr sec = ret_mesh->m_secton_array[uTri.MatIndex];
			sec->m_index_buffer.data.push_back( uTri.WedgeIndex[0] );
			sec->m_index_buffer.data.push_back( uTri.WedgeIndex[1] );
			sec->m_index_buffer.data.push_back( uTri.WedgeIndex[2] );
		}

		//-- copy vertex buffer
		ret_mesh->m_vert_data.reset( new nmesh_vertex_data_skin);
		ret_mesh->m_vert_data->init(m_vertices.size(), true);

		vertex_stream* pos_stream = ret_mesh->m_vert_data->get_stream(0);
		vertex_stream* second_stream  = ret_mesh->m_vert_data->get_stream(1);

		size_t error_vert = 0;
		for(size_t i=0; i<m_vertices.size(); i++)
		{
			skin_vertex& in_vert = m_vertices[i];
			
			pos_stream->set_element(i, 0, _MAX_TO_D3D(in_vert.pos));
			second_stream->set_element(i, 0, _MAX_TO_D3D(in_vert.normal));
			second_stream->set_element(i, 1, _MAX_TO_D3D(in_vert.tangent));
			second_stream->set_element(i, 2, in_vert.uv);

			nASSERT( in_vert.inf_array.size() > 0);

			unsigned char bone_idx[SKIN_MAX_INFLUENCES] = {255, 255, 255, 255};
			float bone_weight[SKIN_MAX_INFLUENCES] = {0};

			if( in_vert.inf_array.size() > SKIN_MAX_INFLUENCES )
			{
				std::sort(in_vert.inf_array.begin(),
					in_vert.inf_array.end(), _inf_compare_weight);

				error_vert++;
			}

			size_t num_inf = std::min(in_vert.inf_array.size(), SKIN_MAX_INFLUENCES);
			float total_weight = 0;
			for(size_t n=0; n<num_inf; n++)
			{
				const bone_inf& inf = in_vert.inf_array[n];
				bone_idx[n] = inf.bone_index;
				bone_weight[n] = inf.weight;

				total_weight += inf.weight;
			}

			nASSERT(SKIN_MAX_INFLUENCES == 4);
			color4ub tidx(bone_idx[0], bone_idx[1], bone_idx[2], bone_idx[3]);
			vector4 twgt(bone_weight[0]/total_weight,
				bone_weight[1]/total_weight,
				bone_weight[2]/total_weight,
				bone_weight[3]/total_weight);

			second_stream->set_element(i, 3, tidx);
			second_stream->set_element(i, 4, twgt);
		}


		//-- 显示统计信息
		size_t total_face = 0;
		for( size_t i=0; i< num_mtl; i++)
		{
			nmesh_section::ptr sec = ret_mesh->m_secton_array[i];
			size_t sec_face = sec->m_index_buffer.get_index_count()/3;
			nASSERT( sec_face > 0);
			total_face += sec_face;
		}

		nLog_Info(_T("%s imported, num vertices = %d, num triangles = %d, num error vert = %d\r\n"),
			full_path.c_str(), m_vertices.size(), total_face, error_vert );
		return ret_mesh;
	}
}//namespace nexus