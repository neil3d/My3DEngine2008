#pragma once
#include "NResourceTexture.h"
#include "NMath.h"

namespace NexusEngine
{
	public value struct NTexAtlasItem
	{
		System::String^ name;
		Vector2			uvStart;
		Vector2			uvEnd;

		[System::ComponentModel::CategoryAttribute("Texture Atlas Item")]
		property System::String^ Name
		{
			System::String^ get()	{	return name; }
			void set(String^ val)	{	name = val; }
		}

		[System::ComponentModel::CategoryAttribute("Texture Atlas Item")]
		property  Vector2 UVStart
		{
			Vector2 get()			{	return uvStart; }
			void set(Vector2 val)	{	uvStart =  val; }
		}

		[System::ComponentModel::CategoryAttribute("Texture Atlas Item")]
		property  Vector2 UVEnd
		{
			Vector2 get()			{	return uvEnd; }
			void set(Vector2 val)	{	uvEnd =  val; }
		}

		void FromNative(const nexus::tex_atlas_item& nitem);
	};

	public ref class NResourceTextureAtlas : public NResourceTexture2D
	{
	public:
		NResourceTextureAtlas(nresource_texture_atlas::ptr nativeRes);

		NTexAtlasItem GetItem(System::String^ name);
		bool FirstItem(NTexAtlasItem% outItem);
		bool NextItem(NTexAtlasItem% outItem);

	private:
		property nresource_texture_atlas* NativePtr
		{
			nresource_texture_atlas* get();
		}
	};
}//namespace NexusEngine