#include "vertex_factory.hlsl"


void vsMain(vfInput vert,	
			out float4 outPos : POSITION,
			out float3 outNormal : TEXCOORD0
			)
{
	outPos = vfTransformPos(vert);
	outNormal = vfGetWorldNormal(vert);
}

float4 psMain(float3 normal : TEXCOORD0) : COLOR
{
	float3 dummyLight = float3(-1,1,1);
	dummyLight = normalize(dummyLight);
	
	float d = saturate(dot(normal, dummyLight));
	d = saturate(d+0.8f);
	
	return float4(d,d,d,1);
}

technique techDefault
{
	pass p0
	{
		ZEnable = True;
		ZWriteEnable = False;
		ZFunc = LESSEQUAL;
		DepthBias = -0.0002;	// 把这个设为正数，则会产生描边的效果
		SlopeScaleDepthBias = 1.9;
		
		FillMode = WireFrame;
		CullMode = None;
		ColorWriteEnable = RED|GREEN|BLUE|ALPHA;
		VertexShader = compile vs_3_0 vsMain();
		PixelShader = compile ps_3_0 psMain();
	}
}