#pragma once
#include "ActorXDefine.h"

//--ActorX skin + skeletal define file
struct PSKFile
{
	std::vector<VPoint>		m_points;
	std::vector<VVertex>	m_vertices;
	std::vector<VTriangle>	m_faces;
	std::vector<VMaterial>	m_mtl_array;
	
	std::vector<FNamedBoneBinary>	m_skeleton_bones;
	std::vector<VRawBoneInfluence>	m_raw_weights;

	bool load_from_file(const nstring& file_name);
	void clear()
	{
		m_points.clear();
		m_vertices.clear();
		m_faces.clear();
		m_mtl_array.clear();
		m_skeleton_bones.clear();
		m_raw_weights.clear();
	}
};


//-- ActorX animation sequence
struct PSAFile
{
	std::vector<FNamedBoneBinary>	m_skeleton_bones;
	std::vector<VAnimation>			m_anims;

	bool load_from_file(const nstring& file_name);
	void clear()
	{
		m_skeleton_bones.clear();
		m_anims.clear();
	}
};
