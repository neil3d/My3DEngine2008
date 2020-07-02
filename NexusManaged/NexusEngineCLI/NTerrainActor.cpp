#include "StdAfx.h"
#include "NTerrainActor.h"

namespace NexusEngine
{
	NTerrainActor::NTerrainActor(nexus::nterrain_actor::ptr nativeTrn):NActor(nativeTrn)
	{
	}

	nterrain_actor* NTerrainActor::NativePtr::get()
	{
		return dynamic_cast<nterrain_actor*>(RawPtr);
	}

	nterrain_actor::ptr NTerrainActor::GetSmartPtr()
	{
		return boost::dynamic_pointer_cast<nterrain_actor>(*m_smart_ptr);
	}

	void NTerrainActor::Create(UInt32 w, UInt32 h, UInt16 initH, UInt32 chunkSize)
	{
		NativePtr->create(w, h, initH, chunkSize);
		this->NativeActorLoaded();
	}

	void NTerrainActor::Move(Vector3 pos, Vector3 scale)
	{
		nexus::vector3* npos = reinterpret_cast<nexus::vector3*>(&pos);
		nexus::vector3* nscale = reinterpret_cast<nexus::vector3*>(&scale);
		NativePtr->move(*npos, *nscale);
	}
}//namespace NexusEngine