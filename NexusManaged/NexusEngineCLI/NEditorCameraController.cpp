#include "StdAfx.h"
#include "NEditorCameraController.h"

namespace NexusEngine
{
	NEditorCameraController::NEditorCameraController(void):m_obj(NULL)
	{}

	void NEditorCameraController::SetNativeObj(camera_controller* obj)
	{
		m_obj = obj;
	}

	void NEditorCameraController::UpdateCamera(NCamera^ cam)
	{
		m_obj->update_camera(cam->NativePtr);
	}

	void NEditorCameraController::ZoomExtents(BoundingBox box, NCamera^ cam)
	{
		nexus::AABBox nbox = box.ToNative();
		m_obj->zoom_extents(nbox, cam->NativePtr);
	}

	bool NEditorCameraController::InputMessgaeProc(System::Windows::Forms::Message% msg)
	{
		bool ret = false;
		switch(msg.Msg)
		{
		case WM_LBUTTONDOWN:
			{
				LPARAM lParam = msg.LParam.ToInt32();
				npoint npt(LOWORD(lParam), HIWORD(lParam));
				m_obj->on_mouse_left_down(npt);
				ret = true;
			}
			break;
		case WM_LBUTTONUP:
			m_obj->on_mouse_left_up();
			ret = true;
			break;
		case WM_RBUTTONDOWN:
			{
				LPARAM lParam = msg.LParam.ToInt32();
				npoint npt(LOWORD(lParam), HIWORD(lParam));
				m_obj->on_mouse_right_down(npt);
			}
			ret = true;
			break;
		case WM_RBUTTONUP:
			m_obj->on_mouse_right_up();
			ret = true;
			break;
		case WM_MOUSEMOVE:
			{
				LPARAM lParam = msg.LParam.ToInt32();
				npoint npt(LOWORD(lParam), HIWORD(lParam));

				WPARAM wParam = msg.WParam.ToInt32();
				m_obj->on_mouse_move(npt, (wParam&MK_CONTROL)!=0);
				ret = true;
			}
			break;
		case WM_MOUSEWHEEL:
			{
				WPARAM wParam = msg.WParam.ToInt32();
				short delta = GET_WHEEL_DELTA_WPARAM(wParam);
				m_obj->on_mouse_wheel(delta);
				ret = true;
			}
			break;
		}

		return ret;
	}

	void NEditorCameraController::SetFactor(float move, float rotate, float wheel)
	{
		m_obj->set_factor(move, rotate, wheel);
	}

	//--------------------------------------------------------------------------------
	NPerspectiveCameraController::NPerspectiveCameraController(void)
	{
		m_obj = new perspective_camera_controller;
		SetNativeObj(m_obj);
	}

	NPerspectiveCameraController::~NPerspectiveCameraController(void)
	{
		delete m_obj;
		m_obj = NULL;
	}

	void NPerspectiveCameraController::SetLookat(Vector3 pos)
	{
		nexus::vector3* pPos = reinterpret_cast<nexus::vector3*>(&pos);
		m_obj->set_look_at( *pPos );
	}

	//--------------------------------------------------------------------------------
	NOrthoCameraController::NOrthoCameraController(void)
	{
		m_obj = new ortho_camera_controller;
		SetNativeObj(m_obj);
	}
	
	NOrthoCameraController::~NOrthoCameraController(void)
	{
		delete m_obj;
		m_obj = NULL;
	}

	void NOrthoCameraController::SetOrientation(System::Char axis)
	{
		m_obj->set_orientation(axis);
	}
}//namespace NexusEngine