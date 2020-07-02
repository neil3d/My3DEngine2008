#include "StdAfx.h"
#include "NCamera.h"

namespace NexusEngine
{
	NCamera::NCamera(void):m_nativeObj(new ncamera)
	{
	}
	
	NCamera::~NCamera(void)
	{
		delete m_nativeObj;
		m_nativeObj = NULL;
	}

	nexus::ncamera* NCamera::NativePtr::get()
	{
		return m_nativeObj;
	}

	void NCamera::SetLookat(Vector3 eye, Vector3 at, Vector3 up)
	{
		nexus::vector3* pEye = reinterpret_cast<nexus::vector3*>(&eye);
		nexus::vector3* pAt = reinterpret_cast<nexus::vector3*>(&at);
		nexus::vector3* pUp = reinterpret_cast<nexus::vector3*>(&up);
		NativePtr->set_lookat(*pEye, *pAt, *pUp);
	}

	void NCamera::SetPerspective(float FOV, int viewportW, int viewportH, float zNear, float zFar)
	{
		NativePtr->set_perspective(FOV, viewportW, viewportH, zNear, zFar);
	}

	void NCamera::SetOrtho(float w, float h, float zNear, float zFar)
	{
		NativePtr->set_ortho(w, h, zNear, zFar);
	}

	Vector3 NCamera::ScreenToWorld(int screenX, int screenY)
	{
		return Vector3::FromNative(
			NativePtr->screen2world(screenX, screenY)
			);
	}

	System::Drawing::Point NCamera::WorldToScreen(Vector3 p)
	{
		nexus::vector3* np = reinterpret_cast<nexus::vector3*>(&p);
		npoint npt = NativePtr->world2screen(*np);
		return System::Drawing::Point(npt.x, npt.y);
	}

	Ray NCamera::GetMouseRay(int cursorX, int cursorY)
	{
		nexus::ray nray;
		NativePtr->get_mouse_ray(cursorX, cursorY, nray);
		return Ray::FromNative(nray);
	}

	NexusEngine::Vector4 NCamera::WorldToScreenWithDepth( Vector3 p )
	{	
		return Vector4::FromNative(NativePtr->world2screen_with_depth(p.ToNative()));
	}
}//namespace NexusEngine