
float4	lgtAmbientColor:LGT_AmbientColor;
float4	lgtDiffuseColor:LGT_DiffuseColor;
float3	lgtDir:LGT_DIRECTION;
float3	eyePos:SYS_EyePos;

void lgtCalulate_WorldSpace(float3 vertPos, float3 vertNormal, float mtlSpecPower,
							out float4 lightDiffuse, out float4 lightSpecular)
{    
    // This is the intensity of the diffuse component
    float diffuse = dot(vertNormal, lgtDir);          // N dot L
    
    // Calculate half-vector (light vector + eye vector)
	// This is used for the specular component
	float3 eyeVec = normalize(vertPos - eyePos);
    float3 halfVec = normalize(lgtDir + eyeVec);
    
    // Dot normal with half-vector
    // This is the intensity of the specular component
    float spec = dot(vertNormal, halfVec);              // N dot H
    
    // Calculate the diffuse & specular factors
    float4 lighting = lit(diffuse, spec, mtlSpecPower); // produces float3(amb=1, diff, spec, 1)
    
	// return
	lightDiffuse = lgtDiffuseColor*lighting.y;
	lightSpecular = lgtDiffuseColor*lighting.z;
}

//---------------------------------------------------------------
struct lgtTangentInfo
{
	float3	lightVec : COLOR0;
	float3	halfVec : COLOR1;
};

void lgtMakeTangentInfo(float3 vertPos, float3x3 tangentBasis,
						out lgtTangentInfo info)
{
	info.lightVec = mul(tangentBasis, lgtDir);
	
	// world space half vector
	float3 eyeVec = normalize(vertPos - eyePos);
    float3 halfVec = normalize(lgtDir + eyeVec);
    
    //
    info.halfVec = mul(tangentBasis, halfVec);
}

void lgtCalculate_TangentSpace(float3 pixelNormal, float mtlSpecPower,
								lgtTangentInfo info,
								out float4 lightDiffuse, out float4 lightSpecular)
{
	float3 lightVec = normalize(info.lightVec);
	float3 halfVec = normalize(info.halfVec);
	
	float diffuse = dot(pixelNormal, lightVec);          // N dot L	
	float spec = dot(pixelNormal, halfVec);              // N dot H
	
	float4 lighting = lit(diffuse, spec, mtlSpecPower);
	
	// return
	lightDiffuse = lgtDiffuseColor*lighting.y;
	lightSpecular = lgtDiffuseColor*lighting.z;
}