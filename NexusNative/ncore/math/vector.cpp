#include "StdAfx.h"
#include "vector.h"

namespace nexus
{
	vector3::vector3(const vector4& data):x(data.x),y(data.y),z(data.z) {}

	inline vector3& vector3::operator = ( const vector4& rk )
	{
		x = rk.x;
		y = rk.y;
		z = rk.z;            

		return *this;
	}
}