#ifndef _NEXUS_RESOURCE_SCENE_MESH_H_
#define _NEXUS_RESOURCE_SCENE_MESH_H_
#include "nresource.h"

namespace nexus
{
	/**
	*	内部使用空间分割来管理静态模型，用来处理体积大、面数多的场景建筑等模型
	*/
	class nresource_scene_mesh :
		public nresource
	{
	public:
		nresource_scene_mesh(const nstring& name_str);
		virtual ~nresource_scene_mesh(void);
	};
}//namespace nexus
#endif //_NEXUS_RESOURCE_SCENE_MESH_H_