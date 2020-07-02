#include "StdAfx.h"
#include "NTerrainMtlSetup.h"

namespace NexusEngine
{
	LayerData FromNative(const layer_data_editor& ld )
	{
		LayerData layer;
		layer.UVScale.x = ld.uv_scale.x;
		layer.UVScale.y = ld.uv_scale.y;
		layer.UVRotate = ld.uv_rotate;
		layer.Specular = ld.spec;
		layer.SpecularPower = ld.spec_power;
		layer.DiffuseTexture.FromNative(ld.diffuse_tex->get_location());
		if (ld.normal_tex)
		{
			layer.NormalTexture.FromNative(ld.normal_tex->get_location());
		}

		return layer;
	}

	layer_data_editor ToNative(LayerData% layer )
	{
		layer_data_editor ld;
		ld.uv_scale.x = layer.UVScale.x;
		ld.uv_scale.y = layer.UVScale.y ;
		ld.uv_rotate = layer.UVRotate;
		ld.spec = layer.Specular;
		ld.spec_power = layer.SpecularPower;
		resource_location diffuseLoc, normalLoc;
		layer.DiffuseTexture.ToNative(diffuseLoc);
		layer.NormalTexture.ToNative(normalLoc);

		ld.diffuse_tex =  nresource_manager::instance()->load_texture_2d(diffuseLoc);
		if (normalLoc.is_valid())
		{
			ld.normal_tex =  nresource_manager::instance()->load_texture_2d(normalLoc);
		}

		return ld;
	}

	NTerrainMtlSetup::NTerrainMtlSetup(nterrain_mtl_setup* borrowedNativeObj) : m_obj(borrowedNativeObj)
	{
	}

	void NTerrainMtlSetup::Create(NResourceLoc^ basicTextureLoc, int alphaWidth, int aplhaHeight)
	{
		resource_location nbasic;
		basicTextureLoc->ToNative(nbasic);

		m_obj->create( nbasic, alphaWidth, aplhaHeight);
	}

	int NTerrainMtlSetup::GetNumLayers()
	{
		return m_obj->get_num_layers();
	}

	LayerData NTerrainMtlSetup::GetLayerData(int i)
	{
		return FromNative(m_obj->get_layer_data(i));
	}

	void NTerrainMtlSetup::SetLayerData(int i,	LayerData layer)
	{
		m_obj->set_layer_data(i,ToNative(layer));
	}

	void NTerrainMtlSetup::GenerateNoise(int i, Windows::Int32Rect rc, int numOctaves, float amplitude, float frequency)
	{
		nstring layerName = m_obj->get_layer_name(i);
		if( !layerName.empty() )
		{
			nrect nrc;
			nrc.left = rc.X;
			nrc.right = rc.X+rc.Width;
			nrc.top = rc.Y;
			nrc.bottom = rc.Y+rc.Height;

			m_obj->generate_noise(layerName, nrc, numOctaves, amplitude, frequency);
		}
	}

	int NTerrainMtlSetup::AddLayer(LayerData layer)
	{
		return m_obj->add_layer(ToNative(layer));
	}

	void NTerrainMtlSetup::UpdateLayer()
	{
		m_obj->update_layer();
	}

	void NTerrainMtlSetup::RemoveLayer(int i)
	{
		m_obj->remove_layer(i);
	}

	void NTerrainMtlSetup::ShiftUpLayer(int i)
	{
		m_obj->shift_up_layer(i);
	}

	void NTerrainMtlSetup::ShiftDownLayer(int i)
	{
		m_obj->shift_down_layer(i);
	}
}//namespace NexusEngine