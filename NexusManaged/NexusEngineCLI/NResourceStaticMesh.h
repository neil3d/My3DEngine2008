/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NResource.h"
#include "NMaterialBase.h"

namespace NexusEngine
{
	/**
	 *	静态模型资源
	*/
	public ref class NResourceStaticMesh : public NResource
	{
	public:	
		//!	从磁盘导入一个模型,作为某级LOD
		void ImportLOD(int lod, System::String^ fullPath);

		//!	设置某级LOD的材质
		void ImportSetMaterial(int lod, int sec, NMaterialBase^ mtl);

		//!	获得LOD级别的个数
		int  GetNumLOD();

		//!	获得某级LOD的mesh的Scetion个数
		int  GetNumSection(int lod);

		//!	获得某级LOD的信息
		void GetLODInfo(int lod, [Out]int% numVert, [Out]int% numSec, [Out]int% numTri);

		//!	获得某级LOD的某个Section使用的材质
		NMaterialBase^ GetMaterial(int lod, int sec);

		//-- Native Wrapper
	public:
		NResourceStaticMesh(nresource_static_mesh::ptr nativeRes);
		nresource_static_mesh::ptr GetSmartPtr();
	protected:
		property nresource_static_mesh* NativePtr
		{
			nresource_static_mesh* get();
		}
	};
}//namespace NexusEngine