#include "StdAfx.h"
#include "NTerrainDecoLayer.h"
#include "NTerrainDecoSetup.h"
namespace NexusEngine
{
	NTerrainDecoLayer::NTerrainDecoLayer(nexus::nterrain_deco_layer::ptr layer,NTerrainDecoSetup^ owner):NativeObjectOwnedBySharedPtr(layer),m_Owner(owner)
	{
	}

	NTerrainStaticMeshDecoLayer::NTerrainStaticMeshDecoLayer( nexus::nterrain_deco_layer::ptr layer ,NTerrainDecoSetup^ owner):NTerrainDecoLayer(layer,owner)
	{

	}

	NTerrainGrassDecoLayer::NTerrainGrassDecoLayer( nterrain_deco_layer::ptr layer ,NTerrainDecoSetup^ owner):NTerrainDecoLayer(layer,owner)
	{

	}

	void NTerrainGrassDecoLayer::Name::set(String^ name)
	{
		if(m_Owner->GetLayer(name)!=nullptr)
		{
			throw gcnew System::Exception(L"×°ÊÎ²ãÃû³ÆÖØ¸´");
			return;
		}
		pin_ptr<const wchar_t> layer_name = PtrToStringChars(name);
		NativePtr->set_name(layer_name);
	}	
};