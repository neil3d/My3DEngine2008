#include "StdAfx.h"
#include "NLocalCharacterController.h"

namespace NexusEngine
{
	NLocalCharacterController::NLocalCharacterController(void)
		: NCharacterController(new nlocal_cc)
	{
	}
}
