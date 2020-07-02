#define NEXUS_MATERIAL	// 使用此宏通知include了material.hlsl的文件

#define MTL_TWO_SIDED	// 在pre pass渲染中，如果材质是双面的则使用这个

#define MTL_LIGHT 0	// 材质是否接受光照
#define MTL_NUM_TEXCOORD 0

struct mtlParameters
{
	float dummy : TEXCOORD0;
};


#include "mtl_const.hlsl"