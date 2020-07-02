#define NEXUS_MATERIAL
#define MTL_LIGHT 1
#define MTL_NUM_TEXCOORD 1

#define MTL_NEED_VERT_NORMAL
#define MTL_USE_ALPHA_MASK
#define MTL_TWO_SIDED

//* TransparentType=AlphaMasked;

texture g_texDiffuse:MTL_DiffuseMap;
int	g_alphaMask:MTL_AlphaMask = 250;

struct mtlParameters
{
	float2 uv[MTL_NUM_TEXCOORD]: TEXCOORD0;
	float3 normal:TEXCOORD1;
};

sampler diffuseSampler = sampler_state
			{
				Texture = <g_texDiffuse>;
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
	return normalize(mtlParam.normal);
}

#define MTL_HAVE_OPACITY
float mtlGetOpacity(mtlParameters mtlParam)
{
	return tex2D(diffuseSampler, mtlParam.uv[0]).a;
}

#include "mtl_const.hlsl"