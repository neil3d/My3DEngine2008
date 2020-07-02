/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NResource.h"

namespace NexusEngine
{
	/**
	 *	材质模板资源, 包装nexus::nmaterial_template
	*/
	public ref class NMaterialTemplate : public NResource
	{
	public:
		NMaterialTemplate(nexus::nmaterial_template::ptr nativeTemp);

		property nexus::nmaterial_template::ptr NativePtr
		{
			nexus::nmaterial_template::ptr get();
		}
	};
}//namespace NexusEngine