#pragma once
#include "NMeshComponent.h"

namespace NexusEngine
{
	public ref class NShapeComponent
		: public NMeshComponent
	{
	public:
		void CreateBox(float width, float height, float depth);
		void CreateSphere(float radius, int numRings, int numSections);
		void SetMaterial(NMaterial^ mtl);

		//-- Native Wrapper
	public:
		NShapeComponent(nexus::nshape_component::ptr nativeComp);
	protected:
		property nshape_component* NativePtr
		{
			nshape_component* get();
		}		
	};
}//namespace NexusEngine