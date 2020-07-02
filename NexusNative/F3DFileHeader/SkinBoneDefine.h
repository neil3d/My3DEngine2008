

#pragma once
#include "MathBaseDefine.h"
#include "FileUtil.h"
#include "TagNodeDefine.h"

namespace F3D
{
	const char* const FSKEL_MAGIC="SKEL";
	struct FSkelHeader
	{
		BYTE	magic[5];
		BYTE	ver;
		DWORD	structOffset;
		DWORD	numFrame;
		DWORD	frameOffset;
		DWORD	reserve[64];
	};
	const char* const FSKIN_MAGIC="SKIN";
	struct tagFSkinHeader
	{
		BYTE	magic[5];
		BYTE	ver;
		DWORD	numVert;
		DWORD	vertOffset;
		DWORD	numSubMesh;
		DWORD	subMeshOffset;
		DWORD	numMtl;
		DWORD	mtlOffset;
		Vector3	max,
				min;
		DWORD	reserve[64];
	};

	//--skin---------------------------------------------------------------
	struct tagBoneInf
	{
		int		boneId;
		float	weight;
		Vector3	offset;
	};
	class SkinVert
	{
	public:
		Vector3				pos;
		Vector3				normal;
		Vector2				uv;
		vector<tagBoneInf>	boneInfs;

	public:
		SkinVert()	{}
		~SkinVert()	{}
		SkinVert(const SkinVert& other)	
		{	Copy(other); }
		const SkinVert& operator = (const SkinVert& other)
		{
			if(this==&other)
				return *this;
			Copy(other);
			return *this;
		}

		void WriteFile(FILE *fp)
		{
			FWriteValue(fp,pos);
			FWriteValue(fp,normal);
			FWriteValue(fp,uv);
			FWriteValVector(fp,boneInfs);
		}
#ifdef VFS_READ
		void ReadFile(IFS *pFS,DWORD hFile)
		{
			FReadValue(pFS,hFile,pos);
			FReadValue(pFS,hFile,normal);
			FReadValue(pFS,hFile,uv);
			FReadValVector(pFS,hFile,boneInfs);
		}
#endif

	private:
		void Copy(const SkinVert& other)
		{
			pos=other.pos;
			normal=other.normal;
			uv=other.uv;
			boneInfs=other.boneInfs;
		}
	};
	//--bone/skeleton------------------------------------------------------
	class Bone
	{
	public:
		Bone()	{}
		~Bone()	{}
		Bone(const Bone& other)
		{
			Copy(other);
		}
		const Bone& operator = (const Bone& other)
		{
			if(this==&other)
				return *this;
			Copy(other);
			return *this;
		}

		void WriteFile(FILE *fp)
		{
			FWriteValue(fp,m_id);
			FWriteValue(fp,m_parentId);
			FWriteValVector(fp,m_children);
			FWriteValue(fp,m_trans);
			FWriteValue(fp,m_rotat);
			FWriteValue(fp,m_bMirrored);
		}
#ifdef VFS_READ
		void ReadFile(IFS *pFS,DWORD hFile)
		{
			FReadValue(pFS,hFile,m_id);
			FReadValue(pFS,hFile,m_parentId);
			FReadValVector(pFS,hFile,m_children);
			FReadValue(pFS,hFile,m_trans);
			FReadValue(pFS,hFile,m_rotat);
			FReadValue(pFS,hFile,m_bMirrored);
		}
#else
		void ReadFile(FILE *fp)
		{
			FReadValue(fp,m_id);
			FReadValue(fp,m_parentId);
			FReadValVector(fp,m_children);
			FReadValue(fp,m_trans);
			FReadValue(fp,m_rotat);
			FReadValue(fp,m_bMirrored);
		}
#endif//#ifdef VFS_READ
	public:
		int				m_id;
		int				m_parentId;		//父节点在Skeleton中的index
		vector<int>		m_children;		//所有子节点
		Vector3			m_trans;		//平移
		Quaternion		m_rotat;		//旋转
		bool			m_bMirrored;
	private:
		void Copy(const Bone& other)
		{
			m_id		=	other.m_id;
			m_parentId	=	other.m_parentId;
			m_children	=	other.m_children;
			m_trans		=	other.m_trans;
			m_rotat		=	other.m_rotat;
			m_bMirrored	=	other.m_bMirrored;
		}
	};

	class SkeletonStruct
	{
	public:
		SkeletonStruct()	{}
		~SkeletonStruct()	{}
		SkeletonStruct(const SkeletonStruct& other)
		{
			m_boneNames=other.m_boneNames;
			m_rootBones=other.m_rootBones;
		}
		const SkeletonStruct& operator = (const SkeletonStruct& other)
		{
			if(this==&other)
				return *this;
			m_boneNames=other.m_boneNames;
			m_rootBones=other.m_rootBones;
			return *this;
		}
		
		void WriteFile(FILE *fp)
		{
			int numBone=m_boneNames.size();
			FWriteValue(fp,numBone);
			for(int i=0;i<numBone;i++)
			{
				FWriteString(fp,m_boneNames[i].c_str());
			}
			FWriteValVector(fp,m_rootBones);
		}
#ifdef VFS_READ
		void ReadFile(IFS *pFS,DWORD hFile)
		{
			int numBone=0;
			FReadValue(pFS,hFile,numBone);
			m_boneNames.clear();
			for(int i=0;i<numBone;i++)
			{
				string tmp;
				FReadString(pFS,hFile,tmp);
				m_boneNames.push_back(tmp);
			}
			FReadValVector(pFS,hFile,m_rootBones);
		}
#else
		void ReadFile(FILE *fp)
		{
			int numBone=0;
			FReadValue(fp,numBone);
			m_boneNames.clear();
			for(int i=0;i<numBone;i++)
			{
				string tmp;
				FReadString(fp,tmp);
				m_boneNames.push_back(tmp);
			}
			FReadValVector(fp,m_rootBones);
		}
#endif//#ifdef VFS_READ
	public:
		vector<string>	m_boneNames;
		vector<int>		m_rootBones;
	};
	/**	\class SkeletonFrame
		\brief 骨骼动画的一个帧
	*/
	class SkeletonFrame
	{
	public:
		SkeletonFrame()		{}
		virtual ~SkeletonFrame()	{}

		UINT GetNumBone() const	{	return m_boneArray.size();}

		void WriteFile(FILE *fp)
		{
			FWriteValue(fp,m_name);
			unsigned int numBone=m_boneArray.size();
			FWriteValue(fp,numBone);
			for(unsigned int i=0;i<numBone;i++)
				m_boneArray[i].WriteFile(fp);
			FWriteValue(fp,m_time);
			m_tagNodes.WriteFile(fp);
		}
#ifdef VFS_READ
		void ReadFile(IFS *pFS,DWORD hFile)
		{
			FReadValue(pFS,hFile,m_name);
			unsigned int numBone=0;
			FReadValue(pFS,hFile,numBone);
			m_boneArray.resize(numBone);
			for(int i=0;i<(int)numBone;i++)
			{
				Bone& bn=m_boneArray[i];
				bn.ReadFile(pFS,hFile);
			}
			FReadValue(pFS,hFile,m_time);
			m_tagNodes.ReadFile(pFS,hFile);
		}
#else
		void ReadFile(FILE *fp)
		{
			FReadValue(fp,m_name);
			unsigned int numBone=0;
			FReadValue(fp,numBone);
			m_boneArray.resize(numBone);
			for(unsigned int i=0;i<numBone;i++)
			{
				Bone& bn=m_boneArray[i];
				bn.ReadFile(fp);
			}
			FReadValue(fp,m_time);
			m_tagNodes.ReadFile(fp);
		}
#endif//#ifdef VFS_READ
	public:
		DWORD			m_name;
		vector<Bone>	m_boneArray;
		float			m_time;
		TagNodeFrame	m_tagNodes;
	private:
		SkeletonFrame(const SkeletonFrame& other);
		const SkeletonFrame& operator = (const SkeletonFrame& other);
	};
}//namespace F3D