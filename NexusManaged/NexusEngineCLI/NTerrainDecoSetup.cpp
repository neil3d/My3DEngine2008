#include "StdAfx.h"
#include "NTerrainDecoSetup.h"

namespace NexusEngine
{
	NTerrainDecoSetup::NTerrainDecoSetup( nexus::nterrain_deco_setup* deco )
	{
		m_NativePtr=deco;
		m_Layers=gcnew System::Collections::Generic::List<NTerrainDecoLayer^>();
		if(deco)
		{
			for(int i=0;i<NativePtr->get_deco_layer_count();++i)
			{
				nterrain_deco_layer::ptr _layer=NativePtr->get_deco_layer(i);
				nstring name=_layer->reflection_get_class()->get_name();
				if(name==_T("nterrain_grass_deco_layer"))
				{
					NTerrainGrassDecoLayer^ layer=gcnew NTerrainGrassDecoLayer(NativePtr->get_deco_layer(i),this);
					if(layer!=nullptr)
					{
						m_Layers->Add(layer);
					}
				}
				else if(name==_T("nterrain_static_mesh_deco_layer"))
				{
					NTerrainStaticMeshDecoLayer^ layer=gcnew NTerrainStaticMeshDecoLayer(NativePtr->get_deco_layer(i),this);
					if(layer!=nullptr)
					{
						m_Layers->Add(layer);
					}
				}
				else
				{
					NTerrainDecoLayer^ layer=gcnew NTerrainDecoLayer(NativePtr->get_deco_layer(i),this);
					if(layer!=nullptr)
					{
						m_Layers->Add(layer);
					}
				}
			}
		}
	}

	NTerrainDecoLayer^ NTerrainDecoSetup::AddLayer( DecoLayerType type,unsigned int size )
	{
		nterrain_deco_layer::ptr nlayer=NativePtr->add_deco_layer((deco_layer_type)type,size);
		if(nlayer==0) return nullptr;
		if(type==DecoLayerType::StaticMeshDecoLayer)
		{
			NTerrainDecoLayer^ layer=gcnew NTerrainStaticMeshDecoLayer(nlayer,this);
			m_Layers->Add(layer);
			return layer;
		}
		else if(type==DecoLayerType::GrassDecoLayer)
		{
			NTerrainDecoLayer^ layer=gcnew NTerrainGrassDecoLayer(nlayer,this);
			m_Layers->Add(layer);
			return layer;
		}
		return nullptr;
	}

	void NTerrainDecoSetup::DeleteLayer( int index )
	{
		m_Layers->RemoveAt(index);
		NativePtr->del_deco_layer(index);
	}

	NTerrainDecoLayer^ NTerrainDecoSetup::GetLayer( String^ name )
	{
		for each(NTerrainDecoLayer^ layer in m_Layers)
		{
			if(layer->Name==name)
			{
				return layer;
			}
		}
		return nullptr;
	}
}