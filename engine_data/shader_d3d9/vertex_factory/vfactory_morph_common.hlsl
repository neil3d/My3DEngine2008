float4x4 g_matWorldViewPrj : SYS_WorldViewProject;
float4x4 g_matWorld: SYS_LocalToWorld;
float2	g_morphWeight: ANIM_MorphAlpha;

struct vfInput
{
//-- source frame
	float3 posA : POSITION;
#if VERT_NUM_NORMAL >0
	float3 normalA : NORMAL;
#endif
#if VERT_NUM_NORMAL >1
	float3 tangentA : TANGENT;
#endif
#if VERT_NUM_TEXCOORD >0	// 只支持一重uv
	float2 uvA : TEXCOORD0;
#endif

//-- dest frame
	float3 posB : POSITION1;
#if VERT_NUM_NORMAL >0
	float3 normalB : NORMAL1;
#endif
#if VERT_NUM_NORMAL >1
	float3 tangentB : TANGENT1;
#endif
#if VERT_NUM_TEXCOORD >0	// 只支持一重uv
	float2 uvB : TEXCOORD1;
#endif
};

struct vfInterpolants
{
#if VERT_NUM_TEXCOORD >0
	float2 uv : TEXCOORD0;
#endif
	float3 normal : TEXCOORD1;
};

float3 vfGetWorldPos(vfInput vert)
{
	float3 lerpPos = vert.posA*g_morphWeight.x + vert.posB*g_morphWeight.y;
	
	return mul(float4(lerpPos,1),g_matWorld);
}

float3 vfGetWorldNormal(vfInput vert)
{
	float3 lerpN = vert.normalA*g_morphWeight.x + vert.normalB*g_morphWeight.y;
	return normalize( mul(lerpN, g_matWorld) );
}

float4 vfTransformPos(vfInput vert)
{
	float3 lerpPos = vert.posA*g_morphWeight.x + vert.posB*g_morphWeight.y;
	return mul(float4(lerpPos,1),g_matWorldViewPrj);
}

void vfMakeInterpolants(vfInput vert, out vfInterpolants inter)
{
#if VERT_NUM_NORMAL >0
	// 在发送到pixel shader会被差值，所以这里无需normalize
	inter.normal = vfGetWorldNormal(vert);
#endif

#if VERT_NUM_TEXCOORD >0
	float2 lerpUV = vert.uvA*g_morphWeight.x + vert.uvB*g_morphWeight.y;
	inter.uv = lerpUV;
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
		mtlParam.uv[i]=inter.uv;	// 只支持一重uv
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
	float3 lerpN = vert.normalA*g_morphWeight.x + vert.normalB*g_morphWeight.y;
	float3 lerpT = vert.tangentA*g_morphWeight.x + vert.tangentB*g_morphWeight.y;
	
	lerpN = normalize(lerpN);
	lerpT = normalize(lerpT);
	
	result[0] = mul(lerpT, (float3x3)g_matWorld);
    result[1] = mul(cross(lerpT, lerpN), (float3x3)g_matWorld);
    result[2] = mul(lerpN, (float3x3)g_matWorld);
#endif

	return result;
}