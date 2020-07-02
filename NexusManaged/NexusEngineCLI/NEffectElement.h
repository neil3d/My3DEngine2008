#pragma once
#include "NativeObject.h"
#include "NMath.h"
#include "NResource.h"

namespace NexusEngine
{
	using namespace Design;

	public enum class EsfxFaceType
	{
		NormalFace			= ESFX_NormalFace,		
		SphereBillboard		= ESFX_SphereBillboard,	
		CylinderBillboard	= ESFX_CylinderBillboard,	
	};

	public ref class NEffectElement
		: public NativeObjectOwnedBySharedPtr
	{
	public:
		static NEffectElement^ CreateEffectElement(String^ nativeClassName, String^ elementName);
		static Collections::ArrayList^ GetNativeClassNameList();

		[Category("EffectElement"), Description("element name")]
		property System::String^ Name
		{
			System::String^ get();
			void set(String^ val);
		}

		[Category("EffectElement"), Description("texture used by effect element")]
		property NResourceLoc Texture
		{
			NResourceLoc get()
			{
				NResourceLoc loc;
				loc.FromNative(NativePtr->m_Texture);
				return loc;
			}
			void set(NResourceLoc val)
			{
				resource_location loc;
				val.ToNative(loc);
				NativePtr->set_material_texture(loc);
			}
		}

		[Category("EffectElement"), Description("sub texture count in horizontal")]
		property int SubUVDivH
		{
			int get() 
			{ 
				return NativePtr->m_sub_uv.num_images_h; 
			}
			void  set(int val) 
			{ 
				NativePtr->m_sub_uv.num_images_h=(val==0?1:val);
			}
		}
		[Category("EffectElement"), Description("sub texture count in vertical")]
		property int SubUVDivV
		{
			int get() 
			{ 
				return NativePtr->m_sub_uv.num_images_v; 
			}
			void  set(int val) 
			{ 
				NativePtr->m_sub_uv.num_images_v=(val==0?1:val);
			}
		}

		[Category("EffectElement"), Description("index of the sub texture used")]
		property int SubUVIndex
		{
			int get() 
			{ 
				return NativePtr->m_sub_uv.image_index; 
			}
			void  set(int val) 
			{ 
				NativePtr->m_sub_uv.image_index=val;
			}
		}

		[Category("EffectElement"), Description("start time")]
		property virtual float StartTime
		{
			float get()			{	return NativePtr->m_start_time;}
			void set(float val)	{	NativePtr->m_start_time=val;}
		}

		[Category("EffectElement"), Description("initialized position")]
		property Vector3 InitPos
		{
			virtual Vector3 get()
			{
				return Vector3::FromNative(NativePtr->m_init_pos);
			}
			virtual void set(Vector3 val)
			{
				NativePtr->m_init_pos = val.ToNative();
			}
		}

		[Category("EffectElement"), Description("initialized rotation")]
		property Vector3 InitRot
		{
			virtual Vector3 get()
			{
				return Vector3::FromNative(NativePtr->m_init_rot);
			}
			virtual void set(Vector3 val)
			{
				NativePtr->m_init_rot = val.ToNative();
			}
		}

		[Category("EffectElement"), Description("initialized revolution")]
		property Vector3 InitRevolution
		{
			virtual Vector3 get()
			{
				return Vector3::FromNative(NativePtr->m_init_revolution);
			}
			virtual void set(Vector3 val)
			{
				NativePtr->m_init_revolution = val.ToNative();
			}
		}

		[Category("EffectElement"), Description("Gravity£¬0 means no gravity effect")]
		property float Gravity
		{
			virtual float get()
			{
				return NativePtr->m_gravity;
			}
			virtual void set(float val)
			{
				NativePtr->m_gravity = val;
			}
		}

		[Browsable(false)]
		property virtual bool Visible
		{
			bool get()
			{
				return NativePtr->is_visible();
			}
			void set(bool value)
			{
				NativePtr->set_visible(value);
			}
		}

		property System::String^ NativeClassName
		{
			System::String^ get()
			{
				return gcnew System::String( NativePtr->reflection_get_class()->get_name() );
			}
		}

		neffect_element::ptr GetNativeSmartPtr()
		{
			return boost::dynamic_pointer_cast<neffect_element>(*m_smart_ptr);
		}

		virtual void Init(String^ name);

	public:
		NEffectElement(neffect_element::ptr nativeObj);

	protected:
		property neffect_element* NativePtr
		{
			neffect_element* get();
		}
		
	};
}//namespace NexusEngine