float4x4 g_matWorldViewPrj : SYS_WorldViewProject;
float4x4 g_matWorld: SYS_LocalToWorld;

struct vfInput
{
	float3 pos : POSITION;

#if VERT_NUM_NORMAL >0
	float3 normal : NORMAL;
#endif

#if VERT_NUM_NORMAL >1
	float3 tangent : TANGENT;
#endif

#if VERT_NUM_TEXCOORD >0
	float2 uv[VERT_NUM_TEXCOORD] : TEXCOORD0;
#endif
};

struct vfInterpolants
{
#if VERT_NUM_TEXCOORD >0
	float2 uv[VERT_NUM_TEXCOORD] : TEXCOORD0;
#endif
	float3 normal : TEXCOORD4;	// 最多4重uv
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
#if VERT_NUM_NORMAL >0
	// 在发送到pixel shader会被差值，所以这里无需normalize
	inter.normal = vfGetWorldNormal(vert);
#endif

#if VERT_NUM_TEXCOORD >0
	for(int i=0; i<VERT_NUM_TEXCOORD; i++)
	{
		inter.uv[i] = vert.uv[i];
	}
#endif
}

#ifdef NEXUS_MATERIAL
void vfMakeMaterialParameters(vfInterpolants inter, out mtlParameters mtlParam)
{
#if MTL_NUM_TEXCOORD==0
	mtlParam.dummy = 0;
#else
	for(int i=0;i<MTL_NUM_TEXCOORD;i++)
	{
		if(i<VERT_NUM_TEXCOORD)
			mtlParam.uv[i]=inter.uv[i];
		else
			mtlParam.uv[i]=float2(0,0);
	}
#endif

#ifdef MTL_NEED_VERT_NORMAL
	#if VERT_NUM_NORMAL >0
		mtlParam.normal = inter.normal;
	#else
		mtlParam.normal = 0;
	#endif
#endif
}
#endif

float3x3 vfGetTangentBasis(vfInput vert)
{
	float3x3 result = 0;

#if VERT_NUM_NORMAL >1
	result[0] = mul(vert.tangent, g_matWorld);
    result[1] = mul(cross(vert.tangent, vert.normal), g_matWorld);
    result[2] = mul(vert.normal, g_matWorld);
#endif

	return result;
}