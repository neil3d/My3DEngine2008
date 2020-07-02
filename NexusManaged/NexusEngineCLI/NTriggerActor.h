#pragma once
#include "NActor.h"
#include "NMath.h"

namespace NexusEngine
{
	/**
	* 包装nexus::ntrigger_actor
	*/
	public ref class NTriggerActor 
		: public NActor
	{
		//-- Native wrapper
	public:
		NTriggerActor(nexus::ntrigger_actor::ptr nativeObj);

		property ntrigger_actor::ptr SmartPtr
		{
			ntrigger_actor::ptr get();
		}	
		
		// info_data_base提供的可供编辑的数据
		[Browsable(false)]
		[Category("Trigger")]
		property System::String^ TriggerID
		{
			String^ get()
			{
				return gcnew System::String(NativePtr->get_data().name.name_str.c_str());
			}
			void set(String^ value)
			{
				pin_ptr<const wchar_t> szNewName = PtrToStringChars(value);
				NativePtr->get_data().name.reset(szNewName);
			}
		}

		// info_data_base提供的可供编辑的数据
		[Category("Trigger")]
		property System::String^ FriendName
		{
			String^ get()
			{
				return gcnew System::String(NativePtr->get_data().friend_name.c_str());
			}
			void set(String^ value)
			{
				pin_ptr<const wchar_t> szNewName = PtrToStringChars(value);
				NativePtr->get_data().friend_name = szNewName;
			}
		}

		// info_data_base提供的可供编辑的数据
		[Category("Trigger")]
		property System::String^ Description
		{
			String^ get()
			{
				return gcnew System::String(NativePtr->get_data().description.c_str());
			}
			void set(String^ value)
			{
				pin_ptr<const wchar_t> szNewName = PtrToStringChars(value);
				NativePtr->get_data().description = szNewName;
			}
		}

		[Category("Trigger")]
		property float Radius
		{
			float get()
			{
				return NativePtr->get_data().radius;
			}
			void set(float value)
			{
				NativePtr->get_data().radius = value;
			}
		}

		[Category("Trigger")]
		property float DelayTime
		{
			float get()
			{
				return NativePtr->get_data().delay_time;
			}
			void set(float value)
			{
				NativePtr->get_data().delay_time = value;
			}
		}

		[Category("Trigger")]
		property bool Locked
		{
			bool get()
			{
				return NativePtr->get_data().locked;
			}
			void set(bool value)
			{
				NativePtr->get_data().locked = value;
			}
		}

		[Category("Trigger")]
		property System::String^ TransferMap
		{
			String^ get()
			{
				return gcnew System::String(NativePtr->get_data().transfer_map.name_str.c_str());
			}
			void set(String^ value)
			{
				pin_ptr<const wchar_t> szNewName = PtrToStringChars(value);
				NativePtr->get_data().transfer_map.reset(szNewName);
			}
		}

		[Category("Trigger")]
		property System::String^ TransferStartPoint
		{
			String^ get()
			{
				return gcnew System::String(NativePtr->get_data().start_point.name_str.c_str());
			}
			void set(String^ value)
			{
				pin_ptr<const wchar_t> szNewName = PtrToStringChars(value);
				NativePtr->get_data().start_point.reset(szNewName);
			}
		}

		[Category("Trigger")]
		property System::String^ InvokeScript
		{
			String^ get()
			{
				return gcnew System::String(NativePtr->get_data().invoke_script.name_str.c_str());
			}
			void set(String^ value)
			{
				pin_ptr<const wchar_t> szNewName = PtrToStringChars(value);
				NativePtr->get_data().invoke_script.reset(szNewName);
			}
		}


	protected:
		property ntrigger_actor* NativePtr
		{
			ntrigger_actor* get();
		}	
	};

} // end of namespace NexusEngine
