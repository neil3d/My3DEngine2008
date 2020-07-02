#include "StdAfx.h"
#include "NFontManager.h"
#include "NEngine.h"
#include "NRenderResourceManager.h"

using namespace System::Collections::Generic;

namespace NexusEngine
{
	NFontManager::NFontManager(void)
	{
	}

	NFontManager::~NFontManager(void)
	{
	}

	NRenderFont^ NFontManager::ConstructFont(FontDescription desc)
	{
		if( m_fontCache.ContainsKey(desc) )
		{
			return m_fontCache[desc];
		}
	
		// 如果没有包含则创建一个新的字体
		NRenderFont^ font = NEngine::Instance()->RenderResManager->NewRenderFont();
		font->Create( desc );
		
		m_fontCache[desc] = font;

		return font;
	}

	bool NFontManager::ContainsFont(FontDescription desc)
	{
		return m_fontCache.ContainsKey(desc);
	}

	bool NFontManager::RemoveFont(NexusEngine::FontDescription description)
	{
		return m_fontCache.Remove(description);
	}


	void NFontManager::ClearFont()
	{
		m_fontCache.Clear();
	}

}

