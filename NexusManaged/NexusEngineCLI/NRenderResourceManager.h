#pragma once
#include "NativeObject.h"

namespace NexusEngine
{
	ref class NRenderFont;
	ref class NUICanvas;

	/**
	* 包装Native class nexus::nrender_resource_manager
	*/
	public ref class NRenderResourceManager
	{
	public:	
		// 创建一个新的字体
		NRenderFont^ NewRenderFont();

		// 创建一个新的UI Canvas
		NUICanvas^ NewUICanvas();

	public:
		explicit NRenderResourceManager(INativeObj^ nativeObj);

		property nrender_resource_manager* NativePtr
		{
			nrender_resource_manager* get();
		}

	private:
		INativeObj^		m_nativeObj;
	};

} // end of namespace NexusEngine
