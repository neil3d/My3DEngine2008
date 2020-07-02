#ifndef _NEXUS_FRUSTUM_H_
#define _NEXUS_FRUSTUM_H_
#include "matrix.h"
#include "geom_def.h"

namespace nexus
{
	class nCoreAPI nfrustum
	{
	public:
		nfrustum(void);
		~nfrustum(void);

		void extract(const matrix44& mat_view, const matrix44& mat_prj);

		bool intersect_bounds(const box_sphere_bounds& bounds) const;	
		bool intersect_box(const AABBox& box) const;		
		
		/** returns 0 if the sphere is totally outside, 1 if it's partially inside, and 2 if it's totally inside
		*/
		int sphere_partial_in_frustum( const vector3& center, float radius ) const;
		int aabbox_partial_in_frustum( const vector3& center, float size ) const;

	private:
		plane	m_planes[6];
	};
}//namespace nexus
#endif //_NEXUS_FRUSTUM_H_