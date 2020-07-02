#include "StdAfx.h"
#include "ColorRect.h"

using namespace System;
using namespace System::Globalization;

namespace NexusEngine
{
	ColorRect::ColorRect(int color)
	{
		left_top = Color4f(color);
		left_bottom = Color4f(color);
		right_top = Color4f(color);
		right_bottom  = Color4f(color);
	}

	ColorRect::ColorRect(Color4f color)
	{
		left_top = color;
		left_bottom = color;
		right_top = color;
		right_bottom = color;
	}

	ColorRect::ColorRect(int lt, int rt, int lb, int rb)
	{
		left_top = Color4f(lt);
		left_bottom = Color4f(lb);
		right_top = Color4f(rt);
		right_bottom  = Color4f(rb);
	}

	ColorRect::ColorRect(Color4f lt, Color4f rt, Color4f lb, Color4f rb)
	{
		left_top = lt;
		left_bottom = lb;
		right_top = rt;
		right_bottom = rb;
	}

	bool ColorRect::operator == ( ColorRect left, ColorRect right )
	{
		return ColorRect::Equals( left, right );
	}

	bool ColorRect::operator != ( ColorRect left, ColorRect right )
	{
		return !ColorRect::Equals( left, right );
	}

	String^ ColorRect::ToString()
	{
		return String::Format( CultureInfo::CurrentCulture,
			"left_top:{0} right_top:{1} left_bottom:{2} right_bottom:{3}",
			left_top.ToString(), 
			right_top.ToString(),
			left_bottom.ToString(),
			right_bottom.ToString() );
	}

	int ColorRect::GetHashCode()
	{
		return left_top.GetHashCode() + right_top.GetHashCode() + left_bottom.GetHashCode() + right_bottom.GetHashCode();
	}

	bool ColorRect::Equals( Object^ value )
	{
		if( value == nullptr )
			return false;

		if( value->GetType() != GetType() )
			return false;

		return Equals( safe_cast<ColorRect>( value ) );
	}

	bool ColorRect::Equals( ColorRect value )
	{
		return ( left_top == value.left_top 
			&& right_top == value.right_top 
			&& left_bottom == value.left_bottom 
			&& right_bottom == value.right_bottom );
	}

	bool ColorRect::Equals( ColorRect% value1, ColorRect% value2 )
	{
		return ( value1.left_top == value2.left_top 
			&& value1.right_top == value2.right_top
			&& value1.left_bottom == value2.left_bottom
			&& value1.right_bottom == value2.right_bottom );
	}

} // end of namespace NexusEngine