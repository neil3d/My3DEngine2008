#define NEXUS_MATERIAL
#define MTL_LIGHT 1
#define MTL_NUM_TEXCOORD 1

#define MTL_NEED_VERT_NORMAL

#define MTL_TWO_SIDED
#define MTL_USE_ALPHA_MASK
int	g_alphaMask:MTL_AlphaMask = 84;

#define SPEED_TREE_FROND_MATERIAL

// textures
texture     g_tCompositeDiffuseLeafMap:SPT_CompositeDiffuseLeafMap;                 // this composite texture contains all of the leaf and frond images
texture     g_tCompositeNormalLeafMap:SPT_CompositeNormalLeafMap;                  // this composite texture contains all of the leaf and frond images
texture     g_tSelfShadowMap;                           // this composite texture contains all of the self-shadow maps (greyscale)

#define SPEEDTREE_FILTER Linear
sampler2D samCompositeDiffuseLeafMap = sampler_state
{
    Texture = <g_tCompositeDiffuseLeafMap>;
    MagFilter = SPEEDTREE_FILTER;
    MipFilter = SPEEDTREE_FILTER;
    MinFilter = SPEEDTREE_FILTER;
};

sampler2D samCompositeNormalLeafMap = sampler_state
{
    Texture = <g_tCompositeNormalLeafMap>;
    MagFilter = SPEEDTREE_FILTER;
    MipFilter = SPEEDTREE_FILTER;
    MinFilter = SPEEDTREE_FILTER;
};

#ifdef SPEEDTREE_SELF_SHADOW_LAYER
sampler2D samSelfShadowMap = sampler_state
{
    Texture = <g_tSelfShadowMap>;
    MagFilter = SPEEDTREE_FILTER;
    MipFilter = SPEEDTREE_FILTER;
    MinFilter = SPEEDTREE_FILTER;
};
#endif

struct mtlParameters
{
#ifdef SPEEDTREE_BRANCH_FADING
    float3 vDiffuseTexCoords    : TEXCOORD0;    // .xy = diffuse texcoords, .z = lod fade hint
#else
    float2 vDiffuseTexCoords    : TEXCOORD0;    // .xy = diffuse texcoords
#endif

#ifdef SPEEDTREE_SELF_SHADOW_LAYER
    float2 vSelfShadowTexCoords : TEXCOORD1;    // .xy = self-shadow texcoords
#endif

#ifdef SPEEDTREE_FROND_NORMAL_MAPPING
    float3 vNormal              : TEXCOORD2;    // .xyz = normal-mapping vector, 
#endif
};

#define MTL_HAVE_DIFFUSE
float4 mtlGetDiffuseColor(mtlParameters mtlParam)
{
	float2 uv = mtlParam.vDiffuseTexCoords.xy;	
	// look up the diffuse layer
    float4 texDiffuse = tex2D(samCompositeDiffuseLeafMap, uv);
    return texDiffuse;
}

#define MTL_HAVE_OPACITY
float mtlGetOpacity(mtlParameters mtlParam)
{
	return mtlGetDiffuseColor(mtlParam).a;
}

#ifdef SPEEDTREE_FROND_NORMAL_MAPPING
#define MTL_HAVE_NORMAL
float3 mtlGetNormal(mtlParameters mtlParam)
{
    // since the normal map normal values (normally ranged [-1,1]) are stored
    // as a color value (ranged [0,1]), they must be uncompressed.  a dot product 
    // is used to compute the diffuse lighting contribution.
    float3 texNormal = tex2D(samCompositeNormalLeafMap, mtlParam.vDiffuseTexCoords.xy).rgb;
    const float3 vHalves = { 0.5f, 0.5f, 0.5f };
    
    texNormal = 2.0f * (texNormal - vHalves);
	return 	normalize(texNormal);
}
#endif


#include "mtl_const.hlsl"