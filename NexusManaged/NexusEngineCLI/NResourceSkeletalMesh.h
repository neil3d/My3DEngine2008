#pragma once
#include "NResource.h"
#include "NMtlBase.h"


namespace NexusEngine
{
	public ref class NResourceSkeletalMesh : public NResource
	{
	public:
		//!	从磁盘导入骨骼模型
		void ImportSkeletalMeshLOD(int lod, System::String^ fullPath);
		//!	设置某级LOD的材质
		void ImportSetMaterial(int lod, int mtl_id, NMtlBase^ mtl);
	
		//!	获得某级LOD的某个Section使用的材质
		NMtlBase^ GetMaterial(int lod, int mtl_id);

		//!	获得LOD级别的个数
		int  GetNumLOD();

		//!	获得某个LOD Mesh有几个Section
		int GetNumSection(int lod);

		//!	获得某级LOD的信息
		void GetLODInfo(int lod, [Out]int% numVert, [Out]int% numSec, [Out]int% numTri);

		property UInt32 LODCount
		{
			UInt32 get()
			{
				return NativePtr->get_num_lod();
			}
		}

		//-- Native Wrapper
	public:
		NResourceSkeletalMesh(nexus::nresource_skeletal_mesh::ptr nativeRes);
		nresource_skeletal_mesh::ptr GetSmartPtr();
	protected:
		property nresource_skeletal_mesh* NativePtr
		{
			nresource_skeletal_mesh* get();
		}
	};
}
