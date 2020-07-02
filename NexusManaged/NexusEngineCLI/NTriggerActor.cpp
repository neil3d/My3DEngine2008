#include "StdAfx.h"
#include "NTriggerActor.h"

namespace NexusEngine
{
	NTriggerActor::NTriggerActor(nexus::ntrigger_actor::ptr nativeObj)
		: NActor(nativeObj)
	{
	}

	ntrigger_actor* NTriggerActor::NativePtr::get()
	{
		return dynamic_cast<ntrigger_actor*>(RawPtr);
	}

	ntrigger_actor::ptr NTriggerActor::SmartPtr::get()
	{
		return boost::dynamic_pointer_cast<ntrigger_actor>(*m_smart_ptr);
	}

} // end of namespace NexusEngine