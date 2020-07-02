#include "StdAfx.h"
#include "NSavedMove.h"
#include "NCharacterController.h"

namespace NexusEngine
{
	NSavedMove::NSavedMove(IReplicateMoveCallback^ callback) 
	{
		m_native_ptr = new nsavedmove(
			nexus::replicate_move_callback_ptr(new ReplicateMoveCallback(callback)));
	}

	NSavedMove::~NSavedMove( void )
	{
		delete m_native_ptr;
		m_native_ptr = NULL;
	}

	void NSavedMove::Init( NCharacterController^ controller )
	{
		NativePtr->init(controller->NativePtr);
	}

	void NSavedMove::Update( float deltaTime, NCharacterController^ controller )
	{
		NativePtr->update(deltaTime, controller->NativePtr);
	}

	void NSavedMove::FlagsToController( Byte flags, NCharacterController^ controller )
	{
		nsavedmove::flags_to_controller(flags, controller->NativePtr);
	}

	System::Byte NSavedMove::ControllerToFlags( NCharacterController^ controller, Byte customFlag)
	{
		return nsavedmove::compressed_controller_flags(controller->NativePtr, customFlag);
	}
}
