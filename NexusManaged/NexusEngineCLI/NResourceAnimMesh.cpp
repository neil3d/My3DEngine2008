#include "StdAfx.h"
#include "NResourceAnimMesh.h"
#include "NWrapperException.h"

namespace NexusEngine
{
	NResourceAnimMesh::NResourceAnimMesh(nexus::nresource_anim_mesh::ptr nativeRes)
		: NResource(nativeRes)
	{
	}

	nresource_anim_mesh::ptr NResourceAnimMesh::GetSmartPtr()
	{
		return boost::dynamic_pointer_cast<nresource_anim_mesh>(*m_nativeResPtr);
	}

	nresource_anim_mesh* NResourceAnimMesh::NativePtr::get()
	{
		return static_cast<nresource_anim_mesh*>( m_nativeResPtr->get() );
	}

	void NResourceAnimMesh::ImportLOD(int lod, System::String^ fullPath)
	{
		BEGIN_NATIVE_GUARD

			pin_ptr<const wchar_t> szFullPath = PtrToStringChars(fullPath);
		NativePtr->import_lod(lod, szFullPath);

		END_NATIVE_GUARD
	}

	int NResourceAnimMesh::GetNumLOD()
	{
		return NativePtr->get_num_lod();
	}

	void NResourceAnimMesh::GetLODInfo(int lod, [Out]int% numVert, [Out]int% numSec, [Out]int% numTri)
	{
		int num_vert, num_sec, num_tri;
		NativePtr->get_lod_info(lod, num_vert, num_sec, num_tri);

		numVert = num_vert;
		numSec = num_sec;
		numTri = num_tri;
	}

	NMaterialBase^ NResourceAnimMesh::GetMaterial(int lod, int sec)
	{
		nmaterial_base::ptr mtl = NativePtr->get_material(lod, sec);

		if( mtl )
			return NMaterialBase::FromNativePtr(mtl);
		else
			return nullptr;
	}

	int NResourceAnimMesh::GetNumSection(int lod)
	{
		return NativePtr->get_num_section(lod);
	}

	void NResourceAnimMesh::ImportSetMaterial(int lod, int sec, NMaterialBase^ mtl)
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

	int NResourceAnimMesh::GetNumSequence()
	{
		return NativePtr->get_num_sequence();
	}

	String^ NResourceAnimMesh::GetSequenceInfo(int i, [Out]float% length)
	{
		nstring name;
		float len;
		NativePtr->get_sequence_info(i, name, len);
		
		length = len;
		return gcnew String(name.c_str());
	}
}//namespace NexusEngine