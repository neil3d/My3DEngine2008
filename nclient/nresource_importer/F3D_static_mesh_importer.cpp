#include "StdAfx.h"
#include "F3D_static_mesh_importer.h"
#include "../F3DFileHeader/StaticMeshFile.h"
#include "../F3DFileHeader/SubMesh.h"
#include "../nengine/engine_define.h"
#include "../nengine/mesh/nmesh_importer.h"
#include "F3D_util.h"

namespace nexus
{
	nDEFINE_CLASS(F3D_static_mesh_importer, nstatic_mesh_importer)

	F3D_static_mesh_importer::F3D_static_mesh_importer(void)
	{
	}

	F3D_static_mesh_importer::~F3D_static_mesh_importer(void)
	{
	}

	nstatic_mesh_lod::ptr F3D_static_mesh_importer::import_from_file(const nstring& full_path)
	{
		FILE* fp = _tfopen(full_path.c_str(), _T("rb"));
		if( fp == NULL )
			nthrow2(_T("file open failed"), full_path.c_str());

		nstatic_mesh_lod::ptr ret_lod( nNew nstatic_mesh_lod );		

		F3D::StaticMeshHeader header;
		fread(&header, sizeof(header), 1, fp);

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


		fclose(fp);

		//--
		ret_lod->m_bounding_box = compute_bounding_box();

		//--
		generate_tangent(ret_lod);
		ret_lod->m_vert_data = create_vertex_data();

		nLog_Info(_T("%s imported, total vert=%d, total section = %d, total face = %d\r\n"), 
			full_path.c_str(), m_vertex_buffer.size(), ret_lod->m_secton_array.size(), total_face);

		return ret_lod;
	}
}// namespace nexus