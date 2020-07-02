/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NResourceStaticMesh.h"
#include "NResourceAnimMesh.h"
#include "NMtlTechShader.h"
#include "ResourceIODef.h"
#include "NResourceStaticMesh.h"
#include "NResourceTexture.h"
#include "NResourceTextureAtlas.h"
#include "NResourceSpecialEffect.h"
#include "NResourceSkeletalAnimSet.h"
#include "NResourceSkeletalMesh.h"

namespace NexusEngine
{
	/**
	 * 资源管理器, 包装nexus::nresource_manager
	*/
	public ref class NResourceManager
	{
	private:
		static NResourceManager^ s_instance;
		NResourceManager(void);
		
	public:
		static property NResourceManager^ Instance
		{
			NResourceManager^ get()
			{
				return s_instance;
			}
		}

		static NResourceManager(void)
		{
			s_instance = gcnew NResourceManager;
		}

	public:
		//!	创建一个空白的静态模型资源
		NResourceStaticMesh^ NewStaticMesh(System::String^ resName);
		NResourceAnimMesh^ NewAnimMesh(System::String^ resName);
		NResourceSpecialEffect^ NewSpecialEffect(System::String^ resName);

		NResourceSkeletalAnimSet^ NewSkeletalAnimSet(System::String^ resName);
		NResourceSkeletalMesh^ NewSkeletalMesh(System::String^ resName);

		//!	从文件系统加载材质模板脚本
		NMtlTechShader^ LoadMtlShader(NResourceLoc loc, EResourceIOMode mode, EResourceIOPriority pri);

		//!	从文件系统加载静态模型资源
		NResourceStaticMesh^ LoadStaticMesh(NResourceLoc loc, EResourceIOMode mode, EResourceIOPriority pri);

		NResourceAnimMesh^ LoadAnimMesh(NResourceLoc loc, EResourceIOMode mode, EResourceIOPriority pri);
		
		NResourceSkeletalAnimSet^ LoadSkeletalAnimSet(NResourceLoc loc, EResourceIOMode mode, EResourceIOPriority pri);
		NResourceSkeletalMesh^ LoadSkeletalMesh(NResourceLoc loc, EResourceIOMode mode, EResourceIOPriority pri);

		//! 加载一个2D纹理
		NResourceTexture2D^ LoadResourceTexture2D(NResourceLoc loc, EResourceIOMode mode,EResourceIOPriority priority);
	

		NResourceTextureAtlas^ LoadTextureAtlas(NResourceLoc loc, EResourceIOMode mode, EResourceIOPriority pri);
	};
}//namespace NexusEngine