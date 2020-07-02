#ifndef _NEXUS_D3D9_UI_CANVAS_H_
#define _NEXUS_D3D9_UI_CANVAS_H_
#include "d3d_ptr.h"

namespace nexus
{
	struct UI_SCREEN_VERTEX 
	{
		union
		{
			struct  
			{
				float		x,y,z,w;
				D3DCOLOR	color;
				float		tu, tv;
				float		tu2, tv2;
			};
			struct  
			{
				D3DXVECTOR4	pos;
				D3DCOLOR	color;
				D3DXVECTOR2	uv;
				D3DXVECTOR2	uv2;
			};
		};		

		static DWORD FVF;
	};

	struct UI_SCREEN_VERTEX_UNTEX 
	{
		union
		{
			struct  
			{
				float		x,y,z,w;
				D3DCOLOR	color;
			};
			struct  
			{
				D3DXVECTOR4	pos;
				D3DCOLOR	color;
			};
		};		

		static DWORD FVF;
	};

	/**
	* 实现2D渲染
	*/ 
	class d3d9_ui_canvas :
		public nui_canvas
	{
	public:
		d3d9_ui_canvas(void);
		virtual ~d3d9_ui_canvas(void);

		virtual void begin(bool depth_enable=false);

		virtual vector2 get_viewport_size();
		
		// 设置裁减区域
		virtual void push_scissor_rect(const frect* rect);
		// pop区域from裁减队列
		virtual void pop_scissor_rect();
		// 获取当前裁剪区域的大小
		virtual void get_scissor_rect(frect& rect);

		// 绘制图像，不使用2D精灵，四个角落可以有不同的色彩
		virtual void draw_texture(nrender_texture2D* texture, const frect& source_area, const frect& target_area,const color_rect& color = color_rect(0xFFFFFFFF) );
		// 使用2D精灵绘制图像
		virtual void draw_texture(nrender_texture2D* texture, const frect& source_area, const frect& target_area,argb_t color = 0xFFFFFFFF );
		// 使用2D精灵绘制图像，带深度和缩放，以目标位置中心为缩放中心
		virtual void draw_texture(nrender_texture2D* texture, const frect& source_area, const frect& target_area,float z,float scale,argb_t color = 0xFFFFFFFF );
		// 使用2D精灵绘制图像，带深度和缩放，需要指定缩放中心
		virtual void draw_texture(nrender_texture2D* texture, const frect& source_area, const frect& target_area,float z,float scale,const vector2& scale_center,argb_t color = 0xFFFFFFFF );
		// 使用2D精灵绘制图像，带旋转，需要指定旋转中心
		virtual void draw_texture(nrender_texture2D* texture, const frect& source_area, const frect& target_area,float rotation,const vector2& rot_center,argb_t color = 0xFFFFFFFF );
		//使用alpha图绘制指定图像,带有绕绘制矩形中心的旋转
		virtual void draw_texture(nrender_texture2D* texture, const frect& source_area, nrender_texture2D* alpha_texture,const frect& alpha_source_area,const frect& target_area,float rotation,argb_t color = 0xFFFFFFFF );
		
		// 绘制文本
		virtual void draw_string(nrender_font* font, const TCHAR* text, const frect& target_area, unsigned int color, unsigned int shadow_color, unsigned int format);
		virtual void draw_string(nrender_font* font, const TCHAR* text, long x, long y, unsigned int color, unsigned int shadow_color);
		//带深度和缩放的绘制文本
		virtual void draw_string(nrender_font* font, const TCHAR* text, const frect& target_area, float z ,float scale ,unsigned int color, unsigned int shadow_color, unsigned int format);
		virtual void draw_string(nrender_font* font, const TCHAR* text, const frect& target_area, float z ,float scale ,const vector2& scale_center,unsigned int color,unsigned int shadow_color, unsigned int format);
		
		// 绘制一个矩形块
		virtual void draw_rect(const frect& target_area,const color_rect& color);
		virtual void draw_rect(const frect& target_area,float z,float scale,const color_rect& color);
		virtual void draw_rect(const frect& target_area,float z,float scale,const vector2& scale_center,const color_rect& color);
		// 画线
		virtual void draw_line( fpoint* points, unsigned int num_points, unsigned int color );
		virtual void draw_line( fpoint* points, unsigned int num_points, float z,float scale,const vector2& scale_center,unsigned int color );

		// 画多边形函数，用于cooldown绘制
		virtual void draw_polygon(fpoint* points,unsigned int num_points, unsigned int color = 0xFFFFFFFF );
		virtual void draw_polygon(fpoint* points,unsigned int num_points, float z,float scale,const vector2& scale_center,unsigned int color = 0xFFFFFFFF );

		virtual void flush();

		virtual void end();
		//-------------------------------------------------------------------------------

	private:
		void set_scissor_rect(const RECT* rc);

		typedef std::deque<RECT> scissor_rect_deque;
		scissor_rect_deque	m_scissor_rects;
	};

} // namespace nexus

#endif // _NEXUS_D3D9_UI_CANVAS_H_