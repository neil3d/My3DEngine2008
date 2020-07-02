#pragma once
#include "NRenderResource.h"
#include "FontEnum.h"
#include "Rect.h"
#include "math/Color4.h"
#include "ColorRect.h"

namespace NexusEngine
{
	ref class NRenderFont;
	ref class NResourceTexture2D;

	public ref class NUICanvas
	{
		RENDER_RESOURCE_CLASS(NUICanvas, nexus::nui_canvas);

	public:
		virtual void Begin();

		virtual Vector2 GetViewPortSize();

		virtual void PushScissorRect( Rect rect );
		virtual void PopScissorRect();
		virtual void GetScissorRect( Rect% rect );

		virtual void DrawTexture(NResourceTexture2D^ texture, Rect sourecRect, Rect targetRect, ColorRect color );
		virtual void DrawTexture(NResourceTexture2D^ texture, Rect sourecRect, Rect targetRect, int color );
		virtual void DrawTexture(NResourceTexture2D^ texture, Rect sourecRect, NResourceTexture2D^ alphaTexture, Rect alphaTextureRect, Rect targetRect,float rotation, int color );
		virtual void DrawTexture(NResourceTexture2D^ texture, Rect sourecRect, Rect targetRect, float z,float scale, int color );
		virtual void DrawTexture(NResourceTexture2D^ texture, Rect sourecRect, Rect targetRect, float rotation,Vector2 rotCenter, int color );
		virtual void DrawTexture(NResourceTexture2D^ texture, Rect sourecRect, Rect targetRect, float z,float scale, Vector2 scaleCenter, int color );
		
		virtual void DrawString(NRenderFont^ font,System::String^ text, Rect targetArea,float z,float scale,DrawTextFormat format, Color4f color, Color4f shadow);
		virtual void DrawString(NRenderFont^ font,System::String^ text, Rect targetArea,float z,float scale,DrawTextFormat format, int color, int shadow);
		virtual void DrawString(NRenderFont^ font,System::String^ text, Rect targetArea,float z,float scale,Vector2 scale_center,DrawTextFormat format, Color4f color, Color4f shadow);
		virtual void DrawString(NRenderFont^ font,System::String^ text, Rect targetArea,float z,float scale,Vector2 scale_center,DrawTextFormat format, int color, int shadow);

		virtual void DrawString(NRenderFont^ font,System::String^ text, Rect targetArea,DrawTextFormat format, Color4f color, Color4f shadow);
		virtual void DrawString(NRenderFont^ font,System::String^ text, Rect targetArea,DrawTextFormat format, int color, int shadow);
		virtual void DrawString( NRenderFont^ font,System::String^ text, int x, int y, Color4f color, Color4f shadow);
		virtual void DrawString( NRenderFont^ font,System::String^ text, int x, int y, int color, int shadow);

		virtual void DrawRectangle( Rect targetArea, ColorRect color );
		virtual void DrawRectangle( Rect targetArea, float z,float scale,ColorRect color );
		virtual void DrawRectangle( Rect targetArea, float z,float scale,Vector2 scale_center,ColorRect color );

		virtual void DrawLine(Vector2 start, Vector2 end, Color4f color);
		virtual void DrawLine(Vector2 start, Vector2 end, int color);
		virtual void DrawLine(Vector2 start, Vector2 end, float z,float scale,Vector2 scale_center, Color4f color);
		virtual void DrawLine(Vector2 start, Vector2 end, float z,float scale,Vector2 scale_center, int color);
		virtual void DrawLine( float x1, float y1, float x2,float y2, Color4f color );
		virtual void DrawLine( float x1, float y1, float x2,float y2, int color );
		virtual void DrawLine( float x1, float y1, float x2,float y2, float z,float scale,Vector2 scale_center, Color4f color );
		virtual void DrawLine( float x1, float y1, float x2,float y2, float z,float scale,Vector2 scale_center, int color );

		virtual void DrawPolygon( array<Vector2>^ points, Color4f color );
		virtual void DrawPolygon( array<Vector2>^ points, int color );
		virtual void DrawPolygon( array<Vector2>^ points, float z,float scale,Vector2 scale_center,Color4f color );
		virtual void DrawPolygon( array<Vector2>^ points, float z,float scale,Vector2 scale_center,int color );

		virtual void Flush();
		virtual void End();
	};

}