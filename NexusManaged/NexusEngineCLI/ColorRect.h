#pragma once
#include "math/Color4.h"
#include "ColorRectConverter.h"


using System::Runtime::InteropServices::OutAttribute;

namespace NexusEngine
{
	using namespace NexusEngineExtension;

	[System::SerializableAttribute]
	[System::Runtime::InteropServices::StructLayout( System::Runtime::InteropServices::LayoutKind::Sequential, Pack = 8 )]
	[System::ComponentModel::TypeConverter( NexusEngine::Design::ColorRectConverter::typeid )]
	[XmlClassSerializable("ColorRect")]
	public value class ColorRect : System::IEquatable<ColorRect>
	{
	public:
		[XmlFieldSerializable("left_top")]
		Color4f left_top;
		[XmlFieldSerializable("right_top")]
		Color4f right_top;
		[XmlFieldSerializable("left_bottom")]
		Color4f left_bottom;
		[XmlFieldSerializable("right_bottom")]
		Color4f right_bottom;
	
		ColorRect(Color4f lt, Color4f rt, Color4f lb, Color4f rb);
		ColorRect(int lt, int rt, int lb, int rb);
		ColorRect(int color);
		ColorRect( Color4f color);

		/// <summary>
		///		Sets the alpha value for all colors in the color rect.
		/// </summary>
		/// <param name="alpha"></param>
		void SetAlpha(float alpha)
		{
			left_top.A = alpha;
			right_top.A = alpha;
			left_bottom.A = alpha;
			right_bottom.A = alpha;
		}

		/// <summary>
		/// Tests for equality between two objects.
		/// </summary>
		/// <param name="left">The first value to compare.</param>
		/// <param name="right">The second value to compare.</param>
		/// <returns><c>true</c> if <paramref name="left"/> has the same value as <paramref name="right"/>; otherwise, <c>false</c>.</returns>
		static bool operator == ( ColorRect left, ColorRect right );

		/// <summary>
		/// Tests for inequality between two objects.
		/// </summary>
		/// <param name="left">The first value to compare.</param>
		/// <param name="right">The second value to compare.</param>
		/// <returns><c>true</c> if <paramref name="left"/> has a different value than <paramref name="right"/>; otherwise, <c>false</c>.</returns>
		static bool operator != ( ColorRect left, ColorRect right );

		/// <summary>
		/// Converts the value of the object to its equivalent string representation.
		/// </summary>
		/// <returns>The string representation of the value of this instance.</returns>
		virtual System::String^ ToString() override;

		/// <summary>
		/// Returns the hash code for this instance.
		/// </summary>
		/// <returns>A 32-bit signed integer hash code.</returns>
		virtual int GetHashCode() override;

		/// <summary>
		/// Returns a value that indicates whether the current instance is equal to a specified object. 
		/// </summary>
		/// <param name="obj">Object to make the comparison with.</param>
		/// <returns><c>true</c> if the current instance is equal to the specified object; <c>false</c> otherwise.</returns>
		virtual bool Equals( System::Object^ obj ) override;

		/// <summary>
		/// Returns a value that indicates whether the current instance is equal to the specified object. 
		/// </summary>
		/// <param name="other">Object to make the comparison with.</param>
		/// <returns><c>true</c> if the current instance is equal to the specified object; <c>false</c> otherwise.</returns>
		virtual bool Equals( ColorRect other );

		/// <summary>
		/// Determines whether the specified object instances are considered equal. 
		/// </summary>
		/// <param name="value1">The first value to compare.</param>
		/// <param name="value2">The second value to compare.</param>
		/// <returns><c>true</c> if <paramref name="value1"/> is the same instance as <paramref name="value2"/> or 
		/// if both are <c>null</c> references or if <c>value1.Equals(value2)</c> returns <c>true</c>; otherwise, <c>false</c>.</returns>
		static bool Equals( ColorRect% value1, ColorRect% value2 );


	
		nexus::color_rect ToNative()
		{
			return nexus::color_rect(left_top.ToArgb(),right_top.ToArgb(),left_bottom.ToArgb(),right_bottom.ToArgb() );
		}

		static ColorRect FromNative( nexus::color_rect color )
		{
			return ColorRect( Color4f(color.left_top), 
				Color4f(color.right_top),
				Color4f(color.right_top),
				Color4f(color.right_bottom) );
		}
	};


} // end of namespace NexusEngine

