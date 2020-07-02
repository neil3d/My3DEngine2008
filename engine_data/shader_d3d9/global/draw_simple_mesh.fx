float4x4 g_matWorldViewPrj : SYS_WorldViewProject;
float4 g_color = float4(1,1,0,1);

struct vfInput
{
	float3 pos : POSITION;
};

void vsMain(vfInput vert,out float4 outPos : POSITION	)
{
	outPos = mul(float4(vert.pos,1),g_matWorldViewPrj);
}

float4 psMain() : COLOR
{
	return g_color;
}

technique techDefault
{
	pass p0
	{
		ZEnable = True;
		ZWriteEnable = True;
		ZFunc = LESSEQUAL;		
				
		FillMode = WireFrame;
		CullMode = None;

		VertexShader = compile vs_3_0 vsMain();
		PixelShader = compile ps_3_0 psMain();
	}
}