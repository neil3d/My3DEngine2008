#ifndef _NEXUS_D3D9_HIT_PROXY_MAP_H_
#define _NEXUS_D3D9_HIT_PROXY_MAP_H_
#include "post_process.h"

namespace nexus
{
	class d3d9_hit_proxy_hash
		: public nhit_proxy_hash
	{
	public:
		d3d9_hit_proxy_hash(void);
		virtual ~d3d9_hit_proxy_hash(void);

		virtual void create(int w, int h);

		void begin_update();
		void end_update();
	private:
		rt_item	m_render_target;	// render target 无法lock，所以搞了另外一个surface
		d3d_surface_ptr m_hd_buffer;
	};
}//namespace nexus
#endif // _NEXUS_D3D9_HIT_PROXY_MAP_H_