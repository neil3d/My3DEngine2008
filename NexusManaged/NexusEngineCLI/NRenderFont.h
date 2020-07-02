#pragma once
#include "NRenderResource.h"
#include "FontEnum.h"
#include "FontDescription.h"
#include "Rect.h"
#include "math/Color4.h"

namespace NexusEngine
{

	/**
	* 对native class nexus::nrender_font 的包装
	* 其生命周期由托管代码代码维护
	*/
	public ref class NRenderFont
	{
		RENDER_RESOURCE_CLASS(NRenderFont, nexus::nrender_font);

	public:
		virtual void Create( FontDescription^ description );
		virtual FontDescription^ Create( System::String^ faceName, int height, int width, FontWeight weight, int mipLevels, bool italic, CharacterSet characterSet, Precision outputPrecision, FontQuality quality, PitchAndFamily pitchAndFamily );
	
		virtual int DrawString( System::String^ text, NexusEngine::Rect rectangle, DrawTextFormat format, Color4f color,Color4f shadow);
		virtual int DrawString( System::String^ text, NexusEngine::Rect rectangle, DrawTextFormat format, int color, int shadow);
		virtual int DrawString( System::String^ text, int x, int y, Color4f color,Color4f shadow);
		virtual int DrawString( System::String^ text, int x, int y, int color, int shadow);

		virtual int DrawString( System::String^ text, NexusEngine::Rect rectangle, float z ,float scale ,DrawTextFormat format, Color4f color,Color4f shadow);
		virtual int DrawString( System::String^ text, NexusEngine::Rect rectangle, float z ,float scale ,Vector2 scale_center,DrawTextFormat format, Color4f color,Color4f shadow);

		virtual int DrawString( System::String^ text, NexusEngine::Rect rectangle, float z ,float scale ,DrawTextFormat format, int color, int shadow);
		virtual int DrawString( System::String^ text, NexusEngine::Rect rectangle, float z ,float scale ,Vector2 scale_center,DrawTextFormat format, int color, int shadow);
		
		virtual Rect MeasureString( System::String^ text, DrawTextFormat format );
		virtual int MeasureString( System::String^ text, DrawTextFormat format, Rect% rectangle );
	};


} // end of namespace NexusEngine

