#pragma once
#include "NativeObject.h"
#include "NResource.h"
#include "RenderDefine.h"

namespace NexusEngine
{
	public ref class NMtlBase : public NativeObjectOwnedBySharedPtr
	{
	public:
		NMtlBase(String^ nameStr);
		virtual ~NMtlBase(void);

		//-- 功能接口
		void CreateTechFromHLSL(String^ nameStr, NResourceLoc shaderLoc);

		//保存到文件
		void SaveToFile( NResourceLoc shaderLoc );

		property String^ Name
		{
			String^ get()
			{	return gcnew String(NativePtr->get_name().c_str()); }
			void set(String^ val)
			{
				pin_ptr<const wchar_t> szName = PtrToStringChars(val);
				NativePtr->set_name(szName);
			}
		}
		property enum class ETransparentType TransType
		{
			enum class ETransparentType get()
			{	return static_cast<NexusEngine::ETransparentType>( NativePtr->get_blend_type() ); }
			void set(enum class ETransparentType val)
			{	NativePtr->set_blend_type(static_cast<nexus::EBlendType>(val));}
		}

		property bool TwoSide
		{
			bool get()
			{	return NativePtr->get_two_side(); }
			void set(bool val)
			{	NativePtr->set_two_side(val);}
		}

		property NResourceLoc	ShaderLoc
		{
			NResourceLoc get()
			{
				NResourceLoc loc;
				loc.FromNative(NativePtr->get_shader_loc());
				return loc;
			}
		}

		//-- Native Wrapper
	protected:
		NMtlBase(nexus::nmtl_base::ptr nativeMtl);		
		property nmtl_base* NativePtr
		{
			nmtl_base* get();
		}

	public:
		static NMtlBase^ FromNativePtr(nexus::nmtl_base::ptr nativeMtl);
		static NMtlBase^ FromFile( NResourceLoc shaderLoc );
		nexus::nmtl_base::ptr GetSmartPtr();	
	};
}//namespace NexusEngine