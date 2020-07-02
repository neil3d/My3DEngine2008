#include "StdAfx.h"
#include "NCharacterController.h"
#include "NGameLevel.h"
#include "NSavedMove.h"
#include "NCharacterStatData.h"

namespace NexusEngine
{
	NCharacterController::NCharacterController( ncharacter_controller* cc )
		: NativeObjectOwnedRaw(cc)
	{
		m_statData = gcnew NCharacterStatData(); 
	}

	NCharacterController::NCharacterController()
		: NativeObjectOwnedRaw(new ncharacter_controller())
	{
		m_statData = gcnew NCharacterStatData(); 
	}

	NCharacterController::~NCharacterController( void )
	{
		if(NativePtr)
		{
			NativePtr->destroy();
		}
	}

	void NCharacterController::Move( NGameLevel^ level, float deltaTime )
	{
		NativePtr->move(level->NativePtr, deltaTime);
	}

	bool NCharacterController::Transport( NGameLevel^ level, Vector3 destPos, bool placeNavAbove )
	{
		return NativePtr->transport(level->NativePtr, destPos.ToNative(), placeNavAbove);
	}

	bool NCharacterController::CanJump()
	{
		return NativePtr->can_jump();
	}

	void NCharacterController::Jump()
	{
		NativePtr->jump();
	}

	bool NCharacterController::CanFly()
	{
		return NativePtr->can_fly();
	}

	void NCharacterController::Fly()
	{
		NativePtr->fly();
	}

	bool NCharacterController::ReachThresholdTest( Vector3 testPosition, Vector3 destination, float upThresholdAdjust, float downThresholdAdjust, float thresholdAdjust )
	{
		return NativePtr->reach_threshold_test(testPosition.ToNative(),
			destination.ToNative(),upThresholdAdjust,downThresholdAdjust,thresholdAdjust);
	}

	bool NCharacterController::ReachedDestination( Vector3 test_position, Vector3 destination )
	{
		return NativePtr->reached_destination(test_position.ToNative(), destination.ToNative());
	}

	ncharacter_controller* NCharacterController::NativePtr::get()
	{
		return dynamic_cast<ncharacter_controller*>(RawPtr);		
	}

	// 如果需要同步则必须设置SavedMove
	void NCharacterController::SavedMove::set(NSavedMove^ value)
	{
		m_savedMove = value;
		NativePtr->set_saved_move(m_savedMove->NativePtr);
	}

	NSavedMove^ NCharacterController::SavedMove::get()
	{
		return m_savedMove;		
	}
}
