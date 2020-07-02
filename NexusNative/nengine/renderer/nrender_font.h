#ifndef _NEXUS_RENDER_FONT_H_
#define _NEXUS_RENDER_FONT_H_
#include "nrender_resource.h"

namespace nexus
{
	class nAPI nrender_font
		: public nrender_resource
	{
	public:
		typedef render_res_ptr<nrender_font> ptr;

		nrender_font(void)			{		}
		virtual ~nrender_font(void)		{		}

		// 创建字体
		virtual void create(const TCHAR* face_name, int width, int height, int weight = 0, bool italic = false) = 0;

		// 绘制文字
		virtual int draw_text(const TCHAR* text, const frect& target_area, unsigned int color, unsigned int shadow_color, unsigned int format) = 0;
		// 绘制字体,居左居上对齐
		virtual int draw_text(const TCHAR* text, long x, long y, unsigned int color, unsigned int shadow_color) = 0; 

		// 绘制文字
		virtual int draw_text(const TCHAR* text, const frect& target_area, float z ,float scale ,unsigned int color, unsigned int shadow_color, unsigned int format) = 0;
		virtual int draw_text(const TCHAR* text, const frect& target_area, float z ,float scale ,const vector2& scale_center,unsigned int color,unsigned int shadow_color, unsigned int format) = 0;

		// 获取文本大小
		virtual void get_text_size( const TCHAR* text, nsize& out_size ) = 0;

		// Computes the bounding rectangle for a given string to be rendered using this font.
		/// ref ID3DXFont::DrawText
		virtual int measure_string( const TCHAR* text, unsigned int format, frect& rectangle ) = 0;

		nDECLARE_VIRTUAL_CLASS(nrender_font)
	};
}//namespace nexus
#endif