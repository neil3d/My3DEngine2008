#include "StdAfx.h"
#include "NRenderFont.h"
#include "math/Color4.h"
#include <vector>

namespace NexusEngine
{

	using namespace std;

	void NRenderFont::Create(NexusEngine::FontDescription ^description)
	{
		pin_ptr<const wchar_t> pinned_text = PtrToStringChars( description->FaceName );
		RawPtr->create(pinned_text,description->Width, description->Height,static_cast<UINT>(description->Weight),description->Italic);
	}

	FontDescription^ NRenderFont::Create(System::String ^faceName, int height, int width, NexusEngine::FontWeight weight,
		int mipLevels, bool italic, NexusEngine::CharacterSet characterSet, NexusEngine::Precision outputPrecision,
		NexusEngine::FontQuality quality, NexusEngine::PitchAndFamily pitchAndFamily)
	{
		FontDescription^ new_description = gcnew FontDescription;
		new_description->FaceName = faceName;
		new_description->Height = height;
		new_description->Width = width;
		new_description->Weight = weight;
		new_description->MipLevels = mipLevels;
		new_description->Italic = italic;
		new_description->CharSet = characterSet;
		new_description->OutputPrecision = outputPrecision;
		new_description->Quality = quality;
		new_description->PitchAndFamily = pitchAndFamily;
		Create(new_description);

		return new_description;
	}

	int NRenderFont::DrawString( System::String^ text, NexusEngine::Rect rectangle, DrawTextFormat format, int color,int shadow )
	{
		pin_ptr<const wchar_t> pinned_text = PtrToStringChars( text );
		return  RawPtr->draw_text(pinned_text,rectangle.ToNative(),color,shadow,static_cast<UINT>(format));
	}

	int NRenderFont::DrawString( System::String^ text, NexusEngine::Rect rectangle, DrawTextFormat format, Color4f color,Color4f shadow)
	{
		return DrawString(text,rectangle,format,color.ToArgb(),shadow.ToArgb());
	}

	int NRenderFont::DrawString( System::String^ text, int x, int y, int color, int shadow)
	{
		pin_ptr<const wchar_t> pinned_text = PtrToStringChars( text );
		return RawPtr->draw_text( pinned_text ,x, y, color,shadow);
	}

	int NRenderFont::DrawString( System::String^ text, int x, int y, Color4f color,Color4f shadow)
	{
		return DrawString(text,x,y,color.ToArgb(),shadow.ToArgb());
	}

	int NRenderFont::DrawString( System::String^ text, NexusEngine::Rect rectangle, float z ,float scale ,DrawTextFormat format, int color, int shadow )
	{
		pin_ptr<const wchar_t> pinned_text = PtrToStringChars( text );
		return  RawPtr->draw_text(pinned_text,rectangle.ToNative(),z,scale,color,shadow,static_cast<UINT>(format));
	}

	int NRenderFont::DrawString( System::String^ text, NexusEngine::Rect rectangle, float z ,float scale ,Vector2 scale_center,DrawTextFormat format, int color, int shadow)
	{
		pin_ptr<const wchar_t> pinned_text = PtrToStringChars( text );
		return  RawPtr->draw_text(pinned_text,rectangle.ToNative(),z,scale,scale_center.ToNative(),color,shadow,static_cast<UINT>(format));
	}

	int NRenderFont::DrawString( System::String^ text, NexusEngine::Rect rectangle, float z ,float scale ,DrawTextFormat format, Color4f color,Color4f shadow )
	{
		return DrawString(text,rectangle,z,scale,format,color.ToArgb(),shadow.ToArgb());
	}

	int NRenderFont::DrawString( System::String^ text, NexusEngine::Rect rectangle, float z ,float scale ,Vector2 scale_center,DrawTextFormat format, Color4f color ,Color4f shadow)
	{
		return DrawString(text,rectangle,z,scale,scale_center,format,color.ToArgb(),shadow.ToArgb());
	}
	Rect NRenderFont::MeasureString( System::String^ text, DrawTextFormat format )
	{
		Rect rc;
		MeasureString(text,format,rc);
		return rc;
	}

	int NRenderFont::MeasureString(System::String ^text, DrawTextFormat format, Rect %rectangle)
	{
		frect nativeRect = rectangle.ToNative();
		pin_ptr<const wchar_t> pinned_text = PtrToStringChars( text );
		int ret = RawPtr->measure_string(pinned_text, static_cast<UINT>(format), nativeRect);
		rectangle = Rect::FromNative( nativeRect );
		return ret;
	}

} // end of namespace NexusEngine
