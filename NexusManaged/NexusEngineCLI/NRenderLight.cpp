#include "StdAfx.h"
#include "NRenderLight.h"

namespace NexusEngine
{
	//-- class NRenderLightProxy ----------------------------------------------------
	NRenderLightProxy::NRenderLightProxy(nexus::nrender_light_proxy* nativeLgt)
		: NativeObjectBorrowed(nativeLgt)
	{
	}

	nexus::nrender_light_proxy* NRenderLightProxy::NativePtr::get()
	{
		return ndynamic_cast<nexus::nrender_light_proxy>(m_obj);			
	}

	//-- class NPointLightProxy ----------------------------------------------------
	NPointLightProxy::NPointLightProxy(nexus::npoint_light_proxy* nativeLgt)
		: NRenderLightProxy(nativeLgt)
	{
	}

	nexus::npoint_light_proxy* NPointLightProxy::NativePtr::get()
	{
		return ndynamic_cast<nexus::npoint_light_proxy>(m_obj);			
	}

	//-- class NDirectionalLightProxy ----------------------------------------------------
	NDirectionalLightProxy::NDirectionalLightProxy(nexus::ndirectional_light_proxy* nativeLgt)
		: NRenderLightProxy(nativeLgt)
	{
	}

	nexus::ndirectional_light_proxy* NDirectionalLightProxy::NativePtr::get()
	{
		return ndynamic_cast<nexus::ndirectional_light_proxy>(m_obj);			
	}
}//namespace NexusEngine