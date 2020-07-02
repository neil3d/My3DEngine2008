#include "StdAfx.h"
#include "NMaterial.h"
#include "NWrapperException.h"

namespace NexusEngine
{
	NMaterial::NMaterial(nexus::nmaterial::ptr nativeMtl):NMaterialBase(nativeMtl)
	{
	}

	nmaterial* NMaterial::NativePtr::get()
	{
		return dynamic_cast<nmaterial*>(RawPtr);
	}

	NMaterial::NMaterial(String^ name):NMaterialBase(name)
	{
		pin_ptr<const wchar_t> szName = PtrToStringChars(name);
		if( m_smart_ptr )
			delete m_smart_ptr;
		
		nexus::nmaterial* mtl = new nexus::nmaterial(szName);
		m_smart_ptr = new boost::shared_ptr<nobject>( mtl );
	}

	void NMaterial::Create(NMaterialTemplate^ temp)
	{
		if( temp == nullptr )
		{
			throw gcnew System::ArgumentException("Bad Material Templater");
		}
		else
		{
			NativePtr->create(temp->NativePtr);
		}
	}

	void NMaterial::Create(NResourceLoc tempLoc)
	{
		BEGIN_NATIVE_GUARD

		nexus::resource_location nloc;
		tempLoc.ToNative(nloc);

		NativePtr->create(nloc);

		END_NATIVE_GUARD
	}

	NMaterialParam NMaterial::GetParameter(int i)
	{
		const nexus::material_parameter_value& nparam = NativePtr->get_parameter(i);

		NMaterialParam ret;
		ret.FromNative(nparam);
		return ret;
	}

	void NMaterial::SetParameter(NMaterialParam param)
	{
		BEGIN_NATIVE_GUARD

		pin_ptr<const wchar_t> szName = PtrToStringChars(param.name);

		switch(param.type)
		{
		case EMaterialParameter::Float:
		case EMaterialParameter::Float2:
		case EMaterialParameter::Float3:
		case EMaterialParameter::Float4:
			{
				nexus::vector4* pVal = reinterpret_cast<nexus::vector4*>(&param.floatVal);
				NativePtr->set_vector_parameter(szName, *pVal);
			}
			break;
		case EMaterialParameter::Texture:
			{
				nexus::resource_location nloc;
				param.textureLoc.ToNative(nloc);
				NativePtr->set_texture_parameter(szName, nloc);
			}
			break;
		case EMaterialParameter::Int:
			{
				NativePtr->set_int_param(szName, param.intVal);
			}
		default:
			break;
		}

		END_NATIVE_GUARD
	}

	nexus::nmaterial::ptr NMaterial::GetSmartPtr()
	{
		return boost::dynamic_pointer_cast<nexus::nmaterial>(*m_smart_ptr);
	}
}//namespace NexusEngine