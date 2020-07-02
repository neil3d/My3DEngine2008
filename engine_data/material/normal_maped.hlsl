#define NEXUS_MATERIAL
#define MTL_LIGHT 1
#define MTL_NUM_TEXCOORD 1

texture g_texDiffuse:MTL_DiffuseMap;
texture g_texNormal:MTL_NormalMap;

struct mtlParameters
{
	float2 uv[MTL_NUM_TEXCOORD]: TEXCOORD0;
};

sampler diffuseSampler = sampler_state
			{
				Texture = <g_texDiffuse>;
				MipFilter = LINEAR;
				MinFilter = LINEAR;
				MagFilter = LINEAR;
			};

sampler normalSampler = sampler_state
			{
				Texture = <g_texNormal>;
				MipFilter = LINEAR;
				MinFilter = LINEAR;
				MagFilter = LINEAR;
			};


#define MTL_HAVE_DIFFUSE
float4 mtlGetDiffuseColor(mtlParameters mtlParam)
{
	return tex2D(diffuseSampler, mtlParam.uv[0]);
}

#define MTL_HAVE_NORMAL
float3 mtlGetNormal(mtlParameters mtlParam)
{
	return 	normalize((tex2D(normalSampler,mtlParam.uv[0]) - 0.5)*2);
}

#include "mtl_const.hlsl"