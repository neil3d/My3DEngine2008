#pragma once
#include "NCharacterController.h"

namespace NexusEngine
{
	/** 
	* 对Native class nlocal_cc的包装
	*/
	public ref class NLocalCharacterController
		: public NCharacterController
	{
	public:
		NLocalCharacterController(void);
	};
}
