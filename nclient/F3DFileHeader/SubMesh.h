
#pragma once
#include "MathBaseDefine.h"
#include "FileUtil.h"

namespace F3D
{
	struct TriFace
	{
		WORD index[3];
	};
	typedef std::vector<TriFace>	FaceBuffer;

	class SubMesh
	{
	public:
		SubMesh()	{}
		virtual ~SubMesh()	{}
	public:
		//--
		WORD	baseIndex;
		int		mtlID;
		WORD	numVert;

		Vector3 max;
		Vector3 min;

		FaceBuffer	baseFaces;

		virtual void WriteFile(FILE *fp)
		{
			FWriteValue(fp,baseIndex);
			FWriteValue(fp,mtlID);
			FWriteValue(fp,numVert);
			FWriteValue(fp,max);
			FWriteValue(fp,min);
			FWriteValVector(fp,baseFaces);
		}

		virtual void ReadFile(FILE *fp)
		{
			FReadValue(fp,baseIndex);
			FReadValue(fp,mtlID);
			FReadValue(fp,numVert);
			FReadValue(fp,max);
			FReadValue(fp,min);
			FReadValVector(fp,baseFaces);
		}

	public:
		SubMesh(const SubMesh& other)
		{	CopySubMesh(&other);	}

		const SubMesh& operator = (const SubMesh& other)
		{
			if(this==&other)
				return *this;
			CopySubMesh(&other);
			return *this;
		}

		void CopySubMesh(const SubMesh* pOther)
		{
			baseIndex	=	pOther->baseIndex;
			mtlID		=	pOther->mtlID;
			numVert		=	pOther->numVert;
			max			=	pOther->max;
			min			=	pOther->min;
			baseFaces	=	pOther->baseFaces;
		}
	};

	class LodSubMesh : public SubMesh
	{
	public:
		LodSubMesh()	{}
		virtual ~LodSubMesh()	{}
	public:
		enum LOD_SUBMESH_CONST
		{	MAX_NUM_LOD=4};

		int			numLod;	//lod0 save in base class
		FaceBuffer	lodFaces[MAX_NUM_LOD];

	public:
		virtual void WriteFile(FILE *fp)
		{
			SubMesh::WriteFile(fp);
			FWriteValue(fp,numLod);
			for(int i=0;i<MAX_NUM_LOD;i++)
				FWriteValVector(fp,lodFaces[i]);
		}

		virtual void ReadFile(FILE *fp)
		{
			SubMesh::ReadFile(fp);
			FReadValue(fp,numLod);
			for(int i=0;i<MAX_NUM_LOD;i++)
				FReadValVector(fp,lodFaces[i]);
		}

	public:
		LodSubMesh(const LodSubMesh& other)
		{	CopyLodSubMesh(&other);	}
		const LodSubMesh& operator = (const LodSubMesh& other)
		{
			if(this==&other)
				return *this;
			CopyLodSubMesh(&other);
			return *this;
		}

	protected:
		void CopyLodSubMesh(const LodSubMesh* pOther)
		{
			numLod=pOther->numLod;
			for(int i=0;i<MAX_NUM_LOD;i++)
				lodFaces[i]=pOther->lodFaces[i];
			SubMesh::CopySubMesh(pOther);
		}
	};
}//namespace F3D