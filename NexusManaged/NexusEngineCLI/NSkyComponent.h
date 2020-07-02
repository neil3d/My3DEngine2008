#pragma once
#include "NActorComponent.h"
#include "NMtlBase.h"
#include "NStaticMeshComponent.h"

namespace NexusEngine
{
	public ref class NSkyComponent: public NStaticMeshComponent
	{
	public:
		[Category("属性"), Description("天空的材质")]
		property NResourceLoc Material
		{
			NResourceLoc get()
			{
				return NResourceLoc(resource_location(GetNativePtr()->get_material_loc()));
			}
			void set(NResourceLoc value)
			{
				resource_location loc;
				value.ToNative(loc);
				GetNativePtr()->set_material_loc(loc);
			}
		}
		//==============================================================================================================================
	private:
		nsky_component* GetNativePtr()
		{
			return dynamic_cast<nsky_component*>(RawPtr);
		}
		//==============================================================================================================================
	public:		
		//==============================================================================================================================
		NSkyComponent(nexus::nsky_component::ptr nativeComp);
		~NSkyComponent();
	};
};
