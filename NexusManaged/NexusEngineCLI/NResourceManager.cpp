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

	NMaterialTemplate^ NResourceManager::LoadMaterialScript(NResourceLoc loc, EResourceIOMode mode, EResourceIOPriority pri)
	{
		BEGIN_NATIVE_GUARD

		nexus::resource_location nloc;
		loc.ToNative(nloc);

		NMaterialTemplate^ ret = gcnew NMaterialTemplate(
			nresource_manager::instance()->load_material_template_script(nloc,
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
}//namespace NexusEngine