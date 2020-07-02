
#pragma once
#include "VertexDefine.h"
#include "MathBaseDefine.h"
#include "TagNodeDefine.h"
#include "FileUtil.h"

namespace F3D
{
	const float FRM_STEP=1.0f/8; //妙
	//--关键帧动画中的一帧
	struct VertFrame
	{
		DWORD					name;
		float					time;//秒
		std::vector<Vert_PNT>	vertices;
		TagNodeFrame			tagNodes;

		void WriteFile(FILE *fp)
		{
			FWriteValue(fp,name);
			FWriteValue(fp,time);
			FWriteValVector(fp,vertices);
			tagNodes.WriteFile(fp);
		}
#ifdef VFS_READ
		void ReadFile(IFS *pFS,DWORD hFile)
		{
			FReadValue(pFS,hFile,name);
			FReadValue(pFS,hFile,time);
			FReadValVector(pFS,hFile,vertices);
			tagNodes.ReadFile(pFS,hFile);
		}
#else
		void ReadFile(FILE *fp)
		{
			FReadValue(fp,name);
			FReadValue(fp,time);
			FReadValVector(fp,vertices);
			tagNodes.ReadFile(fp);
		}
#endif

		VertFrame()
		{}
		VertFrame(const VertFrame& other)
		{
			name		=other.name;
			vertices	=other.vertices;
			tagNodes	=other.tagNodes;
		}
		const VertFrame& operator = (const VertFrame& other)
		{
			if(this == &other)
				return *this;
			name=other.name;
			vertices=other.vertices;
			tagNodes	=other.tagNodes;
			return *this;
		}
	};
	//---------------------
	const char* const FAM_MAGIC="FAM";

	struct FAMHeader
	{
		char	magic[4];
		int		ver;
		
		DWORD	numVert;
		
		DWORD	numMesh;
		DWORD	offsetMesh;
		DWORD	numMtl;
		DWORD	offsetMtl;
		DWORD	numFrame;
		DWORD	offsetFrame;

		Vector3 max,
				min;

		DWORD	animUV;
		DWORD	reserve[63];
	};
};