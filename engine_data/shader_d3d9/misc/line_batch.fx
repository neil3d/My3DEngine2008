
float4x4 g_matWorldViewPrj : SYS_WorldViewProject;

struct LineVertex
{
	float3 pos : POSITION;
	float4 color : COLOR0;
};

void vsMain(LineVertex vert,
			out float4 outPos : POSITION,
			out float4 outColor : COLOR0)
{
	outPos = mul(float4(vert.pos,1),g_matWorldViewPrj);
	outColor = vert.color;
}

float4 psMain(float4 inColor: COLOR0) : COLOR
{
	return inColor;
}

technique techOpaque
{
	pass p0
	{
		ZEnable = True;
		ZWriteEnable = True;
		ZFunc = LESSEQUAL;
		
		VertexShader = compile vs_3_0 vsMain();
		PixelShader = compile ps_3_0 psMain();
	}
}		
