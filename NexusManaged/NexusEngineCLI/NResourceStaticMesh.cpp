#include "StdAfx.h"
#include "NResourceStaticMesh.h"
#include "NWrapperException.h"

namespace NexusEngine
{
	NResourceStaticMesh::NResourceStaticMesh(nresource_static_mesh::ptr nativeRes):NResource(nativeRes)
	{
	}

	nresource_static_mesh* NResourceStaticMesh::NativePtr::get()
	{
		return static_cast<nresource_static_mesh*>( m_nativeResPtr->get() );
	}

	void NResourceStaticMesh::ImportLOD(int lod, System::String^ fullPath)
	{
		BEGIN_NATIVE_GUARD

		pin_ptr<const wchar_t> szFullPath = PtrToStringChars(fullPath);
		NativePtr->import_lod(lod, szFullPath);

		END_NATIVE_GUARD
	}

	nresource_static_mesh::ptr NResourceStaticMesh::GetSmartPtr()
	{
		return boost::dynamic_pointer_cast<nresource_static_mesh>(*m_nativeResPtr);
	}

	int NResourceStaticMesh::GetNumLOD()
	{
		return NativePtr->get_num_lod();
	}

	void NResourceStaticMesh::GetLODInfo(int lod, [Out]int% numVert, [Out]int% numSec, [Out]int% numTri)
	{
		int num_vert, num_sec, num_tri;
		NativePtr->get_lod_info(lod, num_vert, num_sec, num_tri);

		numVert = num_vert;
		numSec = num_sec;
		numTri = num_tri;
	}

	NMaterialBase^ NResourceStaticMesh::GetMaterial(int lod, int sec)
	{
		nmaterial_base::ptr mtl = NativePtr->get_material(lod, sec);

		if( mtl )
			return NMaterialBase::FromNativePtr(mtl);
		else
			return nullptr;
	}

	int NResourceStaticMesh::GetNumSection(int lod)
	{
		return NativePtr->get_num_section(lod);
	}

	void NResourceStaticMesh::ImportSetMaterial(int lod, int sec, NMaterialBase^ mtl)
	{
		if( mtl == nullptr )
		{
			throw gcnew System::ArgumentException("Can't Set Static Mesh Materal to NULL.");
		}
		else
		{
			NativePtr->import_set_material(lod, sec, mtl->GetSmartPtr());
		}
	}
}//namespace NexusEngine