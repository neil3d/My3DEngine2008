/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NativeObject.h"
#include "NRenderElement.h"

namespace NexusEngine
{
	ref class NActorComponent;
	ref class NLevel;

	/**
	 * nexus::nactor的包装类
	*/
	public ref class NActor : public NativeObjectOwnedBySharedPtr
	{
	public:
		//! 给actor创建一个组件, 组件的名称要求在Actor中是唯一的
		NActorComponent^ CreateComponent(System::String^ compName, System::String^ nativeClassName);

		//! 取得Actor的名字, Actor Name要求在某个Level中是唯一的
		[System::ComponentModel::CategoryAttribute("Actor")]
		property System::String^ Name
		{
			System::String^ get()
			{
				return gcnew System::String(NativePtr->get_name().name_str.c_str());
			}
			void set(System::String^ val);
		}

		//! 取得Actor有多少个组件
		[System::ComponentModel::CategoryAttribute("Actor")]
		property int NumComponents
		{
			int get()
			{
				return m_compList->Count;
			}
		}

		//!	Actor是否可以存盘
		[System::ComponentModel::CategoryAttribute("Level Design")]
		property bool Serializable
		{
			bool get()
			{
				return NativePtr->get_serializable();
			}
			void set(bool val)
			{
				NativePtr->set_serializable(val);
			}
		}

		//! Actor可见或者隐藏
		[System::ComponentModel::CategoryAttribute("Actor")]
		property bool Visible
		{
			bool get()
			{
				return NativePtr->get_visible();
			}
			void set(bool val)
			{
				NativePtr->set_visible(val);
			}
		}

		//! Actor冻结(编辑器无法移动)
		[System::ComponentModel::CategoryAttribute("Level Design")]
		property bool Frozen
		{
			bool get()
			{
				return NativePtr->get_frozen();
			}
			void set(bool val)
			{
				NativePtr->set_frozen(val);
			}
		}

		//!	得到某个组件对象
		NActorComponent^ GetComponent(int i)
		{
			return m_compList[i];
		}

		//!	Actor的位置
		[System::ComponentModel::CategoryAttribute("Actor")]
		property Vector3 Location
		{
			Vector3 get()
			{
				const nexus::vector3& nloc = NativePtr->get_space().location;
				return Vector3(nloc.x, nloc.y, nloc.z);
			}

			void set(Vector3 val)
			{
				const object_space& os = NativePtr->get_space();
				nexus::vector3* nval = reinterpret_cast<nexus::vector3*>(&val);
				NativePtr->move(*nval, os.rotation, os.scale);
			}
		}

		//!	Actor的旋转,单位: 角度
		[System::ComponentModel::CategoryAttribute("Actor")]
		property Vector3 Rotation
		{
			Vector3 get()
			{
				const nexus::vector3& nrot = NativePtr->get_space().rotation;
				return Vector3(nRadToDeg(nrot.x), 
					nRadToDeg(nrot.y), 
					nRadToDeg(nrot.z)
					);
			}

			void set(Vector3 val)
			{
				const object_space& os = NativePtr->get_space();				
				NativePtr->move(os.location, 
					nexus::vector3(nDegToRad(val.x), nDegToRad(val.y), nDegToRad(val.z)), 
					os.scale);
			}
		}

		//!	Actor的缩放
		[System::ComponentModel::CategoryAttribute("Actor")]
		property Vector3 Scale
		{
			Vector3 get()
			{
				const nexus::vector3& ns = NativePtr->get_space().scale;
				return Vector3(ns.x, ns.y, ns.z);
			}

			void set(Vector3 val)
			{
				const object_space& os = NativePtr->get_space();
				nexus::vector3* nval = reinterpret_cast<nexus::vector3*>(&val);
				NativePtr->move(os.location, os.rotation, *nval);
			}
		}

		//!	使用PDI接口绘制Actor的Bounding box
		void DrawBounds(NRenderPrimitiveDrawInterface^ PDI, Color4f color);

		void _SetOwner(NLevel^ lv)
		{
			m_owner = lv;
		}
	protected:
		//!	Native组件对象的包装对象集合
		System::Collections::Generic::List<NActorComponent^>^	m_compList;

		NLevel^ m_owner;

		//-- Native Wrapper 
	protected:		
		NActor(nexus::nactor::ptr nativeActor);		

	public:
		static NActor^ FromNativePtr(nexus::nactor::ptr nativeActor, System::String^ nativeClassName);
		void NativeActorLoaded();

		[System::ComponentModel::BrowsableAttribute(false)]
		property nactor* NativePtr
		{
			nactor* get();
		}
	};
}//namespace NexusEngine