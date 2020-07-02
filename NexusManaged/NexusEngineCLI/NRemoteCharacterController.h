#pragma once
#include "NCharacterController.h"

namespace NexusEngine
{
	/** 
	* 对Native class nremote_cc的包装
	*/
	public ref class NRemoteCharacterController 
		: public NCharacterController
	{
	public:
		NRemoteCharacterController(void);
	};

}

