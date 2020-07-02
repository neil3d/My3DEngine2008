/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NResource.h"

namespace NexusEngine
{
	/**
	 *	贴图资源对象, 包装nexus::nresource_texture
	*/
	public ref class NResourceTexture : public NResource
	{
	public:
		NResourceTexture(nresource_texture::ptr nativeRes);
	};
}//namespace NexusEngine