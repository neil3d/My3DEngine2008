#pragma once
#include "NativeObject.h"

namespace NexusEngine
{
	public ref class NSkeletalAnimSequence : public NativeObjectOwnedBySharedPtr
	{
	public:
		NSkeletalAnimSequence(nskeletal_anim_sequence::ptr nativeSequence);

		property nskeletal_anim_sequence* NativePtr
		{
			nskeletal_anim_sequence* get()
			{
				return dynamic_cast<nskeletal_anim_sequence*>(RawPtr);
			}
		}

		property String^ Name
		{
			String^ get()
			{
				return gcnew String(NativePtr->m_name.c_str());
			}
			void set(String^ value)
			{
				pin_ptr<const wchar_t> nativeName = PtrToStringChars(value);
				NativePtr->m_name = nativeName;
			}
		}

		property float Length
		{
			float get()
			{
				return NativePtr->m_length;
			}
		}

		property UInt32 FrameCount
		{
			UInt32 get()
			{
				return NativePtr->get_num_frame();
			}
		}

		/// <summary>
		/// Converts the value of the object to its equivalent string representation.
		/// </summary>
		/// <returns>The string representation of the value of this instance.</returns>
		virtual System::String^ ToString() override;

	};
}
