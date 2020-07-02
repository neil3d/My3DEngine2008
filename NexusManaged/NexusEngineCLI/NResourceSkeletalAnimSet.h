#pragma once
#include "NResource.h"
#include "NMtlBase.h"


namespace NexusEngine
{
	ref class NSkeletalAnimSequence;

	public ref class NResourceSkeletalAnimSet : public NResource
	{
	public:
		//!	从磁盘导入骨骼动画
		void ImportSkeletalAnimSequence(System::String^ fullPath);

		//! 得到动画帧的名称
		String^ GetSequenceName(UInt32 index);

		NSkeletalAnimSequence^ FindSequence(UInt32 index);
		NSkeletalAnimSequence^ FindSequence(String^ sequenceName);

		// 总计有多少动画序列
		property UInt32 SequenceCount
		{
			UInt32 get()
			{
				return NativePtr->get_num_anim();
			}
		}

		int GetBoneCount();
		String^ GetBoneName(int i);
		String^ GetBoneParentName(int i);

		//-- Native Wrapper
	public:
		NResourceSkeletalAnimSet(nexus::nresource_skeletal_anim_set::ptr nativeRes);
		nresource_skeletal_anim_set::ptr GetSmartPtr();
	protected:
		property nresource_skeletal_anim_set* NativePtr
		{
			nresource_skeletal_anim_set* get();
		}
	};
}
