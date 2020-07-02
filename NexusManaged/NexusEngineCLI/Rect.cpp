#include "StdAfx.h"
#include "Rect.h"

using namespace System;
using namespace System::Globalization;

namespace NexusEngine
{
	Rect::Rect(float _left, float _top, float _right, float _buttom)
	{
		this->left = _left;
		this->right = _right;
		this->top = _top;
		this->bottom = _buttom;
	}

	Rect::Rect(Vector2 left_top,Vector2 size)
	{
		left = left_top.x;
		top = left_top.y;
		right = left + size.x;
		bottom = top + size.y;
	}

	void Rect::Offset(Vector2 offest)
	{
		left += offest.x;
		right += offest.x;
		top += offest.y;
		bottom += offest.y;
	}

	bool Rect::IsPointInRect(Vector2 point)
	{
		return Contains(point);
	}

	bool Rect::IsPointInRect(float x, float y)
	{
		return Contains(x,y);
	}

	bool Rect::Contains(float x, float y)
	{
		return ((x >= left) &&
			(x < right) &&
			(y >= top) &&
			(y < bottom));
	}

	bool Rect::Contains(Vector2 point)
	{
		return ((point.x >= left) &&
			(point.x < right) &&
			(point.y >= top) &&
			(point.y < bottom));
	}

	bool Rect::Contains(Rect rect)
	{
		return ((rect.left >= left) &&
			(rect.top >= top) &&
			(rect.right <= right) &&
			(rect.bottom <= bottom));
	}

	bool Rect::Intersection(Rect rect) 
	{
		// check for total exclusion
		return ((right > rect.left) &&
			(left < rect.right) &&
			(bottom > rect.top) &&
			(top < rect.bottom));
	}

	Rect Rect::Intersection(Rect rect1, Rect rect2) 
	{
		// check for total exclusion
		if ((rect1.right > rect2.left) &&
			(rect1.left < rect2.right) &&
			(rect1.bottom > rect2.top) &&
			(rect1.top < rect2.bottom)) 
		{

				Rect temp;

				// fill in temp with the intersection
				temp.left = (rect1.left > rect2.left) ? rect1.left : rect2.left;
				temp.right = (rect1.right < rect2.right) ? rect1.right : rect2.right;
				temp.top = (rect1.top > rect2.top) ? rect1.top : rect2.top;
				temp.bottom = (rect1.bottom < rect2.bottom) ? rect1.bottom : rect2.bottom;

				return temp;
		}

		return Rect(0.0f, 0.0f, 0.0f, 0.0f);
	}

	Rect Rect::operator +(Rect rc, Vector2 offest )
	{
		return Rect(rc.left+offest.x, rc.top+offest.y, rc.right+offest.x, rc.bottom+offest.y );
	}

	Rect Rect::operator -(Rect rc, Vector2 offest)
	{
		return Rect(rc.left-offest.x, rc.top-offest.y, rc.right-offest.x, rc.bottom-offest.y );
	}

	bool Rect::operator ==( Rect rc1, Rect rc2)
	{
		return Rect::Equals(rc1,rc2);
	}

	bool Rect::operator !=(Rect rc1,Rect rc2)
	{
		return !Rect::Equals(rc1,rc2);
	}

	String^ Rect::ToString()
	{
		return String::Format(CultureInfo::CurrentCulture,
			"left:{0} top:{1} right:{2} bottom:{3}",
			left.ToString(CultureInfo::CurrentCulture),
			top.ToString(CultureInfo::CurrentCulture),
			right.ToString(CultureInfo::CurrentCulture),
			bottom.ToString(CultureInfo::CurrentCulture) );
	}

	int Rect::GetHashCode()
	{
		return left.GetHashCode() + top.GetHashCode() + right.GetHashCode() + bottom.GetHashCode();
	}

	bool Rect::Equals(System::Object^ obj)
	{
		if( obj == nullptr )
			return false;

		if( obj->GetType() != GetType() )
			return false;

		return Equals(safe_cast<Rect>(obj));
	}

	bool Rect::Equals(Rect other)
	{
		return (left == other.left
			&& top == other.top
			&& right == other.right
			&& bottom == other.bottom);
	}

	bool Rect::Equals(Rect% rc1, Rect% rc2)
	{
		return ( rc1.left == rc2.left
			&& rc1.top == rc2.top 
			&& rc1.right == rc2.right
			&& rc1.bottom == rc2.bottom );
	}

	void Rect::ConstrainSize( Vector2 max, Vector2 min )
	{
		if(this->Width > max.x) 
		{
			this->Width = max.x;
		}
		else if(this->Width < min.x) 
		{
			this->Width = min.x;
		}

		if(this->Height > max.y) 
		{
			this->Height = max.y;
		}
		else if(this->Height < min.y) 
		{
			this->Height = min.y;
		}
	}

	void Rect::ConstrainSizeMax( Vector2 size )
	{
		if(this->Width > size.x) 
		{
			this->Width = size.x;
		}

		if(this->Height > size.y)
		{
			this->Height = size.y;
		}
	}

	void Rect::ConstrainSizeMin( Vector2 size )
	{
		if(this->Width < size.x) 
		{
			this->Width = size.x;
		}

		if(this->Height < size.y) 
		{
			this->Height = size.y;
		}
	}
} // end of namespace NexusEngine


