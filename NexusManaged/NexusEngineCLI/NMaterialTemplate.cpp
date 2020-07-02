#include "StdAfx.h"
#include "NMaterialTemplate.h"

namespace NexusEngine
{
	NMaterialTemplate::NMaterialTemplate(nexus::nmaterial_template::ptr nativeTemp):NResource(nativeTemp)
	{
	}

	nexus::nmaterial_template::ptr NMaterialTemplate::NativePtr::get()
	{
		return boost::dynamic_pointer_cast<nmaterial_template>(*m_nativeResPtr);
	}
}//namespace NexusEngine