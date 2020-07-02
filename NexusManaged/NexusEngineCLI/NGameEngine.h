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

		//!	执行每帧的Update和Render
		virtual void FrameTick(float deltaTime, NViewport^ vp);

	protected:
		property ngame_engine* NativePtr
		{
			ngame_engine* get();
		}
	};
}//namespace NexusEngine