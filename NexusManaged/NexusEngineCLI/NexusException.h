/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once

namespace NexusEngine
{
	/**
	 *	提供一个引擎异常的基类
	*/
	public ref class NexusException : public System::Exception
	{
	public:
		NexusException(System::String^ info);
	};
}//namespace NexusEngine