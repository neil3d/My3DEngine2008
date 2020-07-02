/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/

#pragma once
#include "math/Vector3.h"
#include "math/BoundingBox.h"
#include "Color.h"

namespace NexusEngine
{
	/**
	 *	基本元素绘图接口, 包装nexus::nrender_primitive_draw_interface
	*/
	public ref class NRenderPrimitiveDrawInterface sealed
	{
	public:
		//!	开始批量画线
		void BeginLineBatch();

		//!	绘制一个指定颜色的线段
		void DrawLine(Vector3 start, Vector3 end, Color4f color);
		void DrawWiredBox(BoundingBox box, Color4f color);

		//!	结束批量画线
		void EndLineBatch();

		//!	简单的文字绘制接口
		void DrawDebugString(int x, int y, System::String^ text, Color4ub color);

		//void draw_line_strip(const std::vector<vector3>& pos_array, const color4f& color) = 0;
		//void draw_simple_mesh(const nrender_simple_mesh* mesh, const color4f& color) = 0;

		//-- Native wrapper
	private:
		nexus::nrender_primitive_draw_interface* m_nativePDI;
	public:
		void SetNativePDI(nexus::nrender_primitive_draw_interface* nativePDI);
		property nexus::nrender_primitive_draw_interface* NativePtr
		{
			nexus::nrender_primitive_draw_interface* get();
		}
	};

	/**
	 *	基本元素绘图回调接口
	*/
	public ref class NRenderElement abstract
	{
	public:
		NRenderElement(void);

		//!	使用PDI进行画图
		virtual void Draw(NRenderPrimitiveDrawInterface^ PDI) abstract;
		virtual void UpdateHitProxy(NRenderPrimitiveDrawInterface^ PDI)
		{
			(void)PDI;
		}
	};

	//!	包装类, 使得native代码可以调用NRenderElement派生类代码
	class NativeRenderElementWrapper : public nexus::nrender_element
	{
		gcroot<NRenderElement^>			m_clrObj;
		gcroot<NRenderPrimitiveDrawInterface^>	m_clrPDI;
	public:
		typedef boost::shared_ptr<NativeRenderElementWrapper> Ptr;

		NativeRenderElementWrapper(NRenderElement^ clrObj);

		bool IsObject(NRenderElement^ clrObj)
		{
			return (NRenderElement^)m_clrObj == clrObj;
		}
		
		virtual void draw(nrender_primitive_draw_interface* PDI);
		virtual void update_hit_proxy(nrender_primitive_draw_interface* PDI);
	};
}//namespace NexusEngine