/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NativeObject.h"
#include "NResource.h"

namespace NexusEngine
{
	/**
	 *	引擎材质基类, 包装nexus::nmaterial_base类对象
	*/
	public ref class NMaterialBase : public NativeObjectOwnedBySharedPtr
	{
	public:
		NMaterialBase(String^ name)	{}

		//!	取得材质的名称
		String^ GetName();

		//!	取得材质所使用的模板的Resource Location
		NResourceLoc GetTemplateLoc();

		//-- Native Wrapper
	protected:
		NMaterialBase(nexus::nmaterial_base::ptr nativeMtl);		
		property nmaterial_base* NativePtr
		{
			nmaterial_base* get();
		}

	public:
		static NMaterialBase^ FromNativePtr(nexus::nmaterial_base::ptr nativeMtl);
		nexus::nmaterial_base::ptr GetSmartPtr();	
	};
}//namespace NexusEngine