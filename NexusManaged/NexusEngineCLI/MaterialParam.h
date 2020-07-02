/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NResource.h"
#include "NMath.h"

namespace NexusEngine
{
	public enum class EMaterialParameter
	{
		None	= EMPT_None,
		Float	= EMPT_Float,
		Float2	= EMPT_Float2,
		Float3	= EMPT_Float3,
		Float4	= EMPT_Float4,
		Texture	= EMPT_Texture,
		Int		= EMPT_Int,
	};

	public enum class ETextureType
	{
		Texture2D = 0,
		CubeMap = 1,
	};

	//!	材质参数
	public value struct NMaterialParam
	{
		String^ name;					// 参数名称
		EMaterialParameter type;		// 参数类型, 根据类型不同, 下面的值分别有效

		int				 intVal;
		Vector4			 floatVal;
		NResourceLoc	 textureLoc;
		ETextureType textureType;

		void FromNative(const nexus::material_parameter& np);
	};
}//namespace NexusEngine