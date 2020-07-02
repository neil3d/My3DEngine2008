/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NativeObject.h"

#define DECLARE_PROPERTY(CLASS,PropName,MemberName)\
	property CLASS PropName\
	{\
	CLASS get()\
		{\
		return CLASS::FromNative(NativePtr->MemberName);\
		}\
		void set(CLASS val)\
		{\
		NativePtr->MemberName = val.ToNative();\
		}\
	}

#define DECLARE_SINGLE_PROPERTY(CLASS,PropName,MemberName)\
	property CLASS PropName\
	{\
	CLASS get()\
		{\
		return NativePtr->MemberName;\
		}\
		void set(CLASS val)\
		{\
		NativePtr->MemberName = val;\
		}\
	}

namespace NexusEngine
{
	ref class NActor;

	/**
	 *	Actor组件基类, 包装nexus::nactor_componet对象接口
	*/
	public ref class NActorComponent : public NativeObjectOwnedBySharedPtr
	{
	public:
		//!	内部接口 : 设置所属的actor
		void _SetOwner(NActor^ owner);

		//!	取得属于哪个Actor对的
		NActor^ GetOwner();

		//!	取得组件名称
		[System::ComponentModel::CategoryAttribute("ActorComponent")]
		property System::String^ Name
		{
			System::String^ get();
		}

		[System::ComponentModel::CategoryAttribute("ActorComponent")]
		property System::String^ NativeClassName
		{
			System::String^ get();
		}

		[System::ComponentModel::CategoryAttribute("ActorComponent")]
		property bool Editable
		{
			bool get();
		}

	protected:
		System::WeakReference^	m_owner;

		//-- Native Wrapper
	protected:
		NActorComponent(nexus::nactor_component::ptr nativeComp);
		
		property nactor_component* NativePtr
		{
			nactor_component* get();
		}
	public:
		static NActorComponent^ FromNativePtr(nexus::nactor_component::ptr nativeComp, System::String^ nativeClassName);
	};
}//namespace NexusEngine