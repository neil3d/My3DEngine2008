#include "StdAfx.h"
#include "NSpecialEffectInstance.h"

namespace NexusEngine
{
	NSpecialEffectInstance::NSpecialEffectInstance(nexus::nspecial_effect_instance::ptr nativeComp)
		: NPrimitiveComponent(nativeComp)
	{
	}

	nexus::nspecial_effect_instance* NSpecialEffectInstance::NativePtr::get()
	{
		return dynamic_cast<nspecial_effect_instance*>(RawPtr);
	}

	void NSpecialEffectInstance::ResetResource(NResourceSpecialEffect^ effectTemplate)
	{
		NativePtr->reset_resource( effectTemplate->GetNativeSmartPtr() );
	}
}//namespace NexusEngine