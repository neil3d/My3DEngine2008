#include "StdAfx.h"
#include "NResourceManager.h"
#include "NWrapperException.h"

namespace NexusEngine
{
	NResourceManager::NResourceManager(void)
	{
	}

	NResourceStaticMesh^ NResourceManager::NewStaticMesh(System::String^ resName)
	{
		pin_ptr<const wchar_t> szResName = PtrToStringChars(resName);
		NResourceStaticMesh^ ret = gcnew NResourceStaticMesh(
			nresource_manager::instance()->new_static_mesh(szResName)
			);
		return ret;
	}

	NResourceAnimMesh^ NResourceManager::NewAnimMesh(System::String^ resName)
	{
		pin_ptr<const wchar_t> szResName = PtrToStringChars(resName);
		NResourceAnimMesh^ ret = gcnew NResourceAnimMesh(
			nresource_manager::instance()->new_anim_mesh(szResName)
			);
		return ret;
	}

	NResourceSpecialEffect^ NResourceManager::NewSpecialEffect(System::String^ resName)
	{
		pin_ptr<const wchar_t> szResName = PtrToStringChars(resName);
		NResourceSpecialEffect^ ret = gcnew NResourceSpecialEffect(
			nresource_manager::instance()->new_special_effect(szResName)
			);
		return ret;
	}

	NMtlTechShader^ NResourceManager::LoadMtlShader(NResourceLoc loc, EResourceIOMode mode, EResourceIOPriority pri)
	{
		BEGIN_NATIVE_GUARD

		nexus::resource_location nloc;
		loc.ToNative(nloc);

		NMtlTechShader^ ret = gcnew NMtlTechShader(
			nresource_manager::instance()->load_mtl_shader(nloc,
			static_cast<nexus::EResourceIOMode>(mode),
			static_cast<nexus::EResourceIOPriority>(pri) )
			);
		return ret;

		END_NATIVE_GUARD		

		return nullptr;
	}

	NResourceStaticMesh^ NResourceManager::LoadStaticMesh(NResourceLoc loc, EResourceIOMode mode, EResourceIOPriority pri)
	{
		BEGIN_NATIVE_GUARD

		nexus::resource_location nloc;
		loc.ToNative(nloc);

		NResourceStaticMesh^ ret = gcnew NResourceStaticMesh(
			nresource_manager::instance()->load_static_mesh(nloc,
			static_cast<nexus::EResourceIOMode>(mode),
			static_cast<nexus::EResourceIOPriority>(pri) )
			);

		return ret;

		END_NATIVE_GUARD
		
		return nullptr;
	}

	NResourceAnimMesh^ NResourceManager::LoadAnimMesh(NResourceLoc loc, EResourceIOMode mode, EResourceIOPriority pri)
	{
		BEGIN_NATIVE_GUARD

			nexus::resource_location nloc;
		loc.ToNative(nloc);

		NResourceAnimMesh^ ret = gcnew NResourceAnimMesh(
			nresource_manager::instance()->load_anim_mesh(nloc,
			static_cast<nexus::EResourceIOMode>(mode),
			static_cast<nexus::EResourceIOPriority>(pri) )
			);

		return ret;

		END_NATIVE_GUARD

			return nullptr;
	}

	NResourceTexture2D^ NResourceManager::LoadResourceTexture2D(NResourceLoc loc, EResourceIOMode mode,EResourceIOPriority priority)
	{
		BEGIN_NATIVE_GUARD

			nexus::resource_location nativeLoc;
		loc.ToNative(nativeLoc);

		NResourceTexture2D^ tex2D = gcnew NResourceTexture2D( 
			nresource_manager::instance()->load_texture_2d(nativeLoc,
			static_cast<nexus::EResourceIOMode>(mode),
			static_cast<nexus::EResourceIOPriority>(priority) ) );

		return tex2D;

		END_NATIVE_GUARD
		return nullptr;
	}

	NResourceTextureAtlas^ NResourceManager::LoadTextureAtlas(NResourceLoc loc, EResourceIOMode mode, EResourceIOPriority pri)
	{
		BEGIN_NATIVE_GUARD

			nexus::resource_location nloc;
		loc.ToNative(nloc);

		NResourceTextureAtlas^ ret = gcnew NResourceTextureAtlas(
			nresource_manager::instance()->load_texture_atlas(nloc,
			static_cast<nexus::EResourceIOMode>(mode),
			static_cast<nexus::EResourceIOPriority>(pri) )
			);

		return ret;

		END_NATIVE_GUARD

			return nullptr;
	}

	NResourceSkeletalAnimSet^ NResourceManager::LoadSkeletalAnimSet( NResourceLoc loc, EResourceIOMode mode, EResourceIOPriority pri )
	{
		BEGIN_NATIVE_GUARD

		nexus::resource_location nloc;
		loc.ToNative(nloc);

		NResourceSkeletalAnimSet^ ret = gcnew NResourceSkeletalAnimSet(
			nresource_manager::instance()->load_skeletal_anim_set(nloc,
			static_cast<nexus::EResourceIOMode>(mode),
			static_cast<nexus::EResourceIOPriority>(pri) )
			);

		return ret;

		END_NATIVE_GUARD

		return nullptr;
	}

	NResourceSkeletalMesh^ NResourceManager::LoadSkeletalMesh( NResourceLoc loc, EResourceIOMode mode, EResourceIOPriority pri )
	{
		BEGIN_NATIVE_GUARD

			nexus::resource_location nloc;
		loc.ToNative(nloc);

		NResourceSkeletalMesh^ ret = gcnew NResourceSkeletalMesh(
			nresource_manager::instance()->load_skeletal_mesh(nloc,
			static_cast<nexus::EResourceIOMode>(mode),
			static_cast<nexus::EResourceIOPriority>(pri) )
			);

		return ret;

		END_NATIVE_GUARD

			return nullptr;
	}

	NResourceSkeletalAnimSet^ NResourceManager::NewSkeletalAnimSet( System::String^ resName )
	{
		pin_ptr<const wchar_t> szResName = PtrToStringChars(resName);
		NResourceSkeletalAnimSet^ ret = gcnew NResourceSkeletalAnimSet(
			nresource_manager::instance()->new_skeletal_anim(szResName)
			);
		return ret;
	}

	NResourceSkeletalMesh^ NResourceManager::NewSkeletalMesh( System::String^ resName )
	{
		pin_ptr<const wchar_t> szResName = PtrToStringChars(resName);
		NResourceSkeletalMesh^ ret = gcnew NResourceSkeletalMesh(
			nresource_manager::instance()->new_skeletal_mesh(szResName)
			);
		return ret;
	}
}//namespace NexusEngine
