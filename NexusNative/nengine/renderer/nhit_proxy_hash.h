#ifndef _NEXUS_HIT_PROXY_MAP_H_
#define _NEXUS_HIT_PROXY_MAP_H_
#include "nrender_resource.h"
#include "render_define.h"

namespace nexus
{
	class nAPI nhit_proxy_hash
		: public nrender_resource
	{
	public:		
		nhit_proxy_hash(void);
		virtual ~nhit_proxy_hash(void);

		virtual void create(int w, int h) = 0;
		
		hit_id get_hit_id(int x, int y);

	protected:
		void alloc_buffer(int width, int height, int pitch);

		int	m_width,
			m_height,
			m_pitch;
		std::vector<hit_id>	m_buffer;

		nDECLARE_VIRTUAL_CLASS(nhit_proxy_hash)
	};
}//namespace nexus
#endif // _NEXUS_HIT_PROXY_MAP_H_