#define NEXUS_MATERIAL
#define MTL_LIGHT 0
#define MTL_NUM_TEXCOORD 1
#define MTL_TWO_SIDED

texture g_texDiffuse:MTL_DiffuseMap;

struct mtlParameters
{
	float3 uv: TEXCOORD0;
};

samplerCUBE diffuseSampler = sampler_state
			{
				Texture = <g_texDiffuse>;
				MipFilter = LINEAR;
				MinFilter = LINEAR;
				MagFilter = LINEAR;
				AddressU = MIRROR;
				AddressV = MIRROR;
				AddressW = MIRROR;
			};

#define MTL_HAVE_DIFFUSE
float4 mtlGetDiffuseColor(mtlParameters mtlParam)
{
	return texCUBE(diffuseSampler, mtlParam.uv);
}

#include "mtl_const.hlsl"