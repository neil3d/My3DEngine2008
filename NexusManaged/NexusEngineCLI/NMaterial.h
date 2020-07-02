/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NMaterialBase.h"
#include "NMaterialTemplate.h"
#include "MaterialParam.h"

namespace NexusEngine
{
	/**
	 *	通用材质类, 包装nexus::nmaterial类对象
	*/
	public ref class NMaterial : public NMaterialBase
	{	
	public:
		NMaterial(String^ name);

		//! 使用指定的材质模板创建
		void Create(NMaterialTemplate^ temp);

		//! Load指定的材质模板, 并创建材质
		void Create(NResourceLoc tempLoc);

		//!	取得材质参数的个数
		int GetNumParameter()	{	return NativePtr->get_num_parameter();}

		//!	取得某个材质参数对象
		NMaterialParam GetParameter(int i);

		//!	设置材质参数对象, 内部使用"参数名称"来查找
		void SetParameter(NMaterialParam param);

		//-- Native Wrapper
	public:
		NMaterial(nexus::nmaterial::ptr nativeMtl);
		nexus::nmaterial::ptr GetSmartPtr();
	protected:
		property nmaterial* NativePtr
		{
			nmaterial* get() new;
		}
	};
}//namespace NexusEngine