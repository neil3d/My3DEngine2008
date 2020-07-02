#pragma once
#include "NActorComponent.h"
#include "math/Color4.h"
#include "math/Vector3.h"
#include "math/Vector2.h"

namespace NexusEngine
{
	public enum class EShapeType : System::Int32
	{
		Sphere		=	nexus::nfog_volume_component::e_sphere,
		Box			=	nexus::nfog_volume_component::e_box
	};

	public ref class NFogComponent
		: public NActorComponent
	{
	public:	
		property EShapeType ShapeType
		{
			EShapeType get()
			{
				return (NexusEngine::EShapeType)(NativePtr->shape_type);
			}

			void set(EShapeType val)
			{
				NativePtr->shape_type = (nexus::nfog_volume_component::eshape)val;
			}
		}

		DECLARE_PROPERTY(Color4f, Color,color)
		DECLARE_SINGLE_PROPERTY(float, Density,global_density)
		DECLARE_SINGLE_PROPERTY(float, FogStartDistance,fog_start_distance)
		DECLARE_PROPERTY(Vector3, BasePoint,base_point)
		DECLARE_PROPERTY(Vector3, DirScaled,dir_scaled)
		DECLARE_PROPERTY(Vector2, SoftEdges,soft_edges)
		
		//-- Native Wrapper
	public:
		NFogComponent(nexus::nfog_volume_component::ptr nativeComp);

		property nfog_volume_component* NativePtr
		{
			nfog_volume_component* get();
		}
	};
}//namespace NexusEngine