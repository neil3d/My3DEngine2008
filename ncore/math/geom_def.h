/**
*	nexus core - math.geometry shapes define
*
*	Author: Neil
*	Date:	Nov, 2008
*/

#ifndef _NEXUS_GEOM_DEFINE_H_
#define _NEXUS_GEOM_DEFINE_H_

#include "vector.h"
#include "matrix.h"

namespace nexus
{
	/** axis-aligned bounding box */
	struct AABBox
	{
		vector3 m_max,
				m_min;
		AABBox()
		{}	
		
		AABBox(const vector3& inMin, const vector3& inMax )
			:m_max(inMax), m_min(inMin)
		{}

		void get_corners(vector3 pt_array[8]) const;
	};

	/** generic bounds volume
	 *	@remark an axis aligned bounding box and bounding sphere with the same origin
	 */
	struct nCoreAPI box_sphere_bounds
	{
		vector3 origin,
				box_extent;
		float	sphere_radius;

		void get_corners(vector3 pt_array[8]) const;
		void transform_by(const matrix44& mat);
		AABBox get_AABBox()const;
	};

	struct nCoreAPI plane
	{
		vector3	normal;
		float	dist;

		void normalize();
		
		float dist_to_point(const vector3& pt) const
		{
			return vec_dot(normal, pt)+dist;
		}
	};

	struct nCoreAPI ray
	{
		vector3	origin;
		vector3 dir;
	};
}//namespace nexus

#endif //_NEXUS_GEOM_DEFINE_H_