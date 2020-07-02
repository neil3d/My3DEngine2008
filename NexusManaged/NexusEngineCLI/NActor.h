/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NativeObject.h"
#include "NRenderElement.h"
#include "NMath.h"

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

		//! 删除所有部件
		bool	RemoveAllComponents()
		{
			return NativePtr->remove_all_component();
		}

		//! 更新所有部件的变换数据
		void UpdateComponentsTransform()
		{
			NativePtr->update_cmp_transform();
		}

		//判断是否与指定Actor类型相同
		bool IsSameTypeOf(NActor^ actor);

		//! 取得Actor的名字, Actor Name要求在某个Level中是唯一的
		[Category("Actor")]
		property System::String^ Name
		{
			System::String^ get()
			{
				return gcnew System::String(NativePtr->get_name().name_str.c_str());
			}
			void set(System::String^ val);
		}

		//! 取得Actor有多少个组件
		[Category("Actor")]
		property int NumComponents
		{
			int get()
			{
				return m_compList->Count;
			}
		}

		//!	Actor是否可以存盘
		[Category("Level Design")]
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
		[Category("Actor")]
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
		[Category("Level Design")]
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

		[Category("Movement")]
		property bool Static
		{
			bool get()
			{
				return NativePtr->get_static();
			}
			void set(bool value)
			{
				NativePtr->set_static(value);
			}
		}

		[Category("Movement")]
		property bool Kinematic
		{
			bool get()
			{
				return NativePtr->is_kinematic();
			}
			void set(bool value)
			{
				NativePtr->set_kinematic(value);
			}
		}

		//!	得到某个组件对象
		NActorComponent^ GetComponent(int i)
		{
			return m_compList[i];
		}

		[Browsable(false)]
		property ObjectSpace SpaceData
		{
			ObjectSpace get();
			void set(ObjectSpace newSpace);
		}

		//!	Actor的位置
		[Category("Actor")]
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
				if(os.location != *nval)
				{
					NativePtr->move(*nval, os.rotation, os.scale);
				}
			}
		}

		//!	Actor的旋转,单位: 角度
		[Category("Actor")]
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

		//!	Actor的旋转,单位: 弧度为单位
		[Category("Actor")]
		property Vector3 RadianRotation
		{
			Vector3 get()
			{
				return Vector3::FromNative(NativePtr->get_space().rotation);
			}

			void set(Vector3 val)
			{
				const object_space& os = NativePtr->get_space();
				vector3 nativeRotation = val.ToNative();
				if(nativeRotation != os.rotation)
				{
					NativePtr->move(os.location, nativeRotation, os.scale);
				}
			}
		}

		//!	Actor的缩放
		[Category("Actor")]
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
				if(*nval != os.scale)
				{
					NativePtr->move(os.location, os.rotation, *nval);
				}
			}
		}

		[Category("Level Design")]
		property String^ Layer
		{
			String^ get()
			{
				return gcnew System::String(NativePtr->get_layer_name().c_str());
			}
			void set(String^ layer_name);
		}

		//!	使用PDI接口绘制Actor的Bounding box
		void DrawBounds(NRenderPrimitiveDrawInterface^ PDI, Color4f color);

		void _SetOwner(NLevel^ lv)
		{
			m_owner = lv;
		}

		void LookAt(Vector3 Position)
		{
			nexus::vector3* position = reinterpret_cast<nexus::vector3*>(&Position);
			NativePtr->look_at(*position);
		}

		[Browsable(false)]
		property bool Destroyed
		{
			bool get() {	return m_destroyed;}
			void set(bool val)	{	m_destroyed = val;}
		}
	protected:
		//!	Native组件对象的包装对象集合
		System::Collections::Generic::List<NActorComponent^>^	m_compList;

		NLevel^ m_owner;
		bool	m_destroyed;

		//-- Native Wrapper 
	protected:
		//！定义缺省的构造函数保证跨语言兼容性
		NActor(){};		
		NActor(nexus::nactor::ptr nativeActor);		

	public:
		//clone接口的优势是可以让每种类型的NActor重载该接口以后可以直接返回自身类型的NActor，这样就不用在FromNativePtr中根据名字来判断了
		virtual NActor^	Clone(nexus::nactor::ptr nativeActor);
		static NActor^ FromNativePtr(nexus::nactor::ptr nativeActor, System::String^ nativeClassName);
		void NativeActorLoaded();

		[Browsable(false)]
		property nactor* NativePtr
		{
			nactor* get();
		}

		[Browsable(false)]
		property nactor::ptr SharedPtr
		{
			nactor::ptr get();
		}
	};
}//namespace NexusEngine