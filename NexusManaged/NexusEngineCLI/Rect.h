#pragma once
#include "math/Vector2.h"
#include "RectConverter.h"

using System::Runtime::InteropServices::OutAttribute;
namespace NexusEngine
{
	using namespace NexusEngineExtension;

	/// <summary>
	/// 定义一个2D矩形区域
	/// </summary>
	[System::SerializableAttribute]
	[System::Runtime::InteropServices::StructLayout( System::Runtime::InteropServices::LayoutKind::Sequential, Pack = 8 )]
	[System::ComponentModel::TypeConverter( NexusEngine::Design::RectConverter::typeid )]
	[XmlClassSerializable("Rect",false)]
	public value class Rect : System::IEquatable<Rect>
	{
	public:
		[XmlFieldSerializable("left")]
		float left;
		[XmlFieldSerializable("top")]
		float top;
		[XmlFieldSerializable("right")]
		float right;
		[XmlFieldSerializable("bottom")]
		float bottom;

	public:
		Rect(float left,float top, float right, float buttom );
		Rect(Vector2 left_top,Vector2 size);

		/// <summary>
		///	获取矩形区域的宽度
		/// </summary>
		property float Width
		{
			float get() { return ( right - left );}
			void set(float value)
			{
				right = left + (value < 0 ? 0 : value);
			}
		}

		/// <summary>
		///	获取矩形区域的高度
		/// </summary>
		property float Height
		{
			float get() {return (bottom - top); }
			void set(float value)
			{
				bottom = top + (value < 0 ? 0 : value);
			}
		}

		property Vector2 Size
		{
			Vector2 get() { return Vector2(right - left,bottom - top); }
			void  set(Vector2 val) { right = left + val.x; bottom = top + val.y; }
		}

		property Vector2 Position
		{
			Vector2 get() { return Vector2(left,top); }
			void set(Vector2 val) { left = val.x; top = val.y; }
		}

		/// <summary>
		///	平移<value>offest</value>
		/// </summary>
		void Offset(Vector2 offest);
		bool IsPointInRect(Vector2 point);
		bool IsPointInRect(float x, float y);
		bool Contains(float x, float y);
		bool Contains(Vector2 rect);
		bool Contains(Rect rect);
		bool Intersection(Rect rect);
		static Rect Intersection(Rect rect1, Rect rect2);

		static Rect operator + (Rect rect, Vector2 offest);
		static Rect operator - (Rect rect, Vector2 offest);
		static bool operator == (Rect left, Rect right);
		static bool operator != (Rect left, Rect right);

		// 将矩形限定在一定范围之内
		void ConstrainSize(Vector2 max, Vector2 min);
		void ConstrainSizeMax(Vector2 size);
		void ConstrainSizeMin(Vector2 size);


		//-- 重载IEquatable接口
		virtual System::String^ ToString() override;
		virtual int GetHashCode() override;
		virtual bool Equals(System::Object^ obj) override;
		virtual bool Equals(Rect other);
		static bool Equals(Rect% rc1, Rect% rc2);

		//--
		static Rect FromNative(const nexus::frect& nrc)
		{
			return Rect(nrc.left, nrc.top, nrc.right, nrc.bottom);
		}

		nexus::frect ToNative()
		{
			return nexus::frect(left,top,right,bottom);
		}

	};

} // end of namespace NexusEngine
