#include "StdAfx.h"
#include "NResourceTexture.h"
#include "NWrapperException.h"
#include "math/Vector2.h"

namespace NexusEngine
{
	NResourceTexture::NResourceTexture(nresource_texture::ptr nativeRes):NResource(nativeRes)
	{
	}

	NResourceTexture2D::NResourceTexture2D(nexus::nresource_texture_2d::ptr nativeObj)
		: NResourceTexture(nativeObj)
	{
	}

	void NResourceTexture2D::LoadFromFile(NResourceLoc loc)
	{
		BEGIN_NATIVE_GUARD

			nexus::resource_location nloc;
		loc.ToNative(nloc);

		NativePtr->load_from_file(nloc);

		END_NATIVE_GUARD
	}

	Vector2 NResourceTexture2D::GetOriginalSize()
	{
		nresource_texture_2d* res = static_cast<nresource_texture_2d*>(NativePtr); 
		fsize org_size = res->get_render_texture2d()->get_original_size();
		return Vector2( org_size.x, org_size.y );
	}

	nexus::nrender_texture2D* NResourceTexture2D::Texture2D::get()
	{
		nresource_texture_2d* res = static_cast<nresource_texture_2d*>(NativePtr); 
		if( res )
		{
			return res->get_render_texture2d();
		}
		return NULL;
	}

}//namespace NexusEngine