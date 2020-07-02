#pragma once
#include "NMtlBase.h"
#include "NResource.h"

namespace NexusEngine
{
	public ref class NMtlCommon : public NMtlBase
	{
	public:
		NMtlCommon(String^ nameStr);
		virtual ~NMtlCommon(void);

		void SetTexture2DParameter(String^ paramName, NResourceLoc texLoc);
		void SetCubeMapParameter(String^ paramName, NResourceLoc texLoc);

		//-- Native Wrapper
	public:
		NMtlCommon(nexus::nmtl_common::ptr nativeMtl);		
	protected:
		property nmtl_common* NativePtr
		{
			nmtl_common* get();
		}
	};
}//namespace NexusEngine