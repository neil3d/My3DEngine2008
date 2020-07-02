#include "StdAfx.h"
#include "NRenderResourceManager.h"
#include "NRenderFont.h"
#include "NUICanvas.h"

namespace NexusEngine
{
	NRenderResourceManager::NRenderResourceManager(INativeObj^ nativeObj)
		:m_nativeObj(nativeObj)
	{
	}
	
	nrender_resource_manager* NRenderResourceManager::NativePtr::get()
	{
		return dynamic_cast<nrender_resource_manager*>(m_nativeObj->GetRawPtr());
	}

	NRenderFont^ NRenderResourceManager::NewRenderFont()
	{
		return gcnew NRenderFont( NativePtr->alloc_render_font() );
	}

	NUICanvas^NRenderResourceManager::NewUICanvas()
	{
		return gcnew NUICanvas( NativePtr->alloc_ui_canvas() );
	}

} // end of namespace NexusEngine