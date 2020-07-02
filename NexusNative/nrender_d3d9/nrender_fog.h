#pragma once
#include "render_proxy.h"

namespace nexus
{
	class nfog_volume_proxy:public nrender_proxy
	{
	public:
		nfog_volume_proxy( const class nfog_volume_component* fog);
		~nfog_volume_proxy(void);

		virtual void render(const class nview_info*  view,class drawing_policy* dp) const;

		const class nfog_volume_component* m_owner;
	};
};
