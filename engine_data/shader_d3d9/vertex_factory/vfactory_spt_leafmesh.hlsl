#include "speed_tree_wind.hlsl"

float4x4 g_matWorldViewPrj : SYS_WorldViewProject;

float4	g_vTreePos : SPT_TreePos;			// each tree is in a unique position and rotation: .xyz = pos, .w = scale
float4  g_vTreeRotationTrig : SPT_TreeRot;	// stores (sin, cos, -sin, 0.0) for an instance's rotation angle (optimizes rotation code)

float2      g_vCameraAngles : SPT_CameraAngles;                     // shared by Billboard.fx and Leaf.fx - stores camera azimuth and pitch for billboarding computations

float4      g_avLeafAngles[SPEEDTREE_MAX_NUM_LEAF_ANGLES]:SPT_LeafAngles; // each element: .x = rock angle, .y = rustle angle
                                                 // each element is a float4, even though only a float2 is needed, to facilitate
                                                 // fast uploads on all platforms (one call to upload whole array)
float4      g_vLeafAngleScalars : SPT_LeafAngleScalars;                 // each tree model has unique scalar values: .x = rock scalar, .y = rustle scalar
float4x4    g_mLeafUnitSquare =                  // unit leaf card that's turned towards the camera and wind-rocked/rustled by the
            {                                    // vertex shader.  card is aligned on YZ plane and centered at (0.0f, 0.0f, 0.0f)
                float4(0.0f, 0.5f, 0.5f, 0.0f), 
                float4(0.0f, -0.5f, 0.5f, 0.0f), 
                float4(0.0f, -0.5f, -0.5f, 0.0f), 
                float4(0.0f, 0.5f, -0.5f, 0.0f)
            };



struct vfInput
{
	float4   vPosition  : POSITION;  // .xyz = position, w = wind param 1
    float4   vTexCoord0 : TEXCOORD0; // .xy = diffuse texcoords, z = wind angle index, w = dimming
    float3   vOrientX   : TEXCOORD1; // .xyz = vector xyz
    float3   vOrientZ   : TEXCOORD2; // .xyz = vector xyz
    float4   vOffset    : TEXCOORD3; // .xyz = mesh placement position, w = wind param 2
#ifdef SPEEDTREE_LEAF_NORMAL_MAPPING
    float3   vTangent   : TEXCOORD4; // .xyz = tangent
#endif
    float3   vNormal    : NORMAL;    // .xyz = normal xyz
};

struct vfInterpolants
{
	float2 vBaseTexCoords : TEXCOORD0;  // .xy = diffuse texcoords
#ifdef SPEEDTREE_LEAF_NORMAL_MAPPING
    float3 vNormal        : TEXCOORD1;  // .xyz = normal-mapping vector
#endif
};

float3 _getLocalPos(vfInput vert)
{
	float3 vPosition = vert.vPosition.xyz;
	// define attribute aliases for readability
    float fWindAngleIndex = vert.vTexCoord0.z; // which wind matrix this leaf card will follow
    float fDimming = vert.vTexCoord0.w; // interior leaves are darker (range = [0.0,1.0])
    float2 vWindParams = float2(vert.vPosition.w, vert.vOffset.w);
    
    float4   vOffset = vert.vOffset;
    // vOffset represents the location where the leaf mesh will be placed - here it is rotated into place
    // and has the wind effect motion applied to it
    vOffset.xz = float2(dot(g_vTreeRotationTrig.ywz, vOffset.xyz), dot(g_vTreeRotationTrig.xwy, vOffset.xyz));
    
    // the wind effect primarily adjusts only the position of the vertex, but it can also be used to
    // affect the lighting vectors.  #define SPEEDTREE_ACCURATE_WIND_LIGHTING to activate it.  note
    // that the binormal is later derived (and thus not adjusted here) from the normal and tangent if used
    // adjust only the position
    WindEffect(vOffset.xyz, vWindParams);
    
    // compute rock and rustle values (all trees share the g_avLeafAngles table), but g_vLeafAngleScalars
    // scales the angles to match wind settings specified in SpeedTreeCAD
    float2 vLeafRockAndRustle = g_vLeafAngleScalars.xy * g_avLeafAngles[fWindAngleIndex].xy;
    
    // vPosition stores the leaf mesh geometry, not yet put into place at position vOffset.
    // leaf meshes rock and rustle, which requires rotations on two axes (rustling is not
    // useful on leaf mesh geometry)
    float3x3 matRockRustle = RotationMatrix_xAxis(vLeafRockAndRustle.x); // rock
    vPosition.xyz = mul(matRockRustle, vPosition.xyz);

    matRockRustle = RotationMatrix_xAxis(vLeafRockAndRustle.x); // rock
    
    // build mesh orientation matrix - cannot be done beforehand on CPU due to wind effect / rotation order issues.
    // it is used to orient each mesh into place at vOffset
    float3 vOrientY = cross(vert.vOrientX, vert.vOrientZ);
    float3x3 matOrientMesh =
    {
        vert.vOrientX, vOrientY, vert.vOrientZ
    };

    // apply orientation matrix to the mesh positon & normal
    vPosition.xyz = mul(matOrientMesh, vPosition.xyz);
    
    // rotate the whole tree (each tree instance can be uniquely rotated) - use optimized z-axis rotation
    // algorithm where float(sin(a), cos(a), -sin(a), 0.0f) is uploaded instead of angle a
    vPosition.xz = float2(dot(g_vTreeRotationTrig.ywz, vPosition.xyz), dot(g_vTreeRotationTrig.xwy, vPosition.xyz));
    
    // put oriented mesh into place at rotated and wind-affected vOffset
    vPosition.xyz += vOffset.xyz;

    // scale the geometry (each tree instance can be uniquely scaled)
    vPosition.xyz *= g_vTreePos.w;
    
    // translate tree into position (must be done after the rotation)
    vPosition.xyz += g_vTreePos.xyz;
    
    return vPosition;
}

float3 vfGetWorldPos(vfInput vert)
{
	return _getLocalPos(vert);
}


float3 vfGetWorldNormal(vfInput vert)
{
	return float3(0,1,0);
}

float4 vfTransformPos(vfInput vert)
{
	return mul(float4(_getLocalPos(vert),1),g_matWorldViewPrj);
}

void vfMakeInterpolants(vfInput vert, out vfInterpolants inter)
{
	inter = (vfInterpolants)0;
	
	// pass through other texcoords exactly as they were received
    inter.vBaseTexCoords.xy = vert.vTexCoord0.xy;

}

#ifdef NEXUS_MATERIAL
void vfMakeMaterialParameters(vfInterpolants inter, out mtlParameters mtlParam)
{
#ifdef SPEED_TREE_LEAF_MATERIAL
	mtlParam.vBaseTexCoords = inter.vBaseTexCoords;
#ifdef SPEEDTREE_LEAF_NORMAL_MAPPING
    mtlParam.vNormal = inter.vNormal;
#endif
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