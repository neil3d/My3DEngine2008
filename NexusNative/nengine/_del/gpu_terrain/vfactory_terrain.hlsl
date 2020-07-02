float4x4 g_matWorldViewPrj : SYS_WorldViewProject;
float4x4 g_matWorld: SYS_LocalToWorld;

float3	g_quad_factor:TRN_QuadFactor = float3(0,0,1); // xy=uv bias, z=scale
float4	g_texel_size:TRN_TexelSize = float4(0,0,1,0);
texture	g_height_map:TRN_HeightMap;

#ifdef TRN_TEX_SPLATTING
#define MAX_LAYERS 5
float4x4 g_uv_matrix[MAX_LAYERS]:TRN_UVTransform;
#endif


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
#ifdef TRN_TEX_SPLATTING
	float2 basic_uv : TEXCOORD1;
	float2 uv[MAX_LAYERS] : TEXCOORD2;
#else
	float2 uv : TEXCOORD1;
#endif
};

// -------------------------------------------------------------
// Computes the vertex normal by sampling the
// vertex texture
// -------------------------------------------------------------
float3 _computeNormal(float2 uv)
{
	float4 tex = float4(uv.x, uv.y ,0, 0);
    float s = tex2Dlod(g_heightmap_sampler, tex-g_texel_size.wyww).x;
    float n = tex2Dlod(g_heightmap_sampler, tex+g_texel_size.wyww).x;
    float w = tex2Dlod(g_heightmap_sampler, tex-g_texel_size.xwww).x;
    float e = tex2Dlod(g_heightmap_sampler, tex+g_texel_size.xwww).x;

    float3 normal = normalize(float3(w-e, 1.0f, s-n)); 
    return normal;
}

float2 _getHeightmapUV(float3 inPos)
{
	float uBias = g_quad_factor.x;
	float vBias = g_quad_factor.y;
	float scale = g_quad_factor.z;
	
	float3 pos = float3(inPos.x*scale+uBias, inPos.y, inPos.z*scale+vBias);
	return float2(pos.x, pos.z);	
}

float3 _getObjectPos(float3 inPos)
{
	float uBias = g_quad_factor.x;
	float vBias = g_quad_factor.y;
	float scale = g_quad_factor.z;
	
	float3 pos = float3(inPos.x*scale+uBias, inPos.y, inPos.z*scale+vBias);
	float4 uv = float4(pos.x, pos.z, 0, 0);	
	
	pos.y *= tex2Dlod(g_heightmap_sampler, uv).x;
	
	return pos;
}

float3 vfGetWorldPos(vfInput vert)
{
	float3 pos = _getObjectPos(vert.pos);
	return mul(float4(pos,1),g_matWorld);
}

float3 vfGetWorldNormal(vfInput vert)
{
	return _computeNormal(_getHeightmapUV(vert.pos));
}

float4 vfTransformPos(vfInput vert)
{
	float3 pos = _getObjectPos(vert.pos);
	return mul(float4(pos,1),g_matWorldViewPrj);
}

void vfMakeInterpolants(vfInput vert, out vfInterpolants inter)
{
	inter.normal = vfGetWorldNormal(vert);	
	
	float2 uv = _getHeightmapUV(vert.pos);
	
#ifdef TRN_TEX_SPLATTING
	inter.basic_uv = uv;
	for(int i=0;i<MAX_LAYERS;i++)
	{
		inter.uv[i] = mul(float4(uv.x, uv.y, 0, 1),g_uv_matrix[i]).xy;
	}
#else
	inter.uv = uv;
#endif
}

#ifdef NEXUS_MATERIAL
void vfMakeMaterialParameters(vfInterpolants inter, out mtlParameters mtlParam)
{
#if MTL_NUM_TEXCOORD==0
	mtlParam.dummy = 0;
#else

#ifdef TRN_TEX_SPLATTING
	mtlParam.basic_uv = inter.basic_uv;
	for(int i=0;i<MTL_NUM_TEXCOORD;i++)
	{
		mtlParam.uv[i]=inter.uv[i];
	}
#else
	for(int i=0;i<MTL_NUM_TEXCOORD;i++)
	{
		mtlParam.uv[i]=inter.uv;
	}
#endif

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