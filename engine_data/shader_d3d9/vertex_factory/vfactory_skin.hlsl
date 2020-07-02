float4x4 g_matWorldViewPrj : SYS_WorldViewProject;
float4x4 g_matWorld: SYS_LocalToWorld;

struct vfInput
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;
};

struct vfInterpolants
{
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
};

float3 vfGetWorldPos(vfInput vert)
{
	return mul(float4(vert.pos,1),g_matWorld);
}

float3 vfGetWorldNormal(vfInput vert)
{
	return normalize( mul(vert.normal, g_matWorld) );
}

float4 vfTransformPos(vfInput vert)
{
	return mul(float4(vert.pos,1),g_matWorldViewPrj);
}

void vfMakeInterpolants(vfInput vert, out vfInterpolants inter)
{
	inter.normal = vfGetWorldNormal(vert);
	inter.uv = vert.uv;
}

#ifdef NEXUS_MATERIAL
void vfMakeMaterialParameters(vfInterpolants inter, out mtlParameters mtlParam)
{
#if MTL_NUM_TEXCOORD==0
	mtlParam.dummy = 0;
#else
	for(int i=0;i<MTL_NUM_TEXCOORD;i++)
	{
		if(i<MTL_NUM_TEXCOORD)
			mtlParam.uv[i]=inter.uv;		
	}
	mtlParam.normal = inter.normal;
#endif
}
#endif

float3x3 vfGetTangentBasis(vfInput vert)
{
	float3x3 result = 0;
	return result;
}