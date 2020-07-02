#define NEXUS_MATERIAL
#define MTL_LIGHT 1
#define MTL_NUM_TEXCOORD 3

#define MTL_NEED_VERT_NORMAL

texture g_texReflect:MTL_ReflectMap;
texture g_texRefract:MTL_RefractMap;

texture g_texDetail:MTL_DetailMap;
texture g_texBump:MTL_BumpMap;

struct mtlParameters
{
	float3 normal:TEXCOORD0;
	float  alpha:TEXCOORD1;
	float2 bumpUV: TEXCOORD2;
	float4 reflectUV: TEXCOORD3;
	float4 refractUV: TEXCOORD4;
};

sampler reflectSampler = sampler_state
			{
				Texture = <g_texReflect>;
				MipFilter = LINEAR;
				MinFilter = LINEAR;
				MagFilter = LINEAR;
				AddressU  = CLAMP;
				AddressV  = CLAMP;
			};
			
sampler refractSampler = sampler_state
			{
				Texture = <g_texRefract>;
				MipFilter = LINEAR;
				MinFilter = LINEAR;
				MagFilter = LINEAR;
				AddressU  = CLAMP;
				AddressV  = CLAMP;
			};
			
sampler detailSampler = sampler_state
			{
				Texture = <g_texDetail>;
				MipFilter = LINEAR;
				MinFilter = LINEAR;
				MagFilter = LINEAR;
				AddressU  = WRAP;
				AddressV  = WRAP;
			};

sampler bumpSampler = sampler_state
			{
				Texture = <g_texBump>;
				MipFilter = LINEAR;
				MinFilter = LINEAR;
				MagFilter = LINEAR;
				AddressU  = WRAP;
				AddressV  = WRAP;
			};

#define MTL_HAVE_DIFFUSE
float4 mtlGetDiffuseColor(mtlParameters mtlParam)
{
	float alpha = saturate(mtlParam.alpha+0.42);

	float2 bump = tex2D(bumpSampler, mtlParam.bumpUV);
	float s = 0.05f;
	float2 dudv = bump * s;	
	dudv -= float2(s, s);
	
	float4 reflectUV = mtlParam.reflectUV;
	reflectUV.x += dudv.x*reflectUV.w;
	reflectUV.y += dudv.y*reflectUV.w;
	float4 reflectColor = tex2Dproj(reflectSampler, reflectUV);
	
	float4 refractUV = mtlParam.refractUV;	
	refractUV.x += dudv.x*refractUV.w*(1.4-alpha);
	refractUV.y += dudv.y*refractUV.w*(1.4-alpha);
	float4 refractColor = tex2Dproj(refractSampler, refractUV);
	
	float4 detailColor = tex2D(detailSampler, mtlParam.bumpUV+dudv);	
	
	reflectColor = reflectColor*0.6f + detailColor*0.4f;
	return refractColor*alpha+reflectColor*(1-alpha);
}

#define MTL_HAVE_NORMAL
float3 mtlGetNormal(mtlParameters mtlParam)
{
	return normalize(mtlParam.normal);
}

#include "mtl_const.hlsl"