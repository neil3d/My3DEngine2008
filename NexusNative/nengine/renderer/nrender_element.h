#ifndef _NEXUS_RENDER_ELEMENT_H_
#define _NEXUS_RENDER_ELEMENT_H_
#include "ncore.h"

namespace nexus
{
	class nmesh_component;
	class nrender_simple_mesh;
	/**
	 *	提供给render element和canvas的绘图接口
	 *	@remarks 由渲染器实现
	*/
	class nAPI nrender_primitive_draw_interface
		: public nobject
	{
	public:
		nrender_primitive_draw_interface(void)		{		}
		virtual ~nrender_primitive_draw_interface(void)	{	}

		virtual void set_transform(const matrix44& matrix)=0;
		//-- 可以使用begin进行batch，如果直接调用draw_line则直接画出
		virtual void begin_line_batch() = 0;
		virtual void draw_line(const vector3& start, const vector3& end, const color4f& color) = 0;
		virtual void draw_wired_box(const AABBox& box, const color4f& color) = 0;
		virtual void draw_wired_sphere(const vector3& pos,float radius, const color4f& color)=0;
		virtual void draw_wired_cone(float bottom_radius, float length, const matrix44& transform, const color4f& color)=0;
		virtual void end_line_batch() = 0;

		virtual void draw_line_strip(const std::vector<vector3>& pos_array, const color4f& color) = 0;

		virtual void draw_simple_mesh(const nrender_simple_mesh* mesh, const color4f& color) = 0;
		virtual void draw_debug_string(size_t x, size_t y, const nstring& text, const color4ub& color) = 0;

		virtual void draw_mesh_component(const nmesh_component* comp, bool hit_proxy, bool disable_ztest) = 0;


		/**
		* Draws a axis-aligned 3 line star.
		*
		* @param	PDI				Draw interface.
		* @param	position		Position of the star.
		* @param	size			Size of the star
		* @param	color			Color of the arrow.
		*/
		void draw_wire_star(const vector3& position, float size, const color4f& color)
		{
			draw_line( position + vector3(1,0,0)*size, position - vector3(1,0,0)*size, color );
			draw_line( position + vector3(0,1,0)*size, position - vector3(0,1,0)*size, color );
			draw_line( position + vector3(0,0,1)*size, position - vector3(0,0,1)*size, color );
		}

		nDECLARE_VIRTUAL_CLASS(nrender_primitive_draw_interface);
	};

	/**
	 *	可以直接在back buff上绘制的对象
	 *	@remark 由使用者派生，并在draw()函数中调用PDI来执行绘图
	*/
	class nAPI nrender_element
		: public nobject
	{
	public:
		typedef shared_ptr<nrender_element> ptr;

		nrender_element(void)		{		}
		virtual ~nrender_element(void)	{	}

		virtual void draw(nrender_primitive_draw_interface* PDI) = 0;
		virtual void update_hit_proxy(nrender_primitive_draw_interface* PDI)
		{
			(void)PDI;
		}

		nDECLARE_VIRTUAL_CLASS(nrender_element);
	};

}//namespace nexus

#endif //_NEXUS_RENDER_ELEMENT_H_