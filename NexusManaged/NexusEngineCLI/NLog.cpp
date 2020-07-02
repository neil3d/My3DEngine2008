#include "StdAfx.h"
#include "NLog.h"

namespace NexusEngine
{
	NNativeLog::NNativeLog(void)
	{
	}

	String^ NNativeLog::GetLastException()
	{
		return gcnew String( nexus::nlog::instance()->get_last_exception().c_str() );
	}
}//namespace NexusEngine