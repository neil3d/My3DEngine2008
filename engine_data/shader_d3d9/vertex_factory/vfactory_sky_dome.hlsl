float4x4 g_matWorldViewPrj : SYS_WorldViewProject;
float4x4 g_matWorld: SYS_LocalToWorld;

struct vfInput
{
	float3 pos : POSITION;
	float4 color : COLOR;
};

struct vfInterpolants
{
	float4 color : TEXCOORD0;
	float4 pos : TEXCOORD1;
};

float3 vfGetWorldPos(vfInput vert)
{
	return mul(float4(vert.pos,1),g_matWorld);
}

float3 vfGetWorldNormal(vfInput vert)
{
	return float3(0,1,0);
}

float4 vfTransformPos(vfInput vert)
{
	// transform as vector not as point
	return mul(float4(vert.pos,1),g_matWorldViewPrj);
}

void vfMakeInterpolants(vfInput vert, out vfInterpolants inter)
{
	inter.color = vert.color;
	inter.pos = vfTransformPos(vert);
}

#ifdef NEXUS_MATERIAL
void vfMakeMaterialParameters(vfInterpolants inter, out mtlParameters mtlParam)
{
#if MTL_NUM_TEXCOORD
	mtlParam.color = inter.color;
	mtlParam.pos = inter.pos;
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