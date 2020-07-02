#pragma once
#include "NResource.h"

namespace NexusEngine
{
	public ref class NMtlTechShader : public NResource
	{
	public:
		NMtlTechShader(nexus::nmtl_tech_shader::ptr nativeRes);

		property nexus::nmtl_tech_shader::ptr NativePtr
		{
			nexus::nmtl_tech_shader::ptr get();
		}
	};
}//namespace NexusEngine