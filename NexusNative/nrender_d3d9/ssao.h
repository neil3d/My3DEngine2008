#pragma once
#include "post_bloom.h"

namespace nexus
{
	enum EQuality
	{
		low,
		medium,
		high
	};

	class ssao :
		public post_process
	{
	public:
		ssao(
			EQuality quality);

		virtual ~ssao(void);
		
		virtual void render() const;
		rt_item		m_items[2];
		EQuality   m_quality;
		float m_occlustion_radius;
		float m_occlustion_power;
		float m_blocker_power;
	};
}