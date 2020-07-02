
#pragma once
//--一些输出插件定义的东西, 外部也可能会用到的
#pragma warning(disable:4251)//Wraning of D3DXVECTOR3 etc need dll export
#include "MathBaseDefine.h"
#include "VertexDefine.h"
#include "MtlDesc.h"
#include "SubMesh.h"

namespace F3D
{
	inline DWORD MakeFourCC( std::string szStr ) 
	{ 
		char str[4];
		memset(str,0,sizeof(str));
		strncpy(str,szStr.c_str(),sizeof(str));
		return (str[3]<<24) | (str[2]<<16) | (str[1]<<8) | str[0]; 
	}

	inline std::string FourCC2Str(DWORD dw)
	{
		char str[5];
		str[0]=(char)(dw&0xff); 
		str[1]=(char)((dw>>8)&0xff); 
		str[2]=(char)((dw>>16)&0xff); 
		str[3]=(char)((dw>>24)&0xff);
		str[4]=0;
		return std::string(str);
	}

	//--Light-----------------------------------
	enum LIGHT_TYPE
	{
		LGT_OMNI,
		LGT_SPOT,
		LGT_DIRECT
	};
	enum DECAY_TYPE
	{
		DK_DECAY_NONE,
		DK_DECAY_DIST,
		DK_DECAY_SQUARE
	};
	enum LIGHT_SHAPE
	{
		SHAPE_CONE,
		SHAPE_SQUARE
	};
	
	struct LightDesc
	{
		LIGHT_TYPE m_LightType;
		bool  m_bShadow;  //whether to project shadow
		Vector3 m_Diffuse;
		Vector3 m_Specular;
		Vector3 m_Ambient;
		float m_NearStart;
		float m_NearEnd;
		float m_FarStart;
		float m_FarEnd;
		
		DECAY_TYPE  m_DecayType;
		float m_DecayRadius;
		
		LIGHT_SHAPE m_LightShape;
		float m_HotSize;	//in degrees with spot light,  in units with direct light
		float m_Falloff;	//in degrees,  in units with direct light
		float m_Aspect;		//only useful when lightshape is square
		
		Vector3	m_Position;//for Omni and spot
		Vector3 m_Direction;//for Direct and spot
	};


	struct MeshTopo
	{
		std::vector<FaceTopo>	faceArray;

		MeshTopo()
		{}
		MeshTopo(const MeshTopo& other)
		{
			faceArray=other.faceArray;
		}
		const MeshTopo& operator = (const MeshTopo& other)
		{
			if(this == &other)
				return *this;
			faceArray=other.faceArray;
			return *this;
		}
	public:
		void ComputeEdgeConnect(const Vert_PNT *pVB);
	};

	struct MeshFaceGroup
	{
		WORD	baseIndex;
		DWORD	mtlID;
		WORD	numVert;
		DWORD	numFaces;
		Vector3 max;
		Vector3 min;

		std::vector<TriFace>	faces;

		MeshTopo topo;//不使用baseIndex
		
		MeshFaceGroup()
		{ baseIndex=0; mtlID=0;}
		
		void Clone(const MeshFaceGroup& other)
		{
			baseIndex=other.baseIndex;
			mtlID=other.mtlID;
			faces=other.faces;
			numVert=other.numVert;
			numFaces=other.numFaces;
			topo=other.topo;
		}
		
		MeshFaceGroup(const MeshFaceGroup& other)
		{
			Clone(other);
		}
		const MeshFaceGroup& operator = (const MeshFaceGroup& other)
		{
			if(this == &other)
				return *this;
			Clone(other);
			return *this;
		}
	};
}//namespace F3D