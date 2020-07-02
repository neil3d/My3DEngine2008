#include "StdAfx.h"
#include "NGameLevel.h"
#include "NLevel.h"

namespace NexusEngine
{
	NGameLevel::NGameLevel(void)
		: NativeObjectOwnedRaw (new ngame_level)
	{
	}

	void NGameLevel::Attach( NLevel^ level )
	{
		if(level != nullptr)
		{
			NativePtr->attach_nav_map(level->NativePtr->get_navigation_map());
		}
	}

	ngame_level* NGameLevel::NativePtr::get()
	{
		return dynamic_cast<ngame_level*>(RawPtr);		
	}

}
