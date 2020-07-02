#include "StdAfx.h"
#include "NMtlBase.h"
#include "NMtlStatic.h"
#include "NWrapperException.h"
#include "NEngine.h"
#include "NFileSystem.h"

namespace NexusEngine
{
	NMtlBase::NMtlBase(String^ nameStr)
	{
	}

	NMtlBase::~NMtlBase(void)
	{
	}

	NMtlBase::NMtlBase(nexus::nmtl_base::ptr nativeMtl)
		: NativeObjectOwnedBySharedPtr(nativeMtl)
	{}

	NMtlBase^ NMtlBase::FromNativePtr(nexus::nmtl_base::ptr nativeMtl)
	{
		NMtlBase^ mtl = nullptr;
		
		if(nativeMtl==NULL) return nullptr;

		nclass* mtlClass = nativeMtl->reflection_get_class();
		nstring className( mtlClass->get_name() );
		if( className == _T("nmtl_static") )
		{
			mtl = gcnew NMtlStatic( boost::dynamic_pointer_cast<nmtl_static>(nativeMtl) );
		}

		return mtl;
	}

	NMtlBase^ NMtlBase::FromFile( NResourceLoc shaderLoc )
	{
		resource_location file_loc;
		shaderLoc.ToNative(file_loc);

		return FromNativePtr(nmtl_base::create_from_file(file_loc));
	}

	nmtl_base* NMtlBase::NativePtr::get()
	{
		return dynamic_cast<nmtl_base*>(RawPtr);
	}

	void NMtlBase::CreateTechFromHLSL(String^ nameStr, NResourceLoc shaderLoc)
	{
		BEGIN_NATIVE_GUARD

		pin_ptr<const wchar_t> szName = PtrToStringChars(nameStr);
		nexus::resource_location nloc;
		shaderLoc.ToNative(nloc);

		NativePtr->create_from_hlsl(szName, nloc);
		END_NATIVE_GUARD
	}

	nexus::nmtl_base::ptr NMtlBase::GetSmartPtr()
	{
		return boost::dynamic_pointer_cast<nmtl_base>(*m_smart_ptr);
	}

	void NMtlBase::SaveToFile( NResourceLoc shaderLoc )
	{
		resource_location file_loc;
		shaderLoc.ToNative(file_loc);

		NativePtr->save_to_file(file_loc);
	}

}//namespace NexusEngine