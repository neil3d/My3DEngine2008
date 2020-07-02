#define NEXUS_MATERIAL
#define MTL_LIGHT 0
#define MTL_NUM_TEXCOORD 1
#define MTL_TWO_SIDED

texture g_sceneDepth:RT_SceneDepth;

sampler depthSampler = sampler_state
			{
				Texture = <g_sceneDepth>;
				MipFilter = None;
				MinFilter = Point;
				MagFilter = Point;
				AddressU  = CLAMP;
				AddressV  = CLAMP;
			};

struct mtlParameters
{
	float4 color: TEXCOORD0;
	float4 pos : TEXCOORD1;
};

#define MTL_HAVE_OPACITY
float mtlGetOpacity(mtlParameters mtlParam)
{
	float4 pos = mtlParam.pos;
	pos /= pos.w;
	
	float2 depthUV = 0.5 * pos.xy + float2( 0.5, 0.5 );
    depthUV.y = 1.0f - depthUV.y;
    
	float d = tex2D(depthSampler, depthUV).r;	

	float z = pos.z*pos.w;
	
	float s = 0.9;
	float e = 1.0;
	
	float a = d/z;
	a = (a-s)/(e-s);
	a = pow(a,64);
	a = 1/exp((1-a)*8);
	
	return a;
}

#define MTL_HAVE_DIFFUSE
float4 mtlGetDiffuseColor(mtlParameters mtlParam)
{
	return mtlParam.color;
	//float a = mtlGetOpacity(mtlParam);
	//return float4(a,a,a,1);
}

#include "mtl_const.hlsl"