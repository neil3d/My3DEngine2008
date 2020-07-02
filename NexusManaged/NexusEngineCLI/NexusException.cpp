#include "StdAfx.h"
#include "NexusException.h"

namespace NexusEngine
{
	NexusException::NexusException(System::String^ info):System::Exception(info)
	{
	}
}//namespace NexusEngine