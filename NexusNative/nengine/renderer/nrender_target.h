#ifndef _NEXUS_RENDER_TARGET_H_
#define _NEXUS_RENDER_TARGET_H_
#include "nrender_texture.h"
#include "../image/image_def.h"

namespace nexus
{
	class nAPI nrender_target 
		: public nrender_texture
	{
	public:
		nrender_target(void)	{}
		virtual ~nrender_target(void)	{}

		virtual void create(size_t w, size_t h, EPixelFormat fmt) = 0;
		virtual void save_to_file(const nstring& file_name) = 0;

		nDECLARE_VIRTUAL_CLASS(nrender_target)
	};

}// namespace nexus

#endif // _NEXUS_RENDER_TARGET_H_