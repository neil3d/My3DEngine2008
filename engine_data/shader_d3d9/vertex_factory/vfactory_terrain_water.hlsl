float4x4 g_matWorldViewPrj : SYS_WorldViewProject;
float4x4 g_matWorld: SYS_LocalToWorld;

float	g_time:SYS_RunTime;

float	g_waterHeight:TRN_WATER_HEIGHT;
float2	g_waterQuadBias:TRN_WATER_QUAD_BIAS;
float2	g_waterQuadScale:TRN_WATER_QUAD_Scale;
float2	g_waterTexelSize:TRN_WATER_TexelSize;

texture	g_height_map:TRN_HeightMap;

float4x4 g_matReflection : TRN_WATER_MatReflection;
float4x4 g_matRefract : TRN_WATER_MatRefract;

sampler g_heightmap_sampler = sampler_state
{
   Texture = <g_height_map>;
   MinFilter = Point;
   MagFilter = Point;
   MipFilter = None;   
   AddressU  = Clamp;
   AddressV  = Clamp;
};

struct vfInput
{
	float3 pos : POSITION;
};

struct vfInterpolants
{
	float3 normal : TEXCOORD0;
	float  alpha : TEXCOORD1;
	
	float2 bumpUV: TEXCOORD2;
	float4 reflectUV: TEXCOORD3;
	float4 refractUV: TEXCOORD4;
};

float2 _getHeightmapUV(float3 pos)
{
	float2 uv = pos.xz;
	return uv*g_waterQuadScale + g_waterQuadBias;
}

float4 _getObjectPos(float3 pos)
{
	float2 p = _getHeightmapUV(pos)*g_waterTexelSize;
	return float4(p.x, g_waterHeight, p.y, 1);
}

float3 vfGetWorldPos(vfInput vert)
{
	float4 objPos = _getObjectPos(vert.pos);
	return mul(objPos, g_matWorld);
}

float3 vfGetWorldNormal(vfInput vert)
{
	return float3(0,1,0);
}

float4 vfTransformPos(vfInput vert)
{
	float4 objPos = _getObjectPos(vert.pos);
	return mul(objPos, g_matWorldViewPrj);	
}

void vfMakeInterpolants(vfInput vert, out vfInterpolants inter)
{
	inter.normal = vfGetWorldNormal(vert);	
	
	float4 wpos = float4(vfGetWorldPos(vert), 1);
		
	inter.reflectUV = mul(wpos, g_matReflection);
	inter.refractUV = mul(wpos, g_matRefract);
	
	// Linear water edge fadeout
	float2 uv = _getHeightmapUV(vert.pos);
	float h = tex2Dlod(g_heightmap_sampler, float4(uv.x,uv.y,0,0)).x;
	
	float fadeH = g_waterHeight*0.6;	
	inter.alpha = saturate( (fadeH-h)/(fadeH-g_waterHeight) );
	
	float2 anim;
	float t = g_time;
	anim.x = sin(t*0.04);
	anim.y = cos(t*0.04);
	inter.bumpUV = uv*8+anim;
}

#ifdef NEXUS_MATERIAL
void vfMakeMaterialParameters(vfInterpolants inter, out mtlParameters mtlParam)
{
#if MTL_NUM_TEXCOORD==0
	mtlParam.dummy = 0;
#else
	mtlParam.bumpUV=inter.bumpUV;
	mtlParam.reflectUV=inter.reflectUV;
	mtlParam.refractUV=inter.refractUV;
	mtlParam.alpha = inter.alpha;
#endif

#ifdef MTL_NEED_VERT_NORMAL
	mtlParam.normal = inter.normal;
#endif
}
#endif

float3x3 vfGetTangentBasis(vfInput vert)
{
	float3x3 result = 0;
	
	float3 normal = vfGetWorldNormal(vert);
	float3 tangent = float3(1,0,0);
	
	result[0] = mul(tangent, g_matWorld);
    result[1] = mul(cross(tangent, normal), g_matWorld);
    result[2] = mul(normal, g_matWorld);
    
	return result;
}