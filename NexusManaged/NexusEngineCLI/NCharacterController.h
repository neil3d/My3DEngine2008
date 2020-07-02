#pragma once
#include "NativeObject.h"
#include "NMath.h"

namespace NexusEngine
{
	using namespace gameframework;
	ref class NGameLevel;
	ref class NSavedMove;
	ref class NCharacterStatData;

	public enum class MovementTypeCLI : System::Byte
	{
		Ground		= nexus::EMove_Ground,
		Jump		= nexus::EMove_Jump,
		Fly			= nexus::EMove_Fly,
		Custom		= nexus::EMove_Custom
	};

	/**
	* 对Native class ncharacter_controller的包装
	*/
	public ref class NCharacterController  : public NativeObjectOwnedRaw
	{
	protected:
		NCharacterController();
		explicit NCharacterController(ncharacter_controller* cc);
		virtual ~NCharacterController(void);

#pragma region Public Methods
	public:
		virtual bool Init(String^ uniqueName) { return NativePtr->init(); }

		virtual void Destory(){ NativePtr->destroy(); }

		virtual void Update(float deltaTime) { NativePtr->update(deltaTime); }

		virtual void Move(NGameLevel^ level, float deltaTime);

		virtual bool Transport(NGameLevel^ level, Vector3 destPos, bool placeNavAbove);

		virtual bool CanJump();

		virtual void Jump();

		virtual bool CanFly();

		virtual void Fly();

		bool ReachThresholdTest(Vector3 testPosition, Vector3 destination, float upThresholdAdjust, float downThresholdAdjust, float thresholdAdjust);
		bool ReachedDestination(Vector3 test_position, Vector3 destination);
#pragma endregion Methods

#pragma region Properties
		property UInt64 CharacterID
		{
			UInt64 get()	
			{	
				return NativePtr->get_character_id();
			}
			void set(UInt64 value)
			{
				NativePtr->set_character_id(value);
			}
		}

		property Vector3 Location
		{
			Vector3 get()	
			{	
				return Vector3::FromNative(NativePtr->get_position());
			}
			void set(Vector3 value)
			{
				NativePtr->set_position(value.ToNative());
			}
		}

		property Vector3 Rotation
		{
			Vector3 get()	
			{	
				return Vector3::FromNative(NativePtr->get_rotation());
			}
			void set(Vector3 value)
			{
				NativePtr->set_rotation(value.ToNative());
			}
		}

		property float Yaw
		{
			float get()
			{
				return NativePtr->get_rotation().y;
			}
			void set(float value)
			{
				const vector3& old_rot = NativePtr->get_rotation();
				NativePtr->set_rotation(vector3(old_rot.x, value, old_rot.z));
			}
		}

		property Vector3 Acceleration
		{
			Vector3 get()	
			{	
				return Vector3::FromNative(NativePtr->get_acceleration());
			}
			void set(Vector3 value)
			{
				NativePtr->set_acceleration(value.ToNative());
			}
		}

		property Vector3 Velocity
		{
			Vector3 get()	
			{	
				return Vector3::FromNative(NativePtr->get_velocity());
			}
			void set(Vector3 value)
			{
				NativePtr->set_velocity(value.ToNative());
			}
		}

		property MovementTypeCLI MovementType
		{
			MovementTypeCLI get()	
			{	
				return (MovementTypeCLI)NativePtr->get_current_movement_type();
			}
			void set(MovementTypeCLI value)
			{
				NativePtr->set_current_movement_type((gameframework::EMovementType)(value));
			}
		}

		property bool Walk
		{
			bool get()	
			{	
				return NativePtr->get_walk();
			}
			void set(bool value)
			{
				NativePtr->set_walk(value);
			}
		}

		property float EyesHeight
		{
			float get()	
			{	
				return NativePtr->get_eyes_height();
			}
			void set(float value)
			{
				NativePtr->set_eyes_height(value);
			}
		}


		property float JumpSpeed
		{
			float get()	
			{	
				return NativePtr->get_jump_speed();
			}
			void set(float value)
			{
				NativePtr->set_jump_speed(value);
			}
		}

		property float SpeedUp
		{			
			float get()	
			{	
				return NativePtr->get_speedup();
			}
			void set(float value)
			{
				NativePtr->set_speedup(value);
			}
		}

		property float MaxGroundSpeed
		{			
			float get()	
			{	
				return NativePtr->get_max_ground_speed();
			}
			void set(float value)
			{
				NativePtr->set_max_ground_speed(value);
			}
		}

		property float MaxFlySpeed
		{			
			float get()	
			{	
				return NativePtr->get_max_fly_speed();
			}
			void set(float value)
			{
				NativePtr->set_max_fly_speed(value);
			}
		}

		property ncharacter_controller* NativePtr
		{
			ncharacter_controller* get();
		}

		// 如果需要同步则必须设置SavedMove
		property NSavedMove^ SavedMove
		{
			NSavedMove^ get();
			void set(NSavedMove^ value);
		}

		property NCharacterStatData^ StatData
		{
			NCharacterStatData^ get()
			{
				return m_statData;
			}
		}
#pragma endregion Properties

	private:
		NSavedMove^ m_savedMove;
		NCharacterStatData^	m_statData;
	};

} // end of namespace NexusEngine
