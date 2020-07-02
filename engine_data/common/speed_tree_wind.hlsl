
float       g_fWindMatrixOffset:SPT_WindMatrixOffset;                 // keeps two instances of the same tree model from using the same wind matrix (range: [0,SPEEDTREE_NUM_WIND_MATRICES])
float4x4    g_amWindMatrices[SPEEDTREE_NUM_WIND_MATRICES]:SPT_WindMatrices;// houses all of the wind matrices shared by all geometry types

#ifdef SPEEDTREE_BRANCH_FADING
float2      g_vBranchLodValues;                 // .x = current LOD, .y = transition radius
#endif

///////////////////////////////////////////////////////////////////////  
//  WindMatrixLerp
//
//  In order to achieve a bending branch effect, each vertex is multiplied
//  by a wind matrix.  This function interpolates between a static point
//  and a fully wind-effected point by the wind weight passed in.

void WindMatrixLerp(inout float3 vCoord, int nIndex, float fWeight)
{
    vCoord = lerp(vCoord, mul(vCoord, g_amWindMatrices[nIndex]), fWeight);
}

///////////////////////////////////////////////////////////////////////  
//  WindEffect
//
//  SpeedTree uses a two-weight wind system that allows the tree model
//  to bend at more than one branch level.
//
//  In order to keep the vertex size small, the four wind parameters are
//  passed using two float values.  It assumes that wind weights are
//  in the range [0,1].  The 0.98 scalar (c_fWeightRange in SpeedTreeWrapper.cpp)
//  maps this range to just less than 1.0, ensuring that the weight will always 
//  be fractional (with the matrix index in the integer part)
//
//      vWindInfo.x = wind_matrix_index1 + wind_weight1 * 0.98
//      vWindInfo.y = wind_matrix_index2 + wind_weight2 * 0.98
//
//  * Caution: Negative wind weights will not work with this scheme.  We rely on the
//             fact that the SpeedTreeRT library clamps wind weights to [0.0, 1.0]


// enable a second-level wind effect, enabling the tree to essentially "bend" at more
// than one level; can be fairly expensive, especially with SPEEDTREE_ACCURATE_WIND_LIGHTING
// enabled
//#define SPEEDTREE_TWO_WEIGHT_WIND

void WindEffect_Normal_Tangent(inout float3 vPosition, inout float3 vNormal, inout float3 vTangent, float2 vWindInfo)
{
    // extract the wind indices & weights:
    //   vWeights.x = [0.0, 0.98] wind weight for wind matrix 1
    //   vWeights.y = [0.0, 0.98] wind weight for wind matrix 2
    //   vIndices.x = integer index for wind matrix 1
    //   vIndices.y = integer index for wind matrix 2
    float2 vWeights = frac(vWindInfo.xy);
    float2 vIndices = vWindInfo.xy - vWeights;

    // this one instruction helps keep two instances of the same base tree from being in
    // sync in their wind behavior; each instance has a unique matrix offset 
    // (g_fWindMatrixOffset.x) which helps keep them from using the same set of 
    // matrices for wind
    vIndices = fmod(vIndices + g_fWindMatrixOffset.xx, SPEEDTREE_NUM_WIND_MATRICES);
    
    // first-level wind effect - interpolate between static position and fully-blown
    // wind position by the wind weight value
    WindMatrixLerp(vPosition, int(vIndices.x), vWeights.x);
    WindMatrixLerp(vNormal, int(vIndices.x), vWeights.x);
    WindMatrixLerp(vTangent, int(vIndices.x), vWeights.x);
    
    // second-level wind effect - interpolate between first-level wind position and 
    // the fully-blown wind position by the second wind weight value
#ifdef SPEEDTREE_TWO_WEIGHT_WIND
    WindMatrixLerp(vPosition, int(vIndices.y), vWeights.y);
    WindMatrixLerp(vNormal, int(vIndices.y), vWeights.y);
    WindMatrixLerp(vTangent, int(vIndices.y), vWeights.y);
#endif
}


void WindEffect_Normal(inout float3 vPosition, inout float3 vNormal, float2 vWindInfo)
{
    // extract the wind indices & weights:
    //   vWeights.x = [0.0, 0.98] wind weight for wind matrix 1
    //   vWeights.y = [0.0, 0.98] wind weight for wind matrix 2
    //   vIndices.x = integer index for wind matrix 1
    //   vIndices.y = integer index for wind matrix 2
    float2 vWeights = frac(vWindInfo.xy);
    float2 vIndices = vWindInfo.xy - vWeights;

    // this one instruction helps keep two instances of the same base tree from being in
    // sync in their wind behavior; each instance has a unique matrix offset 
    // (g_fWindMatrixOffset.x) which helps keep them from using the same set of 
    // matrices for wind
    vIndices = fmod(vIndices + g_fWindMatrixOffset.xx, SPEEDTREE_NUM_WIND_MATRICES);
    
    // first-level wind effect - interpolate between static position and fully-blown
    // wind position by the wind weight value
    WindMatrixLerp(vPosition, int(vIndices.x), vWeights.x);
    WindMatrixLerp(vNormal, int(vIndices.x), vWeights.x);
    
    // second-level wind effect - interpolate between first-level wind position and 
    // the fully-blown wind position by the second wind weight value
#ifdef SPEEDTREE_TWO_WEIGHT_WIND
    WindMatrixLerp(vPosition, int(vIndices.y), vWeights.y);
    WindMatrixLerp(vNormal, int(vIndices.y), vWeights.y);
#endif
}


void WindEffect(inout float3 vPosition, float2 vWindInfo)
{
    // extract the wind indices & weights:
    //   vWeights.x = [0.0, 0.98] wind weight for wind matrix 1
    //   vWeights.y = [0.0, 0.98] wind weight for wind matrix 2
    //   vIndices.x = integer index for wind matrix 1
    //   vIndices.y = integer index for wind matrix 2
    float2 vWeights = frac(vWindInfo.xy);
    float2 vIndices = vWindInfo.xy - vWeights;

    // this one instruction helps keep two instances of the same base tree from being in
    // sync in their wind behavior; each instance has a unique matrix offset 
    // (g_fWindMatrixOffset.x) which helps keep them from using the same set of 
    // matrices for wind
    vIndices = fmod(vIndices + g_fWindMatrixOffset.xx, SPEEDTREE_NUM_WIND_MATRICES);
    
    // first-level wind effect - interpolate between static position and fully-blown
    // wind position by the wind weight value
    WindMatrixLerp(vPosition, int(vIndices.x), vWeights.x);
    
    // second-level wind effect - interpolate between first-level wind position and 
    // the fully-blown wind position by the second wind weight value
#ifdef SPEEDTREE_TWO_WEIGHT_WIND
    WindMatrixLerp(vPosition, int(vIndices.y), vWeights.y);
#endif
}

///////////////////////////////////////////////////////////////////////  
//  RotationMatrix_zAxis
//
//  Constructs a Z-axis rotation matrix

float3x3 RotationMatrix_zAxis(float fAngle)
{
    // compute sin/cos of fAngle
    float2 vSinCos;
    sincos(fAngle, vSinCos.x, vSinCos.y);
    
    return float3x3(vSinCos.y, -vSinCos.x, 0.0f, 
                    vSinCos.x, vSinCos.y, 0.0f, 
                    0.0f, 0.0f, 1.0f);
}


///////////////////////////////////////////////////////////////////////  
//  Rotate_zAxis
//
//  Returns an updated .xy value

float2 Rotate_zAxis(float fAngle, float3 vPoint)
{
    float2 vSinCos;
    sincos(fAngle, vSinCos.x, vSinCos.y);
    
    return float2(dot(vSinCos.yx, vPoint.xy), dot(float2(-vSinCos.x, vSinCos.y), vPoint.xy));
}


///////////////////////////////////////////////////////////////////////  
//  RotationMatrix_yAxis
//
//  Constructs a Y-axis rotation matrix

float3x3 RotationMatrix_yAxis(float fAngle)
{
    // compute sin/cos of fAngle
    float2 vSinCos;
    sincos(fAngle, vSinCos.x, vSinCos.y);
    
    return float3x3(vSinCos.y, 0.0f, vSinCos.x,
                    0.0f, 1.0f, 0.0f,
                    -vSinCos.x, 0.0f, vSinCos.y);
}


///////////////////////////////////////////////////////////////////////  
//  Rotate_yAxis
//
//  Returns an updated .xz value

float2 Rotate_yAxis(float fAngle, float3 vPoint)
{
    float2 vSinCos;
    sincos(fAngle, vSinCos.x, vSinCos.y);
    
    return float2(dot(float2(vSinCos.y, -vSinCos.x), vPoint.xz), dot(vSinCos.xy, vPoint.xz));
}


///////////////////////////////////////////////////////////////////////  
//  RotationMatrix_xAxis
//
//  Constructs a X-axis rotation matrix

float3x3 RotationMatrix_xAxis(float fAngle)
{
    // compute sin/cos of fAngle
    float2 vSinCos;
    sincos(fAngle, vSinCos.x, vSinCos.y);
    
    return float3x3(1.0f, 0.0f, 0.0f,
                    0.0f, vSinCos.y, -vSinCos.x,
                    0.0f, vSinCos.x, vSinCos.y);
}


///////////////////////////////////////////////////////////////////////  
//  Rotate_xAxis
//
//  Returns an updated .yz value

float2 Rotate_xAxis(float fAngle, float3 vPoint)
{
    float2 vSinCos;
    sincos(fAngle, vSinCos.x, vSinCos.y);
    
    return float2(dot(vSinCos.yx, vPoint.yz), dot(float2(-vSinCos.x, vSinCos.y), vPoint.yz));
}


#ifdef SPEEDTREE_BRANCH_FADING
///////////////////////////////////////////////////////////////////////  
//  ComputeFadeValue

float ComputeFadeValue(float fHint)
{
    float fCurrentLod = g_vBranchLodValues.x;
    float fLodRadius = g_vBranchLodValues.y;
    
    float fValue = (fCurrentLod - fHint) / fLodRadius;
    
    // we don't scale the fade from 0.0 to 1.0 because the range typically used by
    // SpeedTree for its alpha fizzle/fade effect is c_fOpaqueAlpha (84) to 
    // c_fClearAlpha (255), making the low end 84 / 255 instead of 0.
    //
    // 0.33f = c_fOpaqueAlpha / 255.0f
    return lerp(0.33f, 1.0f, clamp(fValue, 0.0f, 1.0f));
}
#endif
