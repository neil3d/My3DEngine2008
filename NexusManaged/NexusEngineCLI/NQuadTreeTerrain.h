/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NPrimitiveComponent.h"

namespace NexusEngine
{
	/**
	 *	地形组件包装
	*/
	public ref class NQuadTreeTerrain : public NPrimitiveComponent
	{
	public:
		NQuadTreeTerrain(nexus::nprimitive_component::ptr nativeComp);		
	};
}//namespace NexusEngine