#ifndef _NEXUS_UI_CANVAS_H_
#define _NEXUS_UI_CANVAS_H_
#include "nrender_resource.h"

namespace nexus
{
	class nrender_font;
	/**
	 *	管理一个渲染UI用的render target，并提供绘图接口
	*/
	class nui_canvas
		: public nrender_resource
	{
	public:
		nui_canvas(void)			{	}
		virtual ~nui_canvas(void)	{	}

		virtual void begin() = 0;

		virtual void draw_string(const nrender_font* font, int x, int y, const nstring& text) = 0;

		virtual void end() = 0;

		nDECLARE_VIRTUAL_CLASS(nui_canvas)
	};
}//namespace nexus
#endif //_NEXUS_UI_CANVAS_H_