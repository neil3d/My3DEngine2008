/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once

namespace NexusEngine
{
	public enum class EResourceIOMode
	{
		Auto = EIO_Auto,
		Block = EIO_Block,
	};

	public enum class EResourceIOPriority
	{
		Min		= EIOP_Min,
		Low		= EIOP_Low,
		Normal	= EIOP_Normal,
		High	= EIOP_High,
		Max		= EIOP_Max
	};
}//namespace NexusEngine