#pragma once

namespace NexusEngine
{
	ref class PointerHelper sealed
	{
	private:
		PointerHelper(void)
		{
		}
	public:

		generic<typename T> where T : System::ValueType
			static T GetValue(IntPtr address)
		{
			return *(interior_ptr<T>)(void *)address;
		}

		generic<typename T> where T : System::ValueType
			static void UnsafeSetValue(IntPtr address, T value)
		{
			interior_ptr<T> ptr = (interior_ptr<T>)(void*)address;
			*ptr = value;
		}

		generic<typename T>where T : System::ValueType
			static IntPtr UnsafeGetAddress(T %value)
		{
			pin_ptr<T> pp = &value;
			return IntPtr((void*)pp);
		}

	};
}