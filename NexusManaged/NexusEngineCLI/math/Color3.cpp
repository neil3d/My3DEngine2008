#include "stdafx.h"
#include "Color3.h"

using namespace System;

namespace NexusEngine
{
	Color3::Color3( float red, float green, float blue )
	: R( red ), G( green ), B( blue )
	{
	}

	bool Color3::operator == ( Color3 left, Color3 right )
	{
		return Color3::Equals( left, right );
	}

	bool Color3::operator != ( Color3 left, Color3 right )
	{
		return !Color3::Equals( left, right );
	}

	int Color3::GetHashCode()
	{
		return R.GetHashCode() + G.GetHashCode() + B.GetHashCode();
	}

	bool Color3::Equals( Object^ value )
	{
		if( value == nullptr )
			return false;

		if( value->GetType() != GetType() )
			return false;

		return Equals( safe_cast<Color3>( value ) );
	}

	bool Color3::Equals( Color3 value )
	{
		return ( R == value.R && G == value.G && B == value.B );
	}

	bool Color3::Equals( Color3% value1, Color3% value2 )
	{
		return ( value1.R == value2.R && value1.G == value2.G && value1.B == value2.B );
	}
}