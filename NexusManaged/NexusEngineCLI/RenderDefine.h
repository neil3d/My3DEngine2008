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

	public enum class ETransparentType : System::Int32
	{
		Opaque			= EOpaque,	
		Translucent		= ETranslucent,	
		Additive			= 	EAdditive	
	};

	public enum class EDrawingPolicy
	{
		PreDepth			= EDP_PreDepth,
		BaseColor			= EDP_BaseColor,
		Unlit					= EDP_Unlit,
		Wireframe		= EDP_Wireframe,
		PointLight			= EDP_Point_Light,	//
		SpotLight			= EDP_Spot_Light,	//
		DirectionalLight= EDP_Directional_Light		
	};


}//namespace NexusEngine