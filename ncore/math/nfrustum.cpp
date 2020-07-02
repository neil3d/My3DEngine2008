#include "StdAfx.h"
#include "nfrustum.h"

namespace nexus
{
	nfrustum::nfrustum(void)
	{
	}

	nfrustum::~nfrustum(void)
	{
	}

	void nfrustum::extract(const matrix44& mat_view, const matrix44& mat_prj)
	{
		matrix44 mat_combo = mat_view*mat_prj;

		// Left clipping plane
		m_planes[0].normal.x = mat_combo._14 + mat_combo._11;
		m_planes[0].normal.y = mat_combo._24 + mat_combo._21;
		m_planes[0].normal.z = mat_combo._34 + mat_combo._31;
		m_planes[0].dist = mat_combo._44 + mat_combo._41;
		// Right clipping plane
		m_planes[1].normal.x = mat_combo._14 - mat_combo._11;
		m_planes[1].normal.y = mat_combo._24 - mat_combo._21;
		m_planes[1].normal.z = mat_combo._34 - mat_combo._31;
		m_planes[1].dist = mat_combo._44 - mat_combo._41;
		// Top clipping plane
		m_planes[2].normal.x = mat_combo._14 - mat_combo._12;
		m_planes[2].normal.y = mat_combo._24 - mat_combo._22;
		m_planes[2].normal.z = mat_combo._34 - mat_combo._32;
		m_planes[2].dist = mat_combo._44 - mat_combo._42;
		// Bottom clipping plane
		m_planes[3].normal.x = mat_combo._14 + mat_combo._12;
		m_planes[3].normal.y = mat_combo._24 + mat_combo._22;
		m_planes[3].normal.z = mat_combo._34 + mat_combo._32;
		m_planes[3].dist = mat_combo._44 + mat_combo._42;
		// Near clipping plane
		m_planes[4].normal.x = mat_combo._13;
		m_planes[4].normal.y = mat_combo._23;
		m_planes[4].normal.z = mat_combo._33;
		m_planes[4].dist = mat_combo._43;
		// Far clipping plane
		m_planes[5].normal.x = mat_combo._14 - mat_combo._13;
		m_planes[5].normal.y = mat_combo._24 - mat_combo._23;
		m_planes[5].normal.z = mat_combo._34 - mat_combo._33;
		m_planes[5].dist = mat_combo._44 - mat_combo._43;

		for(int i=0;i<6;i++)
			m_planes[i].normalize();
	}

	bool nfrustum::intersect_bounds(const box_sphere_bounds& bounds) const
	{
		//-- sphere test
		// For each plane, see if sphere is on negative side
		// If so, object is not visible
		for (int p = 0; p < 6; ++p)
		{
			// If the distance from sphere center to plane is negative, and 'more negative' 
			// than the radius of the sphere, sphere is outside frustum
			if (m_planes[p].dist_to_point(bounds.origin) < -bounds.sphere_radius)
			{
				// ALL corners on negative side therefore out of view
				return false;
			}
		}//end of for

		//-- aabbox test
		vector3 pt[8];
		bounds.get_corners(pt);

		// Go through all of the corners of the box and check then again each plane
		// in the frustum.  If all of them are behind one of the planes, then it most
		// like is not in the frustum.
		for(int i=0;i<6;i++)
		{
			if(m_planes[i].dist_to_point(pt[0]) > 0)
				continue;
			if(m_planes[i].dist_to_point(pt[1]) > 0)
				continue;
			if(m_planes[i].dist_to_point(pt[2]) > 0)
				continue;
			if(m_planes[i].dist_to_point(pt[3]) > 0)
				continue;
			if(m_planes[i].dist_to_point(pt[4]) > 0)
				continue;
			if(m_planes[i].dist_to_point(pt[5]) > 0)
				continue;
			if(m_planes[i].dist_to_point(pt[6]) > 0)
				continue;
			if(m_planes[i].dist_to_point(pt[7]) > 0)
				continue;

			//如果所有点在任何一个平面的外面,则BOX在外面
			// If we get here, it isn't in the frustum
			return false;
		}

		// Return a true for the box being inside of the frustum
		return true;
	}
	int nfrustum::sphere_partial_in_frustum( const vector3& center, float radius ) const
	{		
		int c = 0;
		float d;	
		for (int p = 0; p < 6; ++p)
		{			
			d = m_planes[p].dist_to_point(center);
			if ( d <= -radius)	
			{
				return 0;
			}			
			if( d > -radius )
				c++;	
		}//end of for	
		return (c == 6) ? 2 : 1;	
	}

	int nfrustum::aabbox_partial_in_frustum( const vector3& center, float size ) const	
	{
		int c;		
		int c2 = 0;
		for(int i=0; i<6; i++)
		{		
			c = 0;		
			if(m_planes[i].dist_to_point( vector3(center.x-size, center.y-size, center.z-size ) ) > 0)
				c++;	
			if(m_planes[i].dist_to_point( vector3(center.x+size, center.y-size, center.z-size ) ) > 0)		
				c++;
			if(m_planes[i].dist_to_point( vector3(center.x-size, center.y+size, center.z-size ) ) > 0)	
				c++;		
			if(m_planes[i].dist_to_point( vector3(center.x+size, center.y+size, center.z-size ) ) > 0)
				c++;			
			if(m_planes[i].dist_to_point( vector3(center.x-size, center.y-size, center.z+size ) ) > 0)	
				c++;		
			if(m_planes[i].dist_to_point( vector3(center.x+size, center.y-size, center.z+size ) ) > 0)	
				c++;	
			if(m_planes[i].dist_to_point( vector3(center.x-size, center.y+size, center.z+size ) ) > 0)	
				c++;	
			if(m_planes[i].dist_to_point( vector3(center.x+size, center.y+size, center.z+size ) ) > 0)	
				c++;		
			if( c == 0 )	
				return 0;		
			if( c == 8 )	
				c2++;	
		}		
		
		return (c2 == 6) ? 2 : 1;	
	}

	bool nfrustum::intersect_box(const AABBox& box) const
	{
		//-- aabbox test
		vector3 pt[8];
		box.get_corners(pt);

		// Go through all of the corners of the box and check then again each plane
		// in the frustum.  If all of them are behind one of the planes, then it most
		// like is not in the frustum.
		for(int i=0;i<6;i++)
		{
			if(m_planes[i].dist_to_point(pt[0]) > 0)
				continue;
			if(m_planes[i].dist_to_point(pt[1]) > 0)
				continue;
			if(m_planes[i].dist_to_point(pt[2]) > 0)
				continue;
			if(m_planes[i].dist_to_point(pt[3]) > 0)
				continue;
			if(m_planes[i].dist_to_point(pt[4]) > 0)
				continue;
			if(m_planes[i].dist_to_point(pt[5]) > 0)
				continue;
			if(m_planes[i].dist_to_point(pt[6]) > 0)
				continue;
			if(m_planes[i].dist_to_point(pt[7]) > 0)
				continue;

			//如果所有点在任何一个平面的外面,则BOX在外面
			// If we get here, it isn't in the frustum
			return false;
		}

		// Return a true for the box being inside of the frustum
		return true;
	}
}//namespace nexus