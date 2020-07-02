#include "nregion.h"
#include "nworld_object.h"
#include "math/math_const.h"

namespace nexus {

	bool ncircle_region::is_in_region(nworld_object* obj_ptr)
	{
		if (!obj_ptr)
		{
			return false;
		}

		return obj_ptr->is_in_dist(m_pos, m_radius);
	}

	bool nfan_shaped_region::is_in_region(nworld_object* obj_ptr)
	{
		if (!obj_ptr)
		{
			return false;
		}

		if (!obj_ptr->is_in_dist(m_pos, m_radius))
		{
			return false;
		}

		vector3 dir = obj_ptr->get_position() - m_pos;

		if (is_zero(dir.x) && is_zero(dir.z))
		{
			return true;
		}

		float ang = vec_calc_yaw(dir);

		ang -= m_orient;
		return (ang >= -m_angle) && (ang <= m_angle);
	}

	bool nrectangle_region::is_in_region(nworld_object* obj_ptr)
	{
		if (!obj_ptr)
		{
			return false;
		}

		float dist = obj_ptr->get_distance(m_pos);
		float min = m_weight < m_height ? m_weight : m_height;
		
		if (dist <= min)
		{
			return true;
		}

		vector3 dir = obj_ptr->get_position() - m_pos;
		float ang = vec_calc_yaw(dir);
		float dx = dist * sin(ang);
		float dz = dist * cos(ang);

		if (dx <= m_weight && dx >= -m_weight && dz <= m_height && dz >= -m_height)
		{
			return true;
		}

		return false;
	}

} // namespace nexus
