#pragma once
#include "NResource.h"
#include "NMtlBase.h"

namespace NexusEngine
{
	public ref class NResourceAnimMesh : public NResource
	{
	public:	
		//!	从磁盘导入一个模型,作为某级LOD
		void ImportLOD(int lod, System::String^ fullPath);

		//!	设置某级LOD的材质
		void ImportSetMaterial(int lod, int sec, NMtlBase^ mtl);

		//!	获得LOD级别的个数
		int  GetNumLOD();

		//!	获得某级LOD的mesh的Scetion个数
		int  GetNumSection(int lod);

		//!	获得某级LOD的信息
		void GetLODInfo(int lod, [Out]int% numVert, [Out]int% numSec, [Out]int% numTri);

		//!	获得某级LOD的某个Section使用的材质
		NMtlBase^ GetMaterial(int lod, int sec);

		int GetNumSequence();
		String^ GetSequenceInfo(int i, [Out]float% length);

		//-- Native Wrapper
	public:
		NResourceAnimMesh(nexus::nresource_anim_mesh::ptr nativeRes);
		nresource_anim_mesh::ptr GetSmartPtr();
	protected:
		property nresource_anim_mesh* NativePtr
		{
			nresource_anim_mesh* get();
		}
	};
}//namespace NexusEngine