#pragma once
#include "render_proxy.h"

namespace nexus
{
	class nwater_render_proxy : public nrender_proxy
	{
	public:
		nwater_render_proxy( const class nwater_component* water);
		~nwater_render_proxy(void);

		virtual void render(const class nview_info*  view,class drawing_policy* dp) const;

		const class nwater_component* m_owner;
	};

	extern void fft_simulation(float time);
};
