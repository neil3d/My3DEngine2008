#include "StdAfx.h"
#include "NGameEngine.h"

namespace NexusEngine
{
	NGameEngine::NGameEngine(void):NEngine(L"ngame_engine")
	{
	}

	ngame_engine* NGameEngine::NativePtr::get()
	{
		return dynamic_cast<ngame_engine*>(RawPtr);
	}

	void NGameEngine::FrameTick(float deltaTime, NViewport^ vp)
	{
		vp->Update();
		NativePtr->frame_tick(deltaTime, *(vp->GetNaitvePtr()));
	}
}//namespace NexusEngine