/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once

namespace NexusEngine
{
	public enum class ERenderMode : System::Int32
	{
		Wireframe	= ERM_Wireframe,
		Unlit		= ERM_Unlit,
		Lit			= ERM_Lit,
		LightingOnly= ERM_LightingOnly
	};
}//namespace NexusEngine