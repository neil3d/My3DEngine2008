#include "speed_tree_wind.hlsl"

float4x4 g_matWorldViewPrj : SYS_WorldViewProject;

float4	g_vTreePos : SPT_TreePos;			// each tree is in a unique position and rotation: .xyz = pos, .w = scale
float4  g_vTreeRotationTrig : SPT_TreeRot;	// stores (sin, cos, -sin, 0.0) for an instance's rotation angle (optimizes rotation code)


struct vfInput
{
	float4   vPosition   : POSITION;     // xyz = coords, w = self-shadow texcoord s
	float4   vNormal     : TEXCOORD0;    // xyz = normal, w = self-shadow texcoord t
	float4   vTexCoords1 : TEXCOORD1;    // xy = diffuse texcoords, zw = wind parameters
	float4   vTexCoords2 : TEXCOORD2;    // xy = detail texcoords, zw = normal-map texcoords
#ifdef SPEEDTREE_BRANCH_FADING
	float4   vTexCoords3 : TEXCOORD3;    // xyz = tangent (binormal is derived from normal and tangent), w = lod fade hint
#else                    
	float3   vTexCoords3 : TEXCOORD3;    // xyz = tangent (binormal is derived from normal and tangent)
#endif
};

struct vfInterpolants
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
    float4  vNormal               : TEXCOORD2;      // color computed by standard lighting
#endif
#ifdef SPEEDTREE_SELF_SHADOW_LAYER
    float2  vSelfShadowTexCoords  : TEXCOORD1;  // .xy = self-shadow texcoords
#endif
#ifdef SPEEDTREE_BRANCH_DETAIL_LAYER
    float2  vDetailTexCoords      : TEXCOORD3;  // .xy = detail map texcoords
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
    float2 vShadowTexCoords = float2(vert.vPosition.w, vert.vNormal.w);
    float2 vDiffuseTexCoords = float2(vert.vTexCoords1.xy);
    float2 vWindParams = float2(vert.vTexCoords1.zw);
    float2 vDetailTexCoords = float2(vert.vTexCoords2.xy);
    float2 vNormalTexCoords = float2(vert.vTexCoords2.zw);
    float3 vTangent = float3(vert.vTexCoords3.xyz);
    
    float4 vNormal=vert.vNormal;
    vNormal.xz = float2(dot(g_vTreeRotationTrig.ywz, vert.vNormal.xyz), dot(g_vTreeRotationTrig.xwy, vert.vNormal.xyz));
    
    inter.vNormal = vNormal;
    
#ifdef SPEEDTREE_SELF_SHADOW_LAYER
    inter.vSelfShadowTexCoords = MoveSelfShadow(vShadowTexCoords);
#endif

	// pass through other texcoords exactly as they were received
    inter.vDiffuseTexCoords.xy = vDiffuseTexCoords.xy;
#ifdef SPEEDTREE_BRANCH_NORMAL_MAPPING
    inter.vNormalTexCoords.xy = vNormalTexCoords;
#endif
#ifdef SPEEDTREE_BRANCH_DETAIL_LAYER
    inter.vDetailTexCoords.xy = vDetailTexCoords;
#endif

#ifdef SPEEDTREE_BRANCH_FADING
    // compute some fade value (0.0 = transparent, 1.0 = opaque)
    float fLodFadeHint = vert.vTexCoords3.w;
    inter.vDiffuseTexCoords.z = 0; //todo:ComputeFadeValue(fLodFadeHint);
#endif
}

#ifdef NEXUS_MATERIAL
void vfMakeMaterialParameters(vfInterpolants inter, out mtlParameters mtlParam)
{
#ifdef SPEED_TREE_BRANCH_MATERIAL
	mtlParam.vDiffuseTexCoords = inter.vDiffuseTexCoords;
	mtlParam.vNormal = inter.vNormal;
#	ifdef SPEEDTREE_BRANCH_NORMAL_MAPPING
	mtlParam.vNormalTexCoords = inter.vNormalTexCoords;
#	endif

#	ifdef SPEEDTREE_SELF_SHADOW_LAYER
	mtlParam.vSelfShadowTexCoords = inter.vSelfShadowTexCoords;
#	endif

#	ifdef SPEEDTREE_BRANCH_DETAIL_LAYER
	mtlParam.vDetailTexCoords = inter.vDetailTexCoords;
#	endif

#else
	mtlParam.dummy = 0;
#endif
}
#endif

float3x3 vfGetTangentBasis(vfInput vert)
{
	float3 vTangent = float3(vert.vTexCoords3.xyz);
	
	// rotate the tangent by this instance's rotation and derive the binormal from the normal & tangent 
    // in order to keep vertex attribute count lower
	vTangent.xz = float2(dot(g_vTreeRotationTrig.ywz, vTangent.xyz), dot(g_vTreeRotationTrig.xwy, vTangent.xyz));
	
	float3 vNormal = vfGetWorldNormal(vert);
	float3 vBinormal = cross(vNormal, vTangent);
	
	float3x3 result = 0;
	result[0] = vTangent;
    result[1] = vBinormal;
    result[2] = vNormal;
    
	return result;
}