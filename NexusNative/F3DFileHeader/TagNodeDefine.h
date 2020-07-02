
#pragma once
#include "MathBaseDefine.h"
#include "FileUtil.h"

namespace F3D
{
	/**	标志节点,用来存储一个Node的变换信息,相当于没有父子关系的Bone节点*/
	struct tagTagNode
	{
		DWORD			name;		//four cc name
		Vector3			trans;		//平移
		Quaternion		rotat;		//旋转
		bool			bMirrored;	
	};

	class TagNodeFrame
	{
	public:
		vector<tagTagNode>	nodes;
	
	public:
		TagNodeFrame()	{}
		~TagNodeFrame()	{}

		int GetNumNodes() const	{	return nodes.size();}

		void WriteFile(FILE *fp)
		{
			FWriteValVector(fp,nodes);
		}
#ifdef VFS_READ
		void ReadFile(IFS *pFS,DWORD hFile)
		{
			FReadValVector(pFS,hFile,nodes);
		}
#else
		void ReadFile(FILE *fp)
		{
			FReadValVector(fp,nodes);
		}
#endif

		TagNodeFrame(const TagNodeFrame& other)
		{
			nodes=other.nodes;
		}
		const TagNodeFrame& operator = (const TagNodeFrame& other)
		{
			if(this!=&other)
			{
				nodes=other.nodes;
			}
			return *this;
		}
	};
}//endof namespace F3D