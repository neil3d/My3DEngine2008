#include "material.hlsl"
#include "vertex_factory.hlsl"


void vsMain_Mtl(vfInput vert,
			out vfInterpolants inter,
			out float4 outPos : POSITION
			)
{
	vfMakeInterpolants(vert, inter);
	
	outPos = vfTransformPos(vert);
}

float4 psMain_Mtl(vfInterpolants inter) : COLOR
{
	mtlParameters mtlParam;
	vfMakeMaterialParameters(inter, mtlParam);
	
	float4 ret = 0;
	ret.a = mtlGetOpacity(mtlParam);	
	return ret;
}

void vsMain(vfInput vert,
			out float4 outPos : POSITION)
{
	outPos = vfTransformPos(vert);
}

float4 psMain() : COLOR
{
	return float4(1,0,0,1);
}


technique techDefault
{
	pass p0
	{
		ZEnable = True;
		ZWriteEnable = True;
		ZFunc = Less;
		
		FillMode = Solid;
		ColorWriteEnable = 0;
		
		#ifdef MTL_USE_ALPHA_MASK
			AlphaTestEnable = True;
			AlphaRef = g_alphaMask;
			AlphaFunc = GreaterEqual;
		#endif
		
		#ifdef MTL_TWO_SIDED
			CullMode = None;
		#endif
		
		#ifdef MTL_USE_ALPHA_MASK
			VertexShader = compile vs_3_0 vsMain_Mtl();
			PixelShader = compile ps_3_0 psMain_Mtl();
		#else
			VertexShader = compile vs_3_0 vsMain();
			PixelShader = compile ps_3_0 psMain();
		#endif
	}
}

technique techMixedTrans
{
	pass p0
	{
		ZEnable = True;
		ZWriteEnable = True;
		ZFunc = Less;
		
		FillMode = Solid;
		ColorWriteEnable = 0;
		
		AlphaTestEnable = True;
		AlphaRef = 220;
		AlphaFunc = GreaterEqual;
				
		#ifdef MTL_TWO_SIDED
			CullMode = None;
		#endif
		
		VertexShader = compile vs_3_0 vsMain_Mtl();
		PixelShader = compile ps_3_0 psMain_Mtl();
	}
}