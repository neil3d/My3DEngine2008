/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NCamera.h"
#include "math/Vector3.h"
#include "math//BoundingBox.h"

using namespace System::Drawing;

namespace NexusEngine
{
	/**
	 *	编辑器的摄像机控制基类
	*/
	public ref class NEditorCameraController
	{
		camera_controller* m_obj;
	public:
		NEditorCameraController(void);
		
		void SetFactor(float move, float rotate, float wheel);
		void UpdateCamera(NCamera^ cam);
		// 返回是否处理了这个消息
		bool InputMessgaeProc(System::Windows::Forms::Message% msg);

		void ZoomExtents(BoundingBox box, NCamera^ cam);

		[System::ComponentModel::CategoryAttribute("Camera Controller")]
		property float MoveFactor
		{
			float get()	{return m_obj->get_move_factor();}
			void set(float v){m_obj->set_move_factor(v);}			
		}

		[System::ComponentModel::CategoryAttribute("Camera Controller")]
		property float RotateFactor
		{
			float get()	{return m_obj->get_rotate_factor();}
			void set(float v){m_obj->set_rotate_factor(v);}			
		}

		[System::ComponentModel::CategoryAttribute("Camera Controller")]
		property float WheelFactor
		{
			float get()	{return m_obj->get_wheel_factor();}
			void set(float v){m_obj->set_wheel_factor(v);}			
		}

	protected:
		void SetNativeObj(camera_controller* obj);
	};

	/**
	 * 透视视图的摄像机控制
	*/
	public ref class NPerspectiveCameraController : public NEditorCameraController
	{
		perspective_camera_controller*	m_obj;
	public:
		NPerspectiveCameraController(void);
		~NPerspectiveCameraController(void);

		void SetLookat(Vector3 pos);
	};

	/**
	 *	平行投影视图的摄像机控制
	*/
	public ref class NOrthoCameraController : public NEditorCameraController
	{
		ortho_camera_controller* m_obj;
	public:
		NOrthoCameraController(void);
		~NOrthoCameraController(void);

		void SetOrientation(System::Char axis);
	};
}//namespace NexusEngine