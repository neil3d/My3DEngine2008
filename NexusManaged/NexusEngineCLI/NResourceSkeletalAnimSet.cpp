#include "StdAfx.h"
#include "NResourceSkeletalAnimSet.h"
#include "NWrapperException.h"
#include "NSkeletalAnimSequence.h"

namespace NexusEngine
{


	NResourceSkeletalAnimSet::NResourceSkeletalAnimSet(nexus::nresource_skeletal_anim_set::ptr nativeRes)
		: NResource(nativeRes)
	{
	}

	nresource_skeletal_anim_set::ptr NResourceSkeletalAnimSet::GetSmartPtr()
	{
		return boost::dynamic_pointer_cast<nresource_skeletal_anim_set>(*m_nativeResPtr);
	}

	nresource_skeletal_anim_set* NResourceSkeletalAnimSet::NativePtr::get()
	{
		return static_cast<nresource_skeletal_anim_set*>( m_nativeResPtr->get() );
	}

	void NResourceSkeletalAnimSet::ImportSkeletalAnimSequence( System::String^ fullPath )
	{
		BEGIN_NATIVE_GUARD

		pin_ptr<const wchar_t> szFullPath = PtrToStringChars(fullPath);
		NativePtr->import_anim_sequence(szFullPath);

		END_NATIVE_GUARD
	}

	String^ NResourceSkeletalAnimSet::GetSequenceName( UInt32 index )
	{
		BEGIN_NATIVE_GUARD
		nskeletal_anim_sequence::ptr  seq = NativePtr->get_anim(index);
		return gcnew System::String(seq->m_name.c_str());
		END_NATIVE_GUARD
	}

	NSkeletalAnimSequence^ NResourceSkeletalAnimSet::FindSequence( UInt32 index )
	{
		nskeletal_anim_sequence::ptr nativeSeq = NativePtr->get_anim(index);
		if(nativeSeq)
		{
			return gcnew NSkeletalAnimSequence(nativeSeq);
		}
		return nullptr;
	}

	NSkeletalAnimSequence^ NResourceSkeletalAnimSet::FindSequence( String^ sequenceName )
	{
		pin_ptr<const wchar_t> nativeName = PtrToStringChars(sequenceName);
		nskeletal_anim_sequence::ptr nativeSeq = NativePtr->find_anim_by_name(nativeName);
		if(nativeSeq)
		{
			return gcnew NSkeletalAnimSequence(nativeSeq);
		}
		return nullptr;
	}

	int NResourceSkeletalAnimSet::GetBoneCount()
	{
		const nskeleton_define& sk = NativePtr->get_skeleton();
		return (int)sk.m_bones.size();
	}

	String^ NResourceSkeletalAnimSet::GetBoneName(int i)
	{
		const nskeleton_define& sk = NativePtr->get_skeleton();
		return gcnew String( sk.m_bones[i].name.c_str() );
	}

	String^ NResourceSkeletalAnimSet::GetBoneParentName(int i)
	{
		const nskeleton_define& sk = NativePtr->get_skeleton();
		const bone_info& bi = sk.m_bones[i];
		int p = bi.parent_index;
		if( p == -1 )// root
			return gcnew String("root");
		else
			return gcnew String( sk.m_bones[p].name.c_str() );
	}
}
