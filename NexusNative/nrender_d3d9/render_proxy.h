#pragma once

namespace nexus
{
	class nrender_proxy
	{
	public:
		virtual void render(const nview_info* view, drawing_policy* dp) const = 0;
	};
}
