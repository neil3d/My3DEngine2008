#pragma once
#include "NativeObject.h"

namespace NexusEngine
{
	using namespace gameframework;

	ref class NLevel;

	public ref class NGameLevel : public NativeObjectOwnedRaw
	{
	public:
		NGameLevel(void);

		void Attach(NLevel^ level);

		property ngame_level* NativePtr
		{
			ngame_level* get();
		}
	};

} // end of namespace NexusEngine
