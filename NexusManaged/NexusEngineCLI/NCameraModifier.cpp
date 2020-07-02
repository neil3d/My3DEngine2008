#include "StdAfx.h"
#include "NCamera.h"
#include "NCameraModifier.h"

namespace NexusEngine
{
	NCameraModifier::NCameraModifier(void)
	{
		m_camera = new camera_YPR;
	}
	
	NCameraModifier::~NCameraModifier(void)
	{
		delete m_camera;
		m_camera = NULL;
	}

	nexus::camera_YPR* NCameraModifier::NativePtr::get()
	{
		return m_camera;
	}

	float NCameraModifier::MaxDistance::get()
	{
		return maxDistance;
	}

	void NCameraModifier::MaxDistance::set(float value)
	{
		maxDistance = value;
	}
	float NCameraModifier::MinDistance::get()
	{
		return minDistance;
	}

	void NCameraModifier::MinDistance::set(float value)
	{
		minDistance = value;
	}

	float NCameraModifier::MaxPitch::get()
	{
		return maxPitch;
	}

	void NCameraModifier::MaxPitch::set(float value)
	{
		maxPitch = value;
	}

	float NCameraModifier::MinPitch::get()
	{
		return minPitch;
	}

	void NCameraModifier::MinPitch::set(float value)
	{
		minPitch = value;
	}

	float NCameraModifier::Distance::get()
	{
		return NativePtr->m_dist;
	}

	void NCameraModifier::Distance::set(float value)
	{
		if(value==NativePtr->m_dist) return;
		NativePtr->m_dist = clamp<float>(value,minDistance, maxDistance);
		float t=(NativePtr->m_dist-MinDistance)/(MaxDistance-MinDistance);
		Pitch=MaxPitch*(1.0f-t)+MinPitch*t;
	}

	float NCameraModifier::Roll::get()
	{
		return NativePtr->m_roll;
	}

	void NCameraModifier::Roll::set(float value)
	{
		NativePtr->m_roll = value;
	}

	float NCameraModifier::Pitch::get()
	{
		return NativePtr->m_pitch;
	}

	void NCameraModifier::Pitch::set(float value)
	{
		if(value==NativePtr->m_pitch) return;
		NativePtr->m_pitch = clamp<float>(value,minPitch, maxPitch);
	}

	float NCameraModifier::Yaw::get()
	{
		return NativePtr->m_yaw;
	}

	void NCameraModifier::Yaw::set(float value)
	{
		NativePtr->m_yaw = value;
	}

	Vector3 NCameraModifier::EyePosition::get()
	{
		return Vector3::FromNative(NativePtr->m_eye_pos);
	}

	Vector3 NCameraModifier::LookAt::get()
	{
		return Vector3::FromNative(NativePtr->m_look_at);
	}

	void NCameraModifier::LookAt::set(Vector3 value)
	{
		NativePtr->m_look_at = value.ToNative();
	}

	Vector3 NCameraModifier::Direction::get()
	{
		return Vector3::FromNative(NativePtr->get_direction());
	}


	void NCameraModifier::UpdateCamera( NCharacterController^ viewTarget, NCamera^ camera, float deltaTime )
	{
		m_camera->update_camera_view(camera->NativePtr);
	}

	void NCameraModifier::MoveDistance( float deltaDist )
	{
		this->Distance = Distance + deltaDist;
	}

	void NCameraModifier::MoveLookAt( Vector3 deltaLookAt )
	{
		m_camera->move_look_at(deltaLookAt.ToNative());
	}

	void NCameraModifier::MoveYaw( float deltaYaw )
	{
		this->Yaw = Yaw + deltaYaw;
	}

	void NCameraModifier::MovePitch( float deltaPitch )
	{
		this->Pitch = Pitch + deltaPitch;
	}

	void NCameraModifier::MoveRoll( float deltaRoll )
	{
		this->Roll = Roll + deltaRoll;
	}
} // end of namespace NexusEngine
