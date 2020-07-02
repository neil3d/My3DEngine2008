#include "StdAfx.h"
#include "NMtlTechShader.h"

namespace NexusEngine
{
	NMtlTechShader::NMtlTechShader(nexus::nmtl_tech_shader::ptr nativeRes):NResource(nativeRes)
	{
	}

	nexus::nmtl_tech_shader::ptr NMtlTechShader::NativePtr::get()
	{
		return boost::dynamic_pointer_cast<nmtl_tech_shader>(*m_nativeResPtr);
	}
}//namespace NexusEngine