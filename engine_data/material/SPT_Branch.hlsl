#define NEXUS_MATERIAL
#define MTL_LIGHT 1
#define MTL_NUM_TEXCOORD 1

#define MTL_NEED_VERT_NORMAL

#define SPEED_TREE_BRANCH_MATERIAL

#define MTL_USE_ALPHA_MASK
int	g_alphaMask:MTL_AlphaMask = 84;

texture     g_tBranchDiffuseMap : SPT_BranchDiffuseMap;            // bark image, .rgb = diffuse color, .a = alpha noise map
texture     g_tBranchNormalMap : SPT_BranchNormalMap;             // bark normal map, .rgb = xyz normals, .a = unused
#ifdef SPEEDTREE_BRANCH_DETAIL_LAYER
texture     g_tBranchDetailMap : SPT_BranchDetailMap;             // bark detail image, .rgb = color, .a = amount of detail to use
#endif

///////////////////////////////////////////////////////////////////////  
//  Texture samplers
//
//  These are specific to the branch pixel shader.  The self-shadow layer
//  is assumed to be bound to s1.

sampler2D samBranchDiffuseMap = sampler_state
{
    Texture = <g_tBranchDiffuseMap>;
    MagFilter = Linear;
    MipFilter = Linear;
    MinFilter = Linear;
};

sampler2D samBranchNormalMap = sampler_state
{
    Texture = <g_tBranchNormalMap>;
    MagFilter = Linear;
    MipFilter = Linear;
    MinFilter = Linear;
};

#ifdef SPEEDTREE_BRANCH_DETAIL_LAYER
sampler2D samBranchDetailMap = sampler_state
{
    Texture = <g_tBranchDetailMap>;
    MagFilter = Linear;
    MipFilter = Linear;
    MinFilter = Linear;
};
#endif

struct mtlParameters
{
#ifdef SPEEDTREE_BRANCH_FADING
    float3  vDiffuseTexCoords     : TEXCOORD0;  // .xy = diffuse tecoords, .z = lod fade hint
#else
    float2  vDiffuseTexCoords     : TEXCOORD0;  // .xy = diffuse tecoords
#endif
#ifdef SPEEDTREE_BRANCH_NORMAL_MAPPING
    float2  vNormalTexCoords      : TEXCOORD2;  // .xy = normal map texcoords (not necessarily tied to diffuse texcoords)
    float4  vNormal               : TEXCOORD4;  // .xyz = normal-mapping vector, .w = fog
#else
    float4  vNormal               : TEXCOORD2;  // vertex normal
#endif
#ifdef SPEEDTREE_SELF_SHADOW_LAYER
    float2  vSelfShadowTexCoords  : TEXCOORD1;  // .xy = self-shadow texcoords
#endif
#ifdef SPEEDTREE_BRANCH_DETAIL_LAYER
    float2  vDetailTexCoords      : TEXCOORD3;  // .xy = detail map texcoords
#endif
};

#define MTL_HAVE_DIFFUSE
float4 mtlGetDiffuseColor(mtlParameters mtlParam)
{
	// look up the diffuse map layer
    float4 texDiffuse = tex2D(samBranchDiffuseMap, mtlParam.vDiffuseTexCoords.xy);
    
#ifdef SPEEDTREE_BRANCH_DETAIL_LAYER
    // if branch detail layer is active, look it up and lerp between it
    // and the base layer.  this allows for a smooth transition between diffuse
    // and detail layers if the detail layer's alpha map is done correctly
    // (the diffuse map's alpha layer doesn't figure into it)
    float4 texDetail = tex2D(samBranchDetailMap, mtlParam.vDetailTexCoords);
    texDiffuse.rgb = lerp(texDiffuse.rgb, texDetail.rgb, texDetail.a);
#endif
    
    return texDiffuse;
}

#define MTL_HAVE_OPACITY
float mtlGetOpacity(mtlParameters mtlParam)
{
	return mtlGetDiffuseColor(mtlParam).a;
}

#ifdef SPEEDTREE_BRANCH_NORMAL_MAPPING
#define MTL_HAVE_NORMAL
float3 mtlGetNormal(mtlParameters mtlParam)
{
	// since the normal map normal values (normally ranged [-1,1]) are stored
    // as a color value (ranged [0,1]), they must be uncompressed.  a dot product 
    // is used to compute the diffuse lighting contribution.
    float3 texNormal = tex2D(samBranchNormalMap, mtlParam.vNormalTexCoords).rgb;
    const float3 vHalves = { 0.5f, 0.5f, 0.5f };
    
    texNormal = 2.0f * (texNormal - vHalves);
	return 	normalize(texNormal);
}
#endif

#include "mtl_const.hlsl"