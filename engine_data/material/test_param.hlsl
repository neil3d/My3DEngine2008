#define NEXUS_MATERIAL	// 使用此宏通知include了material.hlsl的文件

#define MTL_LIGHT 0	// 材质是否接受光照
#define MTL_NUM_TEXCOORD 0

float g_f1:MTL_MyFloat;
float2 g_f2:MTL_MyVector2;
float3 g_f3:MTL_MyVector3;
float4 g_f4:MTL_MyVector4;
texture g_tex:MTL_MyTexture;

struct mtlParameters
{
	float dummy : TEXCOORD0;
};

#define MTL_HAVE_DIFFUSE
float4 mtlGetDiffuseColor(mtlParameters mtlParam)
{
	return g_f4;
}

#include "mtl_const.hlsl"