#ifndef _NEXUS_SPT_INSTANCE_H_
#define _NEXUS_SPT_INSTANCE_H_
#include "nspeed_base_tree.h"

namespace nexus
{

	///////////////////////////////////////////////////////////////////////  
	//  IdvRandom (int)
	//
	//  nMin <= (return value) <= nMax

	inline int IdvRandom(int nMin, int nMax)
	{
		int nDiff = nMax - nMin + 1;

		return nMin + (rand( ) % nDiff);
	}


	///////////////////////////////////////////////////////////////////////  
	//  IdvRandom (float)
	//
	//  fMin <= (return value) <= fMax

	inline float IdvRandom(float fMin, float fMax)
	{
		float fUnit = float(rand( )) / RAND_MAX;
		float fDiff = fMax - fMin;

		return fMin + fUnit * fDiff;
	}


	struct nAPI nspt_instance
	{
		nspeed_base_tree::ptr	base_spt;
		matrix44				mat_world;		
		box_sphere_bounds		bounds;
		hit_id	m_hit_id;

		vector3	pos;	// 3D position of tree in space
		float	scale;	// relative size of instance (1.0 is unscaled)
		float   rotation;      // rotation of instance along up-axis in radians (0.0 is unrotated)
		vector4	rotation_tri; // sin(m_fRotation), cos(m_fRotation), -sin(m_fRotation), 0.0f
		float   wind_matrix_offset;        // used to assigned instances of the same tree different wind matrices

		//-- lod
		float                       m_fLod;                     // 1.0 = highest (fully 3D), 0.0 = lowest (billboard)
		char                        m_nBranchLodLevel;          // 0 is highest LOD, ranges from [0,CSpeedTreeRT::GetNumBranchLodLevels()-1]
		float                       m_fBranchAlphaValue;        // range [84.0 (opaque), 255.0 (invisible)]

		char                        m_nFrondLodLevel;           // 0 is highest LOD, ranges from [0,CSpeedTreeRT::GetNumFrondLodLevels()-1]
		float                       m_fFrondAlphaValue;         // range [84.0 (opaque), 255.0 (invisible)]

		char                        m_anLeafLodLevels[2];       // 0 is highest LOD, ranges from [0,CSpeedTreeRT::GetNumLeafLodLevels()-1]
		float                       m_afLeafAlphaValues[2];     // range [84.0 (opaque), 255.0 (invisible)]

		bool                        m_bBillboardIsActive;       // true if billboard is fading or all the way in
		float                       m_fBillboardFadeOut;        // range [0.0 (not visible), 1.0 (fully visible)]

		nspt_instance();

		void serialize(narchive& ar);

		void set_rotation(float fRotation)
		{
			// can't use a negative rotation value (billboard shader optimization)
			if (fRotation < 0.0f)
				fRotation = fmod(fRotation, nTwoPI) + nTwoPI;

			rotation = fRotation;
			rotation_tri.x = sinf(fRotation);    
			rotation_tri.y = cosf(fRotation);    
			rotation_tri.z = -rotation_tri.x;
			rotation_tri.w = 0.0f;   
		}

		void update_lod(float lod);
	};
}//namespace nexus

#endif