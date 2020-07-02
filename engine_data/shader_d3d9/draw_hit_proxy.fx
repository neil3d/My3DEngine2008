#include "vertex_factory.hlsl"

float4	g_hitID : Sys_HitProxyID;

// todo: 材质Alpha Mask如何处理？Hit Proxy占用了RGBA，Alpha如何输出
// todo: 双面材质如何处理
void vsMain(vfInput vert,
			out float4 outPos : POSITION)
{
	outPos = vfTransformPos(vert);	
}

float4 psMain() : COLOR
{
	return g_hitID;
}


technique techDefault
{
	pass p0
	{
		ZEnable = True;
		ZWriteEnable = False;
		ZFunc = LESSEQUAL;
		
		FillMode = Solid;		
		
		VertexShader = compile vs_3_0 vsMain();
		PixelShader = compile ps_3_0 psMain();
	}
}

technique techDisableZ
{
	pass p0
	{
		ZEnable = False;
		ZWriteEnable = False;
		ZFunc = LESSEQUAL;
		
		FillMode = Solid;		
		
		VertexShader = compile vs_3_0 vsMain();
		PixelShader = compile ps_3_0 psMain();
	}
}