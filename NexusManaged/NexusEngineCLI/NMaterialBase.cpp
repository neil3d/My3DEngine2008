#include "StdAfx.h"
#include "NMaterialBase.h"
#include "NMaterial.h"

namespace NexusEngine
{
	NMaterialBase::NMaterialBase(nexus::nmaterial_base::ptr nativeMtl)
		: NativeObjectOwnedBySharedPtr(nativeMtl)
	{}

	NMaterialBase^ NMaterialBase::FromNativePtr(nexus::nmaterial_base::ptr nativeMtl)
	{
		NMaterialBase^ mtl = nullptr;

		nclass* mtlClass = nativeMtl->reflection_get_class();
		if( mtlClass == nmaterial::reflection_get_static_class() )
		{
			mtl = gcnew NMaterial( boost::dynamic_pointer_cast<nmaterial>(nativeMtl) );
		}


		return mtl;
	}

	nmaterial_base* NMaterialBase::NativePtr::get()
	{
		return dynamic_cast<nmaterial_base*>(RawPtr);
	}

	String^ NMaterialBase::GetName()
	{
		return gcnew String( NativePtr->get_name().c_str() );
	}

	NResourceLoc NMaterialBase::GetTemplateLoc()
	{
		NResourceLoc loc;
		loc.FromNative( NativePtr->get_template_loc() );
		return loc;
	}

	nexus::nmaterial_base::ptr NMaterialBase::GetSmartPtr()
	{
		return boost::dynamic_pointer_cast<nmaterial_base>(*m_smart_ptr);
	}
}//