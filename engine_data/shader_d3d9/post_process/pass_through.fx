
texture g_texSource : TEX_SOURCE;
sampler texSampler = sampler_state 
{
    Texture   = <g_texSource>;
    AddressU  = CLAMP;
    AddressV  = CLAMP;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;
    SRGBTEXTURE = FALSE;
};

struct vsInput
{
	float3	position : POSITION;
	float2	uv		 : TEXCOORD0;
};

void vsMain(vsInput vert,
			out float4 position : POSITION,
			out float2 uv		: TEXCOORD0
			)
{
	position = float4(vert.position,1);
	uv = vert.uv;
}

float4 psMain(float2 uv : TEXCOORD0): COLOR
{
	return tex2D(texSampler, uv);
}

float4 psMain_Red(float2 uv : TEXCOORD0): COLOR
{
	return tex2D(texSampler, uv).rrra;
}

float4 psMain_Green(float2 uv : TEXCOORD0): COLOR
{
	return tex2D(texSampler, uv).ggga;
}

float4 psMain_Blue(float2 uv : TEXCOORD0): COLOR
{
	return tex2D(texSampler, uv).bbba;
}

technique techDefault
{
	pass p0
	{
		CullMode = None;
		VertexShader = compile vs_1_1 vsMain();
		PixelShader = compile ps_2_0 psMain();
	}
}

technique techRed
{
	pass p0
	{
		CullMode = None;
		VertexShader = compile vs_1_1 vsMain();
		PixelShader = compile ps_2_0 psMain_Red();
	}
}

technique techGreen
{
	pass p0
	{
		CullMode = None;
		VertexShader = compile vs_1_1 vsMain();
		PixelShader = compile ps_2_0 psMain_Green();
	}
}

technique techBlue
{
	pass p0
	{
		CullMode = None;
		VertexShader = compile vs_1_1 vsMain();
		PixelShader = compile ps_2_0 psMain_Blue();
	}
}

technique techAlphaBlend
{
	pass p0
	{
		CullMode = None;
		
		ZEnable = false;
		
		// ¿ªÆôalpha blending
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		AlphaBlendEnable = True;
		
		VertexShader = compile vs_1_1 vsMain();
		PixelShader = compile ps_2_0 psMain();
	}
}