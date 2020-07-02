#pragma once
#include "NActorComponent.h"
#include "math/Color4.h"
#include "math/Vector3.h"
#include "math/Vector2.h"

namespace NexusEngine
{
	public ref class NWaterComponent
		: public NActorComponent
	{
	public:	
		DECLARE_PROPERTY(Color4f, Color,m_water_color)
		DECLARE_SINGLE_PROPERTY(float, Density,m_water_density)
		DECLARE_SINGLE_PROPERTY(float, WaterFadeDepth,m_water_soft_interact_dist)
		DECLARE_SINGLE_PROPERTY(float, FoamFadeDepth,m_foam_soft_interact_dist)
		DECLARE_SINGLE_PROPERTY(float, FoamFactor,m_foam_factor)

		//-- Native Wrapper
	public:
		NWaterComponent(nexus::nwater_component::ptr nativeComp);

		property Vector2 GridSize
		{
			void set(Vector2 value)
			{
				NativePtr->set_size((UINT)value.x,(UINT)value.y);
			}

			Vector2 get()
			{
				return Vector2::FromNative(NativePtr->get_size());
			}
		}

		property nwater_component* NativePtr
		{
			nwater_component* get();
		}
	};
}//namespace NexusEngine