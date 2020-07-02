#include "StdAfx.h"
#include "F3D_vertex_anim_importer.h"
#include "../F3DFileHeader/AnimMeshFile.h"
#include "../F3DFileHeader/SubMesh.h"
#include "../F3DFileHeader/ExportBaseDef.h"
#include "F3D_util.h"
#include "nv_meshmender/NVMeshMender.h"


namespace nexus
{
	void tangent_basis(matrix33& basis, const vector3& v0, const vector3& v1, const vector3& v2, 
		const vector2& t0, const vector2& t1, const vector2& t2, const vector3& n)
	{
		vector3 cp;
		vector3 e0(v1.x - v0.x, t1.x - t0.x, t1.y - t0.y);
		vector3 e1(v2.x - v0.x, t2.x - t0.x, t2.y - t0.y);

		cp = vec_cross(e0,e1);
		if ( fabs(cp.x) > nEPS)
		{
			basis._11 = -cp.y / cp.x;        
			basis._12 = -cp.z / cp.x;
		}

		e0.x = v1.y - v0.y;
		e1.x = v2.y - v0.y;

		cp = vec_cross(e0,e1);
		if ( fabs(cp.x) > nEPS)
		{
			basis._21 = -cp.y / cp.x;        
			basis._22 = -cp.z / cp.x;
		}

		e0.x = v1.z - v0.z;
		e1.x = v2.z - v0.z;

		cp = vec_cross(e0,e1);
		if ( fabs(cp.x) > nEPS)
		{
			basis._31 = -cp.y / cp.x;        
			basis._32 = -cp.z / cp.x;
		}

		// tangent...
		float oonorm = 1.0f / sqrtf(basis._11 * basis._11 + basis._21 * basis._21 + basis._31 * basis._31);
		basis._11 *= oonorm;
		basis._21 *= oonorm;
		basis._31 *= oonorm;

		// binormal...
		oonorm = 1.0f / sqrtf(basis._12 * basis._12 + basis._22 * basis._22 + basis._32 * basis._32);
		basis._12 *= oonorm;
		basis._22 *= oonorm;
		basis._32 *= oonorm;

		// normal...
		// compute the cross product TxB
		basis._13 = basis._21*basis._32 - basis._31*basis._22;
		basis._23 = basis._31*basis._12 - basis._11*basis._32;
		basis._33 = basis._11*basis._22 - basis._21*basis._12;

		oonorm = 1.0f / sqrtf(basis._13 * basis._13 + basis._23 * basis._23 + basis._33 * basis._33);
		basis._13 *= oonorm;
		basis._23 *= oonorm;
		basis._33 *= oonorm;

		// Gram-Schmidt orthogonalization process for B
		// compute the cross product B=NxT to obtain 
		// an orthogonal basis
		basis._12 = basis._23*basis._31 - basis._33*basis._21;
		basis._22 = basis._33*basis._11 - basis._13*basis._31;
		basis._32 = basis._13*basis._21 - basis._23*basis._11;

		if (basis._13 * n.x + basis._23 * n.y + basis._33 * n.z < 0.0f)
		{
			basis._13 = -basis._13;
			basis._23 = -basis._23;
			basis._33 = -basis._33;
		}	
	}


	nDEFINE_CLASS(F3D_vertex_anim_importer, nanim_mesh_importer)

	F3D_vertex_anim_importer::F3D_vertex_anim_importer(void)
	{
	}

	F3D_vertex_anim_importer::~F3D_vertex_anim_importer(void)
	{
	}

	nexus::vector3 _copy_f3d_vector(F3D::Vector3& v)
	{
		return nexus::vector3(v.x, v.y, v.z);
	}

	nexus::vector2 _copy_f3d_vector(F3D::Vector2& v)
	{
		return nexus::vector2(v.x, v.y);
	}

	static void _append_f3d_mesh_frame(nanim_mesh_sequence::ptr seq, shared_ptr<F3D::VertFrame> fframe, bool animUV)
	{
		seq->m_frame_time.push_back(fframe->time);		
		
		//-- process vertices
		std::vector<F3D::Vert_PNT>& fvb = fframe->vertices;

		// --对于新建的sequence
		if( seq->m_tracks.size() <= 0)
			seq->m_tracks.resize(fvb.size());

		float fm = std::numeric_limits<float>::max();
		vector3 vec_m(fm, fm, fm);

		AABBox box;
		box.m_min = vec_m;
		box.m_max = -vec_m;

		for(size_t i=0; i<fvb.size(); i++)
		{
			F3D::Vert_PNT& vt = fvb[i];
			nanim_mesh_track& track = seq->m_tracks[i];

			track.m_pos_keys.push_back( _copy_f3d_vector(vt.pos) );
			track.m_normal_keys.push_back( _copy_f3d_vector(vt.normal) );

			if( animUV )
				track.m_uv_keys.push_back( _copy_f3d_vector(vt.uv) );
			else
			{
				if( track.m_uv_keys.empty() )
					track.m_uv_keys.push_back( _copy_f3d_vector(vt.uv) );
			}
			

			//--
			const F3D::Vector3& pos = vt.pos;

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

		
		box_sphere_bounds bounds;
		bounds.origin = (box.m_max+box.m_min)*0.5f;
		bounds.box_extent = (box.m_max-box.m_min)*0.5f;
		bounds.sphere_radius = vec_distance(bounds.origin, box.m_max);
		seq->m_frame_bounds.push_back(bounds);
	}

	static void _merge_f3d_mesh_frame(shared_ptr<F3D::VertFrame> fframe, anim_mesh_seq_map& seq_map, bool animUV)
	{
		anim_mesh_seq_map::iterator iter = seq_map.find(fframe->name);
		if( iter != seq_map.end() )
		{
			nanim_mesh_sequence::ptr seq = iter->second;
			_append_f3d_mesh_frame(seq, fframe, animUV);			
		}
		else
		{
			//-- create a new sequence
			nanim_mesh_sequence::ptr new_seq( nNew nanim_mesh_sequence );
			new_seq->m_name = conv_string( F3D::FourCC2Str(fframe->name) );
			
			_append_f3d_mesh_frame(new_seq, fframe, animUV);			

			seq_map.insert(make_pair(fframe->name, new_seq));
		}
	}

	nanim_mesh::ptr F3D_vertex_anim_importer::import_from_file(const nstring& full_path)
	{
		FILE* fp = _tfopen(full_path.c_str(), _T("rb"));
		if( fp == NULL )
			nthrow2(_T("file open failed"), full_path.c_str());

		nanim_mesh::ptr ret_mesh( nNew nanim_mesh );
		F3D::FAMHeader header;
		fread(&header,sizeof(header),1,fp);

		//-- load frames
		shared_ptr<F3D::VertFrame> aFrame;

		fseek(fp,header.offsetFrame,SEEK_SET);
		for(DWORD i=0; i< header.numFrame; i++)
		{
			shared_ptr<F3D::VertFrame> fframe( nNew F3D::VertFrame );
			if( !aFrame )
				aFrame = fframe;

			fframe->ReadFile(fp);
			nASSERT(fframe->vertices.size() == header.numVert);

			//-- copy frame
			_merge_f3d_mesh_frame(fframe, m_seq_map, header.animUV!=0);
		}

		//-- load mesh sections
		size_t total_face = 0;
		fseek(fp, header.offsetMesh, SEEK_SET);
		for(DWORD i=0; i<header.numMesh; i++)
		{
			shared_ptr<F3D::LodSubMesh> fmesh( nNew F3D::LodSubMesh );
			fmesh->ReadFile(fp);

			nmesh_section::ptr msec = convert_f3d_sub_mesh(fmesh);
			ret_mesh->m_secton_array.push_back( msec );
			total_face += msec->m_index_buffer.get_index_count()/3;
		}

		
		//-- copy data
		size_t total_frame = 0;
		for(anim_mesh_seq_map::iterator iter = m_seq_map.begin();
			iter != m_seq_map.end(); ++iter)
		{
			total_frame += iter->second->get_num_frame();
			ret_mesh->m_sequence_array.push_back(iter->second);
		}

		clear();

		//-- generate tangent
		if( aFrame )
		{
			//--Step 1 
			//	创建mesh minder的vertex buffer
			size_t num_vert = aFrame->vertices.size();
			std::vector<MeshMender::Vertex>	md_vb;
			md_vb.resize(num_vert);
			

			std::vector<F3D::Vert_PNT>& fvb = aFrame->vertices;
			for(size_t i=0; i<num_vert; i++)
			{
				MeshMender::Vertex& md_vert = md_vb[i];
				F3D::Vert_PNT& vt = fvb[i];
				
				memcpy(&md_vert.pos, &vt.pos, sizeof(vector3));
				memcpy(&md_vert.normal, &vt.normal, sizeof(vector3));
				memcpy(&md_vert.s, &vt.uv, sizeof(vector2));
			}

			for(size_t i=0; i<ret_mesh->m_secton_array.size(); i++)
			{
				nmesh_section::ptr sec = ret_mesh->m_secton_array[i];

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
					MeshMender::CALCULATE_NORMALS,
					MeshMender::DONT_RESPECT_SPLITS,
					MeshMender::DONT_FIX_CYLINDRICAL);
			}

			//-- 把第一帧的计算结果应用到所有帧
			for(size_t s=0; s<ret_mesh->m_sequence_array.size(); s++)
			{
				nanim_mesh_sequence::ptr seq = ret_mesh->m_sequence_array[s];

				for(size_t t=0; t<seq->m_tracks.size(); t++)
				{
					MeshMender::Vertex& md_vert = md_vb[t];
					seq->m_tracks[t].m_tangent_keys.push_back(vector3(md_vert.tangent.x, md_vert.tangent.y, md_vert.tangent.z));
				}
			}//end of for
		}

		//-- log report
		nLog_Info(_T("%s imported, num sequence=%d, num frames=%d, total face=%d, sequence list:"),
			full_path.c_str(), ret_mesh->m_sequence_array.size(), total_frame, total_face);
		for(size_t i=0; i<ret_mesh->m_sequence_array.size(); i++)
		{
			nLog_Append(ret_mesh->m_sequence_array[i]->m_name.c_str());
			nLog_Append(_T(", "));
		}
		nLog_Append(_T("\r\n"));

		ret_mesh->m_mtl_array.resize(header.numMtl);

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

		//--
		fclose(fp);

		return ret_mesh;
	}
}//namespace nexus