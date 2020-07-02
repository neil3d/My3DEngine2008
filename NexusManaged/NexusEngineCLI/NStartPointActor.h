#pragma once
#include "NActor.h"
#include "NMath.h"

namespace NexusEngine
{
	/**
	* °ü×°nexus::nstart_point_actor
	*/
	public ref class NStartPointActor 
		: public NActor
	{
		//-- Native wrapper
	public:
		NStartPointActor(nexus::nstart_point_actor::ptr nativeObj);

		property nstart_point_actor::ptr SmartPtr
		{
			nstart_point_actor::ptr get();
		}	

	protected:
		property nstart_point_actor* NativePtr
		{
			nstart_point_actor* get();
		}	
	};

} // end of namespace NexusEngine
