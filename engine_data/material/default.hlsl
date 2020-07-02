#define NEXUS_MATERIAL	// 使用此宏通知include了material.hlsl的文件

#define MTL_LIGHT 0	// 材质是否接受光照
#define MTL_NUM_TEXCOORD 0

struct mtlParameters
{
	float dummy : TEXCOORD0;
};


#include "mtl_const.hlsl"