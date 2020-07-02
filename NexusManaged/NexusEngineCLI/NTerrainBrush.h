#pragma once
#include "NResource.h"

namespace NexusEngine
{
	public enum class ETrnLerpMode : System::Int32
	{
		None	= ELM_None,
		Linear	= ELM_Linear,
		Cosine	= ELM_Cosine,
		Quadratic = ELM_Quadratic,
	};

	public enum class ETrnBrushShape : System::Int32
	{
		Circle = EBS_Circle,
		Square = EBS_Square,
	};

	public enum class ETrnBurshTarget : System::Int32
	{
		[DescriptionAttribute("Height Map")]  
		Heightmap = EBT_Heightmap,
		[DescriptionAttribute("Alpha Map")]  
		Alphamap = EBT_Alphamap,
		[DescriptionAttribute("Deco Map")]  
		Decomap = EBT_Decomap,
		[DescriptionAttribute("Navigation Map")]  
		NavigationMap = EBT_NavigationMap,
		[DescriptionAttribute("Walkable Map")]  
		WalkableMap = EBT_WalkableMap,
	};

	public enum class ETrnBrushOP : System::Int32
	{
		Paint	= EOP_Paint,		
		Flatten	= EOP_Flatten,
		Smooth	= EOP_Smooth,
		Apply	= EOP_Apply,
		Noise	= EOP_Noise,
	};

	public value struct NTerrainBrush
	{
		ETrnBrushOP		op;
		ETrnBrushShape	shape;
		ETrnLerpMode	lerpMode;
		int				innerRadius;
		int				outerRadius;
		int				strength;
		int				noiseFreq;
		ETrnBurshTarget	target;
		int				targetIndex;

		void ToNative(nexus::nterrain_brush& brush);
	};

}