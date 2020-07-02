#include "StdAfx.h"
#include "NUICanvas.h"
#include "NRenderFont.h"
#include "NResourceTexture.h"

namespace NexusEngine
{
	void NUICanvas::Begin()
	{
		RawPtr->begin();
	}

	void NUICanvas::Flush()
	{
		RawPtr->flush();
	}

	void NUICanvas::End()
	{
		RawPtr->end();
	}

	Vector2 NUICanvas::GetViewPortSize()
	{
		return Vector2::FromNative( RawPtr->get_viewport_size() );
	}

	void NUICanvas::PushScissorRect( Rect rect )
	{
		RawPtr->push_scissor_rect( &rect.ToNative() );
	}

	void NUICanvas::PopScissorRect()
	{
		RawPtr->pop_scissor_rect();
	}

	void NUICanvas::GetScissorRect( Rect% rect )
	{
		frect nativeRect;
		RawPtr->get_scissor_rect(nativeRect);
		rect = Rect::FromNative(nativeRect);
	}

	void NUICanvas::DrawTexture(NResourceTexture2D ^texture, NexusEngine::Rect sourecRect, NexusEngine::Rect targetRect, ColorRect color)
	{
		if( texture == nullptr )
			return;

		RawPtr->draw_texture(texture->Texture2D, sourecRect.ToNative(), targetRect.ToNative(), color.ToNative() );
	}

	void NUICanvas::DrawTexture( NResourceTexture2D^ texture, Rect sourecRect, Rect targetRect, int color )
	{
		if( texture == nullptr )
			return;

		RawPtr->draw_texture(texture->Texture2D, sourecRect.ToNative(), targetRect.ToNative(), color);
	}

	void NUICanvas::DrawTexture( NResourceTexture2D^ texture, Rect sourecRect, Rect targetRect, float z,float scale, int color )
	{
		if( texture == nullptr )
			return;

		RawPtr->draw_texture(texture->Texture2D, sourecRect.ToNative(), targetRect.ToNative(), z,scale,color);
	}

	void NUICanvas::DrawTexture( NResourceTexture2D^ texture, Rect sourecRect, Rect targetRect, float z,float scale, Vector2 scale_center, int color )
	{
		if( texture == nullptr )
			return;

		RawPtr->draw_texture(texture->Texture2D, sourecRect.ToNative(), targetRect.ToNative(), z,scale,scale_center.ToNative(),color);
	}

	void NUICanvas::DrawTexture( NResourceTexture2D^ texture, Rect sourecRect, NResourceTexture2D^ alphaTexture, Rect alphaTextureRect, Rect targetRect, float rotation,int color )
	{
		if( texture == nullptr || alphaTexture==nullptr )
			return;

		RawPtr->draw_texture(texture->Texture2D, sourecRect.ToNative(), alphaTexture->Texture2D,alphaTextureRect.ToNative(),targetRect.ToNative(), rotation,color);
	}

	void NUICanvas::DrawTexture( NResourceTexture2D^ texture, Rect sourecRect, Rect targetRect, float rotation,Vector2 rotCenter, int color )
	{
		if( texture == nullptr )
			return;

		RawPtr->draw_texture(texture->Texture2D, sourecRect.ToNative(), targetRect.ToNative(), rotation,rotCenter.ToNative(),color);
	}

	void NUICanvas::DrawString(NexusEngine::NRenderFont ^font, System::String ^text, Rect targetArea, NexusEngine::DrawTextFormat format, int color,int shadow)
	{
		font->DrawString(text,targetArea,format,color,shadow);
	}

	void NUICanvas::DrawString(NexusEngine::NRenderFont ^font, System::String ^text, Rect targetArea, NexusEngine::DrawTextFormat format, Color4f color,Color4f shadow)
	{
		font->DrawString(text,targetArea,format,color,shadow);
	}

	void NUICanvas::DrawString( NRenderFont^ font,System::String^ text, int x, int y, int color,int shadow)
	{
		font->DrawString(text,x,y,color,shadow);
	}

	void NUICanvas::DrawString( NRenderFont^ font,System::String^ text, int x, int y, Color4f color,Color4f shadow)
	{
		font->DrawString(text,x,y,color,shadow);
	}

	void NUICanvas::DrawString(NexusEngine::NRenderFont ^font, System::String ^text, Rect targetArea,float z,float scale, NexusEngine::DrawTextFormat format, int color,int shadow)
	{
		font->DrawString(text,targetArea,z,scale,format,color,shadow);
	}

	void NUICanvas::DrawString(NexusEngine::NRenderFont ^font, System::String ^text, Rect targetArea, float z,float scale,NexusEngine::DrawTextFormat format, Color4f color, Color4f shadow)
	{
		font->DrawString(text,targetArea,z,scale,format,color,shadow);
	}

	void NUICanvas::DrawString( NRenderFont^ font,System::String^ text, Rect targetArea,float z,float scale,Vector2 scale_center,DrawTextFormat format, Color4f color, Color4f shadow )
	{
		font->DrawString(text,targetArea,z,scale,scale_center,format,color,shadow);
	}

	void NUICanvas::DrawString( NRenderFont^ font,System::String^ text, Rect targetArea,float z,float scale,Vector2 scale_center,DrawTextFormat format, int color, int shadow)
	{
		font->DrawString(text,targetArea,z,scale,scale_center,format,color,shadow);
	}

	void NUICanvas::DrawRectangle(Rect targetArea, ColorRect color)
	{
		RawPtr->draw_rect(targetArea.ToNative(),color.ToNative());
	}

	void NUICanvas::DrawRectangle( Rect targetArea, float z,float scale,ColorRect color )
	{
		RawPtr->draw_rect(targetArea.ToNative(),z,scale,color.ToNative());
	}

	void NUICanvas::DrawRectangle( Rect targetArea, float z,float scale,Vector2 scale_center,ColorRect color )
	{
		RawPtr->draw_rect(targetArea.ToNative(),z,scale,scale_center.ToNative(),color.ToNative());
	}

	void NUICanvas::DrawLine(float x1, float y1, float x2, float y2, int color)
	{
		fpoint points[2];
		points[0] = fpoint(x1,y1);
		points[1] = fpoint(x2,y2);
		RawPtr->draw_line( points,2,color );
	}

	void NUICanvas::DrawLine(float x1, float y1, float x2, float y2, Color4f color)
	{
		DrawLine(x1,y1,x2,y2,color.ToArgb());
	}

	void NUICanvas::DrawLine(Vector2 start, Vector2 end, int color)
	{
		DrawLine(start.x, start.y, end.x, end.y, color);
	}

	void NUICanvas::DrawLine(Vector2 start, Vector2 end, Color4f color)
	{
		DrawLine(start.x, start.y, end.x, end.y, color.ToArgb());
	}

	void NUICanvas::DrawLine( float x1, float y1, float x2,float y2, float z,float scale,Vector2 scale_center, Color4f color )
	{
		fpoint points[2];
		points[0] = fpoint(x1,y1);
		points[1] = fpoint(x2,y2);
		RawPtr->draw_line( points,2,z,scale,scale_center.ToNative(),color.ToArgb() );
	}

	void NUICanvas::DrawLine( float x1, float y1, float x2,float y2, float z,float scale,Vector2 scale_center, int color )
	{
		fpoint points[2];
		points[0] = fpoint(x1,y1);
		points[1] = fpoint(x2,y2);
		RawPtr->draw_line( points,2,z,scale,scale_center.ToNative(),color);
	}

	void NUICanvas::DrawLine( Vector2 start, Vector2 end, float z,float scale,Vector2 scale_center, Color4f color )
	{
		DrawLine(start.x, start.y, end.x, end.y, z,scale,scale_center,color);
	}

	void NUICanvas::DrawLine( Vector2 start, Vector2 end, float z,float scale,Vector2 scale_center, int color )
	{
		DrawLine(start.x, start.y, end.x, end.y, z,scale,scale_center,color);
	}

	void NUICanvas::DrawPolygon( array<Vector2>^ points, int color )
	{
		pin_ptr<Vector2> pinnedPoints = &points[0];
		RawPtr->draw_polygon( reinterpret_cast<fpoint*>( pinnedPoints ), points->Length, color );
	}

	void NUICanvas::DrawPolygon( array<Vector2>^ points, Color4f color )
	{
		DrawPolygon(points,color.ToArgb());
	}

	void NUICanvas::DrawPolygon( array<Vector2>^ points, float z,float scale,Vector2 scale_center,Color4f color )
	{
		pin_ptr<Vector2> pinnedPoints = &points[0];
		RawPtr->draw_polygon( reinterpret_cast<fpoint*>( pinnedPoints ), points->Length, z,scale,scale_center.ToNative(),color.ToArgb() );
	}

	void NUICanvas::DrawPolygon( array<Vector2>^ points, float z,float scale,Vector2 scale_center,int color )
	{
		pin_ptr<Vector2> pinnedPoints = &points[0];
		RawPtr->draw_polygon( reinterpret_cast<fpoint*>( pinnedPoints ), points->Length, z,scale,scale_center.ToNative(),color );
	}
}
