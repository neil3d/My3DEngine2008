#pragma once
#include "NResource.h"
#include "NMath.h"

namespace NexusEngine
{
	public value struct LayerData
	{
		Vector2				UVScale;
		float					UVRotate;		
		float					Specular;
		float					SpecularPower;		
		NResourceLoc	DiffuseTexture;
		NResourceLoc	NormalTexture;
	};

	public ref class NTerrainMtlSetup
	{
	public:
		void Create(NResourceLoc^ basicTextureLoc, int alphaWidth, int aplhaHeight);
		int  GetNumLayers();
		LayerData GetLayerData(int i);
		void SetLayerData(int i,LayerData layer);
		void GenerateNoise(int i, Windows::Int32Rect rc, int numOctaves, float amplitude, float frequency);

		int  AddLayer(LayerData layer);
		void UpdateLayer();
		void RemoveLayer(int i);
		void ShiftUpLayer(int i);
		void ShiftDownLayer(int i);

		//-- Native包装接口
	public:
		NTerrainMtlSetup(nterrain_mtl_setup* borrowedNativeObj);

	private:
		nterrain_mtl_setup*	m_obj;
	};
}//namespace NexusEngine