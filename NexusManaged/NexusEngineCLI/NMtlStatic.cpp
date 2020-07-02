#include "StdAfx.h"
#include "NMtlStatic.h"
#include "NWrapperException.h"

namespace NexusEngine
{
	NMtlStatic::NMtlStatic(String^ nameStr):NMtlCommon(nameStr)
	{
		pin_ptr<const wchar_t> szName = PtrToStringChars(nameStr);
		if( m_smart_ptr )
			delete m_smart_ptr;

		nexus::nmtl_static* mtl = new nexus::nmtl_static(szName);
		m_smart_ptr = new boost::shared_ptr<nobject>( mtl );
	}

	NMtlStatic::~NMtlStatic(void)
	{
	}

	NMtlStatic::NMtlStatic(nexus::nmtl_static::ptr nativeMtl)
		: NMtlCommon(nativeMtl)
	{}

	nmtl_static* NMtlStatic::NativePtr::get()
	{
		return dynamic_cast<nmtl_static*>(RawPtr);
	}

	void NMtlStatic::SetFloatParameter(String^ paramName, Vector4 val)
	{
		pin_ptr<const wchar_t> szName = PtrToStringChars(paramName);
		
		NativePtr->set_value_parameter(conv_string(nstring(szName)), val.ToNative());
	}

	void NMtlStatic::DefaultCreate(NResourceLoc shaderLoc)
	{
		BEGIN_NATIVE_GUARD

		nexus::resource_location nloc;
		shaderLoc.ToNative(nloc);

		NativePtr->default_create(nloc);

		END_NATIVE_GUARD
	}

	int NMtlStatic::GetNumParameter()
	{
		return NativePtr->get_num_texture_param()
			+ NativePtr->get_num_value_param();
	}

	NMaterialParam NMtlStatic::GetParameter(int i)
	{
		int num_tex = NativePtr->get_num_texture_param();
		
		NMaterialParam ret;
		if( i>=num_tex )
		{
			const nexus::material_parameter& nparam = NativePtr->get_value_param(i-num_tex);			
			ret.FromNative(nparam);			
		}
		else
		{
			const nexus::material_parameter& nparam = NativePtr->get_texture_param(i);			
			ret.FromNative(nparam);
		}
		return ret;
	}

	void NMtlStatic::SetParameter(NMaterialParam param)
	{
		BEGIN_NATIVE_GUARD

		pin_ptr<const wchar_t> szName = PtrToStringChars(param.name);
		std::string paramName = conv_string( nstring(szName) );

		switch (param.type)
		{
		case EMaterialParameter::Float:
		case EMaterialParameter::Float2:
		case EMaterialParameter::Float3:
		case EMaterialParameter::Float4:
			{
				nexus::vector4* pVal = reinterpret_cast<nexus::vector4*>(&param.floatVal);
				NativePtr->set_value_parameter(paramName, *pVal);
			}
			break;
		case EMaterialParameter::Texture:
			{
				nexus::resource_location nloc;
				param.textureLoc.ToNative(nloc);
				if( param.textureType == ETextureType::CubeMap )
					NativePtr->set_cube_map_parameter(paramName, nloc);
				else //if( param.textureType == ETextureType::Texture2d )
					NativePtr->set_texture2d_parameter(paramName, nloc);
			}
			break;
		case EMaterialParameter::Int:
			NativePtr->set_value_parameter(paramName, param.intVal);
			break;
		}

		END_NATIVE_GUARD
	}

}//namespace NexusEngine