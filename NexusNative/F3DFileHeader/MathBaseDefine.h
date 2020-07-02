
#pragma once
#include <d3dx9math.h>
#pragma comment(lib,"d3dx9.lib")
#include <string>


namespace F3D
{
	const float FLOAT_MAX=3.402823466e+38f;
	const float FLOAT_EPSILON=1.192092896e-07F;
	const float FLOAT_PI=3.141592654f;

	//Ω«∂»”Îª°∂»ªªÀ„
	inline float _DegToRad(float a) { return a*0.01745329252f;};
	inline float _RadToDeg(float a) { return a*57.29577951f;};

	typedef D3DXVECTOR2		Vector2;
	typedef D3DXVECTOR3		Vector3;
	typedef D3DXVECTOR4		Vector4;
	typedef D3DXQUATERNION	Quaternion;
	typedef D3DXMATRIX		Matrix4;

	struct Matrix2
	{
		float  _11, _12;
		float  _21, _22;
	};

	inline DWORD _MakeFourCC( std::string szStr ) 
	{ 
		char str[4];
		memset(str,0,sizeof(str));
		strncpy(str,szStr.c_str(),sizeof(str));
		return (str[3]<<24) | (str[2]<<16) | (str[1]<<8) | str[0]; 
	}

	inline std::string _FourCC2Str(DWORD dw)
	{
		char str[5];
		str[0]=(char)(dw&0xff); 
		str[1]=(char)((dw>>8)&0xff); 
		str[2]=(char)((dw>>16)&0xff); 
		str[3]=(char)((dw>>24)&0xff);
		str[4]=0;
		return std::string(str);
	}
}//namespace F3D