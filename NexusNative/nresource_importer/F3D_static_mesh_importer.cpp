#include "stdafx.h"
#include "F3D_static_mesh_importer.h"
#include "../F3DFileHeader/StaticMeshFile.h"
#include "../F3DFileHeader/SubMesh.h"
#include "../nengine/engine_define.h"
#include "../nengine/mesh/nmesh_importer.h"
#include "F3D_util.h"

namespace nexus
{
	nDEFINE_CLASS(F3D_static_mesh_importer, nstatic_mesh_importer)
	nDEFINE_CLASS(F3D_scene_mesh_importer, nstatic_mesh_importer)

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

		nLog_Info(_T("importing [%s] ...\r\n"), full_path.c_str());
		F3D::StaticMeshHeader header;
		fread(&header, sizeof(header), 1, fp);

		nstatic_mesh_lod::ptr ret_lod = _load_f3d_static_mesh(header, fp);
		return ret_lod;
	}

	nstatic_mesh_lod::ptr F3D_scene_mesh_importer::import_from_file(const nstring& full_path)
	{
		FILE* fp = _tfopen(full_path.c_str(), _T("rb"));
		if( fp == NULL )
			nthrow2(_T("file open failed"), full_path.c_str());

		nLog_Info(_T("importing [%s] ...\r\n"), full_path.c_str());
		F3D::SceneMeshHeader header;
		fread(&header, sizeof(header), 1, fp);

		nstatic_mesh_lod::ptr ret_lod = _load_f3d_static_mesh(header, fp);
		return ret_lod;
	}
}// namespace nexus