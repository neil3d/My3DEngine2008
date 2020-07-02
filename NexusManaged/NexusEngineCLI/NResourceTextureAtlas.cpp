#include "StdAfx.h"
#include "NResourceTextureAtlas.h"

namespace NexusEngine
{
	void NTexAtlasItem::FromNative(const nexus::tex_atlas_item& nitem)
	{
		name = gcnew System::String( nitem.name.c_str() );
		uvStart = Vector2::FromNative(nitem.uv_start);
		uvEnd = Vector2::FromNative(nitem.uv_end);
	}

	//--
	NResourceTextureAtlas::NResourceTextureAtlas(nresource_texture_atlas::ptr nativeRes):NResourceTexture2D(nativeRes)
	{
	}

	NTexAtlasItem NResourceTextureAtlas::GetItem(System::String^ name)
	{
		pin_ptr<const wchar_t> szName = PtrToStringChars(name);
		nexus::tex_atlas_item nitem = NativePtr->get_item(szName);

		NTexAtlasItem mitem;
		mitem.FromNative(nitem);
		return mitem;
	}

	bool NResourceTextureAtlas::FirstItem(NTexAtlasItem% outItem)
	{
		nexus::tex_atlas_item nitem;
		bool ret = NativePtr->first_item(nitem);
		if( ret )
			outItem.FromNative(nitem);
		return ret;
	}

	bool NResourceTextureAtlas::NextItem(NTexAtlasItem% outItem)
	{
		nexus::tex_atlas_item nitem;
		bool ret = NativePtr->next_item(nitem);
		if( ret )
			outItem.FromNative(nitem);
		return ret;
	}

	nresource_texture_atlas* NResourceTextureAtlas::NativePtr::get()
	{
		return (nresource_texture_atlas*)m_nativeResPtr->get();
	}
}//namespace NexusEngine
