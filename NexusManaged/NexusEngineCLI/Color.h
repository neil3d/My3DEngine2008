#pragma once
#include "math/Color4.h"

namespace NexusEngine
{
	[System::Serializable]
	[System::Runtime::InteropServices::StructLayout(System::Runtime::InteropServices::LayoutKind::Sequential, Pack=8)]
	public value struct Color4ub
	{
		System::Byte R,G,B,A;

		Color4ub(System::Byte r, System::Byte g, System::Byte b, System::Byte a)
		{
			R=r;
			G=g;
			B=b;
			A=a;
		}

	};
}//namespace NexusEngine