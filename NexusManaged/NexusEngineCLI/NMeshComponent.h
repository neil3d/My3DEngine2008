/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NPrimitiveComponent.h"
#include "NMtlBase.h"

namespace NexusEngine
{
	/**
	 *	Mesh类组件的基类,包装nexus::nmesh_component对象
	*/
	public ref class NMeshComponent : public NPrimitiveComponent
	{
	public:		

		//-- Native wrapper
	public:
		NMeshComponent(nexus::nmesh_component::ptr nativeComp);
	protected:
		property nmesh_component* NativePtr
		{
			nmesh_component* get();
		}
		
		property bool bInstance
		{
			bool get()
			{
				return NativePtr->enable_instance();
			}

			void set(bool bTrue)
			{
				NativePtr->set_enable_instance(bTrue);
			}
		}
	};
}//namespace NexusEngine