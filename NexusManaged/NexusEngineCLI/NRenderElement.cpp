#include "StdAfx.h"
#include "NRenderElement.h"

namespace NexusEngine
{
	//-- class NRenderPrimitiveDrawInterface ----------------------------------------------------------
	void NRenderPrimitiveDrawInterface::SetNativePDI(nexus::nrender_primitive_draw_interface* nativePDI)		
	{
		m_nativePDI = nativePDI;
	}

	void NRenderPrimitiveDrawInterface::BeginLineBatch()
	{
		m_nativePDI->begin_line_batch();
	}

	void NRenderPrimitiveDrawInterface::DrawLine(Vector3 start, Vector3 end, Color4f color)
	{
		nexus::vector3* pStart = reinterpret_cast<nexus::vector3*>(&start);
		nexus::vector3* pEnd = reinterpret_cast<nexus::vector3*>(&end);
		nexus::color4f* pColor = reinterpret_cast<nexus::color4f*>(&color);

		m_nativePDI->draw_line(*pStart, *pEnd, *pColor);
	}

	void NRenderPrimitiveDrawInterface::DrawWiredBox(BoundingBox box, Color4f color)
	{
		nexus::AABBox* nbox = reinterpret_cast<nexus::AABBox*>(&box);
		nexus::color4f* nc = reinterpret_cast<nexus::color4f*>(&color);

		m_nativePDI->draw_wired_box(*nbox, *nc);
	}

	void NRenderPrimitiveDrawInterface::EndLineBatch()
	{
		m_nativePDI->end_line_batch();
	}

	void NRenderPrimitiveDrawInterface::DrawDebugString(int x, int y, System::String^ text, Color4ub color)
	{
		nexus::color4ub* pColor = reinterpret_cast<nexus::color4ub*>(&color);
		pin_ptr<const wchar_t> szText = PtrToStringChars(text);

		m_nativePDI->draw_debug_string(x, y, szText, *pColor);
	}

	nexus::nrender_primitive_draw_interface* NRenderPrimitiveDrawInterface::NativePtr::get()
	{
		return m_nativePDI;
	}

	//-- class NRenderElement ----------------------------------------------------------
	NRenderElement::NRenderElement(void)
	{}

	//-- class NativeRenderElementWrapper ----------------------------------------------------------
	NativeRenderElementWrapper::NativeRenderElementWrapper(NRenderElement^ clrObj):m_clrObj(clrObj)
	{
		m_clrPDI = gcnew NRenderPrimitiveDrawInterface;
	}

	void NativeRenderElementWrapper::draw(nrender_primitive_draw_interface* PDI)
	{
		m_clrPDI->SetNativePDI(PDI);
		m_clrObj->Draw(m_clrPDI);
	}

	void NativeRenderElementWrapper::update_hit_proxy(nrender_primitive_draw_interface* PDI)
	{
		m_clrPDI->SetNativePDI(PDI);
		m_clrObj->UpdateHitProxy(m_clrPDI);
	}
}//namespace NexusEngine