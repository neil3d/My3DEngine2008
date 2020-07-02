#include "StdAfx.h"
#include "NexusException.h"

namespace NexusEngine
{
	NexusException::NexusException(System::String^ info):System::ApplicationException(info)
	{
	}
}//namespace NexusEngine