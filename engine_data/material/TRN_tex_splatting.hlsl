#define NEXUS_MATERIAL
#define MTL_LIGHT 1
#define MTL_NUM_TEXCOORD 5

#define MTL_NEED_VERT_NORMAL
#define TRN_TEX_SPLATTING
#define VERTEX_LIGHTING

texture texAlpha:MTL_LayerAlpha;

texture texColor0:MTL_LayerDiffuse0;
texture texColor1:MTL_LayerDiffuse1;
texture texColor2:MTL_LayerDiffuse2;
texture texColor3:MTL_LayerDiffuse3;
texture texColor4:MTL_LayerDiffuse4;

sampler2D alphaSampler = sampler_state
{
	Texture = <texAlpha>;
	MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler2D colorSampler0 = sampler_state
{
	Texture = <texColor0>;
	MipFilter = LINEAR;
   	MinFilter = LINEAR;
   	MagFilter = LINEAR;
};

sampler2D colorSampler1 = sampler_state
{
	Texture = <texColor1>;
	MipFilter = LINEAR;
   	MinFilter = LINEAR;
   	MagFilter = LINEAR;
};

sampler2D colorSampler2 = sampler_state
{
	Texture = <texColor2>;
	MipFilter = LINEAR;
   	MinFilter = LINEAR;
   	MagFilter = LINEAR;
};

sampler2D colorSampler3 = sampler_state
{
	Texture = <texColor3>;
	MipFilter = LINEAR;
   	MinFilter = LINEAR;
   	MagFilter = LINEAR;
};

sampler2D colorSampler4 = sampler_state
{
	Texture = <texColor4>;
	MipFilter = LINEAR;
   	MinFilter = LINEAR;
   	MagFilter = LINEAR;
};

struct mtlParameters
{
	float3 normal:TEXCOORD0;
	float2 basic_uv : TEXCOORD1;
	float2 uv[MTL_NUM_TEXCOORD]: TEXCOORD2;
};

#define MTL_HAVE_DIFFUSE
float4 mtlGetDiffuseColor(mtlParameters mtlParam)
{
	float4 alpha = tex2D(alphaSampler, mtlParam.basic_uv);	// alpha map have no scale,rotate
	
	//-- base layer
	float4 retColor = tex2D(colorSampler0,mtlParam.uv[0]);	
	
	//-- alpha layer 1
	float4 texColor = tex2D(colorSampler1,mtlParam.uv[1]);
	retColor = retColor*(1-alpha.a) + texColor*alpha.a;
	
	//-- alpha layer 2
	texColor = tex2D(colorSampler2,mtlParam.uv[2]);
	retColor = retColor*(1-alpha.r) + texColor*alpha.r;
	
	//-- alpha layer 3
	texColor = tex2D(colorSampler3,mtlParam.uv[3]);
	retColor = retColor*(1-alpha.g) + texColor*alpha.g;
	
	//-- alpha layer 4
	texColor = tex2D(colorSampler4,mtlParam.uv[4]);
	retColor = retColor*(1-alpha.b) + texColor*alpha.b;
	
	return retColor;
}

#define MTL_HAVE_NORMAL
float3 mtlGetNormal(mtlParameters mtlParam)
{
	return normalize(mtlParam.normal);
}

#include "mtl_const.hlsl"