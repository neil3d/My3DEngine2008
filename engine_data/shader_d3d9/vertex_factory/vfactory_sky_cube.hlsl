float4x4 g_matWorldViewPrj : SYS_WorldViewProject;
float4x4 g_matWorld: SYS_LocalToWorld;

struct vfInput
{
	float3 pos : POSITION;
};

struct vfInterpolants
{
	float3 uv : TEXCOORD0;
};

float3 vfGetWorldPos(vfInput vert)
{
	return mul(float4(vert.pos,0),g_matWorld);
}

float3 vfGetWorldNormal(vfInput vert)
{
	return float3(0,1,0);
}

float4 vfTransformPos(vfInput vert)
{
	// transform as vector not as point
	return mul(float4(vert.pos,0),g_matWorldViewPrj).xyww;
}

void vfMakeInterpolants(vfInput vert, out vfInterpolants inter)
{
	inter.uv = vert.pos;
}

#ifdef NEXUS_MATERIAL
void vfMakeMaterialParameters(vfInterpolants inter, out mtlParameters mtlParam)
{
#if MTL_NUM_TEXCOORD
	mtlParam.uv = inter.uv;
#else
	mtlParam.dummy = 0;
#endif
}
#endif

float3x3 vfGetTangentBasis(vfInput vert)
{
	float3x3 result = 0;
	return result;
}