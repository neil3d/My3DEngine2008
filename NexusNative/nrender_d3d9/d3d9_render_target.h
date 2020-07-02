#ifndef _NEXUS_D3D9_RENDER_TARGET_H_
#define _NEXUS_D3D9_RENDER_TARGET_H_
#include "d3d_ptr.h"

namespace nexus
{
	class d3d9_render_target 
		: public nrender_target
	{
	public:
		d3d9_render_target(void);
		virtual ~d3d9_render_target(void);

		virtual void create(size_t w, size_t h, EPixelFormat fmt);
		virtual void save_to_file(const nstring& file_name);

		virtual void* get_handle() const
		{
			return (void*)(m_texture.get());
		}

		IDirect3DSurface9* get_surface()
		{
			return m_surface.get();
		}

	private:
		d3d_texture_ptr	m_texture;
		d3d_surface_ptr	m_surface;
	};
}//namespace nexus

#endif //_NEXUS_D3D9_RENDER_TARGET_H_