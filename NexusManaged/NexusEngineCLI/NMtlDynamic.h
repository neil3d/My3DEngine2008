#pragma once
#include "nmtlcommon.h"

namespace NexusEngine
{
	public ref class NMtlDynamic :	public NMtlCommon
	{
	public:
		NMtlDynamic(String^ nameStr);
		virtual ~NMtlDynamic(void);
	};
}//namespace NexusEngine