#include "StdAfx.h"
#include "geom_def.h"

namespace nexus
{
	void plane::normalize()
	{
		float mag_inv = 1.0f/vec_length( normal );

		normal *= mag_inv;
		dist *= mag_inv;
	}

	nCoreAPI void mat_reflect(matrix44& mat, const plane& pln)
	{
		float a = pln.normal.x;
		float b = pln.normal.y;
		float c = pln.normal.z;
		float d = pln.dist;

		/** from DX SDK D3DXMatrixReflect()*/
		mat._11 = -2 * a * a + 1;	mat._12 = -2 * b * a;		mat._13 = -2 * c * a;		mat._14 = 0;
		mat._21 = -2 * a * b;		mat._22 = -2 * b * b + 1;	mat._23 = -2 * c * b;		mat._24 = 0;
		mat._31 = -2 * a * c;		mat._32 = -2 * b * c;		mat._33 = -2 * c * c + 1;   mat._34 = 0;
		mat._41 = -2 * a * d;		mat._42 = -2 * b * d;		mat._43 = -2 * c * d;		mat._44 = 1;
	}

	AABBox box_sphere_bounds::get_AABBox()const	
	{	
		return AABBox( origin-box_extent, origin+box_extent );
	}	
	
	void AABBox::get_corners(vector3 pt_array[8]) const
	{
		const vector3& box_max = m_max;
		const vector3& box_min = m_min;

		pt_array[0] = box_min;
		pt_array[1] = vector3(box_max.x,box_min.y,box_min.z);
		pt_array[2] = vector3(box_max.x,box_min.y,box_max.z);
		pt_array[3] = vector3(box_min.x,box_min.y,box_max.z);

		pt_array[4] = vector3(box_min.x,box_max.y,box_min.z);
		pt_array[5] = vector3(box_max.x,box_max.y,box_min.z);
		pt_array[6] = box_max;
		pt_array[7] = vector3(box_min.x,box_max.y,box_max.z);		
	}

	void box_sphere_bounds::get_corners(vector3 pt_array[8]) const
	{
		vector3 box_max = origin+box_extent;
		vector3 box_min = origin-box_extent;

		pt_array[0] = box_min;
		pt_array[1] = vector3(box_max.x,box_min.y,box_min.z);
		pt_array[2] = vector3(box_max.x,box_min.y,box_max.z);
		pt_array[3] = vector3(box_min.x,box_min.y,box_max.z);

		pt_array[4] = vector3(box_min.x,box_max.y,box_min.z);
		pt_array[5] = vector3(box_max.x,box_max.y,box_min.z);
		pt_array[6] = box_max;
		pt_array[7] = vector3(box_min.x,box_max.y,box_max.z);		
	}

	void box_sphere_bounds::transform_by(const matrix44& mat)
	{
		vector3 pt[8];
		get_corners(pt);

		for(int i=0; i<8; i++)
			pt[i] = pt[i]*mat;

		float fm = std::numeric_limits<float>::max();
		vector3 vec_m(fm, fm, fm);

		vector3 box_min = vec_m;
		vector3 box_max = -vec_m;

		for(int i=0; i<8; i++)
		{
			const vector3& pos = pt[i];

			if(pos.x > box_max.x)
				box_max.x = pos.x;
			if(pos.y > box_max.y)
				box_max.y = pos.y;
			if(pos.z > box_max.z)
				box_max.z = pos.z;

			if(pos.x < box_min.x)
				box_min.x = pos.x;
			if(pos.y < box_min.y)
				box_min.y = pos.y;
			if(pos.z < box_min.z)
				box_min.z = pos.z;
		}

		origin = (box_max+box_min)*0.5f;
		box_extent = (box_max-box_min)*0.5f;
		sphere_radius = vec_length(box_extent);
	}
}