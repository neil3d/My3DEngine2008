#include "material.hlsl"
#include "vertex_factory.hlsl"
#include "lighting.hlsl"

void vsMain(vfInput vert,
			out vfInterpolants inter
#ifdef VERTEX_LIGHTING
			,out float4 outLgtDiffuse: COLOR0
			,out float4 outLgtSpecular: COLOR1
#else
			,out lgtTangentInfo lgt
#endif
			,out float4 outPos : POSITION
			)
{
	vfMakeInterpolants(vert, inter);

#if MTL_LIGHT 
	#ifdef VERTEX_LIGHTING
		float3 worldPos = vfGetWorldPos(vert);
		float3 worldNormal = vfGetWorldNormal(vert);
		
		lgtCalulate_WorldSpace(worldPos, worldNormal, mtlGetSpecularPower_Vert(),
								outLgtDiffuse, outLgtSpecular);
		#ifdef MTL_TWO_SIDED
			float4 lgtDiffuse2;
			float4 lgtSpecular2;
			
			lgtCalulate_WorldSpace(worldPos, -worldNormal, mtlGetSpecularPower_Vert(),
								lgtDiffuse2, lgtSpecular2);
			outLgtDiffuse += lgtDiffuse2;
			outLgtSpecular += lgtSpecular2;
		#endif
	#else
		lgtMakeTangentInfo(vfGetWorldPos(vert), vfGetTangentBasis(vert), lgt);
	#endif
#endif
	
	outPos = vfTransformPos(vert);
}

float4 psMain(vfInterpolants inter
#ifdef VERTEX_LIGHTING
			,float4 lgtDiffuse: COLOR0
			,float4 lgtSpecular: COLOR1
#else
			,lgtTangentInfo lgt
#endif
			) : COLOR
{
	mtlParameters mtlParam;
	vfMakeMaterialParameters(inter, mtlParam);
	
	float4 result=0;
	
	float4 diffuseColor = mtlGetDiffuseColor(mtlParam);
	float4 specularColor = mtlGetSpecularColor(mtlParam);
	
#if MTL_LIGHT 
	#ifdef VERTEX_LIGHTING
		result = diffuseColor * lgtDiffuse;
		result += specularColor * lgtSpecular;
	#else
		float3 pixelNormal = mtlGetNormal(mtlParam);
		float  specPower = mtlGetSpecularPower(mtlParam);
		float4 lgtDiffuse;
		float4 lgtSpecular;
		
		lgtCalculate_TangentSpace(pixelNormal, specPower, lgt, 
				lgtDiffuse, lgtSpecular);
				
		result = diffuseColor * lgtDiffuse;
		result += specularColor * lgtSpecular;
				
		#ifdef MTL_TWO_SIDED			
			float4 lgtDiffuse2;
			float4 lgtSpecular2;
		
			lgtCalculate_TangentSpace(-pixelNormal, specPower, lgt, 
				lgtDiffuse2, lgtSpecular2);
				
			result += diffuseColor * lgtDiffuse2;
			result += specularColor * lgtSpecular2;
		#endif
		
	#endif
#endif

	result.a = mtlGetOpacity(mtlParam);
	return result;
}

technique techOpaque
{
	pass p0
	{
		ZEnable = True;
		ZWriteEnable = False;
		ZFunc = LESSEQUAL;		
		
		BlendOp = Add;
		SrcBlend = One;
		DestBlend = One;
		AlphaBlendEnable = True;
		
		#ifdef MTL_USE_ALPHA_MASK
			AlphaTestEnable = True;
			AlphaRef = g_alphaMask;
			AlphaFunc = GreaterEqual;
		#endif
		
		#ifdef MTL_TWO_SIDED
			CullMode = None;
		#endif

		VertexShader = compile vs_3_0 vsMain();
		PixelShader = compile ps_3_0 psMain();
	}
}

// 绘制mixed transparent物体的不透明部分
technique techLowTransPass
{
	pass p0
	{
		ZEnable = True;
		ZWriteEnable = False;
		ZFunc = LESSEQUAL;		
		
		BlendOp = Add;
		SrcBlend = One;
		DestBlend = One;
		AlphaBlendEnable = True;
				
		AlphaTestEnable = True;
		AlphaRef = 220;
		AlphaFunc = GreaterEqual;		
		
		#ifdef MTL_TWO_SIDED
			CullMode = None;
		#endif

		VertexShader = compile vs_3_0 vsMain();
		PixelShader = compile ps_3_0 psMain();
	}
}

// 绘制mixed transparent物体的透明部分
technique techHighTransPass
{
	pass p0
	{
		ZEnable = True;
		ZWriteEnable = False;
		ZFunc = LESSEQUAL;		
		
		BlendOp = Add;
		SrcBlend = One;
		DestBlend = One;
		AlphaBlendEnable = True;
		
		/*
		SeparateAlphaBlendenable = True;
		BlendOpAlpha = Max;
		SrcBlendAlpha = One;
		DestBlendAlpha = Zero;		
		*/
		
		AlphaTestEnable = True;
		AlphaRef = 220;
		AlphaFunc = Less;		
		
		#ifdef MTL_TWO_SIDED
			CullMode = None;
		#endif

		VertexShader = compile vs_3_0 vsMain();
		PixelShader = compile ps_3_0 psMain();
	}
}