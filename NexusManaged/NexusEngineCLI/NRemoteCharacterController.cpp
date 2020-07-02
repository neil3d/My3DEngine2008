#include "StdAfx.h"
#include "NRemoteCharacterController.h"

namespace NexusEngine
{
	NRemoteCharacterController::NRemoteCharacterController(void)
		: NCharacterController(new nremote_cc)
	{
	}
}
