#include "StdAfx.h"
#include "NStartPointActor.h"
namespace NexusEngine
{
	NStartPointActor::NStartPointActor(nexus::nstart_point_actor::ptr nativeObj)
		: NActor(nativeObj)
	{
	}

	nstart_point_actor* NStartPointActor::NativePtr::get()
	{
		return dynamic_cast<nstart_point_actor*>(RawPtr);
	}

	nstart_point_actor::ptr NStartPointActor::SmartPtr::get()
	{
		return boost::dynamic_pointer_cast<nstart_point_actor>(*m_smart_ptr);
	}


} // end of namespace NexusEngine