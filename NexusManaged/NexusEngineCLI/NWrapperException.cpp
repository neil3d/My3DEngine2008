#include "StdAfx.h"
#include "NWrapperException.h"

namespace NexusEngine
{
	NexusNativeException::NexusNativeException(System::String^ info):NexusException(info)
	{
	}

	NexusNativeException::NexusNativeException(const TCHAR* szInfo)
		: NexusException(gcnew System::String(szInfo))
	{}

	NexusNativeException::NexusNativeException(const char* szInfo)
		: NexusException(gcnew System::String(szInfo))
	{}
}//namespace NexusEngine