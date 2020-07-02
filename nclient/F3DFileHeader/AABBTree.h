#pragma once
#include "FileUtil.h"
#include "MathBaseDefine.h"
#include "SubMesh.h"
#include <vector>
using namespace std;

namespace F3D
{
	class AABBTreeNode
	{
	public:
		int				m_lv;//0 is root
		int				m_childrenID[2];
		FaceBuffer		m_faces;	//只有leaf node,存face
		Vector3			m_boxMax,
						m_boxMin;//为了避免引入GeomDefine.h

	public:
		AABBTreeNode()
		{
			m_childrenID[0]=m_childrenID[1]=-1;
			m_lv=-1;
		}
		~AABBTreeNode()	{}
		AABBTreeNode(const AABBTreeNode& other)
		{
			Copy(other);
		}
		const AABBTreeNode& operator = (const AABBTreeNode& other)
		{
			if(this!=&other)
				Copy(other);
			return *this;
		}

	private:
		void Copy(const AABBTreeNode& other)
		{
			m_childrenID[0]=other.m_childrenID[0];
			m_childrenID[1]=other.m_childrenID[1];
			m_faces=other.m_faces;
			m_lv=other.m_lv;
		}
	};
	class AABBTree
	{
	public:
		AABBTree(void)	{}
		virtual ~AABBTree(void)
		{
			for(size_t i=0;i<m_nodes.size();i++)
			{
				AABBTreeNode *pNode=m_nodes[i];
				delete pNode;
			}
			m_nodes.clear();
		}

		void WriteFile(FILE *fp)
		{
			int numNodes=m_nodes.size();
			FWriteValue(fp,numNodes);
			for(int i=0;i<numNodes;i++)
			{
				AABBTreeNode& node=*(m_nodes[i]);
				FWriteValue(fp,node.m_childrenID[0]);
				FWriteValue(fp,node.m_childrenID[1]);
				FWriteValVector(fp,node.m_faces);
				FWriteValue(fp,node.m_boxMax);
				FWriteValue(fp,node.m_boxMin);
			}
		}
#ifdef VFS_READ
		void ReadFile(IFS *pFS,DWORD hFile)
		{
			int numNodes;
			FReadValue(pFS,hFile,numNodes);
			for(int i=0;i<numNodes;i++)
			{
				AABBTreeNode *pNewNode=new AABBTreeNode;
				FReadValue(pFS,hFile,pNewNode->m_childrenID[0]);
				FReadValue(pFS,hFile,pNewNode->m_childrenID[1]);
				FReadValVector(pFS,hFile,pNewNode->m_faces);
				FReadValue(pFS,hFile,pNewNode->m_boxMax);
				FReadValue(pFS,hFile,pNewNode->m_boxMin);
				m_nodes.push_back(pNewNode);
			}
		}
#endif

		AABBTreeNode *GetNode(int id)
		{
			if(id>=0 && id<(int)m_nodes.size())
				return m_nodes[id];
			else
				return NULL;
		}
	protected:
		vector<AABBTreeNode*>	m_nodes;//index 0 is root node
	};
}//namespace F3D