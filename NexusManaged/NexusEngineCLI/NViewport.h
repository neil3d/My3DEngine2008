/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NCamera.h"
#include "RenderDefine.h"
#include "NRenderElement.h"

namespace NexusEngine
{
	//! view port
	public ref class NViewport
	{
	public:
		NViewport(void);
		~NViewport(void);

		//! 起点X坐标
		property UInt32 X
		{
			UInt32 get()	{	return m_obj->x;}
			void set(UInt32 val)	{	m_obj->x = val;}
		}

		//! 起点Y坐标
		property UInt32 Y
		{
			UInt32 get()	{	return m_obj->y;}
			void set(UInt32 val)	{	m_obj->y = val;}
		}

		//!	视口宽度
		property UInt32 Width
		{
			UInt32 get()	{	return m_obj->width;}
			void set(UInt32 val)	{	m_obj->width = val;}
		}

		//!	视口高度
		property UInt32 Height
		{
			UInt32 get()	{	return m_obj->height;}
			void set(UInt32 val)	{	m_obj->height = val;}
		}

		//!	z范围最小值
		property float MinZ
		{
			float get()	{	return m_obj->min_z;}
			void set(float val)	{	m_obj->min_z = val;}
		}

		//!	z范围最大值
		property float MaxZ
		{
			float get()	{	return m_obj->max_z;}
			void set(float val)	{	m_obj->max_z = val;}
		}

		//!	摄像机对象
		property NCamera^ Camera
		{
			NCamera^ get()	{	return m_camera;}
		}

		//!	渲染模式
		property ERenderMode RenderMode
		{
			ERenderMode get()	{	return static_cast<NexusEngine::ERenderMode>(m_obj->render_mode);}
			void set(ERenderMode val)	{	m_obj->render_mode = static_cast<nexus::ERenderMode>(val); }
		}

		//!	windows窗口句柄
		property IntPtr HandleWnd
		{
			void set(IntPtr hwnd)	{	m_obj->handle_wnd = hwnd.ToPointer();}
		}

		//! PDI绘图对象
		property NRenderElement^ WidgetsRender
		{
			NRenderElement^ get()	{	return m_widgetsRender;}
			void set(NRenderElement^ val)
			{
				m_widgetsRender = val;
				NativeRenderElementWrapper::ptr newWidgets( new NativeRenderElementWrapper(val) );
				m_obj->widgets_render = newWidgets;
			}
		}

		//!	LOD值
		property float LODBias
		{
			float get()	{	return m_obj->lod_bias;}
			void set(float val)	{	m_obj->lod_bias=val;}
		}

		property bool EnableDynamicShadow
		{
			bool get() {	return m_obj->enable_dynamic_shadow; }
			void set(bool val)	{	m_obj->enable_dynamic_shadow=val;}
		}

		// todo : show_flags

		//!	更新视锥体等内部对象状态
		void Update();

		void CreateHitHash();

		void Destroy();
		

		nviewport* GetNaitvePtr()	{	return m_obj;}
	private:
		NCamera^	m_camera;
		nviewport*	m_obj;
		NRenderElement^	m_widgetsRender;
	};
}//namespace NexusEngine