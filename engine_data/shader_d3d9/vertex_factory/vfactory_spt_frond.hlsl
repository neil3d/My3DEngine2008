#include "speed_tree_wind.hlsl"

float4x4 g_matWorldViewPrj : SYS_WorldViewProject;

float4	g_vTreePos : SPT_TreePos;			// each tree is in a unique position and rotation: .xyz = pos, .w = scale
float4  g_vTreeRotationTrig : SPT_TreeRot;	// stores (sin, cos, -sin, 0.0) for an instance's rotation angle (optimizes rotation code)


struct vfInput
{
	float4     vPosition   : POSITION;  // .xyz = coords, .w = self-shadow s-texcoord
	float4     vNormal     : TEXCOORD0; // .xyz = normal, .w = self-shadow t-texcoord
	float4     vTexCoords1 : TEXCOORD1;  // .xy = diffuse texcoords, .zw = wind parameters
#if defined(SPEEDTREE_BRANCH_FADING) || defined(SPEEDTREE_FROND_NORMAL_MAPPING)
	float4   vMisc       : TEXCOORD2;  // .xyz = tangent, .w = lod fade hint
#endif
};

struct vfInterpolants
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

float3 vfGetWorldPos(vfInput vert)
{
	float3 vPosition = vert.vPosition.xyz;
	
	// scale the geometry (each tree instance can be uniquely scaled)
    vPosition *= g_vTreePos.w;
    vPosition.xz = float2(dot(g_vTreeRotationTrig.ywz, vPosition.xyz), dot(g_vTreeRotationTrig.xwy, vPosition.xyz));
    
    // the wind effect primarily adjusts only the position of the vertex, but it can also be used to
    // affect the lighting vectors.  #define SPEEDTREE_ACCURATE_WIND_LIGHTING to activate it.  note
    // that the binormal is later derived (and thus not adjusted here) from the normal and tangent if used
    // adjust only the position
    float2 vWindParams = float2(vert.vTexCoords1.zw);
    WindEffect(vPosition, vWindParams);
    
    // translate tree into position (must be done after the rotation)
    vPosition.xyz += g_vTreePos.xyz;
    
	return vPosition;
}

float3 vfGetWorldNormal(vfInput vert)
{
	float3 ret = vert.vNormal.xyz;
	// rotate the whole tree (each tree instance can be uniquely rotated) - use optimized z-axis rotation
    // algorithm where float(sin(a), cos(a), -sin(a), 0.0f) is uploaded instead of angle a
	ret.xz = float2(dot(g_vTreeRotationTrig.ywz, vert.vNormal.xyz), dot(g_vTreeRotationTrig.xwy, vert.vNormal.xyz));
	
	return ret;
}

float4 vfTransformPos(vfInput vert)
{
	float3 pos = vfGetWorldPos(vert);
	return mul(float4(pos,1),g_matWorldViewPrj);
}

void vfMakeInterpolants(vfInput vert, out vfInterpolants inter)
{
	// define attribute aliases for readability
    float2 vSelfShadowTexCoords = float2(vert.vPosition.w, vert.vNormal.w);
    float2 vDiffuseTexCoords = float2(vert.vTexCoords1.xy);
    float2 vWindParams = float2(vert.vTexCoords1.zw);
    
#ifdef SPEEDTREE_SELF_SHADOW_LAYER
    inter.vSelfShadowTexCoords = vSelfShadowTexCoords;
#endif

#ifdef SPEEDTREE_BRANCH_FADING
    float fLodFadeHint = vert.vMisc.w;
    inter.vDiffuseTexCoords.z = 0;//ComputeFadeValue(fLodFadeHint);
#endif

	// pass through other texcoords exactly as they were received
    inter.vDiffuseTexCoords.xy = vDiffuseTexCoords;
#ifdef SPEEDTREE_FROND_NORMAL_MAPPING
    inter.vNormal = vfGetWorldNormal(vert);
#endif
}

#ifdef NEXUS_MATERIAL
void vfMakeMaterialParameters(vfInterpolants inter, out mtlParameters mtlParam)
{
#ifdef SPEED_TREE_FROND_MATERIAL

	mtlParam.vDiffuseTexCoords = inter.vDiffuseTexCoords;
#ifdef SPEEDTREE_SELF_SHADOW_LAYER
    mtlParam.vSelfShadowTexCoords = inter.vSelfShadowTexCoords;
#endif

#ifdef SPEEDTREE_FROND_NORMAL_MAPPING
    mtlParam.vNormal=inter.vNormal;
#endif

#else
	mtlParam.dummy = 0;
#endif
}
#endif

float3x3 vfGetTangentBasis(vfInput vert)
{
	float3x3 result = 0;

#if defined(SPEEDTREE_BRANCH_FADING) || defined(SPEEDTREE_FROND_NORMAL_MAPPING)
	float3 vTangent = vert.vMisc.xyz;
	// rotate the tangent by this instance's rotation and derive the binormal from the normal & tangent 
    // in order to keep vertex attribute count lower
	vTangent.xz = float2(dot(g_vTreeRotationTrig.ywz, vTangent.xyz), dot(g_vTreeRotationTrig.xwy, vTangent.xyz));
	
	float3 vNormal = vfGetWorldNormal(vert);	
	float3 vBinormal = -cross(vNormal, vTangent);
	
	result[0] = vTangent;
    result[1] = vBinormal;
    result[2] = vNormal;
#endif

	return result;
}