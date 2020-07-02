#include "StdAfx.h"
#include "NSkeletalAnimSequence.h"

namespace NexusEngine
{
	using namespace System::Globalization;

	NSkeletalAnimSequence::NSkeletalAnimSequence( nskeletal_anim_sequence::ptr nativeSequence )
		: NativeObjectOwnedBySharedPtr(nativeSequence)
	{

	}

	System::String^ NSkeletalAnimSequence::ToString()
	{
		return String::Format( CultureInfo::CurrentCulture, "{0}[{1}f][{2}s]", 
			Name->ToString(CultureInfo::CurrentCulture), 
			FrameCount.ToString(CultureInfo::CurrentCulture), 
			Length.ToString(CultureInfo::CurrentCulture) );
	}
}