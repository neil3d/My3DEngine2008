/**
 *	nexus ngame - nregion
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Jul, 2010
 */

#ifndef _NGAME_NREGION_H_
#define _NGAME_NREGION_H_

#include "ncommon.h"
#include "base_define.h"
#include "math/vector.h"

namespace nexus {

	class nworld_object;

	class nregion : private nnoncopyable
	{
	public:
		nregion() : m_radius(0.0f) {}
		virtual ~nregion() {}

		const vector3& get_center()					{ return m_pos; }
		float get_max_radius()						{ return m_radius; }

		virtual bool is_in_region(nworld_object* obj_ptr)	= 0;

	protected:
		vector3				m_pos;
		float				m_radius;
	};

	class ncircle_region : public nregion
	{
	public:
		ncircle_region() {}
		virtual ~ncircle_region() {}

		void set_param(const vector3& pos, float radius)	{ m_pos = pos; m_radius = radius; }
		virtual bool is_in_region(nworld_object* obj_ptr);
	};

	class nfan_shaped_region : public nregion
	{
	public:
		nfan_shaped_region() : m_orient(0.0f), m_angle(0.0f) {}
		virtual ~nfan_shaped_region() {}

		void set_param(const vector3& pos, float radius, float orient, float angle)
		{
			m_pos		= pos;
			m_radius	= radius;
			m_orient	= orient;
			m_angle		= angle / 2.0f;
		}

		virtual bool is_in_region(nworld_object* obj_ptr);

	private:
		float				m_orient;
		float				m_angle;
	};

	class nrectangle_region : public nregion
	{
	public:
		nrectangle_region() : m_weight(0.0f), m_height(0.0f), m_orient(0.0f) {}
		virtual ~nrectangle_region() {}

		void set_param(const vector3& pos, float radius, float weight, float height, float orient)
		{
			m_pos		= pos;
			m_weight	= weight / 2.0f;
			m_height	= height / 2.0f;
			m_orient	= orient;
			m_radius	= sqrt(m_weight * m_weight + m_height * m_height);
		}

		virtual bool is_in_region(nworld_object* obj_ptr);

	private:
		float				m_weight;
		float				m_height;
		float				m_orient;
	};

} // namespace nexus

#endif // _NGAME_NREGION_H_
