/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NPrimitiveComponent.h"
#include "NResource.h"

namespace NexusEngine
{
	/**
	 *	SpeedTree组件, 管理一个Speed Tree Instance, 包装nexus::nspeed_tree_component类对象
	*/
	public ref class NSpeedTreeComponent : public NPrimitiveComponent
	{
	public:
		NSpeedTreeComponent(nexus::nspeed_tree_component::ptr nativeComp);

		//!	创建内部的Speed Tree Instance
		void Create(NResourceLoc loc);

	private:
		property nspeed_tree_component* NativePtr
		{
			nspeed_tree_component* get();
		}
	};
}//namespace NexusEngine