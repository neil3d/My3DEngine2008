#pragma once
#include "nmtlcommon.h"
#include "NMath.h"
#include "MaterialParam.h"

namespace NexusEngine
{
	public ref class NMtlStatic : public NMtlCommon
	{
	public:
		NMtlStatic(String^ nameStr);
		virtual ~NMtlStatic(void);

		void SetFloatParameter(String^ paramName, Vector4 val);

		//! 创建默认的tech；然后创建一个Tech，绑定到Lighting和Unlit渲染策略
		void DefaultCreate(NResourceLoc shaderLoc);

		//!	取得材质参数的个数
		int GetNumParameter();

		//!	取得某个材质参数对象
		NMaterialParam GetParameter(int i);

		//!	设置材质参数对象, 内部使用"参数名称"来查找
		void SetParameter(NMaterialParam param);

		//-- Native Wrapper
	public:
		NMtlStatic(nexus::nmtl_static::ptr nativeMtl);		
	
	protected:		
		property nmtl_static* NativePtr
		{
			nmtl_static* get();
		}
	};
}//namespace NexusEngine