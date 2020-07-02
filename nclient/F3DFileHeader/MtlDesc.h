
#pragma once

#include "MathBaseDefine.h"
namespace F3D
{
	enum EMapChannel
	{
		EMap_Diffuse=0,
		EMap_Lighting,
		EMap_Specular,
		EMap_Bump,
		EMap_Max
	};
	struct tagMtlDesc
	{
		int m_id;
		Vector4 m_diffuse;
		Vector4 m_ambient;
		Vector4 m_specular;
		Vector4 m_emissive;
		float	m_power;

		bool m_bTwoSide;
		bool m_bOpcityMap;
		BYTE m_opcityVal;

		char m_map[EMap_Max][256];

		DWORD	reserve[8];
	};
	
}//endof namespace F3D