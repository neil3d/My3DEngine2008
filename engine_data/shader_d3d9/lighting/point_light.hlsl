/*
	Standard Blinn light equations:
		Diffuse = LightColor*(N dot L)
		Specular = LightColor*(H dot N)^spec
		Attenuation = a0+a1*d+a2*d^2
		
	参考：nvidia SDK 9 sample -- "vertex light"
*/
float4	lgtAmbientColor:LGT_AmbientColor;
float4	lgtDiffuseColor:LGT_DiffuseColor;
float3	lgtPos:LGT_Position;
float3	lgtAtten:LGT_Attenuation;
float3 eyePos:SYS_EyePos;

void lgtCalulate_WorldSpace(float3 vertPos, float3 vertNormal, float mtlSpecPower,
							out float4 lightDiffuse, out float4 lightSpecular)
{
	// Calculate vector from vertex to light, in world space
    float3 lightVec = (lgtPos - vertPos);
    float  d = length(lightVec);
    lightVec = lightVec / d;
    
	// Get the attenuation
    float attenuation = 1.0 / (lgtAtten.x + d*(lgtAtten.y + d*lgtAtten.z)); 
    
    // This is the intensity of the diffuse component
    float diffuse = dot(vertNormal, lightVec);          // N dot L
    
    // Calculate half-vector (light vector + eye vector)
	// This is used for the specular component
	float3 eyeVec = normalize(vertPos - eyePos);
    float3 halfVec = normalize(lightVec + eyeVec);
    
    // Dot normal with half-vector
    // This is the intensity of the specular component
    float spec = dot(vertNormal, halfVec);              // N dot H
    
    // Calculate the diffuse & specular factors
    float4 lighting = lit(diffuse, spec, mtlSpecPower); // produces float3(amb=1, diff, spec, 1)
    
    // Scale the factors by the attenuation
    lighting = lighting * attenuation;

	// return
	lightDiffuse = lgtAmbientColor + lgtDiffuseColor*lighting.y;
	lightSpecular = lgtDiffuseColor*lighting.z;
}

//-- per pixel lighting
//	优化：重点在于per pixel normal的使用，而per pixel postion没什么意义，所以只将与normal计算相关的参数输出
struct lgtTangentInfo
{
	float4	lightVec : COLOR0;	// w as attenuation
	float3	halfVec : COLOR1;
};

// run in vertex shader
void lgtMakeTangentInfo(float3 vertPos, float3x3 tangentBasis,
						out lgtTangentInfo info)
{
	float3 lightVec = (lgtPos - vertPos);
    float  d = length(lightVec);
    lightVec = lightVec / d;
    
    // Get the attenuation
    float attenuation = 1.0 / (lgtAtten.x + d*(lgtAtten.y + d*lgtAtten.z)); 
    
    // Calculate half-vector (light vector + eye vector)
	// This is used for the specular component
	float3 eyeVec = normalize(vertPos-eyePos);
    float3 halfVec = normalize(lightVec + eyeVec);
    
    // transform to tangent space
    lightVec = mul(tangentBasis, lightVec);
    halfVec = mul(tangentBasis, halfVec);
    
    // return
    info.lightVec.xyz = lightVec.xyz;
    info.lightVec.w = attenuation;
    info.halfVec = halfVec;
}

//-- run in pixel shader
void lgtCalculate_TangentSpace(float3 pixelNormal, float mtlSpecPower,
								lgtTangentInfo info,
								out float4 lightDiffuse, out float4 lightSpecular)
{
	float3 lightVec = normalize(info.lightVec.xyz);
	float3 halfVec = normalize(info.halfVec);
	float attenuation = info.lightVec.w;
	
	float diffuse = dot(pixelNormal, lightVec);          // N dot L
	float spec = dot(pixelNormal, halfVec);    // N dot H
	
	// Calculate the diffuse & specular factors
    float4 lighting = lit(diffuse, spec, mtlSpecPower); // produces float3(amb=1, diff, spec, 1)
 
	// Scale the factors by the attenuation
    lighting = lighting * attenuation;   
    
    // return
	lightDiffuse = lgtAmbientColor + lgtDiffuseColor*lighting.y;
	lightSpecular = lgtDiffuseColor*lighting.z;
}