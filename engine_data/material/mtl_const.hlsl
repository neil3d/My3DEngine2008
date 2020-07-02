/**
	使用const值来补全所有的material通道;
	在material script文件的最后include这个文件;
*/
#ifndef MTL_HAVE_DIFFUSE
float4 mtlGetDiffuseColor(mtlParameters mtlParam)
{
	return float4(0,0.44f,0.18f,1);
}
#endif

#ifndef MTL_HAVE_EMISSIVE
float4 mtlGetEmissiveColor(mtlParameters mtlParam)
{
	return float4(0,0,0,0);
}
#endif

#ifndef MTL_HAVE_SPECULAR_POWER_VERT	// vertex lighting所使用的secpular power
float mtlGetSpecularPower_Vert()
{
	return 4.0f;
}
#endif

#ifndef MTL_HAVE_SPECULAR_POWER
float mtlGetSpecularPower(mtlParameters mtlParam)
{
	return 4.0f;
}
#endif

#ifndef MTL_HAVE_SPECULAR_COLOR
float4 mtlGetSpecularColor(mtlParameters mtlParam)
{
	return float4(0,0,0,1);
}
#endif

#ifndef MTL_HAVE_NORMAL
float3 mtlGetNormal(mtlParameters mtlParam)
{
	return float3(0,1,0);
}
#endif

#ifndef MTL_HAVE_OPACITY
float mtlGetOpacity(mtlParameters mtlParam)
{
	return 1;
}
#endif 