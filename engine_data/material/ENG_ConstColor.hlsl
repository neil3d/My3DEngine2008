#define NEXUS_MATERIAL
#define MTL_LIGHT 0
#define MTL_NUM_TEXCOORD 0

float4 g_diffuseColor : MTL_DiffuseColor;

struct mtlParameters
{
	float dummy : TEXCOORD0;
};


#define MTL_HAVE_DIFFUSE
float4 mtlGetDiffuseColor(mtlParameters mtlParam)
{
	return g_diffuseColor;
}

#include "mtl_const.hlsl"