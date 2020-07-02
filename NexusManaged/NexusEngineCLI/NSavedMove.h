#pragma once
#include "NativeObject.h"
#include "ReplicateMoveCallback.h"

namespace NexusEngine
{
	using namespace gameframework;

	ref class NCharacterController;

	[System::FlagsAttribute]
	public enum class MovementFlag : System::Byte
	{
		PressJump = gameframework::nsavedmove::EMFT_PressJump,
		Custom = gameframework::nsavedmove::EMFT_Custom,
	};

	/**
	* 对Native class nsavedmove的包装
	*/
	public ref class NSavedMove
	{
	public:
		explicit NSavedMove(IReplicateMoveCallback^ callback);
		virtual ~NSavedMove(void);

		virtual void Init(NCharacterController^ controller);

		virtual void Update(float deltaTime, NCharacterController^ controller);

		static Byte ControllerToFlags(NCharacterController^ controller, Byte customFlag);
		static void FlagsToController(Byte flags, NCharacterController^ controller);

		property nsavedmove* NativePtr
		{
			nsavedmove* get()
			{
				return m_native_ptr;
			}
		}

	private:
		nsavedmove*  m_native_ptr;
	};
}
