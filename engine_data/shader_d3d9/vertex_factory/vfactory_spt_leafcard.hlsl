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
	float4   vPosition  : POSITION;  // .xyz = position, w = corner index
    float4   vTexCoord0 : TEXCOORD0; // .xy = diffuse texcoords, zw = wind parameters
    float4   vTexCoord1 : TEXCOORD1; // .x = width, .y = height, .z = pivot x, .w = pivot.y
    float4   vTexCoord2 : TEXCOORD2; // .x = angle.x, .y = angle.y, .z = wind angle index, .w = dimming
#ifdef SPEEDTREE_LEAF_NORMAL_MAPPING
    float3   vTangent   : TEXCOORD3; // .xyz = tangent
#endif
    float3   vNormal    : NORMAL;    // .xyz = normal
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
	// define attribute aliases for readability
    float fAzimuth = g_vCameraAngles.x;      // camera azimuth for billboarding
    float fPitch = g_vCameraAngles.y;        // camera pitch for billboarding
    float2 vSize = vert.vTexCoord1.xy;            // leaf card width & height
    int nCorner = vert.vPosition.w;               // which card corner this vertex represents [0,3]
    float fRotAngleX = vert.vTexCoord2.x;         // angle offset for leaf rocking (helps make it distinct)
    float fRotAngleY = vert.vTexCoord2.y;         // angle offset for leaf rustling (helps make it distinct)
    float fWindAngleIndex = vert.vTexCoord2.z;    // which wind matrix this leaf card will follow
    float2 vPivotPoint = vert.vTexCoord1.zw;      // point about which card will rock and rustle
    float fDimming = vert.vTexCoord2.w;           // interior leaves are darker (range = [0.0,1.0])
    float2 vWindParams = vert.vTexCoord0.zw;      // wind parameters

	
	//--
	float3 vPosition = vert.vPosition.xyz;
	// rotate the whole tree (each tree instance can be uniquely rotated) - use optimized z-axis rotation
    // algorithm where float(sin(a), cos(a), -sin(a), 0.0f) is uploaded instead of angle a
    vPosition.xz = float2(dot(g_vTreeRotationTrig.ywz, vert.vPosition.xyz), dot(g_vTreeRotationTrig.xwy, vert.vPosition.xyz));
    
    // the wind effect primarily adjusts only the position of the vertex, but it can also be used to
    // affect the lighting vectors.  #define SPEEDTREE_ACCURATE_WIND_LIGHTING to activate it.  note
    // that the binormal is later derived (and thus not adjusted here) from the normal and tangent if used
    // adjust only the position
    WindEffect(vPosition.xyz, vWindParams);
    
    // compute rock and rustle values (all trees share the g_avLeafAngles table, but each can be scaled uniquely)
    float2 vLeafRockAndRustle = g_vLeafAngleScalars.xy * g_avLeafAngles[fWindAngleIndex].xy;
    
    // access g_mLeafUnitSquare matrix with corner index and apply scales
    float3 vPivotedPoint = g_mLeafUnitSquare[nCorner].xzy;
    
    // adjust by pivot point so rotation occurs around the correct point
    vPivotedPoint.yz -= vPivotPoint;
    float3 vCorner = vPivotedPoint * vSize.xyx;
    
    // rock & rustling on the card corner
    // * note: The bulk of the LeafCardVS instructions come from this section of code. Please use the
    //         code commented out just below this block if you want a quick and dirty approximation
    //         of the wind as tuned in CAD as opposed to this 100% faithful implementation.  Using the
    //         shorter code reduces the compiled shader length by ~25%.
    //         
    
    float3x3 matRotation = RotationMatrix_yAxis(fAzimuth + fRotAngleX);
    matRotation = mul(matRotation, RotationMatrix_zAxis(fPitch + fRotAngleY));
    matRotation = mul(matRotation, RotationMatrix_yAxis(-vLeafRockAndRustle.y));
    matRotation = mul(matRotation, RotationMatrix_xAxis(vLeafRockAndRustle.x));
    
    vCorner = mul(matRotation, vCorner);
    
    // place and scale the leaf card
    vPosition.xyz += vCorner;
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
	float3 vNormal = vert.vNormal;
	
	// rotate the whole tree (each tree instance can be uniquely rotated) - use optimized z-axis rotation
    // algorithm where float(sin(a), cos(a), -sin(a), 0.0f) is uploaded instead of angle a
	vNormal.xz = float2(dot(g_vTreeRotationTrig.ywz, vNormal.xyz), dot(g_vTreeRotationTrig.xwy, vNormal.xyz));
	
    float fWindAngleIndex = vert.vTexCoord2.z;    // which wind matrix this leaf card will follow
	// compute rock and rustle values (all trees share the g_avLeafAngles table, but each can be scaled uniquely)
    float2 vLeafRockAndRustle = g_vLeafAngleScalars.xy * g_avLeafAngles[fWindAngleIndex].xy;
    
	// perturb normal for wind effect (optional - not previewed or tunable in CAD)
	vNormal.xyz += 0.15f * vLeafRockAndRustle.xxy;
	
	vNormal = normalize(vNormal);
	
	return vNormal;
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
	
#ifdef SPEEDTREE_LEAF_NORMAL_MAPPING
	float3 vTangent = vert.vTangent;
	// rotate the tangent by this instance's rotation and derive the binormal from the normal & tangent 
    // in order to keep vertex attribute count lower
	vTangent.xz = float2(dot(g_vTreeRotationTrig.ywz, vTangent.xyz), dot(g_vTreeRotationTrig.xwy, vTangent.xyz));
	
	float3 vNormal = vfGetWorldNormal(vert);
	float3 vBinormal = cross(vNormal.xyz, vTangent);
	
	result[0] = vTangent;
    result[1] = vBinormal;
    result[2] = vNormal;
#endif
	

	return result;
}