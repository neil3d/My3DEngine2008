#pragma once

namespace NexusEngine
{
	ref class NCamera;
	ref class NCharacterController;

	///<summary>
	/// YPR摄像机控制，ref camera_YPR
	///<summary>
	public ref class NCameraModifier
	{
	protected:
		NCameraModifier(void);
		virtual ~NCameraModifier(void);
		
	public:
		property Vector3 LookAt
		{
			Vector3 get();
			void set(Vector3 value);
		}

		property Vector3 EyePosition
		{
			Vector3 get();
		}

		property float Yaw
		{
			float get();
			void set(float value);
		}

		property float Pitch
		{
			float get();
			void set(float value);
		}

		property float Roll
		{
			float get();
			void set(float value);
		}

		property float Distance
		{
			float get();
			void set(float value);
		}

		property float MinDistance
		{
			float get();
			void set(float value);
		}

		property float MaxDistance
		{
			float get();
			void set(float value);
		}

		property float MinPitch
		{
			float get();
			void set(float value);
		}

		property float MaxPitch
		{
			float get();
			void set(float value);
		}


		// 摄像机的方向
		property Vector3 Direction
		{
			Vector3 get();
		}

		property nexus::camera_YPR* NativePtr
		{
			nexus::camera_YPR* get();
		}
	public:
		virtual void UpdateCamera(NCharacterController^ viewTarget, NCamera^ camera, float deltaTime);

		virtual void MoveDistance(float deltaDist);
		virtual void MoveLookAt(Vector3 deltaLookAt);
		virtual void MoveYaw(float deltaYaw);
		virtual void MovePitch(float deltaPitch);
		virtual void MoveRoll(float deltaRoll);

	protected:
		float minDistance;
		float maxDistance;
		float minPitch;
		float maxPitch;

	private:
		nexus::camera_YPR* m_camera;
	};
}
