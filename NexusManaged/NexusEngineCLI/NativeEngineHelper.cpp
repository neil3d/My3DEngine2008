#include "StdAfx.h"
#include "NativeEngineHelper.h"
#include "timer/ntimestamp.h"

namespace NexusEngine
{

	float NativeEngineHelper::GetTimeSeconds()
	{
		return nexus::get_time();
	}

	System::UInt32 NativeEngineHelper::CheckCRC( System::String^ nameStr )
	{
		pin_ptr<const wchar_t> nativeStr = PtrToStringChars(nameStr);
		nname name(nativeStr);
		return name.name_crc;
	}
}
