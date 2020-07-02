#include "StdAfx.h"
#include "NResourceSkeletalMesh.h"
#include "NWrapperException.h"

namespace NexusEngine
{

	NResourceSkeletalMesh::NResourceSkeletalMesh(nexus::nresource_skeletal_mesh::ptr nativeRes)
		: NResource(nativeRes)
	{
	}

	nresource_skeletal_mesh::ptr NResourceSkeletalMesh::GetSmartPtr()
	{
		return boost::dynamic_pointer_cast<nresource_skeletal_mesh>(*m_nativeResPtr);
	}

	nresource_skeletal_mesh* NResourceSkeletalMesh::NativePtr::get()
	{
		return static_cast<nresource_skeletal_mesh*>( m_nativeResPtr->get() );
	}


	void NResourceSkeletalMesh::ImportSkeletalMeshLOD( int lod, System::String^ fullPath )
	{
		BEGIN_NATIVE_GUARD

		pin_ptr<const wchar_t> szFullPath = PtrToStringChars(fullPath);
		NativePtr->import_lod(lod, szFullPath);

		END_NATIVE_GUARD
	}

	void NResourceSkeletalMesh::ImportSetMaterial( int lod, int mtl_id, NMtlBase^ mtl )
	{
		BEGIN_NATIVE_GUARD
		if( mtl == nullptr )
		{
			throw gcnew System::ArgumentException("Can't Set Static Mesh Material to NULL.");
		}
		else
		{
			NativePtr->import_set_material(lod, mtl_id, mtl->GetSmartPtr());
		}
		END_NATIVE_GUARD
	}

	NMtlBase^ NResourceSkeletalMesh::GetMaterial( int lod, int mtl_id )
	{
		nmtl_base::ptr mtl = NativePtr->get_material(lod, mtl_id);

		if( mtl )
			return NMtlBase::FromNativePtr(mtl);
		else
			return nullptr;
	}

	int NResourceSkeletalMesh::GetNumLOD()
	{
		return NativePtr->get_num_lod();
	}

	int NResourceSkeletalMesh::GetNumSection(int lod)
	{
		return NativePtr->get_num_section(lod);
	}

	void NResourceSkeletalMesh::GetLODInfo( int lod, [Out]int% numVert, [Out]int% numSec, [Out]int% numTri )
	{
		int num_vert, num_sec, num_tri;
		NativePtr->get_lod_info(lod, num_vert, num_sec, num_tri);

		numVert = num_vert;
		numSec = num_sec;
		numTri = num_tri;
	}
}
