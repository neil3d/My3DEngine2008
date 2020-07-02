#include "material.hlsl"
#include "vertex_factory.hlsl"


void vsMain(vfInput vert,
			out vfInterpolants inter,
			out float4 outPos : POSITION
			)
{
	vfMakeInterpolants(vert, inter);
	
	outPos = vfTransformPos(vert);
}

float4 psMain(vfInterpolants inter) : COLOR
{
	mtlParameters mtlParam;
	vfMakeMaterialParameters(inter, mtlParam);
	
	float4 ret = mtlGetEmissiveColor(mtlParam);	
	
#if !MTL_LIGHT 
	ret += mtlGetDiffuseColor(mtlParam);
#endif

	ret.a = mtlGetOpacity(mtlParam);
	return ret;
}

technique techDefault
{
	pass p0
	{
		ZEnable = True;
		ZWriteEnable = False;
		ZFunc = LESSEQUAL;		
		
		#ifdef MTL_USE_ALPHA_MASK
			AlphaTestEnable = True;
			AlphaRef = g_alphaMask;
			AlphaFunc = GreaterEqual;
		#endif
		
		#ifdef MTL_TWO_SIDED
			CullMode = None;
		#endif

		VertexShader = compile vs_3_0 vsMain();
		PixelShader = compile ps_3_0 psMain();
	}
}