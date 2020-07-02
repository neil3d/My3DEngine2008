#pragma once
#include "math/Color4.h"
#include "ColorConverter.h"

namespace NexusEngine
{
	using namespace NexusEngineExtension;

	[System::Serializable]
	[System::Runtime::InteropServices::StructLayout(System::Runtime::InteropServices::LayoutKind::Sequential, Pack=8)]
	[System::ComponentModel::TypeConverter( NexusEngine::Design::Color4ubConverter::typeid )]
	[XmlClassSerializable("Color4ub",false)]
	public value struct Color4ub
	{
		[XmlFieldSerializable("R")]
		System::Byte R;
		[XmlFieldSerializable("G")]
		System::Byte G;
		[XmlFieldSerializable("B")]
		System::Byte B;
		[XmlFieldSerializable("A")]
		System::Byte A;

		Color4ub(System::Byte r, System::Byte g, System::Byte b, System::Byte a)
		{
			R=r;
			G=g;
			B=b;
			A=a;
		}

	};
}//namespace NexusEngine