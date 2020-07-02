/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NativeObject.h"
#include "NActor.h"
#include "NMath.h"

namespace NexusEngine
{
	//!	射线检测的结果
	public value struct NCheckResult
	{
		Vector3	location;
		Vector3	normal;
		float	dist;
	};

	//!	包装nexus::nlevel对象
	public ref class NLevel : public NativeObjectOwnedBySharedPtr
	{
	public:				
		//!	初始化, 创建内部对象
		virtual void Init(System::String^ sceneClassName);

		//!	创建一个Actor, Actor Name要求在关卡中是唯一的
		virtual NActor^ CreateActor(System::String^ actorName, System::String^ nativeClassName);

		//!	销毁指定名称的Actor
		virtual void DestroyActor(System::String^ actorName);

		//!	根据名称查找Actor
		NActor^ FindActor(System::String^ actorName);

		//!	射线检测
		bool LineCheck([Out] NCheckResult% ret, Vector3 start, Vector3 end);

		//!	清空关卡中的所有Actor
		void DestroyAllActors();
		
		//!	取得关卡的名称
		property System::String^ Name
		{
			System::String^ get();
		}

		NActor^ FirstActor();
		NActor^ NextActor();

		void RenameActor(String^ oldName, String^ newName);

	public:
		//-- 包装相关接口 --
		NLevel(nexus::nlevel::ptr nativeLv);
		static NLevel^ FromNativePtr(nexus::nlevel::ptr nativeLv, System::String^ nativeClassName);
		nexus::nlevel::ptr GetNativeSmartPtr();

		void NativeLevelLoaded();
	
		property nexus::nlevel* NativePtr
		{
			nexus::nlevel* get();
		}

	private:
		typedef System::Collections::Generic::Dictionary<System::String^, NActor^> ActorDict;
		
		ActorDict::Enumerator^	m_actorEnum;
		ActorDict^				m_actorDict;
	};
}//namespace NexusEngine