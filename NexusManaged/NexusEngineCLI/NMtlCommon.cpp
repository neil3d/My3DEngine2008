#include "StdAfx.h"
#include "NMtlCommon.h"
#include "NWrapperException.h"

namespace NexusEngine
{
	NMtlCommon::NMtlCommon(String^ nameStr):NMtlBase(nameStr)
	{
	}

	NMtlCommon::~NMtlCommon(void)
	{
	}

	NMtlCommon::NMtlCommon(nexus::nmtl_common::ptr nativeMtl):NMtlBase(nativeMtl)
	{
	}

	nmtl_common* NMtlCommon::NativePtr::get()
	{
		return dynamic_cast<nmtl_common*>(RawPtr);
	}

	void NMtlCommon::SetTexture2DParameter(String^ paramName, NResourceLoc texLoc)
	{
		BEGIN_NATIVE_GUARD

		pin_ptr<const wchar_t> szName = PtrToStringChars(paramName);
		nexus::resource_location nloc;
		texLoc.ToNative(nloc);

		NativePtr->set_texture2d_parameter(conv_string(nstring(szName)), nloc);

		END_NATIVE_GUARD
	}

	void NMtlCommon::SetCubeMapParameter(String^ paramName, NResourceLoc texLoc)
	{
		BEGIN_NATIVE_GUARD

		pin_ptr<const wchar_t> szName = PtrToStringChars(paramName);
		nexus::resource_location nloc;
		texLoc.ToNative(nloc);

		NativePtr->set_cube_map_parameter(conv_string(nstring(szName)), nloc);

		END_NATIVE_GUARD
	}
}//namespace NexusEngine