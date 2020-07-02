#pragma once
#include "NPrimitiveComponent.h"
#include "NResourceSpecialEffect.h"

namespace NexusEngine
{
	public ref class NSpecialEffectInstance : public NPrimitiveComponent
	{
	public:
		void ResetResource(NResourceSpecialEffect^ effectTemplate);
		void Play()
		{
			NativePtr->play();
		}

		property bool Visible
		{
			bool get()
			{
				return NativePtr->is_visible();
			}
			void set(bool value)
			{
				NativePtr->set_visible(value);
			}
		}

		//--
	public:
		NSpecialEffectInstance(nexus::nspecial_effect_instance::ptr nativeComp);
	protected:
		property nexus::nspecial_effect_instance* NativePtr
		{
			nexus::nspecial_effect_instance* get();
		}
	};
}//namespace NexusEngine