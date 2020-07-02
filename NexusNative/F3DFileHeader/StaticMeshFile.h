
#pragma once
#include "MathBaseDefine.h"
#include "VertexDefine.h"

namespace F3D
{
	const char* const StaticMesh_Magic="SMM";
/*--Simple Mesh 文件结构-------------------------
	[StaticMeshHeader]
	[Vert] * header.numVert;
	[baseIndex]-WORD	--|
	[numFace]-DWORD		  |
	[mtlID]-DWORD		  |} *header.numMesh
	[TriFace]*numFace	--|
	[MtlDesc] * header.numMtl;
	*/
	struct StaticMeshHeader
	{
		char	magic[4];
		int		ver;
		EVertType	vertType;
		DWORD	numVert;
		DWORD	offsetVert;
		DWORD	numMesh;
		DWORD	offsetMesh;
		DWORD	numMtl;
		DWORD	offsetMtl;
		DWORD	numTagNode;
		DWORD	offsetTagNode;
		Vector3	min;
		Vector3	max;

		DWORD	reserve[64];
	};

	const char* const SceneMesh_Magic="SCN";

	struct SceneMeshHeader : public StaticMeshHeader
	{
		DWORD	boxTreeOffset;
	};
}//namespace F3D