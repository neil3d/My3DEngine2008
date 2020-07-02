#ifndef _NEXUS_RENDER_FONT_H_
#define _NEXUS_RENDER_FONT_H_
#include "nrender_resource.h"

namespace nexus
{
	class nrender_font
		: public nrender_resource
	{
	public:
		nrender_font(void)			{		}
		virtual ~nrender_font(void)		{		}

		virtual void create(int size) = 0;
		virtual void create(int width, int height, int weight, const nstring& face_name, bool italic) = 0;

		nDECLARE_VIRTUAL_CLASS(nrender_font)
	};
}//namespace nexus
#endif