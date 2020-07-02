/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NEngine.h"
#include "NViewport.h"

namespace NexusEngine
{
	//! 游戏运行时所使用的引擎类
	public ref class NGameEngine : public NEngine
	{
	public:
		NGameEngine(void);

		//!	执行每帧的Update
		virtual void Update(float deltaTime, NViewport^ vp);
		//!	执行每帧的Render
		virtual void Render(NViewport^ vp);

		//!	鼠标拾取关卡中的Actor
		NActor^ PickLevelActor(NViewport^ vp, int mouseX, int mouseY);

		virtual void ScreenShot(System::String^ fileName, UINT32 width, UINT32 height);
		virtual void ViewportShot(System::String^ fileName);

	protected:
		property ngame_engine* NativePtr
		{
			ngame_engine* get();
		}
	};
}//namespace NexusEngine